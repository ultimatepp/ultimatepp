/*-----------------------------------------------------------------
 * Programmer(s): Daniel R. Reynolds @ SMU
 *                David J. Gardner, Radu Serban and Aaron Collier @ LLNL
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
 * Implementation header file for KINSOL's linear solver interface.
 *-----------------------------------------------------------------*/

#ifndef _KINLS_IMPL_H
#define _KINLS_IMPL_H

#include <kinsol/kinsol_ls.h>
#include "kinsol_impl.h"

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif


/*------------------------------------------------------------------
  keys for KINPrintInfo (do not use 1 -> conflict with PRNT_RETVAL)
  ------------------------------------------------------------------*/
#define PRNT_NLI   101
#define PRNT_EPS   102


/*------------------------------------------------------------------
  Types : struct KINLsMemRec, struct *KINLsMem

  The type KINLsMem is a pointer to a KINLsMemRec, which is a
  structure containing fields that must be accessible by LS module
  routines.
  ------------------------------------------------------------------*/
typedef struct KINLsMemRec {

  /* Linear solver type information */
  booleantype iterative;    /* is the solver iterative?    */
  booleantype matrixbased;  /* is a matrix structure used? */

  /* Jacobian construction & storage */
  booleantype jacDQ;   /* SUNTRUE if using internal DQ Jacobian approx. */
  KINLsJacFn jac;      /* Jacobian routine to be called                 */
  void *J_data;        /* J_data is passed to jac                       */

  /* Linear solver, matrix and vector objects/pointers */
  SUNLinearSolver LS;  /* generic iterative linear solver object        */
  SUNMatrix J;         /* problem Jacobian                              */

  /* Solver tolerance adjustment factor (if needed, see kinLsSolve)     */
  realtype tol_fac;

  /* Statistics and associated parameters */
  long int nje;        /* no. of calls to jac                           */
  long int nfeDQ;      /* no. of calls to F due to DQ Jacobian or J*v
                          approximations                                */
  long int npe;        /* npe = total number of precond calls           */
  long int nli;        /* nli = total number of linear iterations       */
  long int nps;        /* nps = total number of psolve calls            */
  long int ncfl;       /* ncfl = total number of convergence failures   */
  long int njtimes;    /* njtimes = total number of calls to jtimes     */

  booleantype new_uu;  /* flag indicating if the iterate has been
                          updated - the Jacobian must be updated or
                          reevaluated (meant to be used by a
                          user-supplied jtimes function                 */

  int last_flag;       /* last error return flag                        */

  /* Preconditioner computation
     (a) user-provided:
         - pdata == user_data
         - pfree == NULL (the user dealocates memory)
     (b) internal preconditioner module
         - pdata == kin_mem
         - pfree == set by the prec. module and called in kinLsFree */
  KINLsPrecSetupFn pset;
  KINLsPrecSolveFn psolve;
  int (*pfree)(KINMem kin_mem);
  void *pdata;

  /* Jacobian times vector compuation
     (a) jtimes function provided by the user:
         - jt_data == user_data
         - jtimesDQ == SUNFALSE
     (b) internal jtimes
         - jt_data == kin_mem
         - jtimesDQ == SUNTRUE */
  booleantype jtimesDQ;
  KINLsJacTimesVecFn jtimes;
  void *jt_data;

} *KINLsMem;


/*------------------------------------------------------------------
  Prototypes of internal functions
  ------------------------------------------------------------------*/

/* Interface routines called by system SUNLinearSolvers */
int kinLsATimes(void *kinmem, N_Vector v, N_Vector z);
int kinLsPSetup(void *kinmem);
int kinLsPSolve(void *kinmem, N_Vector r, N_Vector z,
                realtype tol, int lr);

/* Difference quotient approximation for Jacobian times vector */
int kinLsDQJtimes(N_Vector v, N_Vector Jv, N_Vector u,
                  booleantype *new_u, void *data);

/* Difference-quotient Jacobian approximation routines */
int kinLsDQJac(N_Vector u, N_Vector fu, SUNMatrix Jac,
               void *data, N_Vector tmp1, N_Vector tmp2);

int kinLsDenseDQJac(N_Vector u, N_Vector fu, SUNMatrix Jac,
                    KINMem kin_mem, N_Vector tmp1, N_Vector tmp2);

int kinLsBandDQJac(N_Vector u, N_Vector fu, SUNMatrix Jac,
                   KINMem kin_mem, N_Vector tmp1, N_Vector tmp2);

/* Generic linit/lsetup/lsolve/lfree interface routines for KINSOL to call */
int kinLsInitialize(KINMem kin_mem);
int kinLsSetup(KINMem kin_mem);
int kinLsSolve(KINMem kin_mem, N_Vector x, N_Vector b,
               realtype *sJpnorm, realtype *sFdotJp);
int kinLsFree(KINMem kin_mem);

/* Auxilliary functions */
int kinLsInitializeCounters(KINLsMem kinls_mem);
int kinLs_AccessLMem(void* kinmem, const char *fname,
                     KINMem* kin_mem, KINLsMem *kinls_mem);


/*------------------------------------------------------------------
  Error messages
  ------------------------------------------------------------------*/

#define MSG_LS_KINMEM_NULL    "KINSOL memory is NULL."
#define MSG_LS_MEM_FAIL       "A memory request failed."
#define MSG_LS_BAD_NVECTOR    "A required vector operation is not implemented."
#define MSG_LS_LMEM_NULL      "Linear solver memory is NULL."
#define MSG_LS_NEG_MAXRS      "maxrs < 0 illegal."
#define MSG_LS_BAD_SIZES      "Illegal bandwidth parameter(s). Must have 0 <=  ml, mu <= N-1."

#define MSG_LS_JACFUNC_FAILED "The Jacobian routine failed in an unrecoverable manner."
#define MSG_LS_PSET_FAILED    "The preconditioner setup routine failed in an unrecoverable manner."
#define MSG_LS_PSOLVE_FAILED  "The preconditioner solve routine failed in an unrecoverable manner."
#define MSG_LS_JTIMES_FAILED  "The Jacobian x vector routine failed in an unrecoverable manner."
#define MSG_LS_MATZERO_FAILED "The SUNMatZero routine failed in an unrecoverable manner."


/*------------------------------------------------------------------
  Info messages
  ------------------------------------------------------------------*/

#define INFO_NLI  "nli_inc = %d"

#if defined(SUNDIALS_EXTENDED_PRECISION)

#define INFO_EPS  "residual norm = %12.3Lg  eps = %12.3Lg"

#elif defined(SUNDIALS_DOUBLE_PRECISION)

#define INFO_EPS  "residual norm = %12.3lg  eps = %12.3lg"

#else

#define INFO_EPS  "residual norm = %12.3g  eps = %12.3g"

#endif


#ifdef __cplusplus
}
#endif

#endif
