/* -----------------------------------------------------------------
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
 * This is the main header file for the Trilinos vector wrapper
 * for NVECTOR module.
 *
 * Part I contains declarations specific to the Trilinos vector wrapper
 * implementation.
 *
 * Part II contains the prototype for the constructor
 * N_VMake_Trilinos as well as Trilinos-specific prototypes
 * for various useful vector operations.
 *
 * Notes:
 *
 *   - The definition of the generic N_Vector structure can be
 *     found in the header file sundials_nvector.h.
 *
 *   - The definition of the type realtype can be found in the
 *     header file sundials_types.h, and it may be changed (at the
 *     build configuration stage) according to the user's needs.
 *     The sundials_types.h file also contains the definition
 *     for the type booleantype.
 *
 *   - N_Vector arguments to arithmetic vector operations need not
 *     be distinct. For example, the following call:
 *
 *        N_VLinearSum_Trilinos(a,x,b,y,y);
 *
 *     (which stores the result of the operation a*x+b*y in y)
 *     is legal.
 * -----------------------------------------------------------------*/

#ifndef _NVECTOR_TRILINOS_H
#define _NVECTOR_TRILINOS_H

#include <sundials/sundials_nvector.h>

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif


/*
 * -----------------------------------------------------------------
 * PART I: N_Vector interface to Trilinos vector
 * -----------------------------------------------------------------
 */

/*
 * Dummy _N_VectorContent_Trilinos structure is used for
 * interfacing C with C++ code
 */

struct _N_VectorContent_Trilinos {};

typedef struct _N_VectorContent_Trilinos *N_VectorContent_Trilinos;

/*
 * -----------------------------------------------------------------
 * PART II: functions exported by nvector_Trilinos
 *
 * CONSTRUCTORS:
 *    N_VNewEmpty_Trilinos
 * -----------------------------------------------------------------
 */


/*
 * -----------------------------------------------------------------
 * Function : N_VNewEmpty_Trilinos
 * -----------------------------------------------------------------
 * This function creates a new N_Vector wrapper for a Trilinos
 * vector.
 * -----------------------------------------------------------------
 */

SUNDIALS_EXPORT N_Vector N_VNewEmpty_Trilinos();

/*
 * -----------------------------------------------------------------
 * Trilinos implementations of the vector operations
 * -----------------------------------------------------------------
 */

SUNDIALS_EXPORT N_Vector_ID N_VGetVectorID_Trilinos(N_Vector v);
SUNDIALS_EXPORT N_Vector N_VCloneEmpty_Trilinos(N_Vector w);
SUNDIALS_EXPORT N_Vector N_VClone_Trilinos(N_Vector w);
SUNDIALS_EXPORT void N_VDestroy_Trilinos(N_Vector v);
SUNDIALS_EXPORT void N_VSpace_Trilinos(N_Vector v, sunindextype *lrw, sunindextype *liw);
SUNDIALS_EXPORT void *N_VGetCommunicator_Trilinos(N_Vector v);
SUNDIALS_EXPORT sunindextype N_VGetLength_Trilinos(N_Vector v);
SUNDIALS_EXPORT void N_VLinearSum_Trilinos(realtype a, N_Vector x, realtype b, N_Vector y, N_Vector z);
SUNDIALS_EXPORT void N_VConst_Trilinos(realtype c, N_Vector z);
SUNDIALS_EXPORT void N_VProd_Trilinos(N_Vector x, N_Vector y, N_Vector z);
SUNDIALS_EXPORT void N_VDiv_Trilinos(N_Vector x, N_Vector y, N_Vector z);
SUNDIALS_EXPORT void N_VScale_Trilinos(realtype c, N_Vector x, N_Vector z);
SUNDIALS_EXPORT void N_VAbs_Trilinos(N_Vector x, N_Vector z);
SUNDIALS_EXPORT void N_VInv_Trilinos(N_Vector x, N_Vector z);
SUNDIALS_EXPORT void N_VAddConst_Trilinos(N_Vector x, realtype b, N_Vector z);
SUNDIALS_EXPORT realtype N_VDotProd_Trilinos(N_Vector x, N_Vector y);
SUNDIALS_EXPORT realtype N_VMaxNorm_Trilinos(N_Vector x);
SUNDIALS_EXPORT realtype N_VWrmsNorm_Trilinos(N_Vector x, N_Vector w);
SUNDIALS_EXPORT realtype N_VWrmsNormMask_Trilinos(N_Vector x, N_Vector w, N_Vector id);
SUNDIALS_EXPORT realtype N_VMin_Trilinos(N_Vector x);
SUNDIALS_EXPORT realtype N_VWL2Norm_Trilinos(N_Vector x, N_Vector w);
SUNDIALS_EXPORT realtype N_VL1Norm_Trilinos(N_Vector x);
SUNDIALS_EXPORT void N_VCompare_Trilinos(realtype c, N_Vector x, N_Vector z);
SUNDIALS_EXPORT booleantype N_VInvTest_Trilinos(N_Vector x, N_Vector z);
SUNDIALS_EXPORT booleantype N_VConstrMask_Trilinos(N_Vector c, N_Vector x, N_Vector m);
SUNDIALS_EXPORT realtype N_VMinQuotient_Trilinos(N_Vector num, N_Vector denom);

/* OPTIONAL local reduction kernels (no parallel communication) */
SUNDIALS_EXPORT realtype N_VDotProdLocal_Trilinos(N_Vector x, N_Vector y);
SUNDIALS_EXPORT realtype N_VMaxNormLocal_Trilinos(N_Vector x);
SUNDIALS_EXPORT realtype N_VMinLocal_Trilinos(N_Vector x);
SUNDIALS_EXPORT realtype N_VL1NormLocal_Trilinos(N_Vector x);
SUNDIALS_EXPORT realtype N_VWSqrSumLocal_Trilinos(N_Vector x, N_Vector w);
SUNDIALS_EXPORT realtype N_VWSqrSumMaskLocal_Trilinos(N_Vector x, N_Vector w,
                                                      N_Vector id);
SUNDIALS_EXPORT booleantype N_VInvTestLocal_Trilinos(N_Vector x, N_Vector z);
SUNDIALS_EXPORT booleantype N_VConstrMaskLocal_Trilinos(N_Vector c, N_Vector x,
                                                        N_Vector m);
SUNDIALS_EXPORT realtype N_VMinQuotientLocal_Trilinos(N_Vector num,
                                                      N_Vector denom);
  
#ifdef __cplusplus
}
#endif

#endif /* _NVECTOR_TRILINOS_H */
