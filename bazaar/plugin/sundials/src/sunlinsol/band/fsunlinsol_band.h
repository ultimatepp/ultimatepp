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
 * This file (companion of fsunlinsol_band.c) contains the
 * definitions needed for the initialization of band
 * linear solver operations in Fortran.
 * -----------------------------------------------------------------
 */

#ifndef _FSUNLINSOL_BAND_H
#define _FSUNLINSOL_BAND_H

#include <sunlinsol/sunlinsol_band.h>
#include <sundials/sundials_fnvector.h>

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif

#if defined(SUNDIALS_F77_FUNC)
#define FSUNBANDLINSOL_INIT     SUNDIALS_F77_FUNC(fsunbandlinsolinit, FSUNBANDLINSOLINIT)
#define FSUNMASSBANDLINSOL_INIT SUNDIALS_F77_FUNC(fsunmassbandlinsolinit, FSUNMASSBANDLINSOLINIT)
#else
#define FSUNBANDLINSOL_INIT     fsunbandlinsolinit_
#define FSUNMASSBANDLINSOL_INIT fsunmassbandlinsolinit_
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
 * FSUNBANDLINSOL_INIT - initializes band linear solver for main problem
 * FSUNMASSBANDLINSOL_INIT - initializes band linear solver for mass matrix solve
 */

void FSUNBANDLINSOL_INIT(int *code, int *ier);
void FSUNMASSBANDLINSOL_INIT(int *ier);

#ifdef __cplusplus
}
#endif

#endif
