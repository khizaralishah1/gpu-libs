#ifndef CL_MANAGER_H
#define CL_MANAGER_H

#include <vector>

#include "KernelManager.h"

namespace cl {

class CLManager {
 public:
  CLManager();
  void Init(const std::string& kernels_directory);

  // Logic
  void FurtherLogic();  // TODO: set this up properly
  cl_mem MakeBuffer(int size);
  void SetupKernel(std::string kernel_filename, std::string kernel_name);
  void SetupCommandQueue();

  // Debugging
  void PrintInfo();

 private:
  kern::KernelManager kernel_manager;

  // CONTEXT
  void GetPlatformInfo();
  void SetupDeviceOnPlatform(const cl_platform_id& platform);
  void SetupContext();
  cl_platform_id platform;
  cl_device_id device;
  cl_context context;

  // COMMAND QUEUE (commands: host -> opencl device)
  cl_command_queue queue;

  std::vector<cl_program> programs;
  std::vector<cl_kernel> kernels;
};

}  // namespace cl

#endif  // CL_MANAGER_H