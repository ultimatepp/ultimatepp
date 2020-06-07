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
 * This file (companion of fsunlinsol_spfgmr.c) contains the
 * definitions needed for the initialization of SPFGMR
 * linear solver operations in Fortran.
 * -----------------------------------------------------------------
 */

#ifndef _FSUNLINSOL_SPFGMR_H
#define _FSUNLINSOL_SPFGMR_H

#include <sunlinsol/sunlinsol_spfgmr.h>
#include <sundials/sundials_fnvector.h>

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif

#if defined(SUNDIALS_F77_FUNC)
#define FSUNSPFGMR_INIT            SUNDIALS_F77_FUNC(fsunspfgmrinit,            FSUNSPFGMRINIT)
#define FSUNSPFGMR_SETGSTYPE       SUNDIALS_F77_FUNC(fsunspfgmrsetgstype,       FSUNSPFGMRSETGSTYPE)
#define FSUNSPFGMR_SETPRECTYPE     SUNDIALS_F77_FUNC(fsunspfgmrsetprectype,     FSUNSPFGMRSETPRECTYPE)
#define FSUNSPFGMR_SETMAXRS        SUNDIALS_F77_FUNC(fsunspfgmrsetmaxrs,        FSUNSPFGMRSETMAXRS)
#define FSUNMASSSPFGMR_INIT        SUNDIALS_F77_FUNC(fsunmassspfgmrinit,        FSUNMASSSPFGMRINIT)
#define FSUNMASSSPFGMR_SETGSTYPE   SUNDIALS_F77_FUNC(fsunmassspfgmrsetgstype,   FSUNMASSSPFGMRSETGSTYPE)
#define FSUNMASSSPFGMR_SETPRECTYPE SUNDIALS_F77_FUNC(fsunmassspfgmrsetprectype, FSUNMASSSPFGMRSETPRECTYPE)
#define FSUNMASSSPFGMR_SETMAXRS    SUNDIALS_F77_FUNC(fsunmassspfgmrsetmaxrs,    FSUNMASSSPFGMRSETMAXRS)
#else
#define FSUNSPFGMR_INIT            fsunspfgmrinit_
#define FSUNSPFGMR_SETGSTYPE       fsunspfgmrsetgstype_
#define FSUNSPFGMR_SETPRECTYPE     fsunspfgmrsetprectype_
#define FSUNSPFGMR_SETMAXRS        fsunspfgmrsetmaxrs_
#define FSUNMASSSPFGMR_INIT        fsunmassspfgmrinit_
#define FSUNMASSSPFGMR_SETGSTYPE   fsunmassspfgmrsetgstype_
#define FSUNMASSSPFGMR_SETPRECTYPE fsunmassspfgmrsetprectype_
#define FSUNMASSSPFGMR_SETMAXRS    fsunmassspfgmrsetmaxrs_
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
 * FSUNSPFGMR_INIT - initializes SPFGMR linear solver for main problem
 * FSUNSPFGMR_SETGSTYPE - sets the Gram-Scmidt orthogonalization type for main problem
 * FSUNSPFGMR_SETPRECTYPE - sets the preconditioning type for main problem
 * FSUNSPFGMR_SETMAXRS - sets the maximum number of restarts to allow for main problem
 *
 * FSUNMASSSPFGMR_INIT - initializes SPFGMR linear solver for mass matrix solve
 * FSUNMASSSPFGMR_SETGSTYPE - sets the Gram-Scmidt orthogonalization type for mass matrix solve
 * FSUNMASSSPFGMR_SETPRECTYPE - sets the preconditioning type for mass matrix solve
 * FSUNMASSSPFGMR_SETMAXRS - sets the maximum number of restarts to allow for mass matrix solve
 */

void FSUNSPFGMR_INIT(int *code, int *pretype, int *maxl, int *ier);
void FSUNSPFGMR_SETGSTYPE(int *code, int *gstype, int *ier);
void FSUNSPFGMR_SETPRECTYPE(int *code, int *pretype, int *ier);
void FSUNSPFGMR_SETMAXRS(int *code, int *maxrs, int *ier);
  
void FSUNMASSSPFGMR_INIT(int *pretype, int *maxl, int *ier);
void FSUNMASSSPFGMR_SETGSTYPE(int *gstype, int *ier);
void FSUNMASSSPFGMR_SETPRECTYPE(int *pretype, int *ier);
void FSUNMASSSPFGMR_SETMAXRS(int *maxrs, int *ier);

#ifdef __cplusplus
}
#endif

#endif
