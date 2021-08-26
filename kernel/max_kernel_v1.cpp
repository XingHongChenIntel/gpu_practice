//
// Created by cxh on 2021/8/16.
//
#include <cm/cm.h>

#define CHUCK 64
#define NUM_ELEM_PER_GROUP 512
#define NUM_CHUCK NUM_ELEM_PER_THREAD / CHUCK

extern "C" _GENX_MAIN_ void empty(SurfaceIndex input [[type("buffer_t float")]],
                                  SurfaceIndex output [[type("buffer_t float")]]) {
  uint gidX = cm_group_id(0);
  uint lszX = cm_local_size(0);
  uint tidX = cm_local_id(0);
  uint gctX = cm_group_count(0);

  cm_nbarrier_init(NUM_ELEM_PER_GROUP / CHUCK);
  cm_slm_init(16 * sizeof(float));
  uint slm_buf = cm_slm_alloc(8 * sizeof(float));

  vector<float, CHUCK> inA;
  inA = cm_load<float, CHUCK, DataSize::U32, CacheHint::Cached, CacheHint::Cached>(
      input, (gidX * NUM_ELEM_PER_GROUP + tidX * CHUCK) * sizeof(float));

  vector<float, 1> temp = 0;
  for (int j = 0; j < CHUCK; j++) {
    if (inA(j) > temp(0)) {
      temp(0) = inA(j);
    }
  }
  cm_slm_write(slm_buf, );

  cm_store<float, 1, DataSize::U32, CacheHint::Uncached, CacheHint::WriteBack>(output, gidX * sizeof(float), temp);
}