/*----------------------------------------------------------------- 
 * Programmer(s): Daniel R. Reynolds @ SMU
 *                Radu Serban @ LLNL
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
 * Implementation file for the deprecated direct linear solver interface in 
 * KINSOL; these routines now just wrap the updated KINSOL generic
 * linear solver interface in kinsol_ls.h.
 *-----------------------------------------------------------------*/

#include <kinsol/kinsol_ls.h>
#include <kinsol/kinsol_direct.h>

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif
  
/*=================================================================
  Exported Functions (wrappers for equivalent routines in kinsol_ls.h)
  =================================================================*/
  
int KINDlsSetLinearSolver(void *kinmem, SUNLinearSolver LS, SUNMatrix A)
{ return(KINSetLinearSolver(kinmem, LS, A)); }

int KINDlsSetJacFn(void *kinmem, KINDlsJacFn jac)
{ return(KINSetJacFn(kinmem, jac)); }

int KINDlsGetWorkSpace(void *kinmem, long int *lenrw, long int *leniw)
{ return(KINGetLinWorkSpace(kinmem, lenrw, leniw)); }

int KINDlsGetNumJacEvals(void *kinmem, long int *njevals)
{ return(KINGetNumJacEvals(kinmem, njevals)); }

int KINDlsGetNumFuncEvals(void *kinmem, long int *nfevals)
{ return(KINGetNumLinFuncEvals(kinmem, nfevals)); }

int KINDlsGetLastFlag(void *kinmem, long int *flag)
{ return(KINGetLastLinFlag(kinmem, flag)); }

char *KINDlsGetReturnFlagName(long int flag)
{ return(KINGetLinReturnFlagName(flag)); }

#ifdef __cplusplus
}
#endif

