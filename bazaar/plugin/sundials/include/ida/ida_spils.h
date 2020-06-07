/* -----------------------------------------------------------------
 * Programmer(s): Daniel R. Reynolds @ SMU
 *         Alan Hindmarsh, Radu Serban and Aaron Collier @ LLNL
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
 * Header file for the deprecated Scaled, Preconditioned Iterative
 * Linear Solver interface in IDA; these routines now just wrap
 * the updated IDA generic linear solver interface in ida_ls.h.
 * -----------------------------------------------------------------*/

#ifndef _IDASPILS_H
#define _IDASPILS_H

#include <ida/ida_ls.h>

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif


/*===============================================================
  Function Types (typedefs for equivalent types in ida_ls.h)
  ===============================================================*/

typedef IDALsPrecSetupFn IDASpilsPrecSetupFn;
typedef IDALsPrecSolveFn IDASpilsPrecSolveFn;
typedef IDALsJacTimesSetupFn IDASpilsJacTimesSetupFn;
typedef IDALsJacTimesVecFn IDASpilsJacTimesVecFn;

/*====================================================================
  Exported Functions (wrappers for equivalent routines in ida_ls.h)
  ====================================================================*/

SUNDIALS_EXPORT int IDASpilsSetLinearSolver(void *ida_mem, SUNLinearSolver LS);

SUNDIALS_EXPORT int IDASpilsSetPreconditioner(void *ida_mem, IDASpilsPrecSetupFn pset,
                                              IDASpilsPrecSolveFn psolve);

SUNDIALS_EXPORT int IDASpilsSetJacTimes(void *ida_mem, IDASpilsJacTimesSetupFn jtsetup,
                                        IDASpilsJacTimesVecFn jtimes);

SUNDIALS_EXPORT int IDASpilsSetEpsLin(void *ida_mem, realtype eplifac);

SUNDIALS_EXPORT int IDASpilsSetIncrementFactor(void *ida_mem, realtype dqincfac);

SUNDIALS_EXPORT int IDASpilsGetWorkSpace(void *ida_mem, long int *lenrwLS, long int *leniwLS);

SUNDIALS_EXPORT int IDASpilsGetNumPrecEvals(void *ida_mem, long int *npevals);

SUNDIALS_EXPORT int IDASpilsGetNumPrecSolves(void *ida_mem, long int *npsolves);

SUNDIALS_EXPORT int IDASpilsGetNumLinIters(void *ida_mem, long int *nliters);

SUNDIALS_EXPORT int IDASpilsGetNumConvFails(void *ida_mem, long int *nlcfails);

SUNDIALS_EXPORT int IDASpilsGetNumJTSetupEvals(void *ida_mem, long int *njtsetups);

SUNDIALS_EXPORT int IDASpilsGetNumJtimesEvals(void *ida_mem, long int *njvevals);

SUNDIALS_EXPORT int IDASpilsGetNumResEvals(void *ida_mem, long int *nrevalsLS);

SUNDIALS_EXPORT int IDASpilsGetLastFlag(void *ida_mem, long int *flag);

SUNDIALS_EXPORT char *IDASpilsGetReturnFlagName(long int flag);


#ifdef __cplusplus
}
#endif

#endif
