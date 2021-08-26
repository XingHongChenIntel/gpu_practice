//
// Created by cxh on 2021/8/5.
//
#include "register_case.h"
#include "test_case.h"
#include "utility/l0_helper.h"

class L0Test : public TestCase {
 public:
  std::string get_test_name();
  void run();
};

std::string L0Test::get_test_name() { return "l0_host_test"; }

void L0Test::run() {
  // Setup
  Timer timer;
  // Create kernel
  auto spirvModule = read_binary_file<uint8_t>("api_overhead_benchmark_empty_kernel.spv");
  if (spirvModule.size() == 0) {
    std::cerr << "kernel binary file is empty" << std::endl;
  }

  auto [hDriver, hDevice, hContext] = findDevice();

  // Create a command queue
  ze_command_queue_desc_t commandQueueDesc = {
      ZE_STRUCTURE_TYPE_COMMAND_QUEUE_DESC, nullptr, 0, 0, 0, ZE_COMMAND_QUEUE_MODE_DEFAULT,
      ZE_COMMAND_QUEUE_PRIORITY_NORMAL};
  ze_command_queue_handle_t commandQueue;
  L0_SAFE_CALL(zeCommandQueueCreate(hContext, hDevice, &commandQueueDesc, &commandQueue));

  // Create command list
  ze_command_list_handle_t cmdList;
  ze_command_list_desc_t cmdListDesc{};
  L0_SAFE_CALL(zeCommandListCreate(hContext, hDevice, &cmdListDesc, &cmdList));

  // create module
  ze_module_handle_t module;
  ze_module_desc_t moduleDesc{ZE_STRUCTURE_TYPE_MODULE_DESC};
  moduleDesc.format = ZE_MODULE_FORMAT_IL_SPIRV;
  moduleDesc.pInputModule = reinterpret_cast<const uint8_t *>(spirvModule.data());
  moduleDesc.inputSize = spirvModule.size();
  L0_SAFE_CALL(zeModuleCreate(hContext, hDevice, &moduleDesc, &module, nullptr));

  // create kernel
  ze_kernel_handle_t kernel;
  ze_kernel_desc_t kernelDesc{ZE_STRUCTURE_TYPE_KERNEL_DESC};
  kernelDesc.pKernelName = "empty";
  L0_SAFE_CALL(zeKernelCreate(module, &kernelDesc, &kernel));
  // Configure kernel
  L0_SAFE_CALL(zeKernelSetGroupSize(kernel, 1u, 1u, 1u));

  // construct pipeline
  const ze_group_count_t dispatchTraits{1u, 1u, 1u};
  L0_SAFE_CALL(zeCommandListAppendLaunchKernel(cmdList, kernel, &dispatchTraits, nullptr, 0, nullptr));
  L0_SAFE_CALL(zeCommandListClose(cmdList));

  // Warmup
  L0_SAFE_CALL(zeCommandQueueExecuteCommandLists(commandQueue, 1, &cmdList, nullptr));
  L0_SAFE_CALL(zeCommandQueueSynchronize(commandQueue, std::numeric_limits<uint32_t>::max()));

  // Benchmark
  timer.start();
  L0_SAFE_CALL(zeCommandQueueExecuteCommandLists(commandQueue, 1, &cmdList, nullptr));
  timer.stopAndTime();
  L0_SAFE_CALL(zeCommandQueueSynchronize(commandQueue, std::numeric_limits<uint32_t>::max()));

  L0_SAFE_CALL(zeKernelDestroy(kernel));
  L0_SAFE_CALL(zeModuleDestroy(module));
  L0_SAFE_CALL(zeCommandListDestroy(cmdList));
  L0_SAFE_CALL(zeCommandQueueDestroy(commandQueue));
}

static const RegisterTestCase<L0Test> l0_test;