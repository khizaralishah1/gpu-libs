#include <fstream>
#include <sstream>
#include <cassert>
#include <iostream>
#include <filesystem>

#include "Kernel.h"

std::string kern::LoadKernelFromFile(const std::string& file_name) {
  std::ifstream file(file_name);
  if (!file.is_open()) {
    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
    std::cout << "[kern::LoadKernelFromFile] Failed to open kernel file: " << file_name
              << std::endl;
    assert(false && "Kernel file could not be opened!");
  }
  std::stringstream ss;
  ss << file.rdbuf();
  return ss.str();
}

cl_program kern::BuildProgram(const std::string& file_source, cl_context context,
                              cl_device_id device) {
  // Step 1: Load the kernel
  std::string kernel_source = kern::LoadKernelFromFile(file_source);
  const char* source_str = kernel_source.c_str();
  size_t source_size = kernel_source.size();

  // Step 2: Build the program
  cl_int err;
  cl_program program = clCreateProgramWithSource(context, 1, &source_str, &source_size, &err);
  if (err != CL_SUCCESS) {
    std::cerr << "[kern::BuildProgram] clCreateProgramWithSource failed, error code: " << err
              << std::endl;
    assert(false);
  }
  err = clBuildProgram(program, 1, &device, nullptr, nullptr, nullptr);

  return program;
}

cl_kernel kern::CreateKernelFromProgram(cl_program& program, std::string kernel_name) {
  cl_int err;
  const char* kernel_name_ = kernel_name.c_str();
  cl_kernel kernel = clCreateKernel(program, kernel_name_, &err);
  assert(err == CL_SUCCESS && "[kern::CreateKernelFromProgram] Program has an error!");
  return kernel;
}