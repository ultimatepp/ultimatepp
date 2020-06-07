/* -------------------------------------------------------------------
 * Programmer(s): David J. Gardner and Shelby Lockhart @ LLNL
 * -------------------------------------------------------------------
 * Acknowledgements: This NVECTOR module is based on the NVECTOR
 *                   Serial module by Scott D. Cohen, Alan C.
 *                   Hindmarsh, Radu Serban, and Aaron Collier
 *                   @ LLNL
 * -------------------------------------------------------------------
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
 * This is the header file for the OpenMP 4.5+ implementation of the
 * NVECTOR module.
 *
 * Notes:
 *
 *   - The definition of the generic N_Vector structure can be found
 *     in the header file sundials_nvector.h.
 *
 *   - The definition of the type 'realtype' can be found in the
 *     header file sundials_types.h, and it may be changed (at the
 *     configuration stage) according to the user's needs.
 *     The sundials_types.h file also contains the definition
 *     for the type 'booleantype'.
 *
 *   - N_Vector arguments to arithmetic vector operations need not
 *     be distinct. For example, the following call:
 *
 *       N_VLinearSum_OpenMPDEV(a,x,b,y,y);
 *
 *     (which stores the result of the operation a*x+b*y in y)
 *     is legal.
 * -----------------------------------------------------------------*/

#ifndef _NVECTOR_OPENMPDEV_H
#define _NVECTOR_OPENMPDEV_H

#include <stdio.h>
#include <sundials/sundials_nvector.h>

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif

/*
 * -----------------------------------------------------------------
 * OpenMPDEV implementation of N_Vector
 * -----------------------------------------------------------------
 */

struct _N_VectorContent_OpenMPDEV {
  sunindextype length;   /* vector length       */
  booleantype own_data;  /* data ownership flag */
  realtype *host_data;   /* host data array     */
  realtype *dev_data;    /* device data array   */
};

typedef struct _N_VectorContent_OpenMPDEV *N_VectorContent_OpenMPDEV;

/*
 * -----------------------------------------------------------------
 * Macros NV_CONTENT_OMPDEV, NV_DATA_HOST_OMPDEV, NV_OWN_DATA_OMPDEV,
 *        NV_LENGTH_OMPDEV, and NV_Ith_OMPDEV
 * -----------------------------------------------------------------
 */

#define NV_CONTENT_OMPDEV(v)   ( (N_VectorContent_OpenMPDEV)(v->content) )

#define NV_LENGTH_OMPDEV(v)    ( NV_CONTENT_OMPDEV(v)->length )

#define NV_OWN_DATA_OMPDEV(v)  ( NV_CONTENT_OMPDEV(v)->own_data )

#define NV_DATA_HOST_OMPDEV(v) ( NV_CONTENT_OMPDEV(v)->host_data )

#define NV_DATA_DEV_OMPDEV(v)  ( NV_CONTENT_OMPDEV(v)->dev_data )

/*
 * -----------------------------------------------------------------
 * Functions exported by nvector_openmpdev
 * -----------------------------------------------------------------
 */

SUNDIALS_EXPORT N_Vector N_VNew_OpenMPDEV(sunindextype vec_length);

SUNDIALS_EXPORT N_Vector N_VNewEmpty_OpenMPDEV(sunindextype vec_length);

SUNDIALS_EXPORT N_Vector N_VMake_OpenMPDEV(sunindextype vec_length,
                                           realtype *h_data,
                                           realtype *v_data);

SUNDIALS_EXPORT N_Vector *N_VCloneVectorArray_OpenMPDEV(int count, N_Vector w);

SUNDIALS_EXPORT N_Vector *N_VCloneVectorArrayEmpty_OpenMPDEV(int count, N_Vector w);

SUNDIALS_EXPORT void N_VDestroyVectorArray_OpenMPDEV(N_Vector *vs, int count);

