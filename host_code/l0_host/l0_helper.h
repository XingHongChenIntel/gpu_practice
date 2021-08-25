//
// Created by cxh on 2021/8/25.
//

#ifndef GPU_PRACTICE_HOST_CODE_L0_HOST_L0_HELPER_H_
#define GPU_PRACTICE_HOST_CODE_L0_HOST_L0_HELPER_H_

#include <assert.h>
#include <ze_api.h>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <tuple>
#include <vector>
#include <fstream>

#define L0_SAFE_CALL(call)                                                      \
  {                                                                             \
    auto status = (call);                                                       \
    if (status != 0) {                                                          \
      fprintf(stderr, "%s:%d: L0 error %d\n", __FILE__, __LINE__, (int)status); \
      exit(1);                                                                  \
    }                                                                           \
  }

class Timer {
 public:
  std::chrono::high_resolution_clock::time_point tick, tock;

  void start() { tick = std::chrono::high_resolution_clock::now(); }

  float stopAndTime() {
    tock = std::chrono::high_resolution_clock::now();
    return static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(tock - tick).count());
  }
};

template <typename T>
std::vector<T> read_binary_file(const char *fname, size_t num = 0) {
  std::vector<T> vec;
  std::ifstream ifs(fname, std::ios::in | std::ios::binary);
  if (ifs.good()) {
    ifs.unsetf(std::ios::skipws);
    std::streampos file_size;
    ifs.seekg(0, std::ios::end);
    file_size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    size_t max_num = file_size / sizeof(T);
    vec.resize(num ? (std::min)(max_num, num) : max_num);
    ifs.read(reinterpret_cast<char *>(vec.data()), vec.size() * sizeof(T));
  }
  return vec;
}

inline auto findDevice() {
  // init l0 runtime
  L0_SAFE_CALL(zeInit(ZE_INIT_FLAG_GPU_ONLY));

  // Discover all the driver instances
  uint32_t driverCount = 0;
  L0_SAFE_CALL(zeDriverGet(&driverCount, nullptr));
  fprintf(stderr, "driverCount = %d\n", (int)driverCount);

  ze_driver_handle_t *allDrivers = (ze_driver_handle_t *)malloc(driverCount * sizeof(ze_driver_handle_t));
  L0_SAFE_CALL(zeDriverGet(&driverCount, allDrivers));

  ze_driver_handle_t hDriver = nullptr;
  ze_device_handle_t hDevice = nullptr;
  for (uint32_t i = 0; i < driverCount; ++i) {
    // for each driver, get device
    uint32_t deviceCount = 0;
    hDriver = allDrivers[i];
    L0_SAFE_CALL(zeDeviceGet(hDriver, &deviceCount, nullptr));
    fprintf(stderr, "driver = %d: deviceCount= %d\n", (int)i, (int)deviceCount);
    ze_device_handle_t *allDevices = (ze_device_handle_t *)malloc(deviceCount * sizeof(ze_device_handle_t));
    L0_SAFE_CALL(zeDeviceGet(hDriver, &deviceCount, allDevices));

    for (uint32_t d = 0; d < deviceCount; ++d) {
      ze_device_properties_t device_properties;
      L0_SAFE_CALL(zeDeviceGetProperties(allDevices[d], &device_properties));
      if (ZE_DEVICE_TYPE_GPU == device_properties.type) {
        hDevice = allDevices[d];
        break;
      }
    }
    free(allDevices);
    if (nullptr != hDevice) {
      break;
    }
  }
  free(allDrivers);

  assert(hDriver);
  assert(hDevice);

  ze_context_desc_t contextDesc = {ZE_STRUCTURE_TYPE_CONTEXT_DESC, nullptr, 0};
  ze_context_handle_t hContext = nullptr;
  L0_SAFE_CALL(zeContextCreate(hDriver, &contextDesc, &hContext));

  return std::make_tuple(hDriver, hDevice, hContext);
}

void print_device_info(ze_driver_handle_t hDriver, ze_device_handle_t hDevice, ze_context_handle_t hContext) {
  ze_driver_properties_t driver_properties;
  L0_SAFE_CALL(zeDriverGetProperties(hDriver, &driver_properties))
  std::cout << "driver id : " << driver_properties.uuid.id << std::endl;
  std::cout << "driver version : " << driver_properties.driverVersion << std::endl;

  uint32_t count = 0;
  L0_SAFE_CALL(zeDriverGetExtensionProperties(hDriver, &count, nullptr))
  ze_driver_extension_properties_t *extension_properties =
      (ze_driver_extension_properties_t *)malloc(sizeof(ze_driver_extension_properties_t) * count);
  L0_SAFE_CALL(zeDriverGetExtensionProperties(hDriver, &count, extension_properties))
  for (int i = 0; i < count; i++) {
    std::cout << "the extension name : " << extension_properties[i].name << "\n"
              << "the it's version is : " << extension_properties[i].version << std::endl;
  }
  free(extension_properties);

  ze_device_properties_t device;
  L0_SAFE_CALL(zeDeviceGetProperties(hDevice, &device))
  std::cout << "the l0 device name is " << device.name << std::endl;

  ze_device_module_properties_t module_properties;
  L0_SAFE_CALL(zeDeviceGetModuleProperties(hDevice, &module_properties))
  std::cout << "device support f16 :" << module_properties.fp16flags << "\n"
            << "device support f64 :" << module_properties.fp64flags << std::endl;

  ze_device_memory_properties_t device_memory_properties;
  count = 0;
  L0_SAFE_CALL(zeDeviceGetMemoryProperties(hDevice, &count, &device_memory_properties));
  std::cout << "the total global mem size : " << device_memory_properties.totalSize << std::endl;

  ze_device_compute_properties_t compute_properties;
  L0_SAFE_CALL(zeDeviceGetComputeProperties(hDevice, &compute_properties));
  std::cout << "the compute_propertise : \n"
            << "the maxTotalGroupSize is " << compute_properties.maxTotalGroupSize << "\n"
            << "the maxGroupSizeX is " << compute_properties.maxGroupSizeX << "\n"
            << "the maxGroupSizeY is " << compute_properties.maxGroupSizeY << "\n"
            << "the maxGroupSizeZ is " << compute_properties.maxGroupSizeZ << "\n"
            << "the maxGroupCountX is " << compute_properties.maxGroupCountX << "\n"
            << "the maxGroupCountX is " << compute_properties.maxGroupCountY << "\n"
            << "the maxGroupCountX is " << compute_properties.maxGroupCountZ << "\n"
            << "the maxSharedLocalMemory is " << compute_properties.maxSharedLocalMemory << std::endl;

  ze_device_memory_properties_t memory_properties;
  uint32_t pcount = 0;
  L0_SAFE_CALL(zeDeviceGetMemoryProperties(hDevice, &pcount, &memory_properties));
  std::cout << "the memory_properties.totalSize is " << memory_properties.totalSize << std::endl;
  std::cout << "the maxmemalloc size is " << device.maxMemAllocSize << std::endl;
  std::cout << "the basic eu is : \n"
            << "the numslices is " << device.numSlices << "\n"
            << "the num sub slices is " << device.numSubslicesPerSlice << "\n"
            << "the num eu per subslice is " << device.numEUsPerSubslice << "\n"
            << "the num thread per eu is " << device.numThreadsPerEU << "\n"
            << std::endl;
}



#endif  // GPU_PRACTICE_HOST_CODE_L0_HOST_L0_HELPER_H_
