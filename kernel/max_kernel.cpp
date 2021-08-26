//
// Created by cxh on 2021/8/16.
//
#include <cm/cm.h>

#define CHUCK 64
#define NUM_ELEM_PER_THREAD 512
#define NUM_CHUCK NUM_ELEM_PER_THREAD / CHUCK
const uint initOffset[16] = {0, 256, 512, 768, 1024, 1280, 1536, 1792, 2048, 2304, 2560, 2816, 3072, 3328, 3584, 3840};

extern "C" _GENX_MAIN_ void empty(SurfaceIndex input [[type("buffer_t float")]],
                                  SurfaceIndex output [[type("buffer_t float")]]) {
  uint gidX = cm_group_id(0);
  uint lszX = cm_local_size(0);
  uint tidX = cm_local_id(0);
  uint gctX = cm_group_count(0);

  matrix<float, NUM_CHUCK, CHUCK> inA = 0;
  vector_ref<float, CHUCK> inA1(inA.row(0));
  vector_ref<float, CHUCK> inA2(inA.row(1));
  vector_ref<float, CHUCK> inA3(inA.row(2));
  vector_ref<float, CHUCK> inA4(inA.row(3));
  vector_ref<float, CHUCK> inA5(inA.row(4));
  vector_ref<float, CHUCK> inA6(inA.row(5));
  vector_ref<float, CHUCK> inA7(inA.row(6));
  vector_ref<float, CHUCK> inA8(inA.row(7));

  inA1 = cm_load<float, CHUCK, DataSize::U32, CacheHint::Cached, CacheHint::Cached>(
      input, (gidX * NUM_ELEM_PER_THREAD + 0 * CHUCK) * sizeof(float));
  inA2 = cm_load<float, CHUCK, DataSize::U32, CacheHint::Cached, CacheHint::Cached>(
      input, (gidX * NUM_ELEM_PER_THREAD + 1 * CHUCK) * sizeof(float));
  inA3 = cm_load<float, CHUCK, DataSize::U32, CacheHint::Cached, CacheHint::Cached>(
      input, (gidX * NUM_ELEM_PER_THREAD + 2 * CHUCK) * sizeof(float));
  inA4 = cm_load<float, CHUCK, DataSize::U32, CacheHint::Cached, CacheHint::Cached>(
      input, (gidX * NUM_ELEM_PER_THREAD + 3 * CHUCK) * sizeof(float));
  inA5 = cm_load<float, CHUCK, DataSize::U32, CacheHint::Cached, CacheHint::Cached>(
      input, (gidX * NUM_ELEM_PER_THREAD + 4 * CHUCK) * sizeof(float));
  inA6 = cm_load<float, CHUCK, DataSize::U32, CacheHint::Cached, CacheHint::Cached>(
      input, (gidX * NUM_ELEM_PER_THREAD + 5 * CHUCK) * sizeof(float));
  inA7 = cm_load<float, CHUCK, DataSize::U32, CacheHint::Cached, CacheHint::Cached>(
      input, (gidX * NUM_ELEM_PER_THREAD + 6 * CHUCK) * sizeof(float));
  inA8 = cm_load<float, CHUCK, DataSize::U32, CacheHint::Cached, CacheHint::Cached>(
      input, (gidX * NUM_ELEM_PER_THREAD + 7 * CHUCK) * sizeof(float));

  vector<float, 1> temp = 0;
  for (int i = 0; i < NUM_CHUCK; i++) {
    for (int j = 0; j < CHUCK; j++) {
      if (inA(i, j) > temp(0)) {
        temp(0) = inA(i, j);
      }
    }
  }
  cm_store<float, 1, DataSize::U32, CacheHint::Uncached, CacheHint::WriteBack>(output, gidX * sizeof(float), temp);
}