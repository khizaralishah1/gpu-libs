#ifndef KERNEL_H
#define KERNEL_H

#include <string>

namespace kern {

class Kernel {
 public:
  inline Kernel(std::string file_name, std::string kernel_source) {
    name = file_name;
    source = kernel_source;
    size = source.size();
    kernel = nullptr;
  }

  inline bool Initialized() { return kernel; }

  inline std::string GetName() { return name; }
  inline std::string GetSource() { return source; }
  inline int GetSize() { return size; }

 private:
  std::string name;
  std::string source;
  int size;
  cl_kernel kernel;
};

}  // namespace kern

#endif  // KERNEL_H