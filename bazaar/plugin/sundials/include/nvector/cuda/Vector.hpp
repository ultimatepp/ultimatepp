/*
 * -----------------------------------------------------------------
 * Programmer(s): Slaven Peles, and Cody J. Balos @ LLNL
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
 * Manages vector data layout for CUDA implementation of N_Vector.
 *
 */

#ifndef _NVECTOR_CUDA_HPP_
#define _NVECTOR_CUDA_HPP_

#include <cstdlib>
#include <iostream>

#include <cuda_runtime.h>
#include "ThreadPartitioning.hpp"

#include <nvector/nvector_cuda.h>
#include <sundials/sundials_config.h>

namespace suncudavec
{

template <typename T, typename I>
class Vector : public _N_VectorContent_Cuda
{

public:
  Vector(I N,
         bool use_managed_memory = false, bool allocate_data = true,
         T* const h_vec = nullptr, T* const d_vec = nullptr)
  : size_(N),
    mem_size_(N*sizeof(T)),
    ownPartitioning_(true),
    ownData_(allocate_data),
    managed_mem_(use_managed_memory),
    allocfn_(nullptr),
    freefn_(nullptr),
    h_vec_(h_vec),
    d_vec_(d_vec)
  {
    // Set partitioning
    partStream_ = new StreamPartitioning<T, I>(N, 256);
    partReduce_ = new ReducePartitioning<T, I>(N, 256);

    // Allocate data arrays
    if (allocate_data)
      allocate();
  }

  Vector(I N, cudaStream_t stream,
         bool use_managed_memory = false, bool allocate_data = true,
         T* const h_vec = nullptr, T* const d_vec = nullptr)
  : size_(N),
    mem_size_(N*sizeof(T)),
    ownPartitioning_(true),
    ownData_(allocate_data),
    managed_mem_(use_managed_memory),
    allocfn_(nullptr),
    freefn_(nullptr),
    h_vec_(h_vec),
    d_vec_(d_vec)
  {
    // Set partitioning
    partStream_ = new StreamPartitioning<T, I>(N, 256, stream);
    partReduce_ = new ReducePartitioning<T, I>(N, 256, stream);

    // Allocate data arrays
    if (allocate_data)
      allocate();
  }

  Vector(I N,
         SUNAllocFn allocfn, SUNFreeFn freefn,
         bool allocate_data = true)
  : size_(N),
    mem_size_(N*sizeof(T)),
    ownPartitioning_(true),
    ownData_(allocate_data),
    managed_mem_(true),
    allocfn_(allocfn),
    freefn_(freefn),
    h_vec_(nullptr),
    d_vec_(nullptr)
  {
    // Set partitioning
    partStream_ = new StreamPartitioning<T, I>(N, 256);
    partReduce_ = new ReducePartitioning<T, I>(N, 256, allocfn, freefn);

    // Allocate data arrays
    if (allocate_data)
      allocate();
  }

  Vector(I N, cudaStream_t stream,
         SUNAllocFn allocfn, SUNFreeFn freefn,
         bool allocate_data = true)
  : size_(N),
    mem_size_(N*sizeof(T)),
    ownPartitioning_(true),
    ownData_(allocate_data),
    managed_mem_(true),
    allocfn_(allocfn),
    freefn_(freefn),
    h_vec_(nullptr),
    d_vec_(nullptr)
  {
    // Set partitioning
    partStream_ = new StreamPartitioning<T, I>(N, 256, stream);
    partReduce_ = new ReducePartitioning<T, I>(N, 256, stream, allocfn, freefn);

    // Allocate data arrays
    if (allocate_data)
      allocate();
  }

  // Copy constructor does not copy data array values
  explicit Vector(const Vector& v)
  : size_(v.size()),
    mem_size_(size_*sizeof(T)),
    partStream_(v.partStream_),
    partReduce_(v.partReduce_),
    ownPartitioning_(false),
    ownData_(true),
    managed_mem_(v.managed_mem_),
    allocfn_(v.allocfn_),
    freefn_(v.freefn_),
    h_vec_(nullptr),
    d_vec_(nullptr)
  {
    allocate();
  }

  ~Vector()
  {
    cudaError_t err;

    if (ownPartitioning_) {
      delete partReduce_;
      delete partStream_;
    }

    if (ownData_) {
      if (freefn_) {
        freefn_(d_vec_);
        d_vec_ = nullptr;
        h_vec_ = nullptr;
      } else {
        if (!managed_mem_)
          free(h_vec_);
        err = cudaFree(d_vec_);
        if(err != cudaSuccess)
          std::cerr << "Failed to free device vector "
                    << "in suncudavec::Vector::~Vector "
                    << "(error code " << err << ")\n";
        d_vec_ = nullptr;
        h_vec_ = nullptr;
      }
    }
  }

  void allocate()
  {
    if (allocfn_) {
      allocateCustom();
    } else if (managed_mem_) {
      allocateManaged();
    } else {
      allocateUnmanaged();
    }
  }

  void allocateManaged()
  {
    cudaError_t err;
    err = cudaMallocManaged((void**) &d_vec_, mem_size_);
    if (err != cudaSuccess)
      std::cerr << "Failed to allocate managed vector "
                << "in suncudavec::Vector::allocateManaged "
                << "(error code " << err << ")\n";
    h_vec_ = d_vec_;
  }

