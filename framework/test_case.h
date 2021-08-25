//
// Created by cxh on 2021/8/25.
//

#ifndef HOMEWORK_FRAMEWORK_TEST_CASE_H_
#define HOMEWORK_FRAMEWORK_TEST_CASE_H_

#include <string>

class TestCase {
 public:
  virtual ~TestCase() = default;
  virtual std::string get_test_name() = 0;
  virtual void run() = 0;
};

#endif  // HOMEWORK_FRAMEWORK_TEST_CASE_H_
