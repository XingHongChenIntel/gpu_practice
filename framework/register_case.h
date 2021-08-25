//
// Created by cxh on 2021/8/25.
//

#ifndef GPU_PRACTICE_FRAMEWORK_REGISTER_CASE_H_
#define GPU_PRACTICE_FRAMEWORK_REGISTER_CASE_H_

#include <iostream>

#include "test_case.h"
#include "test_map.h"
template <typename T>
class RegisterTestCase {
 public:
  explicit RegisterTestCase() {
    auto& my_map = TestMap::get();
    auto test_case = std::unique_ptr<TestCase>(new T());
    auto name = test_case->get_test_name();
    if (my_map.find_case(name)) {
      std::cerr << "[error] " << name << " case is duplicate in name!" << std::endl;
    } else {
      my_map.set_case(name, test_case);
    }
  }
};

#endif  // GPU_PRACTICE_FRAMEWORK_REGISTER_CASE_H_
