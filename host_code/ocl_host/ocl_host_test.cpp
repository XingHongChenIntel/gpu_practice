//
// Created by cxh on 2021/8/5.
//
#include <CL/opencl.h>

#include "register_case.h"
#include "test_case.h"

class OclTest : TestCase {
  std::string get_test_name();
  void run();
};

std::string OclTest::get_test_name() { return "ocl_host_test"; }

void OclTest::run() {
  // Setup
  QueueProperties queueProperties =
      QueueProperties::create().setProfiling(arguments.useProfiling).setOoq(arguments.useOoq);
  Opencl opencl(queueProperties);
  cl_int retVal{};
  Timer timer;

  // Get parameters for the enqueue call
  cl_event event{};
  cl_event *eventForNdr = arguments.useEvent ? &event : nullptr;
  const size_t gws = arguments.gws;

  // Create kernel
  const char *source = "__kernel void empty() {}";
  const auto sourceLength = strlen(source);
  cl_program program = clCreateProgramWithSource(opencl.context, 1, &source, &sourceLength, &retVal);
  ASSERT_CL_SUCCESS(retVal);
  ASSERT_CL_SUCCESS(clBuildProgram(program, 1, &opencl.device, nullptr, nullptr, nullptr));
  cl_kernel kernel = clCreateKernel(program, "empty", &retVal);
  ASSERT_CL_SUCCESS(retVal);

  // Warmup, kernel
  ASSERT_CL_SUCCESS(
      clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, nullptr, 0, nullptr, eventForNdr));
  ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));
  if (eventForNdr) {
    ASSERT_CL_SUCCESS(clReleaseEvent(event));
  }

  // Benchmark
  for (int i = 0; i < arguments.iterations; i++) {
    timer.measureStart();
    ASSERT_CL_SUCCESS(
        clEnqueueNDRangeKernel(opencl.commandQueue, kernel, 1, nullptr, &gws, nullptr, 0, nullptr, eventForNdr));
    timer.measureEnd();
    ASSERT_CL_SUCCESS(clFinish(opencl.commandQueue));
    statistics.pushValue(timer.get());
    if (eventForNdr) {
      ASSERT_CL_SUCCESS(clReleaseEvent(event));
    }
  }

  // Cleanup
  ASSERT_CL_SUCCESS(clReleaseKernel(kernel));
  ASSERT_CL_SUCCESS(clReleaseProgram(program));
  return TestResult::Success;
}

static const inline RegisterTestCase<OclTest> oclTest{};