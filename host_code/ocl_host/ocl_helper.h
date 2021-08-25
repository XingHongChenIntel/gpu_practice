//
// Created by cxh on 2021/8/25.
//

#ifndef GPU_PRACTICE_HOST_CODE_OCL_HOST_OCL_HELPER_H_
#define GPU_PRACTICE_HOST_CODE_OCL_HOST_OCL_HELPER_H_
#include <CL/cl2.hpp>

#include <chrono>
#include <fstream>
#include <vector>

class Timer {
 public:
  std::chrono::high_resolution_clock::time_point tick, tock;

  void start() { tick = std::chrono::high_resolution_clock::now(); }

  float stopAndTime() {
    tock = std::chrono::high_resolution_clock::now();
    return static_cast<float>(std::chrono::duration<std::chrono::milliseconds>(tock - tick).count());
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

void find_device(){
  std::vector<cl::Platform> platforms;
  cl::Platform::get(&platforms);

}


#endif  // GPU_PRACTICE_HOST_CODE_OCL_HOST_OCL_HELPER_H_
