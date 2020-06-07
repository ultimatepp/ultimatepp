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
 * This file (companion of fsunlinsol_sptfqmr.c) contains the
 * definitions needed for the initialization of SPTFQMR
 * linear solver operations in Fortran.
 * -----------------------------------------------------------------
 */

#ifndef _FSUNLINSOL_SPTFQMR_H
#define _FSUNLINSOL_SPTFQMR_H

#include <sunlinsol/sunlinsol_sptfqmr.h>
#include <sundials/sundials_fnvector.h>

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif

#if defined(SUNDIALS_F77_FUNC)
#define FSUNSPTFQMR_INIT            SUNDIALS_F77_FUNC(fsunsptfqmrinit,            FSUNSPTFQMRINIT)
#define FSUNSPTFQMR_SETPRECTYPE     SUNDIALS_F77_FUNC(fsunsptfqmrsetprectype,     FSUNSPTFQMRSETPRECTYPE)
#define FSUNSPTFQMR_SETMAXL         SUNDIALS_F77_FUNC(fsunsptfqmrsetmaxl,         FSUNSPTFQMRSETMAXL)
#define FSUNMASSSPTFQMR_INIT        SUNDIALS_F77_FUNC(fsunmasssptfqmrinit,        FSUNMASSSPTFQMRINIT)
#define FSUNMASSSPTFQMR_SETPRECTYPE SUNDIALS_F77_FUNC(fsunmasssptfqmrsetprectype, FSUNMASSSPTFQMRSETPRECTYPE)
#define FSUNMASSSPTFQMR_SETMAXL     SUNDIALS_F77_FUNC(fsunmasssptfqmrsetmaxl,     FSUNMASSSPTFQMRSETMAXL)
#else
#define FSUNSPTFQMR_INIT            fsunsptfqmrinit_
#define FSUNSPTFQMR_SETPRECTYPE     fsunsptfqmrsetprectype_
#define FSUNSPTFQMR_SETMAXL         fsunsptfqmrsetmaxl_
#define FSUNMASSSPTFQMR_INIT        fsunmasssptfqmrinit_
#define FSUNMASSSPTFQMR_SETPRECTYPE fsunmasssptfqmrsetprectype_
#define FSUNMASSSPTFQMR_SETMAXL     fsunmasssptfqmrsetmaxl_
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
 * FSUNSPTFQMR_INIT - initializes SPTFQMR linear solver for main problem
 * FSUNSPTFQMR_SETPRECTYPE - sets the preconditioning type for main problem
 * FSUNSPTFQMR_SETMAXL - sets the max number of iterations for main problem
 *
 * FSUNMASSSPTFQMR_INIT - initializes SPTFQMR linear solver for mass matrix solve
 * FSUNMASSSPTFQMR_SETPRECTYPE - sets the preconditioning type for mass matrix solve
 * FSUNMASSSPTFQMR_SETMAXL - sets the max number of iterations for mass matrix solve
 */

void FSUNSPTFQMR_INIT(int *code, int *pretype, int *maxl, int *ier);
void FSUNSPTFQMR_SETPRECTYPE(int *code, int *pretype, int *ier);
void FSUNSPTFQMR_SETMAXL(int *code, int *maxl, int *ier);

void FSUNMASSSPTFQMR_INIT(int *pretype, int *maxl, int *ier);
void FSUNMASSSPTFQMR_SETPRECTYPE(int *pretype, int *ier);
void FSUNMASSSPTFQMR_SETMAXL(int *maxl, int *ier);

#ifdef __cplusplus
}
#endif

#endif
