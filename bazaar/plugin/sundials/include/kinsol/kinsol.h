/* -----------------------------------------------------------------
 * Programmer(s): Allan Taylor, Alan Hindmarsh, Radu Serban, and
 *                Aaron Collier @ LLNL
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
 * This is the header file for the main KINSOL solver.
 * -----------------------------------------------------------------*/

#ifndef _KINSOL_H
#define _KINSOL_H

#include <stdio.h>
#include <sundials/sundials_nvector.h>
#include <kinsol/kinsol_ls.h>

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif

/* -----------------
 * KINSOL Constants
 * ----------------- */

/*  return values */

#define KIN_SUCCESS             0
#define KIN_INITIAL_GUESS_OK    1
#define KIN_STEP_LT_STPTOL      2

#define KIN_WARNING             99

#define KIN_MEM_NULL            -1
#define KIN_ILL_INPUT           -2
#define KIN_NO_MALLOC           -3
#define KIN_MEM_FAIL            -4
#define KIN_LINESEARCH_NONCONV  -5
#define KIN_MAXITER_REACHED     -6
#define KIN_MXNEWT_5X_EXCEEDED  -7
#define KIN_LINESEARCH_BCFAIL   -8
#define KIN_LINSOLV_NO_RECOVERY -9
#define KIN_LINIT_FAIL          -10
#define KIN_LSETUP_FAIL         -11
#define KIN_LSOLVE_FAIL         -12

#define KIN_SYSFUNC_FAIL        -13
#define KIN_FIRST_SYSFUNC_ERR   -14
#define KIN_REPTD_SYSFUNC_ERR   -15

#define KIN_VECTOROP_ERR        -16

/* Enumeration for eta choice */
#define KIN_ETACHOICE1  1
#define KIN_ETACHOICE2  2
#define KIN_ETACONSTANT 3

/* Enumeration for global strategy */
#define KIN_NONE       0
#define KIN_LINESEARCH 1
#define KIN_PICARD     2
#define KIN_FP         3

/* ------------------------------
 * User-Supplied Function Types
 * ------------------------------ */

typedef int (*KINSysFn)(N_Vector uu, N_Vector fval, void *user_data );

typedef void (*KINErrHandlerFn)(int error_code,
                                const char *module, const char *function,
                                char *msg, void *user_data);

typedef void (*KINInfoHandlerFn)(const char *module, const char *function,
                                 char *msg, void *user_data);

/* -------------------
 * Exported Functions
 * ------------------- */

/* Creation function */
SUNDIALS_EXPORT void *KINCreate(void);

/* Initialization function */
SUNDIALS_EXPORT int KINInit(void *kinmem, KINSysFn func, N_Vector tmpl);

/* Solver function */
SUNDIALS_EXPORT int KINSol(void *kinmem, N_Vector uu, int strategy,
                           N_Vector u_scale, N_Vector f_scale);

/* Optional input functions */
SUNDIALS_EXPORT int KINSetErrHandlerFn(void *kinmem, KINErrHandlerFn ehfun,
                                       void *eh_data);
SUNDIALS_EXPORT int KINSetErrFile(void *kinmem, FILE *errfp);
SUNDIALS_EXPORT int KINSetInfoHandlerFn(void *kinmem, KINInfoHandlerFn ihfun,
                                        void *ih_data);
SUNDIALS_EXPORT int KINSetInfoFile(void *kinmem, FILE *infofp);
SUNDIALS_EXPORT int KINSetUserData(void *kinmem, void *user_data);
SUNDIALS_EXPORT int KINSetPrintLevel(void *kinmemm, int printfl);
SUNDIALS_EXPORT int KINSetMAA(void *kinmem, long int maa);
SUNDIALS_EXPORT int KINSetDampingAA(void *kinmem, realtype beta);
SUNDIALS_EXPORT int KINSetNumMaxIters(void *kinmem, long int mxiter);
SUNDIALS_EXPORT int KINSetNoInitSetup(void *kinmem, booleantype noInitSetup);
SUNDIALS_EXPORT int KINSetNoResMon(void *kinmem, booleantype noNNIResMon);
SUNDIALS_EXPORT int KINSetMaxSetupCalls(void *kinmem, long int msbset);
SUNDIALS_EXPORT int KINSetMaxSubSetupCalls(void *kinmem, long int msbsetsub);
SUNDIALS_EXPORT int KINSetEtaForm(void *kinmem, int etachoice);
SUNDIALS_EXPORT int KINSetEtaConstValue(void *kinmem, realtype eta);
SUNDIALS_EXPORT int KINSetEtaParams(void *kinmem, realtype egamma,
                                    realtype ealpha);
SUNDIALS_EXPORT int KINSetResMonParams(void *kinmem, realtype omegamin,
                                       realtype omegamax);
SUNDIALS_EXPORT int KINSetResMonConstValue(void *kinmem, realtype omegaconst);
SUNDIALS_EXPORT int KINSetNoMinEps(void *kinmem, booleantype noMinEps);
SUNDIALS_EXPORT int KINSetMaxNewtonStep(void *kinmem, realtype mxnewtstep);
SUNDIALS_EXPORT int KINSetMaxBetaFails(void *kinmem, long int mxnbcf);
SUNDIALS_EXPORT int KINSetRelErrFunc(void *kinmem, realtype relfunc);
SUNDIALS_EXPORT int KINSetFuncNormTol(void *kinmem, realtype fnormtol);
SUNDIALS_EXPORT int KINSetScaledStepTol(void *kinmem, realtype scsteptol);
SUNDIALS_EXPORT int KINSetConstraints(void *kinmem, N_Vector constraints);
SUNDIALS_EXPORT int KINSetSysFunc(void *kinmem, KINSysFn func);


/* Optional output functions */
SUNDIALS_EXPORT int KINGetWorkSpace(void *kinmem, long int *lenrw,
                                    long int *leniw);
SUNDIALS_EXPORT int KINGetNumNonlinSolvIters(void *kinmem, long int *nniters);
SUNDIALS_EXPORT int KINGetNumFuncEvals(void *kinmem, long int *nfevals);
SUNDIALS_EXPORT int KINGetNumBetaCondFails(void *kinmem, long int *nbcfails);
SUNDIALS_EXPORT int KINGetNumBacktrackOps(void *kinmem, long int *nbacktr);
SUNDIALS_EXPORT int KINGetFuncNorm(void *kinmem, realtype *fnorm);
SUNDIALS_EXPORT int KINGetStepLength(void *kinmem, realtype *steplength);
SUNDIALS_EXPORT char *KINGetReturnFlagName(long int flag);

/* Free function */
SUNDIALS_EXPORT void KINFree(void **kinmem);


#ifdef __cplusplus
}
#endif

#endif
