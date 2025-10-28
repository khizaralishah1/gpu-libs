#include "Host.h"

cl::Host::Host() {}

void cl::Host::Init(const std::string& kernels_directory) {
  // 1. Find opencl device, set up context, load kernels
  cl_manager.Init(kernels_directory);
}