SUNDIALS_EXPORT sunindextype N_VGetLength_OpenMPDEV(N_Vector v);

SUNDIALS_EXPORT realtype *N_VGetHostArrayPointer_OpenMPDEV(N_Vector v);

SUNDIALS_EXPORT realtype *N_VGetDeviceArrayPointer_OpenMPDEV(N_Vector v);

SUNDIALS_EXPORT void N_VPrint_OpenMPDEV(N_Vector v);

SUNDIALS_EXPORT void N_VPrintFile_OpenMPDEV(N_Vector v, FILE *outfile);

SUNDIALS_EXPORT void N_VCopyToDevice_OpenMPDEV(N_Vector v);

SUNDIALS_EXPORT void N_VCopyFromDevice_OpenMPDEV(N_Vector v);

SUNDIALS_EXPORT N_Vector_ID N_VGetVectorID_OpenMPDEV(N_Vector v);
SUNDIALS_EXPORT N_Vector N_VCloneEmpty_OpenMPDEV(N_Vector w);
SUNDIALS_EXPORT N_Vector N_VClone_OpenMPDEV(N_Vector w);
SUNDIALS_EXPORT void N_VDestroy_OpenMPDEV(N_Vector v);
SUNDIALS_EXPORT void N_VSpace_OpenMPDEV(N_Vector v, sunindextype *lrw, sunindextype *liw);

/* standard vector operations */
SUNDIALS_EXPORT void N_VLinearSum_OpenMPDEV(realtype a, N_Vector x, realtype b, N_Vector y, N_Vector z);
SUNDIALS_EXPORT void N_VConst_OpenMPDEV(realtype c, N_Vector z);
SUNDIALS_EXPORT void N_VProd_OpenMPDEV(N_Vector x, N_Vector y, N_Vector z);
SUNDIALS_EXPORT void N_VDiv_OpenMPDEV(N_Vector x, N_Vector y, N_Vector z);
SUNDIALS_EXPORT void N_VScale_OpenMPDEV(realtype c, N_Vector x, N_Vector z);
SUNDIALS_EXPORT void N_VAbs_OpenMPDEV(N_Vector x, N_Vector z);
SUNDIALS_EXPORT void N_VInv_OpenMPDEV(N_Vector x, N_Vector z);
SUNDIALS_EXPORT void N_VAddConst_OpenMPDEV(N_Vector x, realtype b, N_Vector z);
SUNDIALS_EXPORT realtype N_VDotProd_OpenMPDEV(N_Vector x, N_Vector y);
SUNDIALS_EXPORT realtype N_VMaxNorm_OpenMPDEV(N_Vector x);
SUNDIALS_EXPORT realtype N_VWrmsNorm_OpenMPDEV(N_Vector x, N_Vector w);
SUNDIALS_EXPORT realtype N_VWrmsNormMask_OpenMPDEV(N_Vector x, N_Vector w, N_Vector id);
SUNDIALS_EXPORT realtype N_VMin_OpenMPDEV(N_Vector x);
SUNDIALS_EXPORT realtype N_VWL2Norm_OpenMPDEV(N_Vector x, N_Vector w);
SUNDIALS_EXPORT realtype N_VL1Norm_OpenMPDEV(N_Vector x);
SUNDIALS_EXPORT void N_VCompare_OpenMPDEV(realtype c, N_Vector x, N_Vector z);
SUNDIALS_EXPORT booleantype N_VInvTest_OpenMPDEV(N_Vector x, N_Vector z);
SUNDIALS_EXPORT booleantype N_VConstrMask_OpenMPDEV(N_Vector c, N_Vector x, N_Vector m);
SUNDIALS_EXPORT realtype N_VMinQuotient_OpenMPDEV(N_Vector num, N_Vector denom);

/* fused vector operations */
SUNDIALS_EXPORT int N_VLinearCombination_OpenMPDEV(int nvec, realtype* c,
                                                  N_Vector* V, N_Vector z);
