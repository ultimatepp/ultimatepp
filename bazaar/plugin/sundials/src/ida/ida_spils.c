/*----------------------------------------------------------------- 
 * Programmer(s): Daniel R. Reynolds @ SMU
 *         Alan Hindmarsh, Radu Serban and Aaron Collier @ LLNL
 *-----------------------------------------------------------------
 * SUNDIALS Copyright Start
 * Copyright (c) 2002-2020, Lawrence Livermore National Security
 * and Southern Methodist University.
 * All rights reserved.
 *
 * See the top-level LICENSE and NOTICE files for details.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * SUNDIALS Copyright End
 *-----------------------------------------------------------------
 * Implementation file for the deprecated Scaled and Preconditioned 
 * Iterative Linear Solver interface in IDA; these routines now just
 * wrap the updated IDA generic linear solver interface in ida_ls.h.
 *-----------------------------------------------------------------*/

#include <ida/ida_ls.h>
#include <ida/ida_spils.h>

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif

/*=================================================================
  Exported Functions (wrappers for equivalent routines in ida_ls.h)
  =================================================================*/

int IDASpilsSetLinearSolver(void *ida_mem, SUNLinearSolver LS)
{ return(IDASetLinearSolver(ida_mem, LS, NULL)); }

int IDASpilsSetPreconditioner(void *ida_mem, IDASpilsPrecSetupFn pset, 
                              IDASpilsPrecSolveFn psolve)
{ return(IDASetPreconditioner(ida_mem, pset, psolve)); }

int IDASpilsSetJacTimes(void *ida_mem, IDASpilsJacTimesSetupFn jtsetup,
                        IDASpilsJacTimesVecFn jtimes)
{ return(IDASetJacTimes(ida_mem, jtsetup, jtimes)); }

int IDASpilsSetEpsLin(void *ida_mem, realtype eplifac)
{ return(IDASetEpsLin(ida_mem, eplifac)); }

int IDASpilsSetIncrementFactor(void *ida_mem, realtype dqincfac)
{ return(IDASetIncrementFactor(ida_mem, dqincfac)); }

int IDASpilsGetWorkSpace(void *ida_mem, long int *lenrwLS, long int *leniwLS)
{ return(IDAGetLinWorkSpace(ida_mem, lenrwLS, leniwLS)); }

int IDASpilsGetNumPrecEvals(void *ida_mem, long int *npevals)
{ return(IDAGetNumPrecEvals(ida_mem, npevals)); }

int IDASpilsGetNumPrecSolves(void *ida_mem, long int *npsolves)
{ return(IDAGetNumPrecSolves(ida_mem, npsolves)); }

int IDASpilsGetNumLinIters(void *ida_mem, long int *nliters)
{ return(IDAGetNumLinIters(ida_mem, nliters)); }
  
int IDASpilsGetNumConvFails(void *ida_mem, long int *nlcfails)
{ return(IDAGetNumLinConvFails(ida_mem, nlcfails)); }
  
int IDASpilsGetNumJTSetupEvals(void *ida_mem, long int *njtsetups)
{ return(IDAGetNumJTSetupEvals(ida_mem, njtsetups)); }
  
int IDASpilsGetNumJtimesEvals(void *ida_mem, long int *njvevals)
{ return(IDAGetNumJtimesEvals(ida_mem, njvevals)); }
  
int IDASpilsGetNumResEvals(void *ida_mem, long int *nrevalsLS)
{ return(IDAGetNumLinResEvals(ida_mem, nrevalsLS)); }
  
int IDASpilsGetLastFlag(void *ida_mem, long int *flag)
{ return(IDAGetLastLinFlag(ida_mem, flag)); }
  
char *IDASpilsGetReturnFlagName(long int flag)
{ return(IDAGetLinReturnFlagName(flag)); }

#ifdef __cplusplus
}
#endif

