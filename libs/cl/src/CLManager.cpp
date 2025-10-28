#include <vector>
#include <cassert>
#include <iostream>

#include "CLManager.h"

cl::CLManager::CLManager() {
  platform = nullptr;
  device = nullptr;
  context = nullptr;
}

void cl::CLManager::Init(const std::string& kernels_directory) {
  // Step 1: Available platform
  GetPlatformInfo();

  // Step 2: Set up the opencl device
  SetupDeviceOnPlatform(platform);

  // Step 3: Create the context
  SetupContext();

  // Step 4: Command queue
  SetupCommandQueue();

  // Step 5: Kernels
  kernel_manager.Init(context, device, kernels_directory);

  // Step 6: Memory buffers for our functions (kernels)
}

void cl::CLManager::SetupCommandQueue() {
  // Step 4: Command Queue (modern API)
  // cl_command_queue_properties properties[] = {0};  // No properties specified
  // cl_command_queue queue =
  //     clCreateCommandQueueWithProperties(context, device, properties, nullptr);
  queue = clCreateCommandQueue(context, device, 0, nullptr);
}

void cl::CLManager::FurtherLogic() {
  // CORRECT TILL HERE

  PrintInfo();

  // Step 5: Host data
  int n = 1024;
  std::vector<float> b(n, 2.0f);
  std::vector<float> c(n);

  // Step 6: Buffers
  cl_mem a_buf = MakeBuffer(n);
  cl_mem b_buf = MakeBuffer(n);
  cl_mem c_buf = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * n, nullptr, nullptr);

  // Step 7: Create kernel
  // SetupKernel("libs/kern/VectorAddition.cl", "VectorAdd");

  // Step 9: Set kernel arguments
  // clSetKernelArg(kernels[0], 0, sizeof(cl_mem), &a_buf);
  // clSetKernelArg(kernels[0], 1, sizeof(cl_mem), &b_buf);
  // clSetKernelArg(kernels[0], 2, sizeof(cl_mem), &c_buf);

  // Step 10: Enqueue kernel
  size_t global_work_size = n;
  clEnqueueNDRangeKernel(queue, kernels[0], 1, nullptr, &global_work_size, nullptr, 0, nullptr,
                         nullptr);

  // Step 11: Read result
  clEnqueueReadBuffer(queue, c_buf, CL_TRUE, 0, sizeof(float) * n, c.data(), 0, nullptr, nullptr);

  for (int i = 0; i < 10; ++i) std::cout << "c[" << i << "] = " << c[i] << std::endl;

  // Step 12: Cleanup
  clReleaseKernel(kernels[0]);
  clReleaseProgram(programs[0]);
  clReleaseMemObject(a_buf);
  clReleaseMemObject(b_buf);
  clReleaseMemObject(c_buf);
  clReleaseCommandQueue(queue);
  clReleaseContext(context);
}