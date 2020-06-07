/* -----------------------------------------------------------------------------
 * Programmer(s): Daniel R. Reynolds @ SMU
 *-----------------------------------------------------------------------------
 * SUNDIALS Copyright Start
 * Copyright (c) 2002-2020, Lawrence Livermore National Security
 * and Southern Methodist University.
 * All rights reserved.
 *
 * See the top-level LICENSE and NOTICE files for details.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * SUNDIALS Copyright End
 *-----------------------------------------------------------------------------
 * This file contains the implementation of functions needed for initialization
 * of the SUNNonlinearSolver fixed point module operations in Fortran.
 *---------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

#include "fsunnonlinsol_fixedpoint.h"

/* Define global nonlinsol variables */

SUNNonlinearSolver F2C_CVODE_nonlinsol;
SUNNonlinearSolver F2C_IDA_nonlinsol;
SUNNonlinearSolver F2C_ARKODE_nonlinsol;

/* Declarations of external global variables */

extern N_Vector F2C_CVODE_vec;
extern N_Vector F2C_IDA_vec;
extern N_Vector F2C_ARKODE_vec;

/* Fortran callable interfaces */

void FSUNFIXEDPOINT_INIT(int *code, int *m, int *ier)
{
  *ier = 0;

  switch(*code) {
  case FCMIX_CVODE:
    if (F2C_CVODE_nonlinsol)  SUNNonlinSolFree(F2C_CVODE_nonlinsol);
    F2C_CVODE_nonlinsol = NULL;
    F2C_CVODE_nonlinsol = SUNNonlinSol_FixedPoint(F2C_CVODE_vec, *m);
    if (F2C_CVODE_nonlinsol == NULL) *ier = -1;
    break;
  case FCMIX_IDA:
    if (F2C_IDA_nonlinsol)  SUNNonlinSolFree(F2C_IDA_nonlinsol);
    F2C_IDA_nonlinsol = NULL;
    F2C_IDA_nonlinsol = SUNNonlinSol_FixedPoint(F2C_IDA_vec, *m);
    if (F2C_IDA_nonlinsol == NULL) *ier = -1;
    break;
  case FCMIX_ARKODE:
    if (F2C_ARKODE_nonlinsol)  SUNNonlinSolFree(F2C_ARKODE_nonlinsol);
    F2C_ARKODE_nonlinsol = NULL;
    F2C_ARKODE_nonlinsol = SUNNonlinSol_FixedPoint(F2C_ARKODE_vec, *m);
    if (F2C_ARKODE_nonlinsol == NULL) *ier = -1;
    break;
  default:
    *ier = -1;
  }
}


void FSUNFIXEDPOINT_SETMAXITERS(int *code, int *maxiters, int *ier)
{
  *ier = 0;

  switch(*code) {
  case FCMIX_CVODE:
    if (!F2C_CVODE_nonlinsol) {
      *ier = -1;
      return;
    }
    *ier = SUNNonlinSolSetMaxIters(F2C_CVODE_nonlinsol, *maxiters);
    break;
  case FCMIX_IDA:
    if (!F2C_IDA_nonlinsol) {
      *ier = -1;
      return;
    }
    *ier = SUNNonlinSolSetMaxIters(F2C_IDA_nonlinsol, *maxiters);
    break;
  case FCMIX_ARKODE:
    if (!F2C_ARKODE_nonlinsol) {
      *ier = -1;
      return;
    }
    *ier = SUNNonlinSolSetMaxIters(F2C_ARKODE_nonlinsol, *maxiters);
    break;
  default:
    *ier = -1;
  }
}
