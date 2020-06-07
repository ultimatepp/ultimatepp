/*
 * ----------------------------------------------------------------- 
 * Programmer(s): Daniel Reynolds @ SMU
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
 * This file (companion of fsunlinsol_spgmr.c) contains the
 * definitions needed for the initialization of SPGMR
 * linear solver operations in Fortran.
 * -----------------------------------------------------------------
 */

#ifndef _FSUNLINSOL_SPGMR_H
#define _FSUNLINSOL_SPGMR_H

#include <sunlinsol/sunlinsol_spgmr.h>
#include <sundials/sundials_fnvector.h>

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif

#if defined(SUNDIALS_F77_FUNC)
#define FSUNSPGMR_INIT            SUNDIALS_F77_FUNC(fsunspgmrinit,            FSUNSPGMRINIT)
#define FSUNSPGMR_SETGSTYPE       SUNDIALS_F77_FUNC(fsunspgmrsetgstype,       FSUNSPGMRSETGSTYPE)
#define FSUNSPGMR_SETPRECTYPE     SUNDIALS_F77_FUNC(fsunspgmrsetprectype,     FSUNSPGMRSETPRECTYPE)
#define FSUNSPGMR_SETMAXRS        SUNDIALS_F77_FUNC(fsunspgmrsetmaxrs,        FSUNSPGMRSETMAXRS)
#define FSUNMASSSPGMR_INIT        SUNDIALS_F77_FUNC(fsunmassspgmrinit,        FSUNMASSSPGMRINIT)
#define FSUNMASSSPGMR_SETGSTYPE   SUNDIALS_F77_FUNC(fsunmassspgmrsetgstype,   FSUNMASSSPGMRSETGSTYPE)
#define FSUNMASSSPGMR_SETPRECTYPE SUNDIALS_F77_FUNC(fsunmassspgmrsetprectype, FSUNMASSSPGMRSETPRECTYPE)
#define FSUNMASSSPGMR_SETMAXRS    SUNDIALS_F77_FUNC(fsunmassspgmrsetmaxrs,    FSUNMASSSPGMRSETMAXRS)
#else
#define FSUNSPGMR_INIT            fsunspgmrinit_
#define FSUNSPGMR_SETGSTYPE       fsunspgmrsetgstype_
#define FSUNSPGMR_SETPRECTYPE     fsunspgmrsetprectype_
#define FSUNSPGMR_SETMAXRS        fsunspgmrsetmaxrs_
#define FSUNMASSSPGMR_INIT        fsunmassspgmrinit_
#define FSUNMASSSPGMR_SETGSTYPE   fsunmassspgmrsetgstype_
#define FSUNMASSSPGMR_SETPRECTYPE fsunmassspgmrsetprectype_
#define FSUNMASSSPGMR_SETMAXRS    fsunmassspgmrsetmaxrs_
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
 * FSUNSPGMR_INIT - initializes SPGMR linear solver for main problem
 * FSUNSPGMR_SETGSTYPE - sets the Gram-Scmidt orthogonalization type for main problem
 * FSUNSPGMR_SETPRECTYPE - sets the preconditioning type for main problem
 * FSUNSPGMR_SETMAXRS - sets the maximum number of restarts to allow for main problem
 *
 * FSUNMASSSPGMR_INIT - initializes SPGMR linear solver for mass matrix solve
 * FSUNMASSSPGMR_SETGSTYPE - sets the Gram-Scmidt orthogonalization type for mass matrix solve
 * FSUNMASSSPGMR_SETPRECTYPE - sets the preconditioning type for mass matrix solve
 * FSUNMASSSPGMR_SETMAXRS - sets the maximum number of restarts to allow for mass matrix solve
 */

void FSUNSPGMR_INIT(int *code, int *pretype, int *maxl, int *ier);
void FSUNSPGMR_SETGSTYPE(int *code, int *gstype, int *ier);
void FSUNSPGMR_SETPRECTYPE(int *code, int *pretype, int *ier);
void FSUNSPGMR_SETMAXRS(int *code, int *maxrs, int *ier);

void FSUNMASSSPGMR_INIT(int *pretype, int *maxl, int *ier);
void FSUNMASSSPGMR_SETGSTYPE(int *gstype, int *ier);
void FSUNMASSSPGMR_SETPRECTYPE(int *pretype, int *ier);
void FSUNMASSSPGMR_SETMAXRS(int *maxrs, int *ier);

#ifdef __cplusplus
}
#endif

#endif
