/*
 * ----------------------------------------------------------------------------
 * Programmer(s): Cody J. Balos @ LLNL
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
 * This is the header file for the SuperLU-DIST implementation of the SUNLINSOL
 * module.
 *
 * Part I contains declarations specific to the SuperLU-Dist implementation of
 * the supplied SUNLINSOL module.
 *
 * Part II contains the prototype for the constructor SUNSuperLUDIST as well as
 * implementation-specific prototypes for various useful solver operations.
 *
 * Notes:
 *
 *   - The definition of the generic SUNLinearSolver structure can be found in
 *   the header file sundials_linearsolver.h.
 * ----------------------------------------------------------------------------
 */

#ifndef _SUNLINSOL_SLUDIST_H
#define _SUNLINSOL_SLUDIST_H

#include <mpi.h>
#include <superlu_ddefs.h>
#include <sundials/sundials_linearsolver.h>
#include <sundials/sundials_matrix.h>
#include <sundials/sundials_nvector.h>
#include <sunmatrix/sunmatrix_sparse.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
 * ----------------------------------------------------------------------------
 * PART I: SuperLU-DIST implementation of SUNLinearSolver
 * ----------------------------------------------------------------------------
 */

struct _SUNLinearSolverContent_SuperLUDIST {
  booleantype             first_factorize;
  int                     last_flag;
  realtype                berr;
  gridinfo_t              *grid;
  LUstruct_t              *lu;
  superlu_dist_options_t  *options;
  ScalePermstruct_t       *scaleperm;
  SOLVEstruct_t           *solve;
  SuperLUStat_t           *stat;
  sunindextype            N;
};

typedef struct _SUNLinearSolverContent_SuperLUDIST *SUNLinearSolverContent_SuperLUDIST;


/*
 * ----------------------------------------------------------------------------
 * PART II: Functions exported by sunlinsol_sludist
 * ----------------------------------------------------------------------------
 */

SUNDIALS_EXPORT SUNLinearSolver SUNLinSol_SuperLUDIST(N_Vector y, SUNMatrix A,
                                                      gridinfo_t *grid,
                                                      LUstruct_t *lu,
                                                      ScalePermstruct_t *scaleperm,
                                                      SOLVEstruct_t *solve,
                                                      SuperLUStat_t *stat,
                                                      superlu_dist_options_t *options);

/*
 * ----------------------------------------------------------------------------
 *  Accessor functions.
 * ----------------------------------------------------------------------------
 */

SUNDIALS_EXPORT realtype SUNLinSol_SuperLUDIST_GetBerr(SUNLinearSolver LS);
SUNDIALS_EXPORT gridinfo_t* SUNLinSol_SuperLUDIST_GetGridinfo(SUNLinearSolver LS);
SUNDIALS_EXPORT LUstruct_t* SUNLinSol_SuperLUDIST_GetLUstruct(SUNLinearSolver LS);
SUNDIALS_EXPORT superlu_dist_options_t* SUNLinSol_SuperLUDIST_GetSuperLUOptions(SUNLinearSolver LS);
SUNDIALS_EXPORT ScalePermstruct_t* SUNLinSol_SuperLUDIST_GetScalePermstruct(SUNLinearSolver LS);
SUNDIALS_EXPORT SOLVEstruct_t* SUNLinSol_SuperLUDIST_GetSOLVEstruct(SUNLinearSolver LS);
SUNDIALS_EXPORT SuperLUStat_t* SUNLinSol_SuperLUDIST_GetSuperLUStat(SUNLinearSolver LS);

/*
 * ----------------------------------------------------------------------------
 *  SuperLU-DIST implementations of SUNLinearSolver operations
 * ----------------------------------------------------------------------------
 */

SUNDIALS_EXPORT SUNLinearSolver_Type SUNLinSolGetType_SuperLUDIST(SUNLinearSolver S);
SUNDIALS_EXPORT SUNLinearSolver_ID SUNLinSolGetID_SuperLUDIST(SUNLinearSolver S);
SUNDIALS_EXPORT int SUNLinSolInitialize_SuperLUDIST(SUNLinearSolver S);
SUNDIALS_EXPORT int SUNLinSolSetup_SuperLUDIST(SUNLinearSolver S, SUNMatrix A);
SUNDIALS_EXPORT int SUNLinSolSolve_SuperLUDIST(SUNLinearSolver S, SUNMatrix A,
                                               N_Vector x, N_Vector b, realtype tol);
SUNDIALS_EXPORT sunindextype SUNLinSolLastFlag_SuperLUDIST(SUNLinearSolver S);
SUNDIALS_EXPORT int SUNLinSolSpace_SuperLUDIST(SUNLinearSolver S,
                                              long int *lenrwLS,
                                              long int *leniwLS);
SUNDIALS_EXPORT int SUNLinSolFree_SuperLUDIST(SUNLinearSolver S);


#ifdef __cplusplus
}
#endif

#endif
