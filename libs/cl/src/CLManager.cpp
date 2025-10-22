#include <vector>
#include <cassert>
#include <iostream>

#include "CLManager.h"

void cl::CLManager::Start() {
  // Step 1: Platform
  SetupPlatform();

  // Step 2: Device
  SetupDevice();

  // Step 3: Context
  SetupContext();

  PrintInfo();

  // Step 4: Command Queue (modern API)
  // cl_command_queue_properties properties[] = {0};  // No properties specified
  // cl_command_queue queue =
  //     clCreateCommandQueueWithProperties(context, device, properties, nullptr);
  cl_command_queue queue = clCreateCommandQueue(context, device, 0, nullptr);

  // Step 5: Host data
  int n = 1024;
  std::vector<float> b(n, 2.0f);
  std::vector<float> c(n);

  // Step 6: Buffers
  cl_mem a_buf = MakeBuffer(n);
  cl_mem b_buf = MakeBuffer(n);
  cl_mem c_buf = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * n, nullptr, nullptr);

  // Step 7: Create kernel
  SetupKernel("libs/kern/VectorAddition.cl", "VectorAdd");

  // Step 9: Set kernel arguments
  clSetKernelArg(kernels[0], 0, sizeof(cl_mem), &a_buf);
  clSetKernelArg(kernels[0], 1, sizeof(cl_mem), &b_buf);
  clSetKernelArg(kernels[0], 2, sizeof(cl_mem), &c_buf);

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

void cl::CLManager::SetupDevice() {
  cl_uint num_devices;
  cl_int err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, &num_devices);
  if (err != CL_SUCCESS || num_devices == 0) {
    std::cerr << "[cl::CLManager::SetupDevice] No OpenCL GPU device found. Error: " << err
              << std::endl;
    assert(false);
  }
}

void cl::CLManager::SetupPlatform() {
  cl_uint num_platforms;
  cl_int err = clGetPlatformIDs(1, &platform, &num_platforms);
  if (err != CL_SUCCESS || num_platforms == 0) {
    std::cerr << "[cl::CLManager::SetupPlatform] No OpenCL platform found. Error: " << err
              << std::endl;
    assert(false);
  }
}

void cl::CLManager::SetupContext() {
  cl_context_properties props[] = {CL_CONTEXT_PLATFORM, (cl_context_properties)platform, 0};
  context = clCreateContext(props, 1, &device, nullptr, nullptr, nullptr);
}

void cl::CLManager::PrintInfo() {
  char name[128];
  clGetPlatformInfo(platform, CL_PLATFORM_NAME, sizeof(name), name, nullptr);
  std::cout << "[cl::CLManager::PrintInfo] Platform: " << name << std::endl;

  clGetPlatformInfo(platform, CL_PLATFORM_VENDOR, sizeof(name), name, nullptr);
  std::cout << "[cl::CLManager::PrintInfo] Vendor: " << name << std::endl;

  clGetPlatformInfo(platform, CL_PLATFORM_VERSION, sizeof(name), name, nullptr);
  std::cout << "[cl::CLManager::PrintInfo] Platform Version: " << name << std::endl;

  clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(name), name, nullptr);
  std::cout << "[cl::CLManager::PrintInfo] Device: " << name << std::endl;

  clGetDeviceInfo(device, CL_DEVICE_VERSION, sizeof(name), name, nullptr);
  std::cout << "[cl::CLManager::PrintInfo] Device Version: " << name << std::endl;

  cl_uint compute_units;
  clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(compute_units), &compute_units,
                  nullptr);
  std::cout << "[cl::CLManager::PrintInfo] Compute Units: " << compute_units << std::endl;

  size_t global_mem;
  clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(global_mem), &global_mem, nullptr);
  std::cout << "[cl::CLManager::PrintInfo] Global Memory: " << global_mem / (1024 * 1024) << " MB"
            << std::endl;
}

cl::CLManager::CLManager() {
  platform = nullptr;
  device = nullptr;
  context = nullptr;
}