#ifndef KERNEL_H
#define KERNEL_H

#include <string>
#include <map>

#define CL_TARGET_OPENCL_VERSION 300
#include <OpenCL/cl.h>

namespace kern {

typedef std::pair<const char*, const size_t*> KernelData;

class KernelManager {
 public:
  KernelManager() = default;
  ~KernelManager();

  void Init(cl_context context_, cl_device_id device_,
            const std::string& kernels_source_directory);
  void LoadKernelSources(const std::string& kernels_directory);
  void CreateKernels();

  void Clear();

 private:
  std::vector<cl_program> programs;
  std::vector<cl_kernel> kernels;
  std::map<std::string, KernelData> kernel_sources;

  cl_context context;
  cl_device_id device;
};

std::string LoadKernelFromFile(const std::string& file_name);

}  // namespace kern

#endif  // KERNEL_H