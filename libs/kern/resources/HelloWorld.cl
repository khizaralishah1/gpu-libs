// NOTE: Use OpenCL VS Code extension for better visuals
// IMPORTANT: Kernel function name must be same as that of file name
__kernel void HelloWorld(__global const float* a, __global const float* b,
                          __global float* result) {
  int global_idx = get_global_id(0);  // Where does it come from?
  result[global_idx] = a[global_idx] + b[global_idx];
}