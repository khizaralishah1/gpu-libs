#ifndef CL_MANAGER_H
#define CL_MANAGER_H

#include <vector>

#include "Kernel.h"

namespace cl {

class CLManager {
 public:
  CLManager();
  void Start();
  
  // Logic
  cl_mem MakeBuffer(int size);
  void SetupKernel(std::string kernel_filename, std::string kernel_name);

  // Debugging
  void PrintInfo();



 private:
  void SetupPlatform();
  void SetupDevice();
  void SetupContext();

  cl_platform_id platform;
  cl_device_id device;
  cl_context context;

  std::vector<cl_program> programs;
  std::vector<cl_kernel> kernels;
};

}  // namespace cl

#endif  // CL_MANAGER_H