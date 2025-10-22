#ifndef HOST_H
#define HOST_H

#include "CLManager.h"

namespace cl {
class Host {
 public:
  Host();
  void Start();

 private:
  CLManager cl_manager;
};

}  // namespace cl

#endif  // HOST_H