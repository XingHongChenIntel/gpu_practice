from functools import partial
from clint.textui import progress
from pathlib import Path
import yaml
import shutil
import getpass
import subprocess
import git
import logging
import os
import sys
import requests

onepack = dict()
ROOT_DIR = Path(__file__).parent.parent


# parse tool_configure file
def read_configure(tool):
    if tool is None:
        logging.error(f"The Download method didn't registry!")
    config_file = ROOT_DIR / "tools_configure.yaml"
    with open(config_file, "r") as f:
        tool_package = yaml.load(f, Loader=yaml.FullLoader)
        tool_attr = tool_package.get(tool, None)
        if tool_attr is None:
            logging.error(f"there is no {tool}'s config info in config yaml!")
            raise KeyError
        return tool_attr


# Use decorator to bundle download method and configure attribution
def tool_registry(tool=None):
    tool_attr = read_configure(tool)

    def registry(func):
        new_func = partial(func, tool_attr)
        onepack[tool] = new_func
        return new_func

    return registry


#####################################
# registry tools download method here
#####################################
@tool_registry("kaolin")
def download_kaolin(kwargs):
    local_name = kwargs.get("name", None)
    version = kwargs.get("version", None)
    host = kwargs.get("address", None)

    archive_name = f"{local_name}_{version}.zip"
    package_url = f"{host}/{version}/archive.zip"

    kaolin = Download(archive_name, package_url)
    kaolin.download()
    local_pack = kaolin.get_pack_path()

    kaolin = Unzip(local_pack, f"{local_name}_{version}")
    kaolin.unzip(kaolin.unzip_zipfile)
    return kaolin.get_local_unpack_path()


@tool_registry("gits")
def download_gits(kwargs):
    local_name = kwargs.get("name", None)
    version = kwargs.get("version", None)
    host = kwargs.get("address", None)

    archive_name = f"{local_name}_{version}.sh"
    package_url = f"{host}/{version}/lnx_x64/{version}.sh"

    gits = Download(archive_name, package_url)
    gits.download()

    local_file = gits.get_pack_path()
    tool_dir = get_tool_path()
    local_repo = tool_dir / f"{local_name}_{version}"
    if not os.path.exists(local_repo):
        cmd = f"bash {local_file} {local_repo}"
        install_linux_script(local_repo, cmd)
    else:
        logging.info(f"the {local_repo} is already exists!")
    return local_repo


@tool_registry("pyl0")
def download_pyl0(kwargs):
    local_name = kwargs.get("name", None)
    version = kwargs.get("version", None)
    host = kwargs.get("address", None)

    archive_name = f"{local_name}_{version}.zip"
    package_url = f"{host}/{version}/archive.zip"

    pyl0 = Download(archive_name, package_url)
    pyl0.download()

    local_pack = pyl0.get_pack_path()
    install_dir = ROOT_DIR / "kgen/pyl0_src"
    pyl0 = Unzip(local_pack, local_name, install_dir)
    pyl0.unzip(pyl0.unzip_zipfile)
    return pyl0.get_local_unpack_path()


@tool_registry("marcus")
def download_marcus(kwargs):
    local_name = kwargs.get("name", None)
    version = kwargs.get("version", None)
    host = kwargs.get("address", None)

    marcus = Download(local_name, host, repo_type="git")
    marcus.download()
    return marcus.local_path


@tool_registry("avi")
def download_avi(kwargs):
    local_name = kwargs.get("name", None)
    version = kwargs.get("version", None)
    host = kwargs.get("address", None)

    avi = Download(local_name, host, repo_type="git")
    avi.download()
    return avi.local_path


@tool_registry("sim")
def download_sim(kwargs):
    local_name = kwargs.get("name", None)
    version = kwargs.get("version", None)
    host = kwargs.get("address", None)

    install_name = f"{local_name}_{version}"
    archive = f"{install_name}.zip"
    package_url = f"{host}/{version}/archive.zip"

    sim = Download(archive, package_url)
    sim.download()
    local_pack = sim.get_pack_path()

    sim = Unzip(local_pack, install_name)
    sim.unzip(sim.unzip_zipfile)
    return sim.get_local_unpack_path()


