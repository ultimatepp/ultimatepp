/* -----------------------------------------------------------------
 * Programmer(s): Daniel R. Reynolds @ SMU
 *                Radu Serban @ LLNL
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
 * Header file for the deprecated direct linear solver interface in
 * KINSOL; these routines now just wrap the updated KINSOL generic
 * linear solver interface in kinsol_ls.h.
 * -----------------------------------------------------------------*/

#ifndef _KINDLS_H
#define _KINDLS_H

#include <kinsol/kinsol_ls.h>

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif


/*=================================================================
  Function Types (typedefs for equivalent types in kinsol_ls.h)
  =================================================================*/

typedef KINLsJacFn KINDlsJacFn;

/*===================================================================
  Exported Functions (wrappers for equivalent routines in kinsol_ls.h)
  ===================================================================*/

SUNDIALS_EXPORT int KINDlsSetLinearSolver(void *kinmem, SUNLinearSolver LS, SUNMatrix A);

SUNDIALS_EXPORT int KINDlsSetJacFn(void *kinmem, KINDlsJacFn jac);

SUNDIALS_EXPORT int KINDlsGetWorkSpace(void *kinmem, long int *lenrw, long int *leniw);

SUNDIALS_EXPORT int KINDlsGetNumJacEvals(void *kinmem, long int *njevals);

SUNDIALS_EXPORT int KINDlsGetNumFuncEvals(void *kinmem, long int *nfevals);

SUNDIALS_EXPORT int KINDlsGetLastFlag(void *kinmem, long int *flag);

SUNDIALS_EXPORT char *KINDlsGetReturnFlagName(long int flag);


#ifdef __cplusplus
}
#endif

#endif
