#ifndef HOST_H
#define HOST_H

#include "CLManager.h"

/*
In a typical host program, the programmer defines the context and the
command-queues, defines memory and program objects, and builds any
data structures needed on the host to support the application. Then the
focus shifts to the command-queue. Memory objects are moved from the
host onto the devices; kernel arguments are attached to memory objects
and then submitted to the command-queue for execution. When the ker-
nel has completed its work, memory objects produced in the computation
may be copied back onto the host. Page 19
*/

namespace cl {
class Host {
 public:
  Host();
  void Init(const std::string& kernels_directory);

 private:
  CLManager cl_manager;
};

}  // namespace cl

#endif  // HOST_H