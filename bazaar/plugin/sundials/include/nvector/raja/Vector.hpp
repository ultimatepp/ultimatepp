/*
 * -----------------------------------------------------------------
 * Programmer(s): Slaven Peles @ LLNL
 * -----------------------------------------------------------------
 * SUNDIALS Copyright Start
 * Copyright (c) 2002-2020, Lawrence Livermore National Security
 * and Southern Methodist University.
 * All rights reserved.
 *
 * See the top-level LICENSE and NOTICE files for details.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * SUNDIALS Copyright End
 * -----------------------------------------------------------------
 */


/*
 * Vector class
 *
 * Manages vector data layout for RAJA implementation of N_Vector.
 *
 */

#ifndef _NVECTOR_RAJA_HPP_
#define _NVECTOR_RAJA_HPP_

#include <cstdlib>
#include <iostream>

#include <sundials/sundials_config.h>
#include <nvector/nvector_raja.h>

namespace sunrajavec
{

template <typename T, typename I>
class Vector : public _N_VectorContent_Raja
{
public:
  Vector(I N)
  : size_(N),
    mem_size_(N*sizeof(T))
  {
    allocate();
  }

  // Copy constructor does not copy values
  explicit Vector(const Vector& v)
  : size_(v.size()),
    mem_size_(size_*sizeof(T))
  {
    allocate();
  }

  ~Vector()
  {
    cudaError_t err;
    free(h_vec_);
    err = cudaFree(d_vec_);
    if(err != cudaSuccess)
      std::cout << "Failed to free device vector (error code " << err << ")!\n";
  }


  void allocate()
  {
    cudaError_t err;
    h_vec_ = static_cast<T*>(malloc(mem_size_));
    if(h_vec_ == NULL)
      std::cout << "Failed to allocate host vector!\n";
    err = cudaMalloc((void**) &d_vec_, mem_size_);
    if(err != cudaSuccess)
      std::cout << "Failed to allocate device vector (error code " << err << ")!\n";
  }

  int size() const
  {
    return size_;
  }

  T* host()
  {
    return h_vec_;
  }

  const T* host() const
  {
    return h_vec_;
  }

  T* device()
  {
    return d_vec_;
  }

  const T* device() const
  {
    return d_vec_;
  }

  void copyToDev()
  {
    cudaError_t err = cudaMemcpy(d_vec_, h_vec_, mem_size_, cudaMemcpyHostToDevice);
    if(err != cudaSuccess)
      std::cerr << "Failed to copy vector from host to device (error code " << err << ")!\n";
  }

  void copyFromDev()
  {
    cudaError_t err = cudaMemcpy(h_vec_, d_vec_, mem_size_, cudaMemcpyDeviceToHost);
    if(err != cudaSuccess)
      std::cerr << "Failed to copy vector from device to host (error code " << err << ")!\n";
  }

private:
  I size_;
  I mem_size_;
  T* h_vec_;
  T* d_vec_;
};


} // namespace sunrajavec



#endif // _NVECTOR_RAJA_HPP_
