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



#ifndef _THREAD_PARTITIONING_HPP_
#define _THREAD_PARTITIONING_HPP_

#include <iostream>
#include <cuda_runtime.h>

#include <sundials/sundials_types.h>

namespace suncudavec
{
  
using SUNAllocFn = void* (*)(size_t);
using SUNFreeFn  = void (*)(void*);

template<class T, class I>
class ThreadPartitioning
{

public:
  ThreadPartitioning()
  : block_(1),
    grid_(1),
    shMemSize_(0),
    stream_(0),
    bufferSize_(0),
    allocfn_(nullptr),
    freefn_(nullptr),
    d_buffer_(nullptr),
    h_buffer_(nullptr),
    ownBuffer_(true)
  {}

  ThreadPartitioning(unsigned block,
                     SUNAllocFn allocfn = nullptr,
                     SUNFreeFn freefn = nullptr)
  : block_(block),
    grid_(1),
    shMemSize_(0),
    stream_(0),
    bufferSize_(0),
    allocfn_(allocfn),
    freefn_(freefn),
    d_buffer_(nullptr),
    h_buffer_(nullptr),
    ownBuffer_(true)
  {}
  
  explicit ThreadPartitioning(ThreadPartitioning<T, I>& p)
  : block_(p.block_),
    grid_(p.grid_),
    shMemSize_(p.shMemSize_),
    stream_(p.stream_),
    allocfn_(p.allocfn_),
    freefn_(p.freefn_)
  {}

  virtual ~ThreadPartitioning(){}

  unsigned grid() const
  {
    return grid_;
  }

  unsigned block() const
  {
    return block_;
  }

  unsigned shmem() const
  {
    return shMemSize_;
  }

  cudaStream_t stream() const
  {
    return stream_;
  }

  unsigned int bufferSize()
  {
    return bufferSize_;
  }

  T* devBuffer()
  {
    return d_buffer_;
  }

  const T* devBuffer() const
  {
    return d_buffer_;
  }

  T* hostBuffer()
  {
    return h_buffer_;
  }

  const T* hostBuffer() const
  {
    return h_buffer_;
  }

  void setStream(const cudaStream_t& stream)
  {
    stream_ = stream;
  }

  virtual void copyFromDevBuffer(unsigned int n) const
  {
    std::cerr << "Trying to copy buffer from base class in "
              << "suncudavec::ThreadPartitioning::copyFromDevBuffer\n";
  }

  /* pure virtual functions to get the relevant partitioning information */
  virtual int calcPartitioning(I N, unsigned& grid, unsigned& block, unsigned& shMemSize, cudaStream_t& stream) = 0;
  virtual int calcPartitioning(I N, unsigned& grid, unsigned& block, unsigned& shMemSize) = 0;
  
protected:
  unsigned block_;
  unsigned grid_;
  unsigned shMemSize_;
  unsigned bufferSize_;
  cudaStream_t stream_;
  T* d_buffer_;
  T* h_buffer_;
  bool ownBuffer_;

  /* custom allocators for the internal buffers */
  SUNAllocFn allocfn_;
  SUNFreeFn freefn_;

}; // class ThreadPartitioning



template<class T, class I>
class StreamPartitioning : public ThreadPartitioning<T, I>
{
  using ThreadPartitioning<T, I>::block_;
  using ThreadPartitioning<T, I>::grid_;
  using ThreadPartitioning<T, I>::stream_;

public:
  StreamPartitioning(I N, unsigned block, cudaStream_t stream)
  : ThreadPartitioning<T, I>(block)
  {
    grid_ = (N + block_ - 1) / block_;
    stream_ = stream;
  }
  
  StreamPartitioning(I N, unsigned block)
  : ThreadPartitioning<T, I>(block)
  {
    grid_ = (N + block_ - 1) / block_;
  }

  explicit StreamPartitioning(StreamPartitioning<T, I>& p)
  : ThreadPartitioning<T, I>(p)
  {
  }

  virtual int calcPartitioning(I N, unsigned& grid, unsigned& block, unsigned& shMemSize,
                               cudaStream_t& stream)
  {
    block = block_;
    grid  = (N + block_ - 1) / block_;
    shMemSize = 0;
    stream = stream_;

    return 0;
  }
  
  virtual int calcPartitioning(I N, unsigned& grid, unsigned& block, unsigned& shMemSize)
  {
    block = block_;
    grid  = (N + block_ - 1) / block_;
    shMemSize = 0;

    return 0;
  }

}; // class StreamPartitioning


template<class T, class I=int>
class ReducePartitioning : public ThreadPartitioning<T, I>
{
  using ThreadPartitioning<T, I>::block_;
  using ThreadPartitioning<T, I>::grid_;
  using ThreadPartitioning<T, I>::shMemSize_;
  using ThreadPartitioning<T, I>::stream_;
  using ThreadPartitioning<T, I>::bufferSize_;
  using ThreadPartitioning<T, I>::d_buffer_;
  using ThreadPartitioning<T, I>::h_buffer_;
  using ThreadPartitioning<T, I>::ownBuffer_;
  using ThreadPartitioning<T, I>::allocfn_;
  using ThreadPartitioning<T, I>::freefn_;

public:
  ReducePartitioning(I N, unsigned block,
                     SUNAllocFn allocfn = nullptr, SUNFreeFn freefn = nullptr)
  : ThreadPartitioning<T, I>(block, allocfn, freefn)
  {
    grid_ = (N + (block_ * 2 - 1)) / (block_ * 2);
    shMemSize_ = block_*sizeof(T);
    allocateBuffer(false, allocfn != nullptr);
  }
  
