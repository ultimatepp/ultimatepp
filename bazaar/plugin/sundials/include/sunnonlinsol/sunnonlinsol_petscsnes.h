/* -----------------------------------------------------------------------------
 * Programmer(s): Cody J. Balos @ LLNL
 * -----------------------------------------------------------------------------
 * SUNDIALS Copyright Start
 * Copyright (c) 2002-2020, Lawrence Livermore National Security
 * and Southern Methodist University.
 * All rights reserved.
 *
 * See the top-level LICENSE and NOTICE files for details.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * SUNDIALS Copyright End
 * -----------------------------------------------------------------------------
 * This is the header file for the SUNNonlinearSolver module implementation of
 * a wrapper to the PETSc SNES nonlinear solvers.
 *
 * Part I defines the solver-specific content structure.
 *
 * Part II contains prototypes for the solver constructor and operations.
 * ---------------------------------------------------------------------------*/

#ifndef _SUNNONLINSOL_PETSCSNES_H
#define _SUNNONLINSOL_PETSCSNES_H

#include "sundials/sundials_types.h"
#include "sundials/sundials_nvector.h"
#include "sundials/sundials_nonlinearsolver.h"
#include <petscsnes.h>

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif

/* -----------------------------------------------------------------------------
 * I. Content structure
 * ---------------------------------------------------------------------------*/

struct _SUNNonlinearSolverContent_PetscSNES {
  int sysfn_last_err;              /* last error returned by the system function Sys */
  PetscErrorCode petsc_last_err;   /* last error return by PETSc */
  long int nconvfails;             /* number of nonlinear converge failures (recoverable or not) */
  long int nni;                    /* number of nonlinear iterations */
  void *imem;                      /* SUNDIALS integrator memory */
  SNES snes;                       /* PETSc SNES context */
  Vec r;                           /* nonlinear residual */
  N_Vector y, f;                   /* wrappers for PETSc vectors in system function */
  /* functions provided by the integrator */
  SUNNonlinSolSysFn Sys;           /* nonlinear system function         */
};

typedef struct _SUNNonlinearSolverContent_PetscSNES *SUNNonlinearSolverContent_PetscSNES;

/* -----------------------------------------------------------------------------
 * II: Exported functions
 * ---------------------------------------------------------------------------*/

/* Constructor to create solver and allocates memory */

SUNDIALS_EXPORT SUNNonlinearSolver SUNNonlinSol_PetscSNES(N_Vector y, SNES snes);

/* SUNNonlinearSolver API functions */

SUNDIALS_EXPORT SUNNonlinearSolver_Type SUNNonlinSolGetType_PetscSNES(SUNNonlinearSolver NLS);

SUNDIALS_EXPORT int SUNNonlinSolInitialize_PetscSNES(SUNNonlinearSolver NLS);

SUNDIALS_EXPORT int SUNNonlinSolSolve_PetscSNES(SUNNonlinearSolver NLS,
                                                N_Vector y0, N_Vector y,
                                                N_Vector w, realtype tol,
                                                booleantype callLSetup, void* mem);

SUNDIALS_EXPORT int SUNNonlinSolSetSysFn_PetscSNES(SUNNonlinearSolver NLS,
                                                   SUNNonlinSolSysFn SysFn);

SUNDIALS_EXPORT int SUNNonlinSolGetNumIters_PetscSNES(SUNNonlinearSolver NLS, long int* nni);

SUNDIALS_EXPORT int SUNNonlinSolGetNumConvFails_PetscSNES(SUNNonlinearSolver NLS,
                                                          long int* nconvfails);

SUNDIALS_EXPORT int SUNNonlinSolFree_PetscSNES(SUNNonlinearSolver NLS);

/* Implementation specific functions */

SUNDIALS_EXPORT int SUNNonlinSolGetSNES_PetscSNES(SUNNonlinearSolver NLS, SNES* snes);

SUNDIALS_EXPORT int SUNNonlinSolGetPetscError_PetscSNES(SUNNonlinearSolver NLS,
                                                        PetscErrorCode* err);

SUNDIALS_EXPORT int SUNNonlinSolGetSysFn_PetscSNES(SUNNonlinearSolver NLS,
                                                   SUNNonlinSolSysFn* SysFn);


#ifdef __cplusplus
}
#endif

#endif
