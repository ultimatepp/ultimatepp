/* -----------------------------------------------------------------
 * Programmer(s): Daniel R. Reynolds @ SMU
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
 * This is the main header file for the "ManyVector" implementation
 * of the NVECTOR module.
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
 *       N_VLinearSum_ManyVector(a,x,b,y,y);
 *
 *     (which stores the result of the operation a*x+b*y in y)
 *     is legal.
 * -----------------------------------------------------------------*/

#ifndef _NVECTOR_MANY_VECTOR_H
#define _NVECTOR_MANY_VECTOR_H

#include <stdio.h>
#include <sundials/sundials_nvector.h>

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif

/* -----------------------------------------------------------------
   ManyVector implementation of N_Vector
   ----------------------------------------------------------------- */

struct _N_VectorContent_ManyVector {
  sunindextype  num_subvectors;  /* number of vectors attached      */
  sunindextype  global_length;   /* overall manyvector length       */
  N_Vector*     subvec_array;    /* pointer to N_Vector array       */
  booleantype   own_data;        /* flag indicating data ownership  */
};

typedef struct _N_VectorContent_ManyVector *N_VectorContent_ManyVector;

/* -----------------------------------------------------------------
   functions exported by ManyVector
   ----------------------------------------------------------------- */

SUNDIALS_EXPORT N_Vector N_VNew_ManyVector(sunindextype num_subvectors,
                                           N_Vector *vec_array);

SUNDIALS_EXPORT N_Vector N_VGetSubvector_ManyVector(N_Vector v,
                                                    sunindextype vec_num);

SUNDIALS_EXPORT realtype *N_VGetSubvectorArrayPointer_ManyVector(N_Vector v,
                                                                 sunindextype vec_num);

SUNDIALS_EXPORT int N_VSetSubvectorArrayPointer_ManyVector(realtype *v_data, N_Vector v,
                                                           sunindextype vec_num);

SUNDIALS_EXPORT sunindextype N_VGetNumSubvectors_ManyVector(N_Vector v);

/* standard vector operations */
SUNDIALS_EXPORT N_Vector_ID N_VGetVectorID_ManyVector(N_Vector v);
SUNDIALS_EXPORT N_Vector N_VCloneEmpty_ManyVector(N_Vector w);
SUNDIALS_EXPORT N_Vector N_VClone_ManyVector(N_Vector w);
SUNDIALS_EXPORT void N_VDestroy_ManyVector(N_Vector v);
SUNDIALS_EXPORT void N_VSpace_ManyVector(N_Vector v, sunindextype *lrw,
                                         sunindextype *liw);
SUNDIALS_EXPORT sunindextype N_VGetLength_ManyVector(N_Vector v);
SUNDIALS_EXPORT void N_VLinearSum_ManyVector(realtype a, N_Vector x,
                                             realtype b, N_Vector y,
                                             N_Vector z);
SUNDIALS_EXPORT void N_VConst_ManyVector(realtype c, N_Vector z);
SUNDIALS_EXPORT void N_VProd_ManyVector(N_Vector x, N_Vector y, N_Vector z);
SUNDIALS_EXPORT void N_VDiv_ManyVector(N_Vector x, N_Vector y, N_Vector z);
SUNDIALS_EXPORT void N_VScale_ManyVector(realtype c, N_Vector x, N_Vector z);
SUNDIALS_EXPORT void N_VAbs_ManyVector(N_Vector x, N_Vector z);
SUNDIALS_EXPORT void N_VInv_ManyVector(N_Vector x, N_Vector z);
SUNDIALS_EXPORT void N_VAddConst_ManyVector(N_Vector x, realtype b,
                                            N_Vector z);
SUNDIALS_EXPORT realtype N_VWrmsNorm_ManyVector(N_Vector x, N_Vector w);
SUNDIALS_EXPORT realtype N_VWrmsNormMask_ManyVector(N_Vector x, N_Vector w,
                                                    N_Vector id);
SUNDIALS_EXPORT realtype N_VWL2Norm_ManyVector(N_Vector x, N_Vector w);
SUNDIALS_EXPORT void N_VCompare_ManyVector(realtype c, N_Vector x, N_Vector z);

