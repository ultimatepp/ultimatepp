/*
 * ----------------------------------------------------------------- 
 * Programmer(s): Daniel Reynolds, Ashley Crawford @ SMU
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
 * This file (companion of fsunlinsol_dense.c) contains the
 * definitions needed for the initialization of dense
 * linear solver operations in Fortran.
 * -----------------------------------------------------------------
 */

#ifndef _FSUNLINSOL_DENSE_H
#define _FSUNLINSOL_DENSE_H

#include <sunlinsol/sunlinsol_dense.h>
#include <sundials/sundials_fnvector.h>

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif

#if defined(SUNDIALS_F77_FUNC)
#define FSUNDENSELINSOL_INIT     SUNDIALS_F77_FUNC(fsundenselinsolinit, FSUNDENSELINSOLINIT)
#define FSUNMASSDENSELINSOL_INIT SUNDIALS_F77_FUNC(fsunmassdenselinsolinit, FSUNMASSDENSELINSOLINIT)
#else
#define FSUNDENSELINSOL_INIT     fsundenselinsolinit_
#define FSUNMASSDENSELINSOL_INIT fsunmassdenselinsolinit_
#endif


/* Declarations of global variables */

extern SUNLinearSolver F2C_CVODE_linsol;
extern SUNLinearSolver F2C_IDA_linsol;
extern SUNLinearSolver F2C_KINSOL_linsol;
extern SUNLinearSolver F2C_ARKODE_linsol;
extern SUNLinearSolver F2C_ARKODE_mass_sol;

/* 
 * Prototypes of exported functions 
 *
 * FSUNDENSELINSOL_INIT - initializes dense linear solver for main problem
 * FSUNMASSDENSELINSOL_INIT - initializes dense linear solver for mass matrix solve
 */

void FSUNDENSELINSOL_INIT(int *code, int *ier);
void FSUNMASSDENSELINSOL_INIT(int *ier);

#ifdef __cplusplus
}
#endif

#endif