  void allocateUnmanaged()
  {
    cudaError_t err;

    h_vec_ = static_cast<T*>(malloc(mem_size_));
    if(h_vec_ == nullptr)
      std::cerr << "Failed to allocate host vector "
                << "in suncudavec::Vector::allocateUnmanaged\n";

    err = cudaMalloc((void**) &d_vec_, mem_size_);
    if(err != cudaSuccess)
      std::cerr << "Failed to allocate device vector "
                << "in suncudavec::Vector::allocateUnmanaged "
                << "(error code " << err << ")\n";
  }

  void allocateCustom()
  {
    /* We assume managed memory when a custom allocator is provided */
    d_vec_ = (realtype *) allocfn_(mem_size_);
    if (d_vec_ == nullptr)
      std::cerr << "Failed to allocate vector with user-provied allocator "
                << "in suncudavec::Vector::allocateCustom()\n";
    h_vec_ = d_vec_;
  }

  int size() const
  {
    return size_;
  }

  T* host()
  {
    // If the vector is using managed memory, and a user
    // is accessing a data array, then we need to synchronzie
    // to ensure all kernels have completed since a memcpy
    // won't have to happen.
    if (managed_mem_)
      cudaStreamSynchronize(partReduce_->stream());
    return h_vec_;
  }

  const T* host() const
  {
    // If the vector is using managed memory, and a user
    // is accessing a data array, then we need to synchronzie
    // to ensure all kernels have completed since a memcpy
    // won't have to happen.
    if (managed_mem_)
      cudaStreamSynchronize(partReduce_->stream());
    return h_vec_;
  }

  T* device()
  {
    // If the vector is using managed memory, and a user
    // is accessing a data array, then we need to synchronzie
    // to ensure all kernels have completed since a memcpy
    // won't have to happen.
    if (managed_mem_)
      cudaStreamSynchronize(partReduce_->stream());
    return d_vec_;
  }

  const T* device() const
  {
    // If the vector is using managed memory, and a user
    // is accessing a data array, then we need to synchronzie
    // to ensure all kernels have completed since a memcpy
    // won't have to happen.
    if (managed_mem_)
      cudaStreamSynchronize(partReduce_->stream());
    return d_vec_;
  }

  bool isManaged() const
  {
    return managed_mem_;
  }

  void copyToDev()
  {
    cudaError_t err;

    /* If the host and device pointers are the same, then we don't need
       to do a copy (this happens in the managed memory case), but we
       still need to synchronize the device to adhere to the unified
       memory access rules. */
    if (h_vec_ == d_vec_) {
      err = cudaStreamSynchronize(partReduce_->stream());
      if(err != cudaSuccess)
        std::cerr << "Failed to synchronize stream in "
                  << "suncudavec::Vector::copyToDev "
                  << "(error code " << err << ")\n";
    } else {
      err = cudaMemcpyAsync(d_vec_, h_vec_, mem_size_, cudaMemcpyHostToDevice,
                            partReduce_->stream());
      if(err != cudaSuccess)
        std::cerr << "Failed to copy vector from host to device in "
                  << "suncudavec::Vector::copyToDev "
                  << "(error code " << err << ")\n";
    }
  }

  void copyFromDev()
  {
    cudaError_t err;

    /* If the host and device pointers are the same, then we don't need
       to do a copy (this happens in the managed memory case), but we
       still need to synchronize the device to adhere to the unified
       memory access rules. */
    if (h_vec_ == d_vec_) {
      err = cudaStreamSynchronize(partReduce_->stream());
      if(err != cudaSuccess)
        std::cerr << "Failed to synchronize stream in "
                  << "suncudavec::Vector::copyFromDev "
                  << "(error code " << err << ")\n";
    } else {
      err = cudaMemcpyAsync(h_vec_, d_vec_, mem_size_, cudaMemcpyDeviceToHost,
                            partReduce_->stream());
      if(err != cudaSuccess)
        std::cerr << "Failed to copy vector from device to host in "
                  << "suncudavec::Vector::copyFromDev "
                  << "(error code " << err << ")\n";
    }
  }

  void setPartitioning(ThreadPartitioning<T, I>* stream, ThreadPartitioning<T, I>* reduce)
  {
     if (ownPartitioning_) {
       delete partStream_;
       delete partReduce_;
     }
    partStream_ = stream;
    partReduce_ = reduce;
    ownPartitioning_ = false;
  }

  ThreadPartitioning<T, I>& partStream() const
  {
    return *partStream_;
  }

  ThreadPartitioning<T, I>& partReduce() const
  {
    return *partReduce_;
  }


private:
  I size_;
  I mem_size_;
  T* h_vec_;
  T* d_vec_;
  ThreadPartitioning<T, I>* partStream_;
  ThreadPartitioning<T, I>* partReduce_;
  bool ownPartitioning_;
  bool ownData_;
  bool managed_mem_;
  SUNAllocFn allocfn_;
  SUNFreeFn freefn_;

};


} // namespace suncudavec




#endif // _NVECTOR_CUDA_HPP_