  ReducePartitioning(I N, unsigned block, cudaStream_t stream,
                     SUNAllocFn allocfn = nullptr, SUNFreeFn freefn = nullptr)
  : ThreadPartitioning<T, I>(block, allocfn, freefn)
  {
    grid_ = (N + (block_ * 2 - 1)) / (block_ * 2);
    shMemSize_ = block_*sizeof(T);
    stream_ = stream;
    allocateBuffer(false, allocfn != nullptr);
  }
  
  ReducePartitioning(T *h_buffer, T *d_buffer, I N, unsigned block, cudaStream_t stream = 0)
  : ThreadPartitioning<T, I>(block)
  {
    grid_ = (N + (block_ * 2 - 1)) / (block_ * 2);
    shMemSize_ = block_*sizeof(T);
    stream_ = stream;
    h_buffer_ = h_buffer;
    d_buffer_ = d_buffer;
    ownBuffer_ = false;
  }
  
  explicit ReducePartitioning(ReducePartitioning<T, I>& p)
  : ThreadPartitioning<T, I>(p)
  {
    shMemSize_ = p.shMemSize_;
    /* if device buffer and host buffer are the same, then assume managed memory */
    allocateBuffer(p.d_buffer_ == p.h_buffer_, p.allocfn_ != nullptr);
  }

  ~ReducePartitioning()
  {
    cudaError_t err;

    if (ownBuffer_ && bufferSize_ > 0) {

      if (d_buffer_ == h_buffer_) {
        /* managed memory */
        if (freefn_) {
          freefn_(d_buffer_);
        } else {
          err = cudaFree(d_buffer_);
          if(err != cudaSuccess)
            std::cerr << "Failed to free device vector "
                      << "in suncudavec::ReducePartitioning::~ReducePartitioning "
                      << "(CUDA error code " << err << ")\n";
        }
        d_buffer_ = h_buffer_ = nullptr;
      } else {
        /* unmanaged memory */
        err = cudaFree(d_buffer_);
        if(err != cudaSuccess)
          std::cerr << "Failed to free device vector "
                    << "in suncudavec::ReducePartitioning::~ReducePartitioning "
                    << "(CUDA error code " << err << ")\n";
        free(h_buffer_);
        d_buffer_ = nullptr;
        h_buffer_ = nullptr;
      }

    }
  }

  virtual int calcPartitioning(I N, unsigned& grid, unsigned& block, unsigned& shMemSize,
                               cudaStream_t& stream)
  {
    block = block_;
    grid  = (N + (block_ * 2 - 1)) / (block_ * 2);
    shMemSize = block_ * sizeof(T);
    stream = stream_;

    return 0;
  }
  
  virtual int calcPartitioning(I N, unsigned& grid, unsigned& block, unsigned& shMemSize)
  {
    block = block_;
    grid  = (N + (block_ * 2 - 1)) / (block_ * 2);
    shMemSize = block_ * sizeof(T);

    return 0;
  }

  virtual void copyFromDevBuffer(unsigned int n) const
  {
    cudaError_t err;

    /* If the host and device pointers are the same, then we don't need
       to do a copy (this happens in the managed memory case), but we
       still need to synchronize the device to adhere to the unified
       memory access rules. */
    if (h_buffer_ == d_buffer_) { 
      err = cudaStreamSynchronize(stream_);
      if(err != cudaSuccess)
        std::cerr << "Failed to synchronize stream in "
                  << "suncudavec::ReducePartitioning::copyFromDevBuffer " 
                  << "(CUDA error code " << err << ")\n";
    } else {
      err = cudaMemcpyAsync(h_buffer_, d_buffer_, n*sizeof(T), cudaMemcpyDeviceToHost,
                            stream_);
      if(err != cudaSuccess)
        std::cerr << "Failed to copy vector from device to host in "
                  << "suncudavec::ReducePartitioning::copyFromDevBuffer " 
                  << "(CUDA error code " << err << ")\n";
    }
  }

  static unsigned calcBufferSize(I N, unsigned block)
  {
    return (N + (block * 2 - 1)) / (block * 2) * sizeof(T);
  }

private:
  int allocateBuffer(bool use_managed_memory = false, bool custom_allocator = false)
  {
    cudaError_t err;

    bufferSize_ = grid_ * sizeof(T);
    if (bufferSize_ == 0) return 0;

    if (custom_allocator) {

      d_buffer_ = static_cast<T*>(allocfn_(bufferSize_));
      if(d_buffer_ == NULL)
        std::cerr << "Failed to allocate managed buffer with custom allocator in "
                  << "suncudavec::ReducePartitioning::allocateBuffer\n";
      h_buffer_ = d_buffer_;

    } else if (use_managed_memory) {

      err = cudaMallocManaged((void**) &d_buffer_, bufferSize_);
      if(err != cudaSuccess)
        std::cerr << "Failed to allocate internal managed buffer in "
                  << "suncudavec::ReducePartitioning::allocateBuffer "
                  << "(CUDA error code " << err << ")\n";
      h_buffer_ = d_buffer_;

    } else {

      h_buffer_ = static_cast<T*>(malloc(bufferSize_));
      if(h_buffer_ == NULL)
        std::cerr << "Failed to allocate internal host buffer in "
                  << "suncudavec::ReducePartitioning::allocateBuffer\n";
      err = cudaMalloc((void**) &d_buffer_, bufferSize_);
      if(err != cudaSuccess)
        std::cerr << "Failed to allocate internal device buffer "
                  << "in suncudavec::ReducePartitioning::allocateBuffer "
                  << "(CUDA error code " << err << ")\n";

    }

    return 0;
  }

}; // class ReducePartitioning


} // namespace suncudavec

#endif // _THREAD_PARTITIONING_HPP_
