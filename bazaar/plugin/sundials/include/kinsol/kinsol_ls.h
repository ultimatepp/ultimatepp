/* ----------------------------------------------------------------
 * Programmer(s): Daniel R. Reynolds @ SMU
 *                Scott Cohen, Alan Hindmarsh, Radu Serban, and
 *                Aaron Collier @ LLNL
 * ----------------------------------------------------------------
 * SUNDIALS Copyright Start
 * Copyright (c) 2002-2020, Lawrence Livermore National Security
 * and Southern Methodist University.
 * All rights reserved.
 *
 * See the top-level LICENSE and NOTICE files for details.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * SUNDIALS Copyright End
 * ----------------------------------------------------------------
 * This is the header file for KINSOL's linear solver interface.
 * ----------------------------------------------------------------*/

#ifndef _KINLS_H
#define _KINLS_H

#include <sundials/sundials_direct.h>
#include <sundials/sundials_iterative.h>
#include <sundials/sundials_linearsolver.h>
#include <sundials/sundials_matrix.h>
#include <sundials/sundials_nvector.h>

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif


/*==================================================================
  KINLS Constants
  ==================================================================*/

#define KINLS_SUCCESS      0

#define KINLS_MEM_NULL    -1
#define KINLS_LMEM_NULL   -2
#define KINLS_ILL_INPUT   -3
#define KINLS_MEM_FAIL    -4
#define KINLS_PMEM_NULL   -5
#define KINLS_JACFUNC_ERR -6
#define KINLS_SUNMAT_FAIL -7
#define KINLS_SUNLS_FAIL  -8


/*===============================================================
  KINLS user-supplied function prototypes
  ===============================================================*/

typedef int (*KINLsJacFn)(N_Vector u, N_Vector fu, SUNMatrix J,
                          void *user_data, N_Vector tmp1, N_Vector tmp2);

typedef int (*KINLsPrecSetupFn)(N_Vector uu, N_Vector uscale,
                                N_Vector fval, N_Vector fscale,
                                void *user_data);

typedef int (*KINLsPrecSolveFn)(N_Vector uu, N_Vector uscale,
                                N_Vector fval, N_Vector fscale,
                                N_Vector vv, void *user_data);

typedef int (*KINLsJacTimesVecFn)(N_Vector v, N_Vector Jv, N_Vector uu,
                                  booleantype *new_uu, void *J_data);


/*==================================================================
  KINLS Exported functions
  ==================================================================*/

SUNDIALS_EXPORT int KINSetLinearSolver(void *kinmem, SUNLinearSolver LS,
                                       SUNMatrix A);


/*-----------------------------------------------------------------
  Optional inputs to the KINLS linear solver interface
  -----------------------------------------------------------------*/

SUNDIALS_EXPORT int KINSetJacFn(void *kinmem, KINLsJacFn jac);
SUNDIALS_EXPORT int KINSetPreconditioner(void *kinmem,
                                         KINLsPrecSetupFn psetup,
                                         KINLsPrecSolveFn psolve);
SUNDIALS_EXPORT int KINSetJacTimesVecFn(void *kinmem,
                                        KINLsJacTimesVecFn jtv);

/*-----------------------------------------------------------------
  Optional outputs from the KINLS linear solver interface
  -----------------------------------------------------------------*/

SUNDIALS_EXPORT int KINGetLinWorkSpace(void *kinmem,
                                       long int *lenrwLS,
                                       long int *leniwLS);
SUNDIALS_EXPORT int KINGetNumJacEvals(void *kinmem,
                                      long int *njevals);
SUNDIALS_EXPORT int KINGetNumLinFuncEvals(void *kinmem,
                                          long int *nfevals);
SUNDIALS_EXPORT int KINGetNumPrecEvals(void *kinmem,
                                       long int *npevals);
SUNDIALS_EXPORT int KINGetNumPrecSolves(void *kinmem,
                                        long int *npsolves);
SUNDIALS_EXPORT int KINGetNumLinIters(void *kinmem,
                                      long int *nliters);
SUNDIALS_EXPORT int KINGetNumLinConvFails(void *kinmem,
                                          long int *nlcfails);
SUNDIALS_EXPORT int KINGetNumJtimesEvals(void *kinmem,
                                         long int *njvevals);
SUNDIALS_EXPORT int KINGetLastLinFlag(void *kinmem,
                                      long int *flag);
SUNDIALS_EXPORT char *KINGetLinReturnFlagName(long int flag);


#ifdef __cplusplus
}
#endif

#endif
