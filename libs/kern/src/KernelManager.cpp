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
  std::cout << "[kern::KernelManagerLoadKernelSources] Directory: " << kernels_directory << std::endl;
  for (const auto& kernel_file : std::filesystem::directory_iterator(kernels_directory)) {
    if (kernel_file.is_regular_file() && kernel_file.path().extension() == ".cl") {
      std::string file_name = kernel_file.path().stem().string();  // kernel name without extension
      std::string kernel_source = LoadKernelFromFile(kernel_file.path());
      size_t size = kernel_source.size();
      KernelData kernel_data =
          std::make_pair(kernel_source.c_str(), static_cast<const size_t*>(&size));
      kernel_sources[file_name] = kernel_data;
      std::cout << "[kern::KernelManager::LoadKernels] Loaded kernel: " << file_name << std::endl;
    }
  }

  if (kernel_sources.empty()) {
    std::cerr << "[kern::KernelManager::LoadKernels] Warning: No .cl kernels found in directory "
              << kernels_directory << std::endl;
  }
}

void kern::KernelManager::CreateKernels() {
  for (const auto& kernel_source : kernel_sources) {
    // 1. Create program from the kernel source
    cl_int err;
    KernelData kernel_data = kernel_source.second;
    cl_program program =
        clCreateProgramWithSource(context, 1, &kernel_data.first, kernel_data.second, &err);
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
    const char* kernel_name = kernel_source.first.c_str();
    cl_kernel kernel = clCreateKernel(program, kernel_name, &err);
    assert(err == CL_SUCCESS && "[kern::KernelManager::CreateKernels] Program has an error!\r\n");

    // 3. Store kernel
    kernels.push_back(kernel);
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

void kern::KernelManager::Clear() {
  // Step 1. Release the programs
  for (auto& program : programs) clReleaseProgram(program);
  kernels.clear();
  kernel_sources.clear();
  context = nullptr;
  device = nullptr;
}

kern::KernelManager::~KernelManager() { Clear(); }