/* fused vector operations */
SUNDIALS_EXPORT int N_VLinearCombination_ManyVector(int nvec, realtype* c,
                                                    N_Vector* V, N_Vector z);
SUNDIALS_EXPORT int N_VScaleAddMulti_ManyVector(int nvec, realtype* a,
                                                N_Vector x, N_Vector* Y,
                                                N_Vector* Z);
SUNDIALS_EXPORT int N_VDotProdMulti_ManyVector(int nvec, N_Vector x,
                                               N_Vector *Y,
                                               realtype* dotprods);

/* vector array operations */
SUNDIALS_EXPORT int N_VLinearSumVectorArray_ManyVector(int nvec,
                                                       realtype a, N_Vector* X,
                                                       realtype b, N_Vector* Y,
                                                       N_Vector* Z);
SUNDIALS_EXPORT int N_VScaleVectorArray_ManyVector(int nvec, realtype* c,
                                                   N_Vector* X, N_Vector* Z);
SUNDIALS_EXPORT int N_VConstVectorArray_ManyVector(int nvecs, realtype c,
                                                   N_Vector* Z);
SUNDIALS_EXPORT int N_VWrmsNormVectorArray_ManyVector(int nvecs, N_Vector* X,
                                                      N_Vector* W, realtype* nrm);
SUNDIALS_EXPORT int N_VWrmsNormMaskVectorArray_ManyVector(int nvec,
                                                          N_Vector* X,
                                                          N_Vector* W,
                                                          N_Vector id,
                                                          realtype* nrm);

/* OPTIONAL local reduction kernels (no parallel communication) */
SUNDIALS_EXPORT realtype N_VDotProdLocal_ManyVector(N_Vector x, N_Vector y);
SUNDIALS_EXPORT realtype N_VMaxNormLocal_ManyVector(N_Vector x);
SUNDIALS_EXPORT realtype N_VMinLocal_ManyVector(N_Vector x);
SUNDIALS_EXPORT realtype N_VL1NormLocal_ManyVector(N_Vector x);
SUNDIALS_EXPORT realtype N_VWSqrSumLocal_ManyVector(N_Vector x, N_Vector w);
SUNDIALS_EXPORT realtype N_VWSqrSumMaskLocal_ManyVector(N_Vector x, N_Vector w,
                                                        N_Vector id);
SUNDIALS_EXPORT booleantype N_VInvTestLocal_ManyVector(N_Vector x, N_Vector z);
SUNDIALS_EXPORT booleantype N_VConstrMaskLocal_ManyVector(N_Vector c, N_Vector x,
                                                          N_Vector m);
SUNDIALS_EXPORT realtype N_VMinQuotientLocal_ManyVector(N_Vector num,
                                                        N_Vector denom);

/* -----------------------------------------------------------------
   Enable / disable fused vector operations
   ----------------------------------------------------------------- */

SUNDIALS_EXPORT int N_VEnableFusedOps_ManyVector(N_Vector v, booleantype tf);

SUNDIALS_EXPORT int N_VEnableLinearCombination_ManyVector(N_Vector v, booleantype tf);
SUNDIALS_EXPORT int N_VEnableScaleAddMulti_ManyVector(N_Vector v, booleantype tf);
SUNDIALS_EXPORT int N_VEnableDotProdMulti_ManyVector(N_Vector v, booleantype tf);

SUNDIALS_EXPORT int N_VEnableLinearSumVectorArray_ManyVector(N_Vector v, booleantype tf);
SUNDIALS_EXPORT int N_VEnableScaleVectorArray_ManyVector(N_Vector v, booleantype tf);
SUNDIALS_EXPORT int N_VEnableConstVectorArray_ManyVector(N_Vector v, booleantype tf);
SUNDIALS_EXPORT int N_VEnableWrmsNormVectorArray_ManyVector(N_Vector v, booleantype tf);
SUNDIALS_EXPORT int N_VEnableWrmsNormMaskVectorArray_ManyVector(N_Vector v, booleantype tf);

#ifdef __cplusplus
}
#endif
#endif
