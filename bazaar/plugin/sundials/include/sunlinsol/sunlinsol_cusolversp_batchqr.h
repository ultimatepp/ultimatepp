/* ----------------------------------------------------------------------------
 * Programmer(s): Cody J. Balos @ LLNL
 * ----------------------------------------------------------------------------
 * Based on work by Donald Wilcox @ LBNL
 * ----------------------------------------------------------------------------
 * SUNDIALS Copyright Start
 * Copyright (c) 2002-2020, Lawrence Livermore National Security
 * and Southern Methodist University.
 * All rights reserved.
 *
 * See the top-level LICENSE and NOTICE files for details.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * SUNDIALS Copyright End
 * ----------------------------------------------------------------------------
 * Header file for cuSolverSp batched QR SUNLinearSolver interface.
 * ----------------------------------------------------------------------------*/

#ifndef _SUNLINSOL_CUSOLVERSP_H
#define _SUNLINSOL_CUSOLVERSP_H

#include <cuda_runtime.h>
#include <cusolverSp.h>

#include <sundials/sundials_linearsolver.h>
#include <sundials/sundials_matrix.h>
#include <sundials/sundials_nvector.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
 * ----------------------------------------------------------------------------
 * PART I: cuSolverSp implementation of SUNLinearSolver
 * ----------------------------------------------------------------------------
 */

struct _SUNLinearSolverContent_cuSolverSp_batchQR {
  int                last_flag;            /* last return flag                                     */
  booleantype        first_factorize;      /* is this the first factorization?                     */
  size_t             internal_size;        /* size of cusolver internal buffer for Q and R         */
  size_t             workspace_size;       /* size of cusolver memory block for num. factorization */
  cusolverSpHandle_t cusolver_handle;      /* cuSolverSp context                                   */
  csrqrInfo_t        info;                 /* opaque cusolver data structure                       */
  void*              workspace;            /* memory block used by cusolver                        */
  const char*        desc;                 /* description of this linear solver                    */
};

typedef struct _SUNLinearSolverContent_cuSolverSp_batchQR *SUNLinearSolverContent_cuSolverSp_batchQR;


/*
 * ----------------------------------------------------------------------------
 * PART II: Functions exported by sunlinsol_sludist
 * ----------------------------------------------------------------------------
 */

SUNDIALS_EXPORT SUNLinearSolver SUNLinSol_cuSolverSp_batchQR(N_Vector y, SUNMatrix A,
                                                             cusolverSpHandle_t cusol_handle);


/*
 * ----------------------------------------------------------------------------
 *  cuSolverSp implementations of SUNLinearSolver operations
 * ----------------------------------------------------------------------------
 */

SUNDIALS_EXPORT SUNLinearSolver_Type SUNLinSolGetType_cuSolverSp_batchQR(SUNLinearSolver S);

SUNDIALS_EXPORT SUNLinearSolver_ID SUNLinSolGetID_cuSolverSp_batchQR(SUNLinearSolver S);

SUNDIALS_EXPORT int SUNLinSolInitialize_cuSolverSp_batchQR(SUNLinearSolver S);

SUNDIALS_EXPORT int SUNLinSolSetup_cuSolverSp_batchQR(SUNLinearSolver S,
                                                      SUNMatrix A);

SUNDIALS_EXPORT int SUNLinSolSolve_cuSolverSp_batchQR(SUNLinearSolver S,
                                                      SUNMatrix A,
                                                      N_Vector x,
                                                      N_Vector b,
                                                      realtype tol);

SUNDIALS_EXPORT sunindextype SUNLinSolLastFlag_cuSolverSp_batchQR(SUNLinearSolver S);

SUNDIALS_EXPORT int SUNLinSolFree_cuSolverSp_batchQR(SUNLinearSolver S);


/*
 * ----------------------------------------------------------------------------
 *  Additional get and set functions.
 * ----------------------------------------------------------------------------
 */

SUNDIALS_EXPORT void SUNLinSol_cuSolverSp_batchQR_GetDescription(SUNLinearSolver S,
                                                                 char** desc);

SUNDIALS_EXPORT void SUNLinSol_cuSolverSp_batchQR_SetDescription(SUNLinearSolver S,
                                                                 const char* desc);

SUNDIALS_EXPORT void SUNLinSol_cuSolverSp_batchQR_GetDeviceSpace(SUNLinearSolver S,
                                                                 size_t* cuSolverInternal,
                                                                 size_t* cuSolverWorkspace);                                                                


#ifdef __cplusplus
}
#endif

#endif
