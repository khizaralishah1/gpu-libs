#include "CLManager.h"

cl_mem cl::CLManager::MakeBuffer(int size) {
  std::vector<float> buffer(size, 1.0f);
  return clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * size,
                        buffer.data(), nullptr);
}

void cl::CLManager::SetupKernel(std::string kernel_filename, std::string kernel_name) {
  programs.push_back(kern::BuildProgram(kernel_filename, context, device));
  kernels.push_back(kern::CreateKernelFromProgram(programs[0], kernel_name));
}