//
// Created by cxh on 2021/8/16.
//
#include <memory.h>

#include <cstdlib>
#include <sstream>

#include "register_case.h"
#include "test_case.h"
#include "validate.h"
#include "utility/l0_helper.h"

#define CLASSNAME L0MaxValue

class CLASSNAME : public TestCase {
 public:
  std::string get_test_name();
  void run();
};

std::string CLASSNAME::get_test_name() { return "l0_max_value"; }

static void cpu_compute(float *input_buf, float *output_buf, uint32_t row, uint32_t column) {
  for (uint32_t i = 0; i < row; i++) {
    output_buf[i] = std::numeric_limits<float>::min();
    for (uint32_t j = 0; j < column; j++) {
      if (input_buf[i * column + j] > output_buf[i]) {
        output_buf[i] = input_buf[i * column + j];
      }
    }
  }
}

void CLASSNAME::run() {
  std::cout << get_test_name() << "is running ..." << std::endl;
  // Setup
  Timer timer;
  // Create kernel
  auto spirvModule = read_binary_file<uint8_t>("../kernel/max_kernel_L0.bin");
  if (spirvModule.size() == 0) {
    std::cerr << "kernel binary file is empty" << std::endl;
  }

  auto [hDriver, hDevice, hContext] = findDevice();
  print_device_info(hDriver, hDevice, hContext);
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
  moduleDesc.format = ZE_MODULE_FORMAT_NATIVE;
  moduleDesc.pInputModule = reinterpret_cast<const uint8_t *>(spirvModule.data());
  moduleDesc.inputSize = spirvModule.size();
  L0_SAFE_CALL(zeModuleCreate(hContext, hDevice, &moduleDesc, &module, nullptr));

  // construct pipeline

  // alloc buffer on host and device.
  uint32_t row = 512;
  uint32_t column = 512;
  float *inputBuf = (float *)malloc(sizeof(float) * row * column);
  fill_buf(inputBuf, row * column);

  float *outputBuf = (float *)malloc(sizeof(float) * row);
  float *outputBuf_ref = (float *)malloc(sizeof(float) * row);
  memset(outputBuf, 0, sizeof(float) * row);
  memset(outputBuf_ref, 1, sizeof(float) * row);

  ze_device_mem_alloc_desc_t device_desc = {ZE_STRUCTURE_TYPE_DEVICE_MEM_ALLOC_DESC, nullptr, 0, 0};
  void *v0 = nullptr;
  void *v1 = nullptr;
  L0_SAFE_CALL(zeMemAllocDevice(hContext, &device_desc, sizeof(float) * row * column, 4096, hDevice, &v0));
  L0_SAFE_CALL(zeMemAllocDevice(hContext, &device_desc, sizeof(float) * row, 4096, hDevice, &v1));

  // create kernel
  ze_kernel_handle_t kernel;
  ze_kernel_desc_t kernelDesc{ZE_STRUCTURE_TYPE_KERNEL_DESC};
  kernelDesc.pKernelName = "empty";
  L0_SAFE_CALL(zeKernelCreate(module, &kernelDesc, &kernel));
  // Configure kernel
  L0_SAFE_CALL(zeKernelSetGroupSize(kernel, 1u, 1u, 1u));
  L0_SAFE_CALL(zeKernelSetArgumentValue(kernel, 0, sizeof(v0), &v0));
  L0_SAFE_CALL(zeKernelSetArgumentValue(kernel, 1, sizeof(v1), &v1));

  L0_SAFE_CALL(zeCommandListAppendMemoryCopy(cmdList, v0, inputBuf, sizeof(float) * row * column, nullptr, 0, nullptr));
  L0_SAFE_CALL(zeCommandListAppendBarrier(cmdList, nullptr, 0, nullptr));
  ze_group_count_t groupCount = {512, 1, 1};
  L0_SAFE_CALL(zeCommandListAppendLaunchKernel(cmdList, kernel, &groupCount, nullptr, 0, nullptr));
  L0_SAFE_CALL(zeCommandListAppendMemoryCopy(cmdList, outputBuf, v1, sizeof(float) * row, nullptr, 0, nullptr));
  L0_SAFE_CALL(zeCommandListAppendBarrier(cmdList, nullptr, 0, nullptr));
  L0_SAFE_CALL(zeCommandListClose(cmdList));

  timer.start();
  L0_SAFE_CALL(zeCommandQueueExecuteCommandLists(commandQueue, 1, &cmdList, nullptr));
  L0_SAFE_CALL(zeCommandQueueSynchronize(commandQueue, std::numeric_limits<uint32_t>::max()));
  float timed;
  timed = timer.stopAndTime();
  std::cout << "execution time is : " << timed << std::endl;

  cpu_compute(inputBuf, outputBuf_ref, row, column);
  if (check(outputBuf, outputBuf_ref, row)) {
    std::cout << "The test case passed!!!" << std::endl;
  } else {
    std::cout << "Test failed!!" << std::endl;
  }
  L0_SAFE_CALL(zeMemFree(hContext, v0));
  L0_SAFE_CALL(zeMemFree(hContext, v1));
  L0_SAFE_CALL(zeKernelDestroy(kernel));
  L0_SAFE_CALL(zeModuleDestroy(module));
  L0_SAFE_CALL(zeCommandListDestroy(cmdList));
  L0_SAFE_CALL(zeCommandQueueDestroy(commandQueue));
  free(outputBuf);
  free(outputBuf_ref);
  free(inputBuf);
}

static const inline RegisterTestCase<CLASSNAME> l0_max_value;