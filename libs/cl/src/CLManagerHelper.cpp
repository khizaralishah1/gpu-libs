#include <iostream>
#include <cassert>

#include "CLManager.h"

void cl::CLManager::SetupDeviceOnPlatform(const cl_platform_id& platform_) {
  cl_uint num_devices;
  cl_int err = clGetDeviceIDs(platform_, CL_DEVICE_TYPE_GPU, 1, &device, &num_devices);
  if (err != CL_SUCCESS || num_devices == 0) {
    std::cerr << "[cl::CLManager::SetupDevice] No OpenCL GPU device found. Error: " << err
              << std::endl;
    assert(false);
  }
}

void cl::CLManager::GetPlatformInfo() {
  cl_uint num_platforms;
  cl_int err = clGetPlatformIDs(1, &platform, &num_platforms);
  if (err != CL_SUCCESS || num_platforms == 0) {
    std::cerr << "[cl::CLManager::GetPlatformInfo] No OpenCL platform found. Error: " << err
              << std::endl;
    assert(false);
  }
}

void cl::CLManager::SetupContext() {
  cl_context_properties properties[] = {CL_CONTEXT_PLATFORM, (cl_context_properties)platform, 0};
  context = clCreateContext(properties, 1, &device, nullptr, nullptr, nullptr);
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