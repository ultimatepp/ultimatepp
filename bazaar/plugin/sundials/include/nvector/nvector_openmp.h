/* -----------------------------------------------------------------
 * Programmer(s): David J. Gardner and Carol S. Woodward @ LLNL
 * -----------------------------------------------------------------
 * Acknowledgements: This NVECTOR module is based on the NVECTOR
 *                   Serial module by Scott D. Cohen, Alan C.
 *                   Hindmarsh, Radu Serban, and Aaron Collier
 *                   @ LLNL
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
 * This is the header file for the OpenMP implementation of the
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
 *       N_VLinearSum_OpenMP(a,x,b,y,y);
 *
 *     (which stores the result of the operation a*x+b*y in y)
 *     is legal.
 * -----------------------------------------------------------------*/

#ifndef _NVECTOR_OPENMP_H
#define _NVECTOR_OPENMP_H

#include <stdio.h>
#include <sundials/sundials_nvector.h>

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif

/*
 * -----------------------------------------------------------------
 * OpenMP implementation of N_Vector
 * -----------------------------------------------------------------
 */

struct _N_VectorContent_OpenMP {
  sunindextype length;   /* vector length            */
  booleantype own_data;  /* data ownership flag      */
  realtype *data;        /* data array               */
  int num_threads;       /* number of OpenMP threads */
};

typedef struct _N_VectorContent_OpenMP *N_VectorContent_OpenMP;

/*
 * -----------------------------------------------------------------
 * Macros NV_CONTENT_OMP, NV_DATA_OMP, NV_OWN_DATA_OMP,
 *        NV_LENGTH_OMP, and NV_Ith_OMP
 * -----------------------------------------------------------------
 */

#define NV_CONTENT_OMP(v)  ( (N_VectorContent_OpenMP)(v->content) )

#define NV_LENGTH_OMP(v)   ( NV_CONTENT_OMP(v)->length )

#define NV_NUM_THREADS_OMP(v)   ( NV_CONTENT_OMP(v)->num_threads )

#define NV_OWN_DATA_OMP(v) ( NV_CONTENT_OMP(v)->own_data )

#define NV_DATA_OMP(v)     ( NV_CONTENT_OMP(v)->data )

#define NV_Ith_OMP(v,i)    ( NV_DATA_OMP(v)[i] )

/*
 * -----------------------------------------------------------------
 * Functions exported by nvector_openmp
 * -----------------------------------------------------------------
 */

SUNDIALS_EXPORT N_Vector N_VNew_OpenMP(sunindextype vec_length, int num_threads);

SUNDIALS_EXPORT N_Vector N_VNewEmpty_OpenMP(sunindextype vec_length, int num_threads);

SUNDIALS_EXPORT N_Vector N_VMake_OpenMP(sunindextype vec_length, realtype *v_data,
                                        int num_threads);

SUNDIALS_EXPORT N_Vector* N_VCloneVectorArray_OpenMP(int count, N_Vector w);

SUNDIALS_EXPORT N_Vector* N_VCloneVectorArrayEmpty_OpenMP(int count, N_Vector w);

SUNDIALS_EXPORT void N_VDestroyVectorArray_OpenMP(N_Vector* vs, int count);

SUNDIALS_EXPORT sunindextype N_VGetLength_OpenMP(N_Vector v);

SUNDIALS_EXPORT void N_VPrint_OpenMP(N_Vector v);

SUNDIALS_EXPORT void N_VPrintFile_OpenMP(N_Vector v, FILE *outfile);


SUNDIALS_EXPORT N_Vector_ID N_VGetVectorID_OpenMP(N_Vector v);
SUNDIALS_EXPORT N_Vector N_VCloneEmpty_OpenMP(N_Vector w);
SUNDIALS_EXPORT N_Vector N_VClone_OpenMP(N_Vector w);
SUNDIALS_EXPORT void N_VDestroy_OpenMP(N_Vector v);
SUNDIALS_EXPORT void N_VSpace_OpenMP(N_Vector v, sunindextype *lrw, sunindextype *liw);
SUNDIALS_EXPORT realtype *N_VGetArrayPointer_OpenMP(N_Vector v);
SUNDIALS_EXPORT void N_VSetArrayPointer_OpenMP(realtype *v_data, N_Vector v);

/* standard vector operations */
SUNDIALS_EXPORT void N_VLinearSum_OpenMP(realtype a, N_Vector x, realtype b, N_Vector y, N_Vector z);
SUNDIALS_EXPORT void N_VConst_OpenMP(realtype c, N_Vector z);
SUNDIALS_EXPORT void N_VProd_OpenMP(N_Vector x, N_Vector y, N_Vector z);
SUNDIALS_EXPORT void N_VDiv_OpenMP(N_Vector x, N_Vector y, N_Vector z);
SUNDIALS_EXPORT void N_VScale_OpenMP(realtype c, N_Vector x, N_Vector z);
SUNDIALS_EXPORT void N_VAbs_OpenMP(N_Vector x, N_Vector z);
SUNDIALS_EXPORT void N_VInv_OpenMP(N_Vector x, N_Vector z);
SUNDIALS_EXPORT void N_VAddConst_OpenMP(N_Vector x, realtype b, N_Vector z);
SUNDIALS_EXPORT realtype N_VDotProd_OpenMP(N_Vector x, N_Vector y);
SUNDIALS_EXPORT realtype N_VMaxNorm_OpenMP(N_Vector x);
SUNDIALS_EXPORT realtype N_VWrmsNorm_OpenMP(N_Vector x, N_Vector w);
SUNDIALS_EXPORT realtype N_VWrmsNormMask_OpenMP(N_Vector x, N_Vector w, N_Vector id);
SUNDIALS_EXPORT realtype N_VMin_OpenMP(N_Vector x);
SUNDIALS_EXPORT realtype N_VWL2Norm_OpenMP(N_Vector x, N_Vector w);
SUNDIALS_EXPORT realtype N_VL1Norm_OpenMP(N_Vector x);
SUNDIALS_EXPORT void N_VCompare_OpenMP(realtype c, N_Vector x, N_Vector z);
SUNDIALS_EXPORT booleantype N_VInvTest_OpenMP(N_Vector x, N_Vector z);
SUNDIALS_EXPORT booleantype N_VConstrMask_OpenMP(N_Vector c, N_Vector x, N_Vector m);
SUNDIALS_EXPORT realtype N_VMinQuotient_OpenMP(N_Vector num, N_Vector denom);

/* fused vector operations */
SUNDIALS_EXPORT int N_VLinearCombination_OpenMP(int nvec, realtype* c,
                                                N_Vector* V, N_Vector z);
SUNDIALS_EXPORT int N_VScaleAddMulti_OpenMP(int nvec, realtype* a, N_Vector x,
                                            N_Vector* Y, N_Vector* Z);
SUNDIALS_EXPORT int N_VDotProdMulti_OpenMP(int nvec, N_Vector x,
                                           N_Vector* Y, realtype* dotprods);

/* vector array operations */
SUNDIALS_EXPORT int N_VLinearSumVectorArray_OpenMP(int nvec,
                                                   realtype a, N_Vector* X,
                                                   realtype b, N_Vector* Y,
                                                   N_Vector* Z);
SUNDIALS_EXPORT int N_VScaleVectorArray_OpenMP(int nvec, realtype* c,
                                               N_Vector* X, N_Vector* Z);
SUNDIALS_EXPORT int N_VConstVectorArray_OpenMP(int nvecs, realtype c,
                                               N_Vector* Z);
SUNDIALS_EXPORT int N_VWrmsNormVectorArray_OpenMP(int nvecs, N_Vector* X,
                                                  N_Vector* W, realtype* nrm);
SUNDIALS_EXPORT int N_VWrmsNormMaskVectorArray_OpenMP(int nvecs, N_Vector* X,
                                                      N_Vector* W, N_Vector id,
                                                      realtype* nrm);
SUNDIALS_EXPORT int N_VScaleAddMultiVectorArray_OpenMP(int nvec, int nsum,
                                                       realtype* a,
                                                       N_Vector* X,
                                                       N_Vector** Y,
                                                       N_Vector** Z);
SUNDIALS_EXPORT int N_VLinearCombinationVectorArray_OpenMP(int nvec, int nsum,
                                                           realtype* c,
                                                           N_Vector** X,
                                                           N_Vector* Z);

/* OPTIONAL local reduction kernels (no parallel communication) */
SUNDIALS_EXPORT realtype N_VWSqrSumLocal_OpenMP(N_Vector x, N_Vector w);
SUNDIALS_EXPORT realtype N_VWSqrSumMaskLocal_OpenMP(N_Vector x, N_Vector w,
                                                    N_Vector id);


/*
 * -----------------------------------------------------------------
 * Enable / disable fused vector operations
 * -----------------------------------------------------------------
 */

SUNDIALS_EXPORT int N_VEnableFusedOps_OpenMP(N_Vector v, booleantype tf);

SUNDIALS_EXPORT int N_VEnableLinearCombination_OpenMP(N_Vector v, booleantype tf);
SUNDIALS_EXPORT int N_VEnableScaleAddMulti_OpenMP(N_Vector v, booleantype tf);
SUNDIALS_EXPORT int N_VEnableDotProdMulti_OpenMP(N_Vector v, booleantype tf);

SUNDIALS_EXPORT int N_VEnableLinearSumVectorArray_OpenMP(N_Vector v, booleantype tf);
SUNDIALS_EXPORT int N_VEnableScaleVectorArray_OpenMP(N_Vector v, booleantype tf);
SUNDIALS_EXPORT int N_VEnableConstVectorArray_OpenMP(N_Vector v, booleantype tf);
SUNDIALS_EXPORT int N_VEnableWrmsNormVectorArray_OpenMP(N_Vector v, booleantype tf);
SUNDIALS_EXPORT int N_VEnableWrmsNormMaskVectorArray_OpenMP(N_Vector v, booleantype tf);
SUNDIALS_EXPORT int N_VEnableScaleAddMultiVectorArray_OpenMP(N_Vector v, booleantype tf);
SUNDIALS_EXPORT int N_VEnableLinearCombinationVectorArray_OpenMP(N_Vector v, booleantype tf);

#ifdef __cplusplus
}
#endif

#endif
