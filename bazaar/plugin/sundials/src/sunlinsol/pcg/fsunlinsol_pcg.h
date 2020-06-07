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
 * This file (companion of fsunlinsol_pcg.c) contains the
 * definitions needed for the initialization of PCG
 * linear solver operations in Fortran.
 * -----------------------------------------------------------------
 */

#ifndef _FSUNLINSOL_PCG_H
#define _FSUNLINSOL_PCG_H

#include <sunlinsol/sunlinsol_pcg.h>
#include <sundials/sundials_fnvector.h>

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif

#if defined(SUNDIALS_F77_FUNC)
#define FSUNPCG_INIT            SUNDIALS_F77_FUNC(fsunpcginit,            FSUNPCGINIT)
#define FSUNPCG_SETPRECTYPE     SUNDIALS_F77_FUNC(fsunpcgsetprectype,     FSUNPCGSETPRECTYPE)
#define FSUNPCG_SETMAXL         SUNDIALS_F77_FUNC(fsunpcgsetmaxl,         FSUNPCGSETMAXL)
#define FSUNMASSPCG_INIT        SUNDIALS_F77_FUNC(fsunmasspcginit,        FSUNMASSPCGINIT)
#define FSUNMASSPCG_SETPRECTYPE SUNDIALS_F77_FUNC(fsunmasspcgsetprectype, FSUNMASSPCGSETPRECTYPE)
#define FSUNMASSPCG_SETMAXL     SUNDIALS_F77_FUNC(fsunmasspcgsetmaxl,     FSUNMASSPCGSETMAXL)
#else
#define FSUNPCG_INIT            fsunpcginit_
#define FSUNPCG_SETPRECTYPE     fsunpcgsetprectype_
#define FSUNPCG_SETMAXL         fsunpcgsetmaxl_
#define FSUNMASSPCG_INIT        fsunmasspcginit_
#define FSUNMASSPCG_SETPRECTYPE fsunmasspcgsetprectype_
#define FSUNMASSPCG_SETMAXL     fsunmasspcgsetmaxl_
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
 * FSUNPCG_INIT - initializes PCG linear solver for main problem
 * FSUNPCG_SETPRECTYPE - sets preconditioning type for main problem
 * FSUNPCG_SETMAXL - sets the max number of iterations for main problem
 *
 * FSUNMASSPCG_INIT - initializes PCG linear solver for mass matrix solve
 * FSUNMASSPCG_SETPRECTYPE - sets preconditioning type for mass matrix solve
 * FSUNMASSPCG_SETMAXL - sets the max number of iterations for mass matrix solve
 */

void FSUNPCG_INIT(int *code, int *pretype, int *maxl, int *ier);
void FSUNPCG_SETPRECTYPE(int *code, int *pretype, int *ier);
void FSUNPCG_SETMAXL(int *code, int *maxl, int *ier);

void FSUNMASSPCG_INIT(int *pretype, int *maxl, int *ier);
void FSUNMASSPCG_SETPRECTYPE(int *pretype, int *ier);
void FSUNMASSPCG_SETMAXL(int *maxl, int *ier);

#ifdef __cplusplus
}
#endif

#endif
