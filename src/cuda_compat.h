#pragma once

// This header provides CUDA compatibility macros and utilities
// to allow code to be compiled for both CPU and GPU
#ifndef CUDA_ENABLED
#define CUDA_ENABLED 0
#endif

#if CUDA_ENABLED
    #include <cuda_runtime.h>
    #define CUDA_HOST __host__
    #define CUDA_DEVICE __device__
    #define CUDA_HOST_DEVICE __host__ __device__
    #define CUDA_GLOBAL __global__
#else
    #define CUDA_HOST
    #define CUDA_DEVICE
    #define CUDA_HOST_DEVICE
    #define CUDA_GLOBAL
#endif


#if CUDA_ENABLED
inline void calculateGridSize(dim3 &grid, dim3 &block, int width, int height) {
    block = dim3(16, 16);
    grid = dim3((width + block.x - 1) / block.x, (height + block.y - 1) / block.y);
}
#endif