@tool_registry("sdk")
def download_sdk(kwargs):
    # usr configure
    local_name = kwargs.get("name", None)
    version = kwargs.get("version", None)
    host = kwargs.get("address", None)

    # get archive url, according to version and platform
    install_name = f"ComputeSDK_Linux_internal_{version}"
    archive = f"ComputeSDK_Linux_internal_{version}.tar.gz"
    package_url = f"{host}/Linux/{version}/artifacts/{archive}"

    # download repo
    sdk = Download(archive, package_url)
    sdk.download()
    local_pack = sdk.get_pack_path()
    # unpack
    sdk = Unzip(local_pack, install_name)
    sdk.unzip()
    local_unpack = sdk.get_local_unpack_path()
    workspace_path = os.path.join(local_unpack, "workspace")

    # install fulsim
    package_archive = os.path.join(workspace_path, "opt", "intel", "sim")
    if not os.path.exists(package_archive):
        logging.info(f" set workspace to {workspace_path}")
        script = os.path.join(local_unpack, "setup_workspace-20.04.sh")
        cmd = "yes | bash " + script + " " + workspace_path
        install_linux_script(workspace_path, cmd)

    download_sdk_sim(workspace_path, "PVC")
    download_sdk_sim(workspace_path, "ATS")
    download_sdk_sim(workspace_path, "ELG")
    return local_unpack


def download_sdk_sim(workspace_path, sim_verion):
    host = workspace_path
    archive = sim_verion
    package_archive = os.path.join(workspace_path, "opt", "intel", "sim", archive)
    package_url = os.path.join(workspace_path, "get_sim.sh")

    if os.path.exists(package_archive):
        logging.info(f" Found sdk fulsim in {host}")
        return

    usr, pwd = Download.return_usr_pwd()
    if usr is None:
        usr, pwd = get_usr_pwd()
    logging.info(f" start download sdk fulsim, please waiting ....")
    cmd = f"bash {package_url} --user {usr} --password {pwd} {sim_verion}"
    install_linux_script(package_archive, cmd)


######################################################
#  download functions
######################################################
def check_download_list(auto_download=True):
    for tool, func in onepack.items():
        if auto_download:
            logging.info(f"start check {tool}!")
            func()
        else:
            choice = input("do you want to download " + tool + "[y/n]: ")
            if str.lower(choice) == "y":
                logging.info("downloading ...")
                func()


def get_tool_path():
    tool_path = ROOT_DIR / "third_party" / sys.platform
    if not tool_path.exists():
        tool_path.mkdir(parents=True)
    return tool_path


def is_target_exist(target):
    if os.path.exists(target):
        logging.info(f" Found {target}")
        sys.stdout.flush()
        return True
    return False


def get_usr_pwd():
    print("no -u USERNAME, Please enter your usr name")
    usr = input("Please enter your usr name:")
    print(f"Hi User {usr}, Please enter your password: ")
    try:
        pwd = getpass.getpass()
    except Exception as error:
        logging.error("ERROR", error)
    Download.set_usr_pwd(usr, pwd)
    return usr, pwd


class Download:
    __usr = None
    __pwd = None

    def __init__(self, package_name, package_url, repo_type="release"):
        self.package_name = package_name
        self.package_url = package_url
        tool_dir = get_tool_path()
        self.local_path = tool_dir / package_name
        self.repo_type = repo_type

    def get_pack_path(self):
        return self.local_path

    @staticmethod
    def return_usr_pwd():
        return Download.__usr, Download.__pwd

    @staticmethod
    def set_usr_pwd(usr, pwd):
        Download.__usr = usr
        Download.__pwd = pwd

    def download_release(self):
        if Download.__usr is None:
            Download.__usr, Download.__pwd = get_usr_pwd()
        logging.info(f"{self.package_name} which is downloading from {self.package_url}")

        try:
            r = requests.get(self.package_url, stream=True, auth=(self.__usr, self.__pwd), verify=False, timeout=15)
            len = int(r.headers["content-length"])
            with open(self.local_path, "wb") as fd:
                for chunk in progress.bar(r.iter_content(chunk_size=1024), expected_size=(len / 1024) + 1, width=100):
                    if chunk:
                        fd.write(chunk)
            flen = os.path.getsize(self.local_path)
            logging.debug(f"the context len is {len} and file len is {flen}")
        except requests.ConnectionError as e:
            logging.error("connection error, this may cause by proxy or wrong package version:", e)
        except requests.HTTPError as e:
            logging.error("http error, this may cause by wrong usr or password :", e)
        except requests.Timeout as e:
            logging.error("the connection time out", e)
        except Exception as e:
            logging.error(f"\nError Message: {e}")
            logging.info(f"\n ... download interrupted, cleaning-up")
            sys.stdout.flush()
            if os.path.exists(self.local_path):
                os.remove(self.local_path)

    def download_git(self):
        print(f" start clone {self.package_name} ....")
        try:
            git.Repo.clone_from(self.package_url, self.local_path)
        except Exception as e:
            logging.error(f"\nError Message: {e}")
            logging.info(f"\n ... download interrupted, cleaning-up")
            logging.warning(f"please make sure you have the right to access the 1source intel and setup git environment."
                            f"please follow https://1source.intel.com/onboard")
            sys.stdout.flush()
            if os.path.exists(self.local_path):
                os.remove(self.local_path)

    def download(self):
        # check if tool already download
        if is_target_exist(self.local_path):
            logging.info(f"the {self.local_path} is already exists!")
            return
        if self.repo_type == "release":
            self.download_release()
        if self.repo_type == "git":
            self.download_git()


class Unzip:
    def __init__(self, zip_pack, install_name, install_dir=None):
        self.zip_pack = zip_pack
        if install_dir is None:
            install_dir = os.path.dirname(zip_pack)
        self.install_dir = install_dir
        self.local_path = os.path.join(install_dir, install_name)

    def get_local_unpack_path(self):
        return self.local_path

    def unzip_zipfile(self, zip_pack, install_path):
        install_dir = self.install_dir

        # it won't create new dir when we use zipfile on linux.
        if sys.platform == "linux":
            install_dir = self.local_path

        import zipfile

        with zipfile.ZipFile(self.zip_pack, "r") as zip_ref:
            print(f" start to unzip the archive to {install_dir}, please wait .....")
            zip_ref.extractall(install_dir)
        print(f" unzip the archive finished!")

    def unzip_tarfile(self, zip_pack, install_path):
        import tarfile

        with tarfile.open(self.zip_pack) as tar:
            print(f" start to unzip the archive to {self.install_dir}, please wait .....")
            tar.extractall(self.install_dir)
        print(f" unzip the archive finished!")

    def unzip_func(self, zip_pack, install_path):
        if sys.platform == "win32":
            self.unzip_zipfile(self.zip_pack, self.install_dir)
        if sys.platform == "linux":
            self.unzip_tarfile(self.zip_pack, self.install_dir)

    def unzip(self, func=None):
        if is_target_exist(self.local_path):
            logging.info(f"the {self.local_path} is already exists")
            return

        print(f" Unpacking to {self.install_dir}")
        sys.stdout.flush()
        try:
            if func is None:
                self.unzip_func(self.zip_pack, self.install_dir)
            else:
                func(self.zip_pack, self.install_dir)
        except Exception as e:
            print(f" ... unpacking interrupted, Error:{e} \ncleaning-up")
            sys.stdout.flush()
            shutil.rmtree(self.local_path)


def install_linux_script(archive_unpacked, cmd):
    # TODO  print subprocess info
    try:
        res = subprocess.call(cmd, shell=True, stdout=subprocess.DEVNULL, stderr=subprocess.STDOUT)
    except Exception as e:
        print(f" ... set workspace failed, error:{e} \ncleaning-up")
        sys.stdout.flush()
        shutil.rmtree(archive_unpacked)


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)
    check_download_list()
