#include <fstream>
#include <sstream>
#include <cassert>
#include <iostream>
#include <filesystem>
#include <utility>

#include "KernelManager.h"

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
  file.close();
  return ss.str();
}

void kern::KernelManager::LoadKernelSources(const std::string& kernels_directory) {
  std::cout << "[kern::KernelManagerLoadKernelSources] Directory: " << kernels_directory
            << std::endl;
  for (const auto& kernel_file : std::filesystem::directory_iterator(kernels_directory)) {
    if (kernel_file.is_regular_file() && kernel_file.path().extension() == ".cl") {
      std::string file_name = kernel_file.path().stem().string();  // kernel name without extension
      std::string kernel_source = LoadKernelFromFile(kernel_file.path());
      Kernel kernel(file_name, kernel_source);
      kernels.push_back(kernel);
      std::cout << "[kern::KernelManager::LoadKernels] Loaded kernel: " << file_name << std::endl;
    }
  }

  if (kernel_sources.empty()) {
    std::cerr << "[kern::KernelManager::LoadKernels] Warning: No .cl kernels found in directory "
              << kernels_directory << std::endl;
  }
}

void kern::KernelManager::CreateKernels() {
  for (const auto& kernel_ : kernels) {
    // 1. Create program from the kernel source
    cl_int err;
    std::string kernel_name = kernel_.GetName();
    const char* kernel_source_ptr = kernel_.GetSource().c_str();
    const size_t kernel_size_ptr = kernel_.GetSize();
    cl_program program =
        clCreateProgramWithSource(context, 1, &kernel_source_ptr, &kernel_size_ptr, &err);
    if (err != CL_SUCCESS) {
      std::cerr
          << "[kern::KernelManager::CreateKernels] clCreateProgramWithSource failed, error code: "
          << err << std::endl;
      return;
    }

    // 2. Build the program
    err = clBuildProgram(program, 0, nullptr, nullptr, nullptr, nullptr);
    if (err != CL_SUCCESS) {
      std::cerr << "[kern::KernelManager::CreateKernels] clBuildProgram failed, error code: "
                << err << std::endl;
      return;  // TODO: be safe
    }

    // 3. Create the kernel (function) from this program
    cl_kernel kernel = clCreateKernel(program, kernel_name.c_str(), &err);
    if (err != CL_SUCCESS) {
      std::cerr << "[kern::KernelManager::CreateKernels] Kernel name: " << kernel_name.c_str()
                << std::endl;
      std::cerr << "[kern::KernelManager::CreateKernels] Program has an error! code: " << err
                << std::endl;
      if (err == CL_INVALID_KERNEL_NAME) {
        std::cerr << "[kern::KernelManager::CreateKernels] IMPORTANT: Kernel function name must "
                     "be same as that of file name."
                  << std::endl;
      }
      return;
    }

    // 3. Store kernel
    kernels[kernel_name] = kernel;
    programs.push_back(program);
  }
}

void kern::KernelManager::Init(cl_context context_, cl_device_id device_,
                               const std::string& kernels_source_directory) {
  // Step 0:
  context = context_;
  device = device_;

  // Step 1: Load the kernels
  LoadKernelSources(kernels_source_directory);

  // Step 2: Build the program for each kernel source
  CreateKernels();
}

template <typename... Args>
void KernelArguments(const std::string& kernel_name, const Args&... args) {
  cl_kernel& kernel = kernels[kernel_name];
  SetKernelArgs(kernel, 0, args...);
}

// Recursive helper to set each argument
template <typename T, typename... Rest>
void SetKernelArgs(cl::Kernel& kernel, int index, const T& arg, const Rest&... rest) {
  kernel.setArg(index, arg);
  SetKernelArgs(kernel, index + 1, rest...);
}

// Base case (end of recursion)
inline void SetKernelArgs(cl::Kernel&, int) {}

// Main helper: takes kernel name, kernel object, and any number of args
template <typename... Args>
void KernelArguments(const std::string& kernel_name, cl::Kernel& kernel, const Args&... args) {
  std::cout << "Setting arguments for kernel: " << kernel_name << std::endl;
  SetKernelArgs(kernel, 0, args...);
}

template <typename T>
void kern::KernelManager::CreateMemory(std::vector<T>& memory_buffer) {
  cl_int err;
  clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                 sizeof(T) * memory_buffer.size(), memory_buffer.data(), &err);
  if (err != CL_SUCCESS) {
    std::cout << "[kern::KernelManager::CreateMemory] Failed to create memory" << std::endl;
  }
}

void kern::KernelManager::Clear() {
  // Step 1. Release the programs
  for (auto& kernel : kernels) clReleaseKernel(kernel);
  for (auto& program : programs) clReleaseProgram(program);
  kernels.clear();
  kernel_sources.clear();
  context = nullptr;
  device = nullptr;
}

kern::KernelManager::~KernelManager() { Clear(); }