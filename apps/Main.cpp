#include <iostream>

#include "Host.h"

int main() {
  std::string kernels_directory = CMAKE_SOURCE_DIRECTORY;
  kernels_directory = kernels_directory + "/libs/kern/resources";

  cl::Host host;

  // Initialize opencl context, device and setup stuff between
  // host and opencl device
  host.Init(kernels_directory);

  return 0;
}