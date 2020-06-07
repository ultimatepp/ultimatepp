/*
 * -----------------------------------------------------------------
 * Programmer(s): Cody J. Balos @ LLNL
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
 * This header files defines internal utility functions and macros
 * for working with CUDA.
 * -----------------------------------------------------------------
 */

#include <stdio.h>

#include <cuda_runtime.h>
#include <cusolverSp.h>
#include <cusparse.h>

#include <sundials/sundials_types.h>

#ifndef _SUNDIALS_CUDA_H
#define _SUNDIALS_CUDA_H

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif

/* ---------------------------------------------------------------------------
 * Constants
 * ---------------------------------------------------------------------------*/

#define CUDA_WARP_SIZE 32


/* ---------------------------------------------------------------------------
 * Utility macros
 * ---------------------------------------------------------------------------*/

#define SUNDIALS_CUDA_VERIFY(cuerr) SUNDIALS_CUDA_Assert(cuerr, __FILE__, __LINE__)
#define SUNDIALS_CUSPARSE_VERIFY(cuerr) SUNDIALS_CUSPARSE_Assert(cuerr, __FILE__, __LINE__)
#define SUNDIALS_CUSOLVER_VERIFY(cuerr) SUNDIALS_CUSOLVER_Assert(cuerr, __FILE__, __LINE__)


/* ---------------------------------------------------------------------------
 * Utility functions
 * ---------------------------------------------------------------------------*/
inline booleantype SUNDIALS_CUDA_Assert(cudaError_t cuerr, const char *file, int line)
{
  if (cuerr != cudaSuccess)
  {
#ifdef SUNDIALS_DEBUG
    fprintf(stderr,
            "ERROR in CUDA runtime operation: %s %s:%d\n",
            cudaGetErrorString(cuerr), file, line);
#endif
    return SUNFALSE; /* Assert failed */
  }
  return SUNTRUE; /* Assert OK */
}

inline booleantype SUNDIALS_CUSPARSE_Assert(cusparseStatus_t status, const char *file, int line)
{
  if (status != CUSPARSE_STATUS_SUCCESS)
  {
#ifdef SUNDIALS_DEBUG
    fprintf(stderr,
            "ERROR in cuSPARSE runtime operation: cusparseStatus_t = %d %s:%d\n",
            status, file, line);
#endif
    return SUNFALSE; /*  Assert failed */
  }
  return SUNTRUE; /* Assert OK */
}

inline booleantype SUNDIALS_CUSOLVER_Assert(cusolverStatus_t status, const char *file, int line)
{
  if (status != CUSOLVER_STATUS_SUCCESS)
  {
#ifdef SUNDIALS_DEBUG
    fprintf(stderr,
            "ERROR in cuSOLVER runtime operation: cusolverStatus_t = %d %s:%d\n",
            status, file, line);
#endif
    return SUNFALSE; /*  Assert failed */
  }
  return SUNTRUE; /* Assert OK */
}


#ifdef __cplusplus  /* wrapper to enable C++ usage */
}
#endif

#endif /* _SUNDIALS_CUDA_H */