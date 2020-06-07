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
 * This file (companion of fsunlinsol_spbcgs.c) contains the
 * definitions needed for the initialization of SPBCGS
 * linear solver operations in Fortran.
 * -----------------------------------------------------------------
 */

#ifndef _FSUNLINSOL_SPBCGS_H
#define _FSUNLINSOL_SPBCGS_H

#include <sunlinsol/sunlinsol_spbcgs.h>
#include <sundials/sundials_fnvector.h>

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif

#if defined(SUNDIALS_F77_FUNC)
#define FSUNSPBCGS_INIT            SUNDIALS_F77_FUNC(fsunspbcgsinit,            FSUNSPBCGSINIT)
#define FSUNSPBCGS_SETPRECTYPE     SUNDIALS_F77_FUNC(fsunspbcgssetprectype,     FSUNSPBCGSSETPRECTYPE)
#define FSUNSPBCGS_SETMAXL         SUNDIALS_F77_FUNC(fsunspbcgssetmaxl,         FSUNSPBCGSSETMAXL)
#define FSUNMASSSPBCGS_INIT        SUNDIALS_F77_FUNC(fsunmassspbcgsinit,        FSUNMASSSPBCGSINIT)
#define FSUNMASSSPBCGS_SETPRECTYPE SUNDIALS_F77_FUNC(fsunmassspbcgssetprectype, FSUNMASSSPBCGSSETPRECTYPE)
#define FSUNMASSSPBCGS_SETMAXL     SUNDIALS_F77_FUNC(fsunmassspbcgssetmaxl,     FSUNMASSSPBCGSSETMAXL)
#else
#define FSUNSPBCGS_INIT            fsunspbcgsinit_
#define FSUNSPBCGS_SETPRECTYPE     fsunspbcgssetprectype_
#define FSUNSPBCGS_SETMAXL         fsunspbcgssetmaxl_
#define FSUNMASSSPBCGS_INIT        fsunmassspbcgsinit_
#define FSUNMASSSPBCGS_SETPRECTYPE fsunmassspbcgssetprectype_
#define FSUNMASSSPBCGS_SETMAXL     fsunmassspbcgssetmaxl_
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
 * FSUNSPBCGS_INIT - initializes SPBCGS linear solver for main problem
 * FSUNSPBCGS_SETPRECTYPE - sets the preconditioning type for main problem
 * FSUNSPBCGS_SETMAXL - sets the max number of iterations for main problem
 *
 * FSUNMASSSPBCGS_INIT - initializes SPBCGS linear solver for mass matrix solve
 * FSUNMASSSPBCGS_SETPRECTYPE - sets the preconditioning type for mass matrix solve
 * FSUNMASSSPBCGS_SETMAXL - sets the max number of iterations for mass matrix solve
 */

void FSUNSPBCGS_INIT(int *code, int *pretype, int *maxl, int *ier);
void FSUNSPBCGS_SETPRECTYPE(int *code, int *pretype, int *ier);
void FSUNSPBCGS_SETMAXL(int *code, int *maxl, int *ier);

void FSUNMASSSPBCGS_INIT(int *pretype, int *maxl, int *ier);
void FSUNMASSSPBCGS_SETPRECTYPE(int *pretype, int *ier);
void FSUNMASSSPBCGS_SETMAXL(int *maxl, int *ier);

#ifdef __cplusplus
}
#endif

#endif
