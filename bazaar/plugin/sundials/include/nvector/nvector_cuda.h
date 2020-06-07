/* -----------------------------------------------------------------
 * Programmer(s): Slaven Peles and Cody J. Balos @ LLNL
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
 * This is the header file for the CUDA implementation of the
 * NVECTOR module.
 *
 * Notes:
 *
 *   - The definition of the generic N_Vector structure can be found
 *     in the header file sundials_nvector.h.
 *
 *   - The definitions of the types 'realtype' and 'sunindextype' can
 *     be found in the header file sundials_types.h, and it may be
 *     changed (at the configuration stage) according to the user's needs.
 *     The sundials_types.h file also contains the definition
 *     for the type 'booleantype'.
 *
 *   - N_Vector arguments to arithmetic vector operations need not
 *     be distinct. For example, the following call:
 *
 *       N_VLinearSum_Cuda(a,x,b,y,y);
 *
 *     (which stores the result of the operation a*x+b*y in y)
 *     is legal.
 * -----------------------------------------------------------------*/

#ifndef _NVECTOR_CUDA_H
#define _NVECTOR_CUDA_H

#include <stdio.h>
#include <sundials/sundials_nvector.h>
#include <sundials/sundials_config.h>

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif

/*
 * -----------------------------------------------------------------
 * CUDA implementation of N_Vector
 * -----------------------------------------------------------------
 */

/*
 * CUDA implementation of the N_Vector 'content' is in C++ class
 * Vector. The class inherits from structure _N_VectorContent_Cuda
 * to create C <--> C++ interface.
 */

struct _N_VectorContent_Cuda {};

typedef struct _N_VectorContent_Cuda *N_VectorContent_Cuda;

/*
 * -----------------------------------------------------------------
 * Functions exported by nvector_cuda
 * -----------------------------------------------------------------
 */

SUNDIALS_EXPORT N_Vector N_VNew_Cuda(sunindextype length);

SUNDIALS_EXPORT N_Vector N_VNewManaged_Cuda(sunindextype length);

SUNDIALS_EXPORT N_Vector N_VNewEmpty_Cuda();

SUNDIALS_EXPORT N_Vector N_VMake_Cuda(sunindextype length,
                                      realtype *h_vdata,
                                      realtype *d_vdata);

SUNDIALS_EXPORT N_Vector N_VMakeManaged_Cuda(sunindextype length,
                                             realtype *vdata);

SUNDIALS_EXPORT N_Vector N_VMakeWithManagedAllocator_Cuda(sunindextype length,
                                                          void* (*allocfn)(size_t),
                                                          void (*freefn)(void*));

SUNDIALS_EXPORT sunindextype N_VGetLength_Cuda(N_Vector v);

SUNDIALS_EXPORT realtype *N_VGetHostArrayPointer_Cuda(N_Vector v);

SUNDIALS_EXPORT realtype *N_VGetDeviceArrayPointer_Cuda(N_Vector v);

SUNDIALS_EXPORT booleantype N_VIsManagedMemory_Cuda(N_Vector x);

SUNDIALS_EXPORT void N_VSetCudaStream_Cuda(N_Vector x, cudaStream_t *stream);

SUNDIALS_EXPORT void N_VCopyToDevice_Cuda(N_Vector v);

SUNDIALS_EXPORT void N_VCopyFromDevice_Cuda(N_Vector v);

SUNDIALS_EXPORT void N_VPrint_Cuda(N_Vector v);

SUNDIALS_EXPORT void N_VPrintFile_Cuda(N_Vector v, FILE *outfile);

SUNDIALS_EXPORT N_Vector N_VCloneEmpty_Cuda(N_Vector w);
SUNDIALS_EXPORT N_Vector N_VClone_Cuda(N_Vector w);
SUNDIALS_EXPORT void N_VDestroy_Cuda(N_Vector v);
SUNDIALS_EXPORT void N_VSpace_Cuda(N_Vector v, sunindextype *lrw, sunindextype *liw);

