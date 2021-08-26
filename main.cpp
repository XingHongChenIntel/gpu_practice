#include <test_case.h>
#include <test_map.h>

void print_help(){
  std::cout << "All workload which can run : " << std::endl;
  TestMap::get().print_All();
}

int main(int argc, char *argv[]) {
  auto &test_map = TestMap::get();
  // parse the config.
  for (int i = 1; i < argc; i++) {
    std::string testCase(argv[i]);
    TestCase *workload = test_map.find_case(testCase);
    if (workload) {
      workload->run();
    } else {
      print_help();
    }
  }
  return 0;
}
