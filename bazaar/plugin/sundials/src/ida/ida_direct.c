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
 * IDA; these routines now just wrap the updated IDA generic
 * linear solver interface in ida_ls.h.
 *-----------------------------------------------------------------*/

#include <ida/ida_ls.h>
#include <ida/ida_direct.h>

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif

/*=================================================================
  Exported Functions (wrappers for equivalent routines in ida_ls.h)
  =================================================================*/

int IDADlsSetLinearSolver(void *ida_mem, SUNLinearSolver LS, SUNMatrix A)
{ return(IDASetLinearSolver(ida_mem, LS, A)); }

int IDADlsSetJacFn(void *ida_mem, IDADlsJacFn jac)
{ return(IDASetJacFn(ida_mem, jac)); }

int IDADlsGetWorkSpace(void *ida_mem, long int *lenrwLS, long int *leniwLS)
{ return(IDAGetLinWorkSpace(ida_mem, lenrwLS, leniwLS)); }
  
int IDADlsGetNumJacEvals(void *ida_mem, long int *njevals)
{ return(IDAGetNumJacEvals(ida_mem, njevals)); }
  
int IDADlsGetNumResEvals(void *ida_mem, long int *nfevalsLS)
{ return(IDAGetNumLinResEvals(ida_mem, nfevalsLS)); }
  
int IDADlsGetLastFlag(void *ida_mem, long int *flag)
{ return(IDAGetLastLinFlag(ida_mem, flag)); }

char *IDADlsGetReturnFlagName(long int flag)
{ return(IDAGetLinReturnFlagName(flag)); }


#ifdef __cplusplus
}
#endif

