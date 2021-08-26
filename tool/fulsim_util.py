from filelock import FileLock
import json
import os
from subprocess import Popen, PIPE
import multiprocessing as mp
import numpy as np
import time
import sys
import pathlib


class FulsimManager:
    """Context manager for fulsim resource acquire and release"""

    @staticmethod
    def __acquire_port(range_low, range_high):
        run_fulsim = False
        pid = str(os.getpid())
        cur_path = os.path.dirname(os.path.abspath(__file__))
        file = os.path.join(os.path.abspath(cur_path), 'pid_port.json')
        with FileLock(str(file) + '.lock'):
            if os.path.isfile(file):
                with open(file, 'r+') as fn:
                    pid_port = json.load(fn)
                    tbx_port = None

                    if pid in pid_port.keys():
                        tbx_port = pid_port[pid]
                        run_fulsim = False

                    if tbx_port is None:
                        tbx_port = np.random.randint(range_low, range_high)
                        while tbx_port in pid_port.values():
                            tbx_port = np.random.randint(range_low, range_high)
                        run_fulsim = True

                    pid_port[pid] = tbx_port
                    fn.seek(0)
                    json.dump(pid_port, fn)
            else:
                with open(file, 'a') as fn:
                    tbx_port = np.random.randint(range_low, range_high)
                    pid_port = {pid: tbx_port}

                    json.dump(pid_port, fn)
                    run_fulsim = True
                    fn.close()

        return tbx_port, run_fulsim

    @staticmethod
    def __killproc(proc):
        try:
            if os.name == 'nt':
                Popen(['taskkill', '/F', '/T', '/PID', str(proc.pid)], shell=True)
            else:
                os.kill(proc.pid, 0)
                os.kill(proc.pid, 9)
        except Exception as e:
            raise Exception(f"Cannot kill pid {proc.pid} Error {str(e)}")

    @staticmethod
    def run_command(cmd):
        p = Popen(cmd, shell=True, stdout=PIPE, stderr=PIPE)
        output, err = p.communicate()
        print(output)
        print(err)

    def run_fulsim(self, cmd):
        p = Popen(cmd.split(), universal_newlines=True, stdout=PIPE, stderr=PIPE, bufsize=1)
        timeout = 20
        time0 = time.time()
        for line in p.stdout:
            if time.time() - time0 > timeout:
                self.__killproc(p)
                raise Exception("Fulsim not ready")
            print(line, end='')  # process line here
            last_line = line
            if line.startswith('DONE'):
                break

        if not last_line.startswith('DONE'):
            self.__killproc(p)
            raise Exception("Fulsim not ready" + last_line)

        return p

    def __init__(self, port_range, fulsim_path, device_op, fulsim_flags=''):
        tbx_port, new_fulsim = self.__acquire_port(port_range[0], port_range[1])
        self.tbx_port = tbx_port
        if new_fulsim:
            if sys.platform == "linux":
                cmd = rf"{fulsim_path}/AubLoad -device {device_op} {fulsim_flags} -socket tcp:{tbx_port}"
            else:
                cmd = rf"{fulsim_path}\AubLoad.exe -device {device_op} {fulsim_flags} -socket tcp:{tbx_port}"
            self.proc = self.run_fulsim(cmd)

    def __enter__(self):
        return self.tbx_port

    def __exit__(self, etype, value, traceback):
        pass
        # self.__killproc(self.proc)


def get_name(command):
    return command.replace("--", "").replace(" ", "")


def execute_command(command):
    name = get_name(command)
    command = "./clpeak" + " --presilicon " + command
    os.environ["AUBDumpCaptureFileName"] = name + ".aub"
    fulsim_path = os.getenv("CSDK_SIM")
    print(fulsim_path)
    print(os.getenv("AUBDumpCaptureFileName"))
    port_range = list({6200, 6500})
    device_op = "pvcxt:1tx8x8x8:b0"
    trace_path = pathlib.Path(__file__).parent / name
    fulsim_flags = f"-swsbcheck fatal -computeOptions waitforeu -enableFeature 1406907150 2203915042 2206197252 14010626195 1408789165 -enablefeature :kaolinObserverAndTraceRecording -kout {trace_path}"
    with FulsimManager(port_range, fulsim_path, device_op, fulsim_flags=fulsim_flags) as tbx_port:
        time.sleep(3)
        os.environ["TbxPort"] = f"{tbx_port}"
        FulsimManager.run_command(command)


if __name__ == "__main__":
    workloads = ["--global-bandwidth"] 
                # "--compute-dp", "--compute-integer", "--compute-intfast", "--compute-xmx --int8",
                # "--compute-xmx --bf16", "--transfer-bandwidth", "--global-bandwidth"]
    PROCESS = 10
    with mp.Pool(PROCESS) as pool:
        results = [pool.apply_async(execute_command, (work,)) for work in workloads]
        for r in results:
            print('\t', r.get())
        pool.close()
        pool.join()
    
    kaolin_commands = []
    for work in workloads:
        trace_path = str(pathlib.Path(__file__).parent / get_name(work))
        kaolin_commands.append(
            f"$KAOLIN_PATH/bin/kaolin12"
            f" -trace {trace_path}"
            f" -stat {trace_path}_trace.stat"
            f" -tgout {trace_path}_trace_timegraph.txt"
            f" -tgcfg $KAOLIN_PATH/tg/graphprof.txt,$KAOLIN_PATH/tg/gpgpu.txt"
            f" -tginterval 1000 "
            f" -cfg $KAOLIN_PATH/cfg/pvc_gen12_16x4x16_c0.cfg "
            f" -Eu.OldestFirst false"
        )

    with mp.Pool(PROCESS) as pool:
        results = [pool.apply_async(FulsimManager.run_command, (kaolin_command,)) for kaolin_command in kaolin_commands]
        for r in results:
            print('\t', r.get())
        pool.close()
        pool.join()
