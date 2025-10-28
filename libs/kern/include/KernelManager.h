#ifndef KERNEL_MANAGER_H
#define KERNEL_MANAGER_H

#include <string>
#include <map>

#define CL_TARGET_OPENCL_VERSION 300
#include <OpenCL/cl.h>

#include "Kernel.h"

namespace kern {

typedef std::tuple<std::string, std::string, int> KernelData;

class KernelManager {
 public:
  KernelManager() = default;
  ~KernelManager();

  void Init(cl_context context_, cl_device_id device_,
            const std::string& kernels_source_directory);
  void LoadKernelSources(const std::string& kernels_directory);
  void CreateKernels();

  template <typename T>
  void CreateMemory(std::vector<T>& memory_buffer);

  template <typename... Args>
  void KernelArguments(const std::string& kernel_name, const Args&... args);

  void Clear();

 private:
  std::vector<cl_program> programs;
  std::vector<Kernel> kernels;
  std::vector<KernelData> kernel_sources;

  cl_context context;
  cl_device_id device;
};

std::string LoadKernelFromFile(const std::string& file_name);

}  // namespace kern

#endif  // KERNEL_MANAGER_H