/* standard vector operations */
SUNDIALS_EXPORT void N_VLinearSum_Cuda(realtype a, N_Vector x, realtype b, N_Vector y, N_Vector z);
SUNDIALS_EXPORT void N_VConst_Cuda(realtype c, N_Vector z);
SUNDIALS_EXPORT void N_VProd_Cuda(N_Vector x, N_Vector y, N_Vector z);
SUNDIALS_EXPORT void N_VDiv_Cuda(N_Vector x, N_Vector y, N_Vector z);
SUNDIALS_EXPORT void N_VScale_Cuda(realtype c, N_Vector x, N_Vector z);
SUNDIALS_EXPORT void N_VAbs_Cuda(N_Vector x, N_Vector z);
SUNDIALS_EXPORT void N_VInv_Cuda(N_Vector x, N_Vector z);
SUNDIALS_EXPORT void N_VAddConst_Cuda(N_Vector x, realtype b, N_Vector z);
SUNDIALS_EXPORT realtype N_VDotProd_Cuda(N_Vector x, N_Vector y);
SUNDIALS_EXPORT realtype N_VMaxNorm_Cuda(N_Vector x);
SUNDIALS_EXPORT realtype N_VWrmsNorm_Cuda(N_Vector x, N_Vector w);
SUNDIALS_EXPORT realtype N_VWrmsNormMask_Cuda(N_Vector x, N_Vector w, N_Vector id);
SUNDIALS_EXPORT realtype N_VMin_Cuda(N_Vector x);
SUNDIALS_EXPORT realtype N_VWL2Norm_Cuda(N_Vector x, N_Vector w);
SUNDIALS_EXPORT realtype N_VL1Norm_Cuda(N_Vector x);
SUNDIALS_EXPORT void N_VCompare_Cuda(realtype c, N_Vector x, N_Vector z);
SUNDIALS_EXPORT booleantype N_VInvTest_Cuda(N_Vector x, N_Vector z);
SUNDIALS_EXPORT booleantype N_VConstrMask_Cuda(N_Vector c, N_Vector x, N_Vector m);
SUNDIALS_EXPORT realtype N_VMinQuotient_Cuda(N_Vector num, N_Vector denom);

/* fused vector operations */
SUNDIALS_EXPORT int N_VLinearCombination_Cuda(int nvec, realtype* c, N_Vector* X,
                                              N_Vector Z);
SUNDIALS_EXPORT int N_VScaleAddMulti_Cuda(int nvec, realtype* c, N_Vector X,
                                          N_Vector* Y, N_Vector* Z);
SUNDIALS_EXPORT int N_VDotProdMulti_Cuda(int nvec, N_Vector x, N_Vector* Y,
                                         realtype* dotprods);

/* vector array operations */
SUNDIALS_EXPORT int N_VLinearSumVectorArray_Cuda(int nvec,
                                                 realtype a, N_Vector* X,
                                                 realtype b, N_Vector* Y,
                                                 N_Vector* Z);
SUNDIALS_EXPORT int N_VScaleVectorArray_Cuda(int nvec, realtype* c, N_Vector* X,
                                             N_Vector* Z);
SUNDIALS_EXPORT int N_VConstVectorArray_Cuda(int nvec, realtype c, N_Vector* Z);
SUNDIALS_EXPORT int N_VScaleAddMultiVectorArray_Cuda(int nvec, int nsum,
                                                     realtype* a, N_Vector* X,
                                                     N_Vector** Y, N_Vector** Z);
SUNDIALS_EXPORT int N_VLinearCombinationVectorArray_Cuda(int nvec, int nsum,
                                                         realtype* c,
                                                         N_Vector** X,
                                                         N_Vector* Z);
SUNDIALS_EXPORT int N_VWrmsNormVectorArray_Cuda(int nvec, N_Vector* X,
                                                N_Vector* W, realtype* nrm);
SUNDIALS_EXPORT int N_VWrmsNormMaskVectorArray_Cuda(int nvec, N_Vector* X,
                                                    N_Vector* W, N_Vector id,
                                                    realtype* nrm);

/* OPTIONAL local reduction kernels (no parallel communication) */
SUNDIALS_EXPORT realtype N_VWSqrSumLocal_Cuda(N_Vector x, N_Vector w);
SUNDIALS_EXPORT realtype N_VWSqrSumMaskLocal_Cuda(N_Vector x, N_Vector w, N_Vector id);


/*
 * -----------------------------------------------------------------
 * Enable / disable fused vector operations
 * -----------------------------------------------------------------
 */

SUNDIALS_EXPORT int N_VEnableFusedOps_Cuda(N_Vector v, booleantype tf);

SUNDIALS_EXPORT int N_VEnableLinearCombination_Cuda(N_Vector v, booleantype tf);
SUNDIALS_EXPORT int N_VEnableScaleAddMulti_Cuda(N_Vector v, booleantype tf);
SUNDIALS_EXPORT int N_VEnableDotProdMulti_Cuda(N_Vector v, booleantype tf);

SUNDIALS_EXPORT int N_VEnableLinearSumVectorArray_Cuda(N_Vector v, booleantype tf);
SUNDIALS_EXPORT int N_VEnableScaleVectorArray_Cuda(N_Vector v, booleantype tf);
SUNDIALS_EXPORT int N_VEnableConstVectorArray_Cuda(N_Vector v, booleantype tf);
SUNDIALS_EXPORT int N_VEnableWrmsNormVectorArray_Cuda(N_Vector v, booleantype tf);
SUNDIALS_EXPORT int N_VEnableWrmsNormMaskVectorArray_Cuda(N_Vector v, booleantype tf);
SUNDIALS_EXPORT int N_VEnableScaleAddMultiVectorArray_Cuda(N_Vector v, booleantype tf);
SUNDIALS_EXPORT int N_VEnableLinearCombinationVectorArray_Cuda(N_Vector v, booleantype tf);

#ifdef __cplusplus
}
#endif

#endif
