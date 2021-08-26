//
// Created by cxh on 2021/8/25.
//

#ifndef GPU_PRACTICE_FRAMEWORK_VALIDATE_H_
#define GPU_PRACTICE_FRAMEWORK_VALIDATE_H_

#include <math.h>

static inline float IsAlmostEqual(float a, float b) {
  return std::fabs(a - b) < std::numeric_limits<float>::epsilon() * std::fabs(a + b) ||
         std::fabs(a - b) < std::numeric_limits<float>::min();
}

static inline bool check(float *dst, float *cpu_dst, int n) {
  float err = 0.f;
  for (int i = 0; i < n; ++i) {
    if (!IsAlmostEqual(dst[i], cpu_dst[i])) {
      printf("Error, index %d: Wanted %f, got %f\n", i, cpu_dst[i], dst[i]);
      return false;
    }
  }
  return true;
}

static const int buf_scope = 1000;
static inline float rand_float() { return static_cast<float>(rand() % buf_scope); }

template <typename T>
static inline void fill_buf(T *input_buf, uint32_t size) {
  for (uint32_t i = 0; i < size; i++) {
    input_buf[i] = static_cast<T>(rand_float());
  }
}

#endif  // GPU_PRACTICE_FRAMEWORK_VALIDATE_H_