SUNDIALS_EXPORT int N_VScaleAddMulti_OpenMPDEV(int nvec, realtype* a, N_Vector x,
                                              N_Vector* Y, N_Vector* Z);
SUNDIALS_EXPORT int N_VDotProdMulti_OpenMPDEV(int nvec, N_Vector x,
                                             N_Vector *Y, realtype* dotprods);

/* vector array operations */
SUNDIALS_EXPORT int N_VLinearSumVectorArray_OpenMPDEV(int nvec,
                                                     realtype a, N_Vector* X,
                                                     realtype b, N_Vector* Y,
                                                     N_Vector* Z);
SUNDIALS_EXPORT int N_VScaleVectorArray_OpenMPDEV(int nvec, realtype* c,
                                                 N_Vector* X, N_Vector* Z);
SUNDIALS_EXPORT int N_VConstVectorArray_OpenMPDEV(int nvecs, realtype c,
                                                 N_Vector* Z);
SUNDIALS_EXPORT int N_VWrmsNormVectorArray_OpenMPDEV(int nvecs, N_Vector* X,
                                                    N_Vector* W, realtype* nrm);
SUNDIALS_EXPORT int N_VWrmsNormMaskVectorArray_OpenMPDEV(int nvecs, N_Vector* X,
                                                        N_Vector* W, N_Vector id,
                                                        realtype* nrm);
SUNDIALS_EXPORT int N_VScaleAddMultiVectorArray_OpenMPDEV(int nvec, int nsum,
                                                         realtype* a,
                                                         N_Vector* X,
                                                         N_Vector** Y,
                                                         N_Vector** Z);
SUNDIALS_EXPORT int N_VLinearCombinationVectorArray_OpenMPDEV(int nvec, int nsum,
                                                             realtype* c,
                                                             N_Vector** X,
                                                             N_Vector* Z);

/* OPTIONAL local reduction kernels (no parallel communication) */
SUNDIALS_EXPORT realtype N_VWSqrSumLocal_OpenMPDEV(N_Vector x, N_Vector w);
SUNDIALS_EXPORT realtype N_VWSqrSumMaskLocal_OpenMPDEV(N_Vector x, N_Vector w,
                                                       N_Vector id);

  
/*
 * -----------------------------------------------------------------
 * Enable / disable fused vector operations
 * -----------------------------------------------------------------
 */

SUNDIALS_EXPORT int N_VEnableFusedOps_OpenMPDEV(N_Vector v, booleantype tf);

SUNDIALS_EXPORT int N_VEnableLinearCombination_OpenMPDEV(N_Vector v, booleantype tf);
SUNDIALS_EXPORT int N_VEnableScaleAddMulti_OpenMPDEV(N_Vector v, booleantype tf);
SUNDIALS_EXPORT int N_VEnableDotProdMulti_OpenMPDEV(N_Vector v, booleantype tf);

SUNDIALS_EXPORT int N_VEnableLinearSumVectorArray_OpenMPDEV(N_Vector v, booleantype tf);
SUNDIALS_EXPORT int N_VEnableScaleVectorArray_OpenMPDEV(N_Vector v, booleantype tf);
SUNDIALS_EXPORT int N_VEnableConstVectorArray_OpenMPDEV(N_Vector v, booleantype tf);
SUNDIALS_EXPORT int N_VEnableWrmsNormVectorArray_OpenMPDEV(N_Vector v, booleantype tf);
SUNDIALS_EXPORT int N_VEnableWrmsNormMaskVectorArray_OpenMPDEV(N_Vector v, booleantype tf);
SUNDIALS_EXPORT int N_VEnableScaleAddMultiVectorArray_OpenMPDEV(N_Vector v, booleantype tf);
SUNDIALS_EXPORT int N_VEnableLinearCombinationVectorArray_OpenMPDEV(N_Vector v, booleantype tf);

#ifdef __cplusplus
}
#endif

#endif
