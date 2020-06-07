/*-----------------------------------------------------------------
 * Programmer(s): Daniel R. Reynolds @ SMU
 *                Scott Cohen, Alan Hindmarsh, Radu Serban, 
 *                  and Aaron Collier @ LLNL
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
 * Header file for the deprecated Scaled Preconditioned Iterative 
 * Linear Solver interface in KINSOL; these routines now just wrap 
 * the updated KINSOL generic linear solver interface in kinsol_ls.h.
 *-----------------------------------------------------------------*/

#include <kinsol/kinsol_ls.h>
#include <kinsol/kinsol_spils.h>

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif

/*=================================================================
  Exported Functions (wrappers for equivalent routines in kinsol_ls.h)
  =================================================================*/

int KINSpilsSetLinearSolver(void *kinmem, SUNLinearSolver LS)
{ return(KINSetLinearSolver(kinmem, LS, NULL)); }

int KINSpilsSetPreconditioner(void *kinmem, KINSpilsPrecSetupFn psetup,
                              KINSpilsPrecSolveFn psolve)
{ return(KINSetPreconditioner(kinmem, psetup, psolve)); }
  
int KINSpilsSetJacTimesVecFn(void *kinmem, KINSpilsJacTimesVecFn jtv)
{ return(KINSetJacTimesVecFn(kinmem, jtv)); }

int KINSpilsGetWorkSpace(void *kinmem, long int *lenrwLS, long int *leniwLS)
{ return(KINGetLinWorkSpace(kinmem, lenrwLS, leniwLS)); }
  
int KINSpilsGetNumPrecEvals(void *kinmem, long int *npevals)
{ return(KINGetNumPrecEvals(kinmem, npevals)); }
  
int KINSpilsGetNumPrecSolves(void *kinmem, long int *npsolves)
{ return(KINGetNumPrecSolves(kinmem, npsolves)); }
  
int KINSpilsGetNumLinIters(void *kinmem, long int *nliters)
{ return(KINGetNumLinIters(kinmem, nliters)); }
  
int KINSpilsGetNumConvFails(void *kinmem, long int *nlcfails)
{ return(KINGetNumLinConvFails(kinmem, nlcfails)); }
  
int KINSpilsGetNumJtimesEvals(void *kinmem, long int *njvevals)
{ return(KINGetNumJtimesEvals(kinmem, njvevals)); }
  
int KINSpilsGetNumFuncEvals(void *kinmem, long int *nfevals)
{ return(KINGetNumLinFuncEvals(kinmem, nfevals)); }
  
int KINSpilsGetLastFlag(void *kinmem, long int *flag)
{ return(KINGetLastLinFlag(kinmem, flag)); }
  
char *KINSpilsGetReturnFlagName(long int flag)
{ return(KINGetLinReturnFlagName(flag)); }


#ifdef __cplusplus
}
#endif

