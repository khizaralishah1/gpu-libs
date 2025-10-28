#include "CLManager.h"

cl_mem cl::CLManager::MakeBuffer(int size) {
  std::vector<float> buffer(size, 1.0f);
  return clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * size,
                        buffer.data(), nullptr);
}
