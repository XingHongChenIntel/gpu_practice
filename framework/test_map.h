//
// Created by cxh on 2021/8/25.
//

#ifndef HOMEWORK_FRAMEWORK_TEST_MAP_H_
#define HOMEWORK_FRAMEWORK_TEST_MAP_H_
#include <iostream>
#include <map>
#include <memory>

#include "test_case.h"
class TestMap {
 public:
  static TestMap &get() {
    static TestMap case_map;
    return case_map;
  }
  TestCase *find_case(const std::string &name) {
    auto it = test_map.find(name);
    if (it == test_map.end()) {
      std::cerr << "Unknow test case, " << name << " didn't register !" << std::endl;
      return nullptr;
    }
    return it->second.get();
  }

  void set_case(const std::string &name, std::unique_ptr<TestCase> &test_case) {
    test_map[name] = std::move(test_case);
  }

  struct LexicographicalLess {
    bool operator()(const std::string &left, const std::string &right) const { return left < right; }
  };

 private:
  TestMap(){};
  std::map<std::string, std::unique_ptr<TestCase>, LexicographicalLess> test_map;
};

#endif  // HOMEWORK_FRAMEWORK_TEST_MAP_H_
