#ifndef KERNEL_H
#define KERNEL_H

#include <string>

#define CL_TARGET_OPENCL_VERSION 300
#include <OpenCL/cl.h>

namespace kern {

std::string LoadKernelFromFile(const std::string& file_name);
cl_program BuildProgram(const std::string& file_source, cl_context context, cl_device_id device);
cl_kernel CreateKernelFromProgram(cl_program& program, std::string kernel_name);

class Kernel {
 public:
  Kernel() = default;

 private:
};

}  // namespace kern

#endif  // KERNEL_H