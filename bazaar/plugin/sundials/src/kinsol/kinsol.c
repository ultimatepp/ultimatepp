/*
 * -----------------------------------------------------------------
 * Programmer(s): Allan Taylor, Alan Hindmarsh, Radu Serban, Carol Woodward,
 *                John Loffeld, and Aaron Collier @ LLNL
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
 * This is the implementation file for the main KINSol solver.
 * It is independent of the KINSol linear solver in use.
 * -----------------------------------------------------------------
 *
 * EXPORTED FUNCTIONS
 * ------------------
 *   Creation and allocation functions
 *     KINCreate
 *     KINInit
 *   Main solver function
 *     KINSol
 *   Deallocation function
 *     KINFree
 *
 * PRIVATE FUNCTIONS
 * -----------------
 *     KINCheckNvector
 *   Memory allocation/deallocation
 *     KINAllocVectors
 *     KINFreeVectors
 *   Initial setup
 *     KINSolInit
 *   Step functions
 *     KINLinSolDrv
 *     KINFullNewton
 *     KINLineSearch
 *     KINConstraint
 *     KINFP
 *     KINPicardAA
 *   Stopping tests
 *     KINStop
 *     KINForcingTerm
 *   Norm functions
 *     KINScFNorm
 *     KINScSNorm
 *   KINSOL Verbose output functions
 *     KINPrintInfo
 *     KINInfoHandler
 *   KINSOL Error Handling functions
 *     KINProcessError
 *     KINErrHandler
 * -----------------------------------------------------------------
 */

/*
 * =================================================================
 * IMPORTED HEADER FILES
 * =================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <math.h>

#include "kinsol_impl.h"
#include <sundials/sundials_math.h>

/*
 * =================================================================
 * KINSOL PRIVATE CONSTANTS
 * =================================================================
 */

#define HALF      RCONST(0.5)
#define ZERO      RCONST(0.0)
#define ONE       RCONST(1.0)
#define ONEPT5    RCONST(1.5)
#define TWO       RCONST(2.0)
#define THREE     RCONST(3.0)
#define FIVE      RCONST(5.0)
#define TWELVE    RCONST(12.0)
#define POINT1    RCONST(0.1)
#define POINT01   RCONST(0.01)
#define POINT99   RCONST(0.99)
#define THOUSAND  RCONST(1000.0)
#define ONETHIRD  RCONST(0.3333333333333333)
#define TWOTHIRDS RCONST(0.6666666666666667)
#define POINT9    RCONST(0.9)
#define POINT0001 RCONST(0.0001)

/*
 * =================================================================
 * KINSOL ROUTINE-SPECIFIC CONSTANTS
 * =================================================================
 */

/*
 * Control constants for lower-level functions used by KINSol
 * ----------------------------------------------------------
 *
 * KINStop return value requesting more iterations
 *    RETRY_ITERATION
 *    CONTINUE_ITERATIONS
 *
 * KINFullNewton, KINLineSearch, KINFP, and KINPicardAA return values:
 *    KIN_SUCCESS
 *    KIN_SYSFUNC_FAIL
 *    STEP_TOO_SMALL
 *
 * KINConstraint return values:
 *    KIN_SUCCESS
 *    CONSTR_VIOLATED
 */

#define RETRY_ITERATION     -998
#define CONTINUE_ITERATIONS -999
#define STEP_TOO_SMALL      -997
#define CONSTR_VIOLATED     -996

/*
 * Algorithmic constants
 * ---------------------
 *
 * MAX_RECVR   max. no. of attempts to correct a recoverable func error
 */

#define MAX_RECVR      5

/*
 * Keys for KINPrintInfo
 * ---------------------
 */

#define PRNT_RETVAL     1
#define PRNT_NNI        2
#define PRNT_TOL        3
#define PRNT_FMAX       4
#define PRNT_PNORM      5
#define PRNT_PNORM1     6
#define PRNT_FNORM      7
#define PRNT_LAM        8
#define PRNT_ALPHA      9
#define PRNT_BETA      10
#define PRNT_ALPHABETA 11
#define PRNT_ADJ       12

/*
 * =================================================================
 * PRIVATE FUNCTION PROTOTYPES
 * =================================================================
 */

static booleantype KINCheckNvector(N_Vector tmpl);
static booleantype KINAllocVectors(KINMem kin_mem, N_Vector tmpl);
static int KINSolInit(KINMem kin_mem);
static int KINConstraint(KINMem kin_mem );
static void KINForcingTerm(KINMem kin_mem, realtype fnormp);
static void KINFreeVectors(KINMem kin_mem);

static int  KINFullNewton(KINMem kin_mem, realtype *fnormp,
                          realtype *f1normp, booleantype *maxStepTaken);
static int  KINLineSearch(KINMem kin_mem, realtype *fnormp,
                          realtype *f1normp, booleantype *maxStepTaken);
static int  KINPicardAA(KINMem kin_mem, long int *iter, realtype *R,
                        realtype *gamma, realtype *fmax);
static int  KINFP(KINMem kin_mem);

static int  KINLinSolDrv(KINMem kinmem);
static int  KINPicardFcnEval(KINMem kin_mem, N_Vector gval, N_Vector uval,
                             N_Vector fval1);
static realtype KINScFNorm(KINMem kin_mem, N_Vector v, N_Vector scale);
static realtype KINScSNorm(KINMem kin_mem, N_Vector v, N_Vector u);
static int KINStop(KINMem kin_mem, booleantype maxStepTaken,
                   int sflag);
static int AndersonAcc(KINMem kin_mem, N_Vector gval, N_Vector fv, N_Vector x,
                       N_Vector x_old, long int iter, realtype *R, realtype *gamma);

/*
 * =================================================================
 * EXPORTED FUNCTIONS IMPLEMENTATION
 * =================================================================
 */

/*
 * -----------------------------------------------------------------
 * Creation and allocation functions
 * -----------------------------------------------------------------
 */

/*
 * Function : KINCreate
 *
 * KINCreate creates an internal memory block for a problem to
 * be solved by KINSOL. If successful, KINCreate returns a pointer
 * to the problem memory. This pointer should be passed to
 * KINInit. If an initialization error occurs, KINCreate prints
 * an error message to standard error and returns NULL.
 */

void *KINCreate(void)
{
  KINMem kin_mem;
  realtype uround;

  kin_mem = NULL;
  kin_mem = (KINMem) malloc(sizeof(struct KINMemRec));
  if (kin_mem == NULL) {
    KINProcessError(kin_mem, 0, "KINSOL", "KINCreate", MSG_MEM_FAIL);
    return(NULL);
  }

  /* Zero out kin_mem */
  memset(kin_mem, 0, sizeof(struct KINMemRec));

  /* set uround (unit roundoff) */

  kin_mem->kin_uround = uround = UNIT_ROUNDOFF;

  /* set default values for solver optional inputs */

  kin_mem->kin_func             = NULL;
  kin_mem->kin_user_data        = NULL;
  kin_mem->kin_uu               = NULL;
  kin_mem->kin_unew             = NULL;
  kin_mem->kin_fval             = NULL;
  kin_mem->kin_gval             = NULL;
  kin_mem->kin_uscale           = NULL;
  kin_mem->kin_fscale           = NULL;
  kin_mem->kin_pp               = NULL;
  kin_mem->kin_constraints      = NULL;
  kin_mem->kin_vtemp1           = NULL;
  kin_mem->kin_vtemp2           = NULL;
  kin_mem->kin_fold_aa          = NULL;
  kin_mem->kin_gold_aa          = NULL;
  kin_mem->kin_df_aa            = NULL;
  kin_mem->kin_dg_aa            = NULL;
  kin_mem->kin_q_aa             = NULL;
  kin_mem->kin_gamma_aa         = NULL;
  kin_mem->kin_R_aa             = NULL;
  kin_mem->kin_ipt_map          = NULL;
  kin_mem->kin_cv               = NULL;
  kin_mem->kin_Xv               = NULL;
  kin_mem->kin_lmem             = NULL;
  kin_mem->kin_m_aa             = 0;
  kin_mem->kin_aamem_aa         = 0;
  kin_mem->kin_setstop_aa       = 0;
  kin_mem->kin_beta_aa          = ONE;
  kin_mem->kin_damping_aa       = SUNFALSE;
  kin_mem->kin_constraintsSet   = SUNFALSE;
  kin_mem->kin_ehfun            = KINErrHandler;
  kin_mem->kin_eh_data          = kin_mem;
  kin_mem->kin_errfp            = stderr;
  kin_mem->kin_ihfun            = KINInfoHandler;
  kin_mem->kin_ih_data          = kin_mem;
  kin_mem->kin_infofp           = stdout;
  kin_mem->kin_printfl          = PRINTFL_DEFAULT;
  kin_mem->kin_mxiter           = MXITER_DEFAULT;
  kin_mem->kin_noInitSetup      = SUNFALSE;
  kin_mem->kin_msbset           = MSBSET_DEFAULT;
  kin_mem->kin_noResMon         = SUNFALSE;
  kin_mem->kin_msbset_sub       = MSBSET_SUB_DEFAULT;
  kin_mem->kin_update_fnorm_sub = SUNFALSE;
  kin_mem->kin_mxnbcf           = MXNBCF_DEFAULT;
  kin_mem->kin_sthrsh           = TWO;
  kin_mem->kin_noMinEps         = SUNFALSE;
  kin_mem->kin_mxnstepin        = ZERO;
  kin_mem->kin_sqrt_relfunc     = SUNRsqrt(uround);
  kin_mem->kin_scsteptol        = SUNRpowerR(uround,TWOTHIRDS);
  kin_mem->kin_fnormtol         = SUNRpowerR(uround,ONETHIRD);
  kin_mem->kin_etaflag          = KIN_ETACHOICE1;
  kin_mem->kin_eta              = POINT1;     /* default for KIN_ETACONSTANT */
  kin_mem->kin_eta_alpha        = TWO;        /* default for KIN_ETACHOICE2  */
  kin_mem->kin_eta_gamma        = POINT9;     /* default for KIN_ETACHOICE2  */
  kin_mem->kin_MallocDone       = SUNFALSE;
  kin_mem->kin_eval_omega       = SUNTRUE;
  kin_mem->kin_omega            = ZERO;       /* default to using min/max    */
  kin_mem->kin_omega_min        = OMEGA_MIN;
  kin_mem->kin_omega_max        = OMEGA_MAX;

  /* initialize lrw and liw */

  kin_mem->kin_lrw = 17;
  kin_mem->kin_liw = 22;

  /* NOTE: needed since KINInit could be called after KINSetConstraints */

  kin_mem->kin_lrw1 = 0;
  kin_mem->kin_liw1 = 0;

  return((void *) kin_mem);
}

/*
 * Function : KINInit
 *
 * KINInit allocates memory for a problem or execution of KINSol.
 * If memory is successfully allocated, KIN_SUCCESS is returned.
 * Otherwise, an error message is printed and an error flag
 * returned.
 */

int KINInit(void *kinmem, KINSysFn func, N_Vector tmpl)
{
  sunindextype liw1, lrw1;
  KINMem kin_mem;
  booleantype allocOK, nvectorOK;

  /* check kinmem */

  if (kinmem == NULL) {
    KINProcessError(NULL, KIN_MEM_NULL, "KINSOL", "KINInit", MSG_NO_MEM);
    return(KIN_MEM_NULL);
  }
  kin_mem = (KINMem) kinmem;

  if (func == NULL) {
    KINProcessError(kin_mem, KIN_ILL_INPUT, "KINSOL", "KINInit", MSG_FUNC_NULL);
    return(KIN_ILL_INPUT);
  }

  /* check if all required vector operations are implemented */

  nvectorOK = KINCheckNvector(tmpl);
  if (!nvectorOK) {
    KINProcessError(kin_mem, KIN_ILL_INPUT, "KINSOL", "KINInit", MSG_BAD_NVECTOR);
    return(KIN_ILL_INPUT);
  }

  /* set space requirements for one N_Vector */

  if (tmpl->ops->nvspace != NULL) {
    N_VSpace(tmpl, &lrw1, &liw1);
    kin_mem->kin_lrw1 = lrw1;
    kin_mem->kin_liw1 = liw1;
  }
  else {
    kin_mem->kin_lrw1 = 0;
    kin_mem->kin_liw1 = 0;
  }

  /* allocate necessary vectors */

  allocOK = KINAllocVectors(kin_mem, tmpl);
  if (!allocOK) {
    KINProcessError(kin_mem, KIN_MEM_FAIL, "KINSOL", "KINInit", MSG_MEM_FAIL);
    free(kin_mem); kin_mem = NULL;
    return(KIN_MEM_FAIL);
  }

  /* copy the input parameter into KINSol state */

  kin_mem->kin_func = func;

  /* set the linear solver addresses to NULL */

  kin_mem->kin_linit  = NULL;
  kin_mem->kin_lsetup = NULL;
  kin_mem->kin_lsolve = NULL;
  kin_mem->kin_lfree  = NULL;
  kin_mem->kin_lmem   = NULL;

  /* problem memory has been successfully allocated */

  kin_mem->kin_MallocDone = SUNTRUE;

  return(KIN_SUCCESS);
}

/*
 * -----------------------------------------------------------------
 * Main solver function
 * -----------------------------------------------------------------
 */

/*
 * Function : KINSol
 *
 * KINSol (main KINSOL driver routine) manages the computational
 * process of computing an approximate solution of the nonlinear
 * system F(uu) = 0. The KINSol routine calls the following
 * subroutines:
 *
 *  KINSolInit    checks if initial guess satisfies user-supplied
 *                constraints and initializes linear solver
 *
 *  KINLinSolDrv  interfaces with linear solver to find a
 *                solution of the system J(uu)*x = b (calculate
 *                Newton step)
 *
 *  KINFullNewton/KINLineSearch  implement the global strategy
 *
 *  KINForcingTerm  computes the forcing term (eta)
 *
 *  KINStop  determines if an approximate solution has been found
 */

int KINSol(void *kinmem, N_Vector u, int strategy_in,
           N_Vector u_scale, N_Vector f_scale)
{
  realtype fnormp, f1normp, epsmin, fmax=ZERO;
  KINMem kin_mem;
  int ret, sflag;
  booleantype maxStepTaken;

  /* intialize to avoid compiler warning messages */

  maxStepTaken = SUNFALSE;
  f1normp = fnormp = -ONE;

  /* initialize epsmin to avoid compiler warning message */

  epsmin = ZERO;

  /* check for kinmem non-NULL */

  if (kinmem == NULL) {
    KINProcessError(NULL, KIN_MEM_NULL, "KINSOL", "KINSol", MSG_NO_MEM);
    return(KIN_MEM_NULL);
  }
  kin_mem = (KINMem) kinmem;

  if(kin_mem->kin_MallocDone == SUNFALSE) {
    KINProcessError(NULL, KIN_NO_MALLOC, "KINSOL", "KINSol", MSG_NO_MALLOC);
    return(KIN_NO_MALLOC);
  }

  /* load input arguments */

  kin_mem->kin_uu = u;
  kin_mem->kin_uscale = u_scale;
  kin_mem->kin_fscale = f_scale;
  kin_mem->kin_globalstrategy = strategy_in;

  /* CSW:
     Call fixed point solver if requested.  Note that this should probably
     be forked off to a FPSOL solver instead of kinsol in the future. */
  if ( kin_mem->kin_globalstrategy == KIN_FP ) {
    if (kin_mem->kin_uu == NULL) {
      KINProcessError(kin_mem, KIN_ILL_INPUT, "KINSOL", "KINSol", MSG_UU_NULL);
      return(KIN_ILL_INPUT);
    }

    if (kin_mem->kin_constraintsSet != SUNFALSE) {
      KINProcessError(kin_mem, KIN_ILL_INPUT, "KINSOL", "KINSol", MSG_CONSTRAINTS_NOTOK);
      return(KIN_ILL_INPUT);
    }

    if (kin_mem->kin_printfl > 0)
      KINPrintInfo(kin_mem, PRNT_TOL, "KINSOL", "KINSol", INFO_TOL, kin_mem->kin_scsteptol, kin_mem->kin_fnormtol);

    kin_mem->kin_nfe = kin_mem->kin_nnilset = kin_mem->kin_nnilset_sub = kin_mem->kin_nni = kin_mem->kin_nbcf = kin_mem->kin_nbktrk = 0;
    ret = KINFP(kin_mem);

    switch(ret) {
    case KIN_SYSFUNC_FAIL:
      KINProcessError(kin_mem, KIN_SYSFUNC_FAIL, "KINSOL", "KINSol", MSG_SYSFUNC_FAILED);
      break;
    case KIN_MAXITER_REACHED:
      KINProcessError(kin_mem, KIN_MAXITER_REACHED, "KINSOL", "KINSol", MSG_MAXITER_REACHED);
      break;
    }

    return(ret);
  }

  /* initialize solver */
  ret = KINSolInit(kin_mem);
  if (ret != KIN_SUCCESS) return(ret);

  kin_mem->kin_ncscmx = 0;

  /* Note: The following logic allows the choice of whether or not
     to force a call to the linear solver setup upon a given call to
     KINSol */

  if (kin_mem->kin_noInitSetup) kin_mem->kin_sthrsh = ONE;
  else                          kin_mem->kin_sthrsh = TWO;

  /* if eps is to be bounded from below, set the bound */

  if (kin_mem->kin_inexact_ls && !(kin_mem->kin_noMinEps))
    epsmin = POINT01 * kin_mem->kin_fnormtol;


  /* if omega is zero at this point, make sure it will be evaluated
     at each iteration based on the provided min/max bounds and the
     current function norm. */
  if (kin_mem->kin_omega == ZERO) kin_mem->kin_eval_omega = SUNTRUE;
  else                            kin_mem->kin_eval_omega = SUNFALSE;


  /* CSW:
     Call fixed point solver for Picard method if requested.
     Note that this should probably be forked off to a part of an
     FPSOL solver instead of kinsol in the future. */
  if ( kin_mem->kin_globalstrategy == KIN_PICARD ) {

    if (kin_mem->kin_gval == NULL) {
      kin_mem->kin_gval = N_VClone(kin_mem->kin_unew);
      if (kin_mem->kin_gval == NULL) {
        KINProcessError(kin_mem, KIN_MEM_FAIL, "KINSOL", "KINSol", MSG_MEM_FAIL);
        return(KIN_MEM_FAIL);
      }
      kin_mem->kin_liw += kin_mem->kin_liw1;
      kin_mem->kin_lrw += kin_mem->kin_lrw1;
    }
    ret = KINPicardAA(kin_mem, &(kin_mem->kin_nni), kin_mem->kin_R_aa, kin_mem->kin_gamma_aa, &fmax);

    return(ret);
  }


  for(;;){

    kin_mem->kin_retry_nni = SUNFALSE;

    kin_mem->kin_nni++;

    /* calculate the epsilon (stopping criteria for iterative linear solver)
       for this iteration based on eta from the routine KINForcingTerm */

    if (kin_mem->kin_inexact_ls) {
      kin_mem->kin_eps = (kin_mem->kin_eta + kin_mem->kin_uround) * kin_mem->kin_fnorm;
      if(!(kin_mem->kin_noMinEps)) kin_mem->kin_eps = SUNMAX(epsmin, kin_mem->kin_eps);
    }

    repeat_nni:

    /* call the appropriate routine to calculate an acceptable step pp */

    sflag = 0;

    if (kin_mem->kin_globalstrategy == KIN_NONE) {

      /* Full Newton Step*/

      /* call KINLinSolDrv to calculate the (approximate) Newton step, pp */
      ret = KINLinSolDrv(kin_mem);
      if (ret != KIN_SUCCESS) break;

      sflag = KINFullNewton(kin_mem, &fnormp, &f1normp, &maxStepTaken);

      /* if sysfunc failed unrecoverably, stop */
      if ((sflag == KIN_SYSFUNC_FAIL) || (sflag == KIN_REPTD_SYSFUNC_ERR)) {
        ret = sflag;
        break;
      }

    } else if (kin_mem->kin_globalstrategy == KIN_LINESEARCH) {

      /* Line Search */

      /* call KINLinSolDrv to calculate the (approximate) Newton step, pp */
      ret = KINLinSolDrv(kin_mem);
      if (ret != KIN_SUCCESS) break;

      sflag = KINLineSearch(kin_mem, &fnormp, &f1normp, &maxStepTaken);

      /* if sysfunc failed unrecoverably, stop */
      if ((sflag == KIN_SYSFUNC_FAIL) || (sflag == KIN_REPTD_SYSFUNC_ERR)) {
        ret = sflag;
        break;
      }

      /* if too many beta condition failures, then stop iteration */
      if (kin_mem->kin_nbcf > kin_mem->kin_mxnbcf) {
        ret = KIN_LINESEARCH_BCFAIL;
        break;
      }

    }

    if ( (kin_mem->kin_globalstrategy != KIN_PICARD) &&
         (kin_mem->kin_globalstrategy != KIN_FP) ) {

      /* evaluate eta by calling the forcing term routine */
      if (kin_mem->kin_callForcingTerm) KINForcingTerm(kin_mem, fnormp);

      kin_mem->kin_fnorm = fnormp;

      /* call KINStop to check if tolerances where met by this iteration */
      ret = KINStop(kin_mem, maxStepTaken, sflag);

      if (ret == RETRY_ITERATION) {
        kin_mem->kin_retry_nni = SUNTRUE;
        goto repeat_nni;
      }
    }

    /* update uu after the iteration */
    N_VScale(ONE, kin_mem->kin_unew, kin_mem->kin_uu);

    kin_mem->kin_f1norm = f1normp;

    /* print the current nni, fnorm, and nfe values if printfl > 0 */

    if (kin_mem->kin_printfl > 0)
      KINPrintInfo(kin_mem, PRNT_NNI, "KINSOL", "KINSol", INFO_NNI, kin_mem->kin_nni, kin_mem->kin_nfe, kin_mem->kin_fnorm);

    if (ret != CONTINUE_ITERATIONS) break;

    fflush(kin_mem->kin_errfp);

  }  /* end of loop; return */



  if (kin_mem->kin_printfl > 0)
    KINPrintInfo(kin_mem, PRNT_RETVAL, "KINSOL", "KINSol", INFO_RETVAL, ret);

  switch(ret) {
  case KIN_SYSFUNC_FAIL:
    KINProcessError(kin_mem, KIN_SYSFUNC_FAIL, "KINSOL", "KINSol", MSG_SYSFUNC_FAILED);
    break;
  case KIN_REPTD_SYSFUNC_ERR:
    KINProcessError(kin_mem, KIN_REPTD_SYSFUNC_ERR, "KINSOL", "KINSol", MSG_SYSFUNC_REPTD);
    break;
  case KIN_LSETUP_FAIL:
    KINProcessError(kin_mem, KIN_LSETUP_FAIL, "KINSOL", "KINSol", MSG_LSETUP_FAILED);
    break;
  case KIN_LSOLVE_FAIL:
    KINProcessError(kin_mem, KIN_LSOLVE_FAIL, "KINSOL", "KINSol", MSG_LSOLVE_FAILED);
    break;
  case KIN_LINSOLV_NO_RECOVERY:
    KINProcessError(kin_mem, KIN_LINSOLV_NO_RECOVERY, "KINSOL", "KINSol", MSG_LINSOLV_NO_RECOVERY);
    break;
  case KIN_LINESEARCH_NONCONV:
    KINProcessError(kin_mem, KIN_LINESEARCH_NONCONV, "KINSOL", "KINSol", MSG_LINESEARCH_NONCONV);
    break;
  case KIN_LINESEARCH_BCFAIL:
    KINProcessError(kin_mem, KIN_LINESEARCH_BCFAIL, "KINSOL", "KINSol", MSG_LINESEARCH_BCFAIL);
    break;
  case KIN_MAXITER_REACHED:
    KINProcessError(kin_mem, KIN_MAXITER_REACHED, "KINSOL", "KINSol", MSG_MAXITER_REACHED);
    break;
  case KIN_MXNEWT_5X_EXCEEDED:
    KINProcessError(kin_mem, KIN_MXNEWT_5X_EXCEEDED, "KINSOL", "KINSol", MSG_MXNEWT_5X_EXCEEDED);
    break;
  }

  return(ret);
}

/*
 * -----------------------------------------------------------------
 * Deallocation function
 * -----------------------------------------------------------------
 */

/*
 * Function : KINFree
 *
 * This routine frees the problem memory allocated by KINInit.
 * Such memory includes all the vectors allocated by
 * KINAllocVectors, and the memory lmem for the linear solver
 * (deallocated by a call to lfree).
 */

void KINFree(void **kinmem)
{
  KINMem kin_mem;

  if (*kinmem == NULL) return;

  kin_mem = (KINMem) (*kinmem);
  KINFreeVectors(kin_mem);

  /* call lfree if non-NULL */

  if (kin_mem->kin_lfree != NULL) kin_mem->kin_lfree(kin_mem);

  free(*kinmem);
  *kinmem = NULL;
}

/*
 * =================================================================
 * PRIVATE FUNCTIONS
 * =================================================================
 */

/*
 * Function : KINCheckNvector
 *
 * This routine checks if all required vector operations are
 * implemented (excluding those required by KINConstraint). If all
 * necessary operations are present, then KINCheckNvector returns
 * SUNTRUE. Otherwise, SUNFALSE is returned.
 */

static booleantype KINCheckNvector(N_Vector tmpl)
{
  if ((tmpl->ops->nvclone     == NULL) ||
      (tmpl->ops->nvdestroy   == NULL) ||
      (tmpl->ops->nvlinearsum == NULL) ||
      (tmpl->ops->nvprod      == NULL) ||
      (tmpl->ops->nvdiv       == NULL) ||
      (tmpl->ops->nvscale     == NULL) ||
      (tmpl->ops->nvabs       == NULL) ||
      (tmpl->ops->nvinv       == NULL) ||
      (tmpl->ops->nvmaxnorm   == NULL) ||
      (tmpl->ops->nvmin       == NULL) ||
      (tmpl->ops->nvwl2norm   == NULL)) return(SUNFALSE);
  else return(SUNTRUE);
}

/*
 * -----------------------------------------------------------------
 * Memory allocation/deallocation
 * -----------------------------------------------------------------
 */

/*
 * Function : KINAllocVectors
 *
 * This routine allocates the KINSol vectors. If all memory
 * allocations are successful, KINAllocVectors returns SUNTRUE.
 * Otherwise all allocated memory is freed and KINAllocVectors
 * returns SUNFALSE.
 */

static booleantype KINAllocVectors(KINMem kin_mem, N_Vector tmpl)
{
  /* allocate unew, fval, pp, vtemp1 and vtemp2. */
  /* allocate df, dg, q, for Anderson Acceleration, Broyden and EN */

  if (kin_mem->kin_unew == NULL) {
    kin_mem->kin_unew = N_VClone(tmpl);
    if (kin_mem->kin_unew == NULL) return(SUNFALSE);
    kin_mem->kin_liw += kin_mem->kin_liw1;
    kin_mem->kin_lrw += kin_mem->kin_lrw1;
  }

  if (kin_mem->kin_fval == NULL) {
    kin_mem->kin_fval = N_VClone(tmpl);
    if (kin_mem->kin_fval == NULL) {
      N_VDestroy(kin_mem->kin_unew);
      kin_mem->kin_liw -= kin_mem->kin_liw1;
      kin_mem->kin_lrw -= kin_mem->kin_lrw1;
      return(SUNFALSE);
    }
    kin_mem->kin_liw += kin_mem->kin_liw1;
    kin_mem->kin_lrw += kin_mem->kin_lrw1;
  }

  if (kin_mem->kin_pp == NULL) {
    kin_mem->kin_pp = N_VClone(tmpl);
    if (kin_mem->kin_pp == NULL) {
      N_VDestroy(kin_mem->kin_unew);
      N_VDestroy(kin_mem->kin_fval);
      kin_mem->kin_liw -= 2*kin_mem->kin_liw1;
      kin_mem->kin_lrw -= 2*kin_mem->kin_lrw1;
      return(SUNFALSE);
    }
    kin_mem->kin_liw += kin_mem->kin_liw1;
    kin_mem->kin_lrw += kin_mem->kin_lrw1;
  }

  if (kin_mem->kin_vtemp1 == NULL) {
    kin_mem->kin_vtemp1 = N_VClone(tmpl);
    if (kin_mem->kin_vtemp1 == NULL) {
      N_VDestroy(kin_mem->kin_unew);
      N_VDestroy(kin_mem->kin_fval);
      N_VDestroy(kin_mem->kin_pp);
      kin_mem->kin_liw -= 3*kin_mem->kin_liw1;
      kin_mem->kin_lrw -= 3*kin_mem->kin_lrw1;
      return(SUNFALSE);
    }
    kin_mem->kin_liw += kin_mem->kin_liw1;
    kin_mem->kin_lrw += kin_mem->kin_lrw1;
  }

  if (kin_mem->kin_vtemp2 == NULL) {
    kin_mem->kin_vtemp2 = N_VClone(tmpl);
    if (kin_mem->kin_vtemp2 == NULL) {
      N_VDestroy(kin_mem->kin_unew);
      N_VDestroy(kin_mem->kin_fval);
      N_VDestroy(kin_mem->kin_pp);
      N_VDestroy(kin_mem->kin_vtemp1);
      kin_mem->kin_liw -= 4*kin_mem->kin_liw1;
      kin_mem->kin_lrw -= 4*kin_mem->kin_lrw1;
      return(SUNFALSE);
    }
    kin_mem->kin_liw += kin_mem->kin_liw1;
    kin_mem->kin_lrw += kin_mem->kin_lrw1;
  }

  /* Vectors for Anderson acceleration */

  if (kin_mem->kin_m_aa) {

    if (kin_mem->kin_R_aa == NULL) {
      kin_mem->kin_R_aa = (realtype *) malloc((kin_mem->kin_m_aa*kin_mem->kin_m_aa) * sizeof(realtype));
      if (kin_mem->kin_R_aa == NULL) {
        KINProcessError(kin_mem, 0, "KINSOL", "KINAllocVectors", MSG_MEM_FAIL);
        N_VDestroy(kin_mem->kin_unew);
        N_VDestroy(kin_mem->kin_fval);
        N_VDestroy(kin_mem->kin_pp);
        N_VDestroy(kin_mem->kin_vtemp1);
        N_VDestroy(kin_mem->kin_vtemp2);
        kin_mem->kin_liw -= 5*kin_mem->kin_liw1;
        kin_mem->kin_lrw -= 5*kin_mem->kin_lrw1;
        return(KIN_MEM_FAIL);
      }
    }

    if (kin_mem->kin_gamma_aa == NULL) {
      kin_mem->kin_gamma_aa = (realtype *) malloc(kin_mem->kin_m_aa * sizeof(realtype));
      if (kin_mem->kin_gamma_aa == NULL) {
        KINProcessError(kin_mem, 0, "KINSOL", "KINAllocVectors", MSG_MEM_FAIL);
        N_VDestroy(kin_mem->kin_unew);
        N_VDestroy(kin_mem->kin_fval);
        N_VDestroy(kin_mem->kin_pp);
        N_VDestroy(kin_mem->kin_vtemp1);
        N_VDestroy(kin_mem->kin_vtemp2);
        free(kin_mem->kin_R_aa);
        kin_mem->kin_liw -= 5*kin_mem->kin_liw1;
        kin_mem->kin_lrw -= 5*kin_mem->kin_lrw1;
        return(KIN_MEM_FAIL);
      }
    }

    if (kin_mem->kin_ipt_map == NULL) {
      kin_mem->kin_ipt_map = (long int *) malloc(kin_mem->kin_m_aa * sizeof(long int));
      if (kin_mem->kin_ipt_map == NULL) {
        KINProcessError(kin_mem, 0, "KINSOL", "KINAllocVectors", MSG_MEM_FAIL);
        N_VDestroy(kin_mem->kin_unew);
        N_VDestroy(kin_mem->kin_fval);
        N_VDestroy(kin_mem->kin_pp);
        N_VDestroy(kin_mem->kin_vtemp1);
        N_VDestroy(kin_mem->kin_vtemp2);
        free(kin_mem->kin_R_aa);
        free(kin_mem->kin_gamma_aa);
        kin_mem->kin_liw -= 5*kin_mem->kin_liw1;
        kin_mem->kin_lrw -= 5*kin_mem->kin_lrw1;
        return(KIN_MEM_FAIL);
      }
    }

    if (kin_mem->kin_cv == NULL) {
      kin_mem->kin_cv = (realtype *) malloc(2 * (kin_mem->kin_m_aa+1) * sizeof(realtype));
      if (kin_mem->kin_cv == NULL) {
        KINProcessError(kin_mem, 0, "KINSOL", "KINAllocVectors", MSG_MEM_FAIL);
        N_VDestroy(kin_mem->kin_unew);
        N_VDestroy(kin_mem->kin_fval);
        N_VDestroy(kin_mem->kin_pp);
        N_VDestroy(kin_mem->kin_vtemp1);
        N_VDestroy(kin_mem->kin_vtemp2);
        free(kin_mem->kin_R_aa);
        free(kin_mem->kin_gamma_aa);
        free(kin_mem->kin_ipt_map);
        kin_mem->kin_liw -= 5*kin_mem->kin_liw1;
        kin_mem->kin_lrw -= 5*kin_mem->kin_lrw1;
        return(KIN_MEM_FAIL);
      }
    }

    if (kin_mem->kin_Xv == NULL) {
      kin_mem->kin_Xv = (N_Vector *) malloc(2 * (kin_mem->kin_m_aa+1) * sizeof(N_Vector));
      if (kin_mem->kin_Xv == NULL) {
        KINProcessError(kin_mem, 0, "KINSOL", "KINAllocVectors", MSG_MEM_FAIL);
        N_VDestroy(kin_mem->kin_unew);
        N_VDestroy(kin_mem->kin_fval);
        N_VDestroy(kin_mem->kin_pp);
        N_VDestroy(kin_mem->kin_vtemp1);
        N_VDestroy(kin_mem->kin_vtemp2);
        free(kin_mem->kin_R_aa);
        free(kin_mem->kin_gamma_aa);
        free(kin_mem->kin_ipt_map);
        free(kin_mem->kin_cv);
        kin_mem->kin_liw -= 5*kin_mem->kin_liw1;
        kin_mem->kin_lrw -= 5*kin_mem->kin_lrw1;
        return(KIN_MEM_FAIL);
      }
    }

    if (kin_mem->kin_fold_aa == NULL) {
      kin_mem->kin_fold_aa = N_VClone(tmpl);
      if (kin_mem->kin_fold_aa == NULL) {
        N_VDestroy(kin_mem->kin_unew);
        N_VDestroy(kin_mem->kin_fval);
        N_VDestroy(kin_mem->kin_pp);
        N_VDestroy(kin_mem->kin_vtemp1);
        N_VDestroy(kin_mem->kin_vtemp2);
        free(kin_mem->kin_R_aa);
        free(kin_mem->kin_gamma_aa);
        free(kin_mem->kin_ipt_map);
        free(kin_mem->kin_cv);
        free(kin_mem->kin_Xv);
        kin_mem->kin_liw -= 5*kin_mem->kin_liw1;
        kin_mem->kin_lrw -= 5*kin_mem->kin_lrw1;
        return(SUNFALSE);
      }
      kin_mem->kin_liw += kin_mem->kin_liw1;
      kin_mem->kin_lrw += kin_mem->kin_lrw1;
    }

    if (kin_mem->kin_gold_aa == NULL) {
      kin_mem->kin_gold_aa = N_VClone(tmpl);
      if (kin_mem->kin_gold_aa == NULL) {
        N_VDestroy(kin_mem->kin_unew);
        N_VDestroy(kin_mem->kin_fval);
        N_VDestroy(kin_mem->kin_pp);
        N_VDestroy(kin_mem->kin_vtemp1);
        N_VDestroy(kin_mem->kin_vtemp2);
        free(kin_mem->kin_R_aa);
        free(kin_mem->kin_gamma_aa);
        free(kin_mem->kin_ipt_map);
        free(kin_mem->kin_cv);
        free(kin_mem->kin_Xv);
        N_VDestroy(kin_mem->kin_fold_aa);
        kin_mem->kin_liw -= 6*kin_mem->kin_liw1;
        kin_mem->kin_lrw -= 6*kin_mem->kin_lrw1;
        return(SUNFALSE);
      }
      kin_mem->kin_liw += kin_mem->kin_liw1;
      kin_mem->kin_lrw += kin_mem->kin_lrw1;
    }

    if (kin_mem->kin_df_aa == NULL) {
      kin_mem->kin_df_aa = N_VCloneVectorArray((int) kin_mem->kin_m_aa,tmpl);
      if (kin_mem->kin_df_aa == NULL) {
        N_VDestroy(kin_mem->kin_unew);
        N_VDestroy(kin_mem->kin_fval);
        N_VDestroy(kin_mem->kin_pp);
        N_VDestroy(kin_mem->kin_vtemp1);
        N_VDestroy(kin_mem->kin_vtemp2);
        free(kin_mem->kin_R_aa);
        free(kin_mem->kin_gamma_aa);
        free(kin_mem->kin_ipt_map);
        free(kin_mem->kin_cv);
        free(kin_mem->kin_Xv);
        N_VDestroy(kin_mem->kin_fold_aa);
        N_VDestroy(kin_mem->kin_gold_aa);
        kin_mem->kin_liw -= 7*kin_mem->kin_liw1;
        kin_mem->kin_lrw -= 7*kin_mem->kin_lrw1;
        return(SUNFALSE);
      }
      kin_mem->kin_liw += kin_mem->kin_m_aa * kin_mem->kin_liw1;
      kin_mem->kin_lrw += kin_mem->kin_m_aa * kin_mem->kin_lrw1;
    }

    if (kin_mem->kin_dg_aa == NULL) {
      kin_mem->kin_dg_aa = N_VCloneVectorArray((int) kin_mem->kin_m_aa,tmpl);
      if (kin_mem->kin_dg_aa == NULL) {
        N_VDestroy(kin_mem->kin_unew);
        N_VDestroy(kin_mem->kin_fval);
        N_VDestroy(kin_mem->kin_pp);
        N_VDestroy(kin_mem->kin_vtemp1);
        N_VDestroy(kin_mem->kin_vtemp2);
        free(kin_mem->kin_R_aa);
        free(kin_mem->kin_gamma_aa);
        free(kin_mem->kin_ipt_map);
        free(kin_mem->kin_cv);
        free(kin_mem->kin_Xv);
        N_VDestroy(kin_mem->kin_fold_aa);
        N_VDestroy(kin_mem->kin_gold_aa);
        N_VDestroyVectorArray(kin_mem->kin_df_aa, (int) kin_mem->kin_m_aa);
        kin_mem->kin_liw -= (7 + kin_mem->kin_m_aa) * kin_mem->kin_liw1;
        kin_mem->kin_lrw -= (7 + kin_mem->kin_m_aa) * kin_mem->kin_lrw1;
        return(SUNFALSE);
      }
      kin_mem->kin_liw += kin_mem->kin_m_aa * kin_mem->kin_liw1;
      kin_mem->kin_lrw += kin_mem->kin_m_aa * kin_mem->kin_lrw1;
    }

    if (kin_mem->kin_q_aa == NULL) {
      kin_mem->kin_q_aa = N_VCloneVectorArray((int) kin_mem->kin_m_aa,tmpl);
      if (kin_mem->kin_q_aa == NULL) {
        N_VDestroy(kin_mem->kin_unew);
        N_VDestroy(kin_mem->kin_fval);
        N_VDestroy(kin_mem->kin_pp);
        N_VDestroy(kin_mem->kin_vtemp1);
        N_VDestroy(kin_mem->kin_vtemp2);
        free(kin_mem->kin_R_aa);
        free(kin_mem->kin_gamma_aa);
        free(kin_mem->kin_ipt_map);
        free(kin_mem->kin_cv);
        free(kin_mem->kin_Xv);
        N_VDestroy(kin_mem->kin_fold_aa);
        N_VDestroy(kin_mem->kin_gold_aa);
        N_VDestroyVectorArray(kin_mem->kin_df_aa, (int) kin_mem->kin_m_aa);
        N_VDestroyVectorArray(kin_mem->kin_dg_aa, (int) kin_mem->kin_m_aa);
        kin_mem->kin_liw -= (7 + 2 * kin_mem->kin_m_aa) * kin_mem->kin_liw1;
        kin_mem->kin_lrw -= (7 + 2 * kin_mem->kin_m_aa) * kin_mem->kin_lrw1;
        return(SUNFALSE);
      }
      kin_mem->kin_liw += kin_mem->kin_m_aa * kin_mem->kin_liw1;
      kin_mem->kin_lrw += kin_mem->kin_m_aa * kin_mem->kin_lrw1;
    }
  }

  return(SUNTRUE);
}

/*
 * KINFreeVectors
 *
 * This routine frees the KINSol vectors allocated by
 * KINAllocVectors.
 */

static void KINFreeVectors(KINMem kin_mem)
{
  if (kin_mem->kin_unew != NULL) {
    N_VDestroy(kin_mem->kin_unew);
    kin_mem->kin_unew = NULL;
    kin_mem->kin_lrw -= kin_mem->kin_lrw1;
    kin_mem->kin_liw -= kin_mem->kin_liw1;
  }

  if (kin_mem->kin_fval != NULL) {
    N_VDestroy(kin_mem->kin_fval);
    kin_mem->kin_fval = NULL;
    kin_mem->kin_lrw -= kin_mem->kin_lrw1;
    kin_mem->kin_liw -= kin_mem->kin_liw1;
  }

  if (kin_mem->kin_pp != NULL) {
    N_VDestroy(kin_mem->kin_pp);
    kin_mem->kin_pp = NULL;
    kin_mem->kin_lrw -= kin_mem->kin_lrw1;
    kin_mem->kin_liw -= kin_mem->kin_liw1;
  }

  if (kin_mem->kin_vtemp1 != NULL) {
    N_VDestroy(kin_mem->kin_vtemp1);
    kin_mem->kin_vtemp1 = NULL;
    kin_mem->kin_lrw -= kin_mem->kin_lrw1;
    kin_mem->kin_liw -= kin_mem->kin_liw1;
  }

  if (kin_mem->kin_vtemp2 != NULL) {
    N_VDestroy(kin_mem->kin_vtemp2);
    kin_mem->kin_vtemp2 = NULL;
    kin_mem->kin_lrw -= kin_mem->kin_lrw1;
    kin_mem->kin_liw -= kin_mem->kin_liw1;
  }

  if (kin_mem->kin_gval != NULL) {
    N_VDestroy(kin_mem->kin_gval);
    kin_mem->kin_gval = NULL;
    kin_mem->kin_lrw -= kin_mem->kin_lrw1;
    kin_mem->kin_liw -= kin_mem->kin_liw1;
  }

  if (kin_mem->kin_R_aa != NULL) {
    free(kin_mem->kin_R_aa);
    kin_mem->kin_R_aa = NULL;
  }

  if (kin_mem->kin_gamma_aa != NULL) {
    free(kin_mem->kin_gamma_aa);
    kin_mem->kin_gamma_aa = NULL;
  }

  if (kin_mem->kin_ipt_map != NULL) {
    free(kin_mem->kin_ipt_map);
    kin_mem->kin_ipt_map = NULL;
  }

  if (kin_mem->kin_cv != NULL) {
    free(kin_mem->kin_cv);
    kin_mem->kin_cv = NULL;
  }

  if (kin_mem->kin_Xv != NULL) {
    free(kin_mem->kin_Xv);
    kin_mem->kin_Xv = NULL;
  }

  if (kin_mem->kin_fold_aa != NULL) {
    N_VDestroy(kin_mem->kin_fold_aa);
    kin_mem->kin_fold_aa = NULL;
    kin_mem->kin_lrw -= kin_mem->kin_lrw1;
    kin_mem->kin_liw -= kin_mem->kin_liw1;
  }

  if (kin_mem->kin_gold_aa != NULL) {
    N_VDestroy(kin_mem->kin_gold_aa);
    kin_mem->kin_gold_aa = NULL;
    kin_mem->kin_lrw -= kin_mem->kin_lrw1;
    kin_mem->kin_liw -= kin_mem->kin_liw1;
  }

  if (kin_mem->kin_df_aa != NULL) {
    N_VDestroyVectorArray(kin_mem->kin_df_aa, (int) kin_mem->kin_m_aa);
    kin_mem->kin_df_aa = NULL;
    kin_mem->kin_lrw -= kin_mem->kin_m_aa * kin_mem->kin_lrw1;
    kin_mem->kin_liw -= kin_mem->kin_m_aa * kin_mem->kin_liw1;
  }

  if (kin_mem->kin_dg_aa != NULL) {
    N_VDestroyVectorArray(kin_mem->kin_dg_aa, (int) kin_mem->kin_m_aa);
    kin_mem->kin_dg_aa = NULL;
    kin_mem->kin_lrw -= kin_mem->kin_m_aa * kin_mem->kin_lrw1;
    kin_mem->kin_liw -= kin_mem->kin_m_aa * kin_mem->kin_liw1;
  }

  if (kin_mem->kin_q_aa != NULL) {
    N_VDestroyVectorArray(kin_mem->kin_q_aa, (int) kin_mem->kin_m_aa);
    kin_mem->kin_q_aa = NULL;
    kin_mem->kin_lrw -= kin_mem->kin_m_aa * kin_mem->kin_lrw1;
    kin_mem->kin_liw -= kin_mem->kin_m_aa * kin_mem->kin_liw1;
  }

  if (kin_mem->kin_constraints != NULL) {
    N_VDestroy(kin_mem->kin_constraints);
    kin_mem->kin_constraints = NULL;
    kin_mem->kin_lrw -= kin_mem->kin_lrw1;
    kin_mem->kin_liw -= kin_mem->kin_liw1;
  }

  return;
}

/*
 * -----------------------------------------------------------------
 * Initial setup
 * -----------------------------------------------------------------
 */

/*
 * KINSolInit
 *
 * KINSolInit initializes the problem for the specific input
 * received in this call to KINSol (which calls KINSolInit). All
 * problem specification inputs are checked for errors. If any error
 * occurs during initialization, it is reported to the file whose
 * file pointer is errfp.
 *
 * The possible return values for KINSolInit are:
 *   KIN_SUCCESS : indicates a normal initialization
 *
 *   KIN_ILL_INPUT : indicates that an input error has been found
 *
 *   KIN_INITIAL_GUESS_OK : indicates that the guess uu
 *                          satisfied the system func(uu) = 0
 *                          within the tolerances specified
 */

static int KINSolInit(KINMem kin_mem)
{
  int retval;
  realtype fmax;

  /* check for illegal input parameters */

  if (kin_mem->kin_uu == NULL) {
    KINProcessError(kin_mem, KIN_ILL_INPUT, "KINSOL", "KINSolInit", MSG_UU_NULL);
    return(KIN_ILL_INPUT);
  }

  /* check for valid strategy */

  if ( (kin_mem->kin_globalstrategy != KIN_NONE) &&
       (kin_mem->kin_globalstrategy != KIN_LINESEARCH) &&
       (kin_mem->kin_globalstrategy != KIN_PICARD) &&
       (kin_mem->kin_globalstrategy != KIN_FP) ) {
    KINProcessError(kin_mem, KIN_ILL_INPUT, "KINSOL", "KINSolInit", MSG_BAD_GLSTRAT);
    return(KIN_ILL_INPUT);
  }

  if (kin_mem->kin_uscale == NULL)  {
    KINProcessError(kin_mem, KIN_ILL_INPUT, "KINSOL", "KINSolInit", MSG_BAD_USCALE);
    return(KIN_ILL_INPUT);
  }

  if (N_VMin(kin_mem->kin_uscale) <= ZERO){
    KINProcessError(kin_mem, KIN_ILL_INPUT, "KINSOL", "KINSolInit", MSG_USCALE_NONPOSITIVE);
    return(KIN_ILL_INPUT);
  }

  if (kin_mem->kin_fscale == NULL)  {
    KINProcessError(kin_mem, KIN_ILL_INPUT, "KINSOL", "KINSolInit", MSG_BAD_FSCALE);
    return(KIN_ILL_INPUT);
  }

  if (N_VMin(kin_mem->kin_fscale) <= ZERO){
    KINProcessError(kin_mem, KIN_ILL_INPUT, "KINSOL", "KINSolInit", MSG_FSCALE_NONPOSITIVE);
    return(KIN_ILL_INPUT);
  }

  if ( (kin_mem->kin_constraints != NULL) &&
       ( (kin_mem->kin_globalstrategy == KIN_PICARD) ||
         (kin_mem->kin_globalstrategy == KIN_FP) ) ) {
    KINProcessError(kin_mem, KIN_ILL_INPUT, "KINSOL", "KINSolInit", MSG_CONSTRAINTS_NOTOK);
    return(KIN_ILL_INPUT);
  }


  /* set the constraints flag */

  if (kin_mem->kin_constraints == NULL)
    kin_mem->kin_constraintsSet = SUNFALSE;
  else {
    kin_mem->kin_constraintsSet = SUNTRUE;
    if ((kin_mem->kin_constraints->ops->nvconstrmask  == NULL) ||
        (kin_mem->kin_constraints->ops->nvminquotient == NULL)) {
      KINProcessError(kin_mem, KIN_ILL_INPUT, "KINSOL", "KINSolInit", MSG_BAD_NVECTOR);
      return(KIN_ILL_INPUT);
    }
  }

  /* check the initial guess uu against the constraints */

  if (kin_mem->kin_constraintsSet) {
    if (!N_VConstrMask(kin_mem->kin_constraints, kin_mem->kin_uu, kin_mem->kin_vtemp1)) {
      KINProcessError(kin_mem, KIN_ILL_INPUT, "KINSOL", "KINSolInit", MSG_INITIAL_CNSTRNT);
      return(KIN_ILL_INPUT);
    }
  }

  /* all error checking is complete at this point */

  if (kin_mem->kin_printfl > 0)
    KINPrintInfo(kin_mem, PRNT_TOL, "KINSOL", "KINSolInit", INFO_TOL, kin_mem->kin_scsteptol, kin_mem->kin_fnormtol);

  /* calculate the default value for mxnewtstep (maximum Newton step) */

  if (kin_mem->kin_mxnstepin == ZERO) kin_mem->kin_mxnewtstep = THOUSAND * N_VWL2Norm(kin_mem->kin_uu, kin_mem->kin_uscale);
  else                                kin_mem->kin_mxnewtstep = kin_mem->kin_mxnstepin;

  if (kin_mem->kin_mxnewtstep < ONE) kin_mem->kin_mxnewtstep = ONE;

  /* additional set-up for inexact linear solvers */

  if (kin_mem->kin_inexact_ls) {

    /* set up the coefficients for the eta calculation */

    kin_mem->kin_callForcingTerm = (kin_mem->kin_etaflag != KIN_ETACONSTANT);

    /* this value is always used for choice #1 */

    if (kin_mem->kin_etaflag == KIN_ETACHOICE1) kin_mem->kin_eta_alpha = (ONE + SUNRsqrt(FIVE)) * HALF;

    /* initial value for eta set to 0.5 for other than the
       KIN_ETACONSTANT option */

    if (kin_mem->kin_etaflag != KIN_ETACONSTANT) kin_mem->kin_eta = HALF;

    /* disable residual monitoring if using an inexact linear solver */

    kin_mem->kin_noResMon = SUNTRUE;

  } else {

    kin_mem->kin_callForcingTerm = SUNFALSE;

  }

  /* initialize counters */

  kin_mem->kin_nfe = kin_mem->kin_nnilset = kin_mem->kin_nnilset_sub = kin_mem->kin_nni = kin_mem->kin_nbcf = kin_mem->kin_nbktrk = 0;

  /* see if the initial guess uu satisfies the nonlinear system */
  retval = kin_mem->kin_func(kin_mem->kin_uu, kin_mem->kin_fval, kin_mem->kin_user_data); kin_mem->kin_nfe++;

  if (retval < 0) {
    KINProcessError(kin_mem, KIN_SYSFUNC_FAIL, "KINSOL", "KINSolInit",
                    MSG_SYSFUNC_FAILED);
    return(KIN_SYSFUNC_FAIL);
  } else if (retval > 0) {
    KINProcessError(kin_mem, KIN_FIRST_SYSFUNC_ERR, "KINSOL", "KINSolInit",
                    MSG_SYSFUNC_FIRST);
    return(KIN_FIRST_SYSFUNC_ERR);
  }

  fmax = KINScFNorm(kin_mem, kin_mem->kin_fval, kin_mem->kin_fscale);
  if (fmax <= (POINT01 * kin_mem->kin_fnormtol)) {
    kin_mem->kin_fnorm = N_VWL2Norm(kin_mem->kin_fval, kin_mem->kin_fscale);
    return(KIN_INITIAL_GUESS_OK);
  }

  if (kin_mem->kin_printfl > 1)
    KINPrintInfo(kin_mem, PRNT_FMAX, "KINSOL", "KINSolInit", INFO_FMAX, fmax);

  /* initialize the linear solver if linit != NULL */

  if (kin_mem->kin_linit != NULL) {
    retval = kin_mem->kin_linit(kin_mem);
    if (retval != 0) {
      KINProcessError(kin_mem, KIN_LINIT_FAIL, "KINSOL", "KINSolInit", MSG_LINIT_FAIL);
      return(KIN_LINIT_FAIL);
    }
  }

  /* initialize the L2 (Euclidean) norms of f for the linear iteration steps */

  kin_mem->kin_fnorm = N_VWL2Norm(kin_mem->kin_fval, kin_mem->kin_fscale);
  kin_mem->kin_f1norm = HALF * kin_mem->kin_fnorm * kin_mem->kin_fnorm;
  kin_mem->kin_fnorm_sub = kin_mem->kin_fnorm;

  if (kin_mem->kin_printfl > 0)
    KINPrintInfo(kin_mem, PRNT_NNI, "KINSOL", "KINSolInit",
                 INFO_NNI, kin_mem->kin_nni, kin_mem->kin_nfe, kin_mem->kin_fnorm);

  /* problem has now been successfully initialized */

  return(KIN_SUCCESS);
}

/*
 * -----------------------------------------------------------------
 * Step functions
 * -----------------------------------------------------------------
 */

/*
 * KINLinSolDrv
 *
 * This routine handles the process of solving for the approximate
 * solution of the Newton equations in the Newton iteration.
 * Subsequent routines handle the nonlinear aspects of its
 * application.
 */

static int KINLinSolDrv(KINMem kin_mem)
{
  N_Vector x, b;
  int retval;

  if ((kin_mem->kin_nni - kin_mem->kin_nnilset) >= kin_mem->kin_msbset) {
    kin_mem->kin_sthrsh = TWO;
    kin_mem->kin_update_fnorm_sub = SUNTRUE;
  }

  for(;;){

    kin_mem->kin_jacCurrent = SUNFALSE;

    if ((kin_mem->kin_sthrsh > ONEPT5) && (kin_mem->kin_lsetup != NULL)) {
      retval = kin_mem->kin_lsetup(kin_mem);
      kin_mem->kin_jacCurrent = SUNTRUE;
      kin_mem->kin_nnilset = kin_mem->kin_nni;
      kin_mem->kin_nnilset_sub = kin_mem->kin_nni;
      if (retval != 0) return(KIN_LSETUP_FAIL);
    }

    /* rename vectors for readability */

    b = kin_mem->kin_unew;
    x = kin_mem->kin_pp;

    /* load b with the current value of -fval */

    N_VScale(-ONE, kin_mem->kin_fval, b);

    /* call the generic 'lsolve' routine to solve the system Jx = b */

    retval = kin_mem->kin_lsolve(kin_mem, x, b, &(kin_mem->kin_sJpnorm),
                                 &(kin_mem->kin_sFdotJp));

    if (retval == 0)                          return(KIN_SUCCESS);
    else if (retval < 0)                      return(KIN_LSOLVE_FAIL);
    else if ((kin_mem->kin_lsetup == NULL) || (kin_mem->kin_jacCurrent)) return(KIN_LINSOLV_NO_RECOVERY);

    /* loop back only if the linear solver setup is in use
       and Jacobian information is not current */

    kin_mem->kin_sthrsh = TWO;

  }
}

/*
 * KINFullNewton
 *
 * This routine is the main driver for the Full Newton
 * algorithm. Its purpose is to compute unew = uu + pp in the
 * direction pp from uu, taking the full Newton step. The
 * step may be constrained if the constraint conditions are
 * violated, or if the norm of pp is greater than mxnewtstep.
 */

static int KINFullNewton(KINMem kin_mem, realtype *fnormp, realtype *f1normp,
                         booleantype *maxStepTaken)
{
  realtype pnorm, ratio;
  booleantype fOK;
  int ircvr, retval;

  *maxStepTaken = SUNFALSE;
  pnorm = N_VWL2Norm(kin_mem->kin_pp, kin_mem->kin_uscale);
  ratio = ONE;
  if (pnorm > kin_mem->kin_mxnewtstep) {
    ratio = kin_mem->kin_mxnewtstep / pnorm;
    N_VScale(ratio, kin_mem->kin_pp, kin_mem->kin_pp);
    pnorm = kin_mem->kin_mxnewtstep;
  }

  if (kin_mem->kin_printfl > 0)
    KINPrintInfo(kin_mem, PRNT_PNORM, "KINSOL", "KINFullNewton", INFO_PNORM, pnorm);

  /* If constraints are active, then constrain the step accordingly */

  kin_mem->kin_stepl = pnorm;
  kin_mem->kin_stepmul = ONE;
  if (kin_mem->kin_constraintsSet) {
    retval = KINConstraint(kin_mem);
    if (retval == CONSTR_VIOLATED) {
      /* Apply stepmul set in KINConstraint */
      ratio *= kin_mem->kin_stepmul;
      N_VScale(kin_mem->kin_stepmul, kin_mem->kin_pp, kin_mem->kin_pp);
      pnorm *= kin_mem->kin_stepmul;
      kin_mem->kin_stepl = pnorm;
      if (kin_mem->kin_printfl > 0)
        KINPrintInfo(kin_mem, PRNT_PNORM, "KINSOL", "KINFullNewton", INFO_PNORM, pnorm);
      if (pnorm <= kin_mem->kin_scsteptol) {
        N_VLinearSum(ONE, kin_mem->kin_uu, ONE, kin_mem->kin_pp, kin_mem->kin_unew);
        return(STEP_TOO_SMALL);}
    }
  }

  /* Attempt (at most MAX_RECVR times) to evaluate function at the new iterate */

  fOK = SUNFALSE;

  for (ircvr = 1; ircvr <= MAX_RECVR; ircvr++) {

    /* compute the iterate unew = uu + pp */
    N_VLinearSum(ONE, kin_mem->kin_uu, ONE, kin_mem->kin_pp, kin_mem->kin_unew);

    /* evaluate func(unew) and its norm, and return */
    retval = kin_mem->kin_func(kin_mem->kin_unew, kin_mem->kin_fval, kin_mem->kin_user_data); kin_mem->kin_nfe++;

    /* if func was successful, accept pp */
    if (retval == 0) {fOK = SUNTRUE; break;}

    /* if func failed unrecoverably, give up */
    else if (retval < 0) return(KIN_SYSFUNC_FAIL);

    /* func failed recoverably; cut step in half and try again */
    ratio *= HALF;
    N_VScale(HALF, kin_mem->kin_pp, kin_mem->kin_pp);
    pnorm *= HALF;
    kin_mem->kin_stepl = pnorm;
  }

  /* If func() failed recoverably MAX_RECVR times, give up */

  if (!fOK) return(KIN_REPTD_SYSFUNC_ERR);

  /* Evaluate function norms */

  *fnormp = N_VWL2Norm(kin_mem->kin_fval, kin_mem->kin_fscale);
  *f1normp = HALF * (*fnormp) * (*fnormp);

  /* scale sFdotJp and sJpnorm by ratio for later use in KINForcingTerm */

  kin_mem->kin_sFdotJp *= ratio;
  kin_mem->kin_sJpnorm *= ratio;

  if (kin_mem->kin_printfl > 1)
    KINPrintInfo(kin_mem, PRNT_FNORM, "KINSOL", "KINFullNewton", INFO_FNORM, *fnormp);

  if (pnorm > (POINT99 * kin_mem->kin_mxnewtstep)) *maxStepTaken = SUNTRUE;

  return(KIN_SUCCESS);
}

/*
 * KINLineSearch
 *
 * The routine KINLineSearch implements the LineSearch algorithm.
 * Its purpose is to find unew = uu + rl * pp in the direction pp
 * from uu so that:
 *                                    t
 *  func(unew) <= func(uu) + alpha * g  (unew - uu) (alpha = 1.e-4)
 *
 *    and
 *                                   t
 *  func(unew) >= func(uu) + beta * g  (unew - uu) (beta = 0.9)
 *
 * where 0 < rlmin <= rl <= rlmax.
 *
 * Note:
 *             mxnewtstep
 *  rlmax = ----------------   if uu+pp is feasible
 *          ||uscale*pp||_L2
 *
 *  rlmax = 1   otherwise
 *
 *    and
 *
 *                 scsteptol
 *  rlmin = --------------------------
 *          ||           pp         ||
 *          || -------------------- ||_L-infinity
 *          || (1/uscale + SUNRabs(uu)) ||
 *
 *
 * If the system function fails unrecoverably at any time, KINLineSearch
 * returns KIN_SYSFUNC_FAIL which will halt the solver.
 *
 * We attempt to corect recoverable system function failures only before
 * the alpha-condition loop; i.e. when the solution is updated with the
 * full Newton step (possibly reduced due to constraint violations).
 * Once we find a feasible pp, we assume that any update up to pp is
 * feasible.
 *
 * If the step size is limited due to constraint violations and/or
 * recoverable system function failures, we set rlmax=1 to ensure
 * that the update remains feasible during the attempts to enforce
 * the beta-condition (this is not an issue while enforcing the alpha
 * condition, as rl can only decrease from 1 at that stage)
 */

static int KINLineSearch(KINMem kin_mem, realtype *fnormp, realtype *f1normp,
                         booleantype *maxStepTaken)
{
  realtype pnorm, ratio, slpi, rlmin, rlength, rl, rlmax, rldiff;
  realtype rltmp, rlprev, pt1trl, f1nprv, rllo, rlinc, alpha, beta;
  realtype alpha_cond, beta_cond, rl_a, tmp1, rl_b, tmp2, disc;
  int ircvr, nbktrk_l, retval;
  booleantype firstBacktrack, fOK;

  /* Initializations */

  nbktrk_l = 0;          /* local backtracking counter */
  ratio    = ONE;        /* step change ratio          */
  alpha    = POINT0001;
  beta     = POINT9;

  firstBacktrack = SUNTRUE;
  *maxStepTaken = SUNFALSE;

  rlprev = f1nprv = ZERO;

  /* Compute length of Newton step */

  pnorm = N_VWL2Norm(kin_mem->kin_pp, kin_mem->kin_uscale);
  rlmax = kin_mem->kin_mxnewtstep / pnorm;
  kin_mem->kin_stepl = pnorm;

  /* If the full Newton step is too large, set it to the maximum allowable value */

  if(pnorm > kin_mem->kin_mxnewtstep ) {
    ratio = kin_mem->kin_mxnewtstep / pnorm;
    N_VScale(ratio, kin_mem->kin_pp, kin_mem->kin_pp);
    pnorm = kin_mem->kin_mxnewtstep;
    rlmax = ONE;
    kin_mem->kin_stepl = pnorm;
  }

  /* If constraint checking is activated, check and correct violations */

  kin_mem->kin_stepmul = ONE;

  if(kin_mem->kin_constraintsSet){
    retval = KINConstraint(kin_mem);
    if(retval == CONSTR_VIOLATED){
      /* Apply stepmul set in KINConstraint */
      N_VScale(kin_mem->kin_stepmul, kin_mem->kin_pp, kin_mem->kin_pp);
      ratio *= kin_mem->kin_stepmul;
      pnorm *= kin_mem->kin_stepmul;
      rlmax = ONE;
      kin_mem->kin_stepl = pnorm;
      if (kin_mem->kin_printfl > 0) KINPrintInfo(kin_mem, PRNT_PNORM1, "KINSOL", "KINLineSearch", INFO_PNORM1, pnorm);
      if (pnorm <= kin_mem->kin_scsteptol) {
        N_VLinearSum(ONE, kin_mem->kin_uu, ONE, kin_mem->kin_pp, kin_mem->kin_unew);
        return(STEP_TOO_SMALL);}
    }
  }

  /* Attempt (at most MAX_RECVR times) to evaluate function at the new iterate */

  fOK = SUNFALSE;

  for (ircvr = 1; ircvr <= MAX_RECVR; ircvr++) {

    /* compute the iterate unew = uu + pp */
    N_VLinearSum(ONE, kin_mem->kin_uu, ONE, kin_mem->kin_pp, kin_mem->kin_unew);

    /* evaluate func(unew) and its norm, and return */
    retval = kin_mem->kin_func(kin_mem->kin_unew, kin_mem->kin_fval, kin_mem->kin_user_data); kin_mem->kin_nfe++;

    /* if func was successful, accept pp */
    if (retval == 0) {fOK = SUNTRUE; break;}

    /* if func failed unrecoverably, give up */
    else if (retval < 0) return(KIN_SYSFUNC_FAIL);

    /* func failed recoverably; cut step in half and try again */
    N_VScale(HALF, kin_mem->kin_pp, kin_mem->kin_pp);
    ratio *= HALF;
    pnorm *= HALF;
    rlmax = ONE;
    kin_mem->kin_stepl = pnorm;

  }

  /* If func() failed recoverably MAX_RECVR times, give up */

  if (!fOK) return(KIN_REPTD_SYSFUNC_ERR);

  /* Evaluate function norms */

  *fnormp = N_VWL2Norm(kin_mem->kin_fval, kin_mem->kin_fscale);
  *f1normp = HALF * (*fnormp) * (*fnormp) ;

  /* Estimate the line search value rl (lambda) to satisfy both ALPHA and BETA conditions */

  slpi = kin_mem->kin_sFdotJp * ratio;
  rlength = KINScSNorm(kin_mem, kin_mem->kin_pp, kin_mem->kin_uu);
  rlmin = (kin_mem->kin_scsteptol) / rlength;
  rl = ONE;

  if (kin_mem->kin_printfl > 2)
    KINPrintInfo(kin_mem, PRNT_LAM, "KINSOL", "KINLineSearch", INFO_LAM, rlmin, kin_mem->kin_f1norm, pnorm);

  /* Loop until the ALPHA condition is satisfied. Terminate if rl becomes too small */

  for(;;) {

    /* Evaluate test quantity */

    alpha_cond = kin_mem->kin_f1norm + (alpha * slpi * rl);

    if (kin_mem->kin_printfl > 2)
      KINPrintInfo(kin_mem, PRNT_ALPHA, "KINSOL", "KINLinesearch",
                   INFO_ALPHA, *fnormp, *f1normp, alpha_cond, rl);

    /* If ALPHA condition is satisfied, break out from loop */

    if ((*f1normp) <= alpha_cond) break;

    /* Backtracking. Use quadratic fit the first time and cubic fit afterwards. */

    if (firstBacktrack) {

      rltmp = -slpi / (TWO * ((*f1normp) - kin_mem->kin_f1norm - slpi));
      firstBacktrack = SUNFALSE;

    } else {

      tmp1 = (*f1normp) - kin_mem->kin_f1norm - (rl * slpi);
      tmp2 = f1nprv - kin_mem->kin_f1norm - (rlprev * slpi);
      rl_a = ((ONE / (rl * rl)) * tmp1) - ((ONE / (rlprev * rlprev)) * tmp2);
      rl_b = ((-rlprev / (rl * rl)) * tmp1) + ((rl / (rlprev * rlprev)) * tmp2);
      tmp1 = ONE / (rl - rlprev);
      rl_a *= tmp1;
      rl_b *= tmp1;
      disc = (rl_b * rl_b) - (THREE * rl_a * slpi);

      if (SUNRabs(rl_a) < kin_mem->kin_uround) {        /* cubic is actually just a quadratic (rl_a ~ 0) */
        rltmp = -slpi / (TWO * rl_b);
      } else {                         /* real cubic */
        rltmp = (-rl_b + SUNRsqrt(disc)) / (THREE * rl_a);
      }
    }
      if (rltmp > (HALF * rl)) rltmp = HALF * rl;

    /* Set new rl (do not allow a reduction by a factor larger than 10) */

    rlprev = rl;
    f1nprv = (*f1normp);
    pt1trl = POINT1 * rl;
    rl = SUNMAX(pt1trl, rltmp);
    nbktrk_l++;

    /* Update unew and re-evaluate function */

    N_VLinearSum(ONE, kin_mem->kin_uu, rl, kin_mem->kin_pp, kin_mem->kin_unew);

    retval = kin_mem->kin_func(kin_mem->kin_unew, kin_mem->kin_fval, kin_mem->kin_user_data); kin_mem->kin_nfe++;
    if (retval != 0) return(KIN_SYSFUNC_FAIL);

    *fnormp = N_VWL2Norm(kin_mem->kin_fval, kin_mem->kin_fscale);
    *f1normp = HALF * (*fnormp) * (*fnormp) ;

    /* Check if rl (lambda) is too small */

    if (rl < rlmin) {
      /* unew sufficiently distinct from uu cannot be found.
         copy uu into unew (step remains unchanged) and
         return STEP_TOO_SMALL */
      N_VScale(ONE, kin_mem->kin_uu, kin_mem->kin_unew);
      return(STEP_TOO_SMALL);
    }

  } /* end ALPHA condition loop */


  /* ALPHA condition is satisfied. Now check the BETA condition */

  beta_cond = kin_mem->kin_f1norm + (beta * slpi * rl);

  if ((*f1normp) < beta_cond) {

    /* BETA condition not satisfied */

    if ((rl == ONE) && (pnorm < kin_mem->kin_mxnewtstep)) {

      do {

        rlprev = rl;
        f1nprv = *f1normp;
        rl = SUNMIN((TWO * rl), rlmax);
        nbktrk_l++;

        N_VLinearSum(ONE, kin_mem->kin_uu, rl, kin_mem->kin_pp, kin_mem->kin_unew);
        retval = kin_mem->kin_func(kin_mem->kin_unew, kin_mem->kin_fval, kin_mem->kin_user_data); kin_mem->kin_nfe++;
        if (retval != 0) return(KIN_SYSFUNC_FAIL);
        *fnormp = N_VWL2Norm(kin_mem->kin_fval, kin_mem->kin_fscale);
        *f1normp = HALF * (*fnormp) * (*fnormp);

        alpha_cond = kin_mem->kin_f1norm + (alpha * slpi * rl);
        beta_cond = kin_mem->kin_f1norm + (beta * slpi * rl);

        if (kin_mem->kin_printfl > 2)
          KINPrintInfo(kin_mem, PRNT_BETA, "KINSOL", "KINLineSearch",
                       INFO_BETA, *f1normp, beta_cond, rl);

      } while (((*f1normp) <= alpha_cond) &&
               ((*f1normp) < beta_cond) && (rl < rlmax));

    } /* end if (rl == ONE) block */

    if ((rl < ONE) || ((rl > ONE) && (*f1normp > alpha_cond))) {

      rllo = SUNMIN(rl, rlprev);
      rldiff = SUNRabs(rlprev - rl);

      do {

        rlinc = HALF * rldiff;
        rl = rllo + rlinc;
        nbktrk_l++;

        N_VLinearSum(ONE, kin_mem->kin_uu, rl, kin_mem->kin_pp, kin_mem->kin_unew);
        retval = kin_mem->kin_func(kin_mem->kin_unew, kin_mem->kin_fval, kin_mem->kin_user_data); kin_mem->kin_nfe++;
        if (retval != 0) return(KIN_SYSFUNC_FAIL);
        *fnormp = N_VWL2Norm(kin_mem->kin_fval, kin_mem->kin_fscale);
        *f1normp = HALF * (*fnormp) * (*fnormp);

        alpha_cond = kin_mem->kin_f1norm + (alpha * slpi * rl);
        beta_cond = kin_mem->kin_f1norm + (beta * slpi * rl);

        if (kin_mem->kin_printfl > 2)
          KINPrintInfo(kin_mem, PRNT_ALPHABETA, "KINSOL", "KINLineSearch",
                       INFO_ALPHABETA, *f1normp, alpha_cond, beta_cond, rl);

        if ((*f1normp) > alpha_cond) rldiff = rlinc;
        else if (*f1normp < beta_cond) {
          rllo = rl;
          rldiff = rldiff - rlinc;
        }

      } while ((*f1normp > alpha_cond) ||
               ((*f1normp < beta_cond) && (rldiff >= rlmin)));

      if ( (*f1normp < beta_cond) || ((rldiff < rlmin) && (*f1normp > alpha_cond)) ) {

        /* beta condition could not be satisfied or rldiff too small
           and alpha_cond not satisfied, so set unew to last u value
           that satisfied the alpha condition and continue */

        N_VLinearSum(ONE, kin_mem->kin_uu, rllo, kin_mem->kin_pp, kin_mem->kin_unew);
        retval = kin_mem->kin_func(kin_mem->kin_unew, kin_mem->kin_fval, kin_mem->kin_user_data); kin_mem->kin_nfe++;
        if (retval != 0) return(KIN_SYSFUNC_FAIL);
        *fnormp = N_VWL2Norm(kin_mem->kin_fval, kin_mem->kin_fscale);
        *f1normp = HALF * (*fnormp) * (*fnormp);

        /* increment beta-condition failures counter */

        kin_mem->kin_nbcf++;

      }

    }  /* end of if (rl < ONE) block */

  }  /* end of if (f1normp < beta_cond) block */

  /* Update number of backtracking operations */

  kin_mem->kin_nbktrk += nbktrk_l;

  if (kin_mem->kin_printfl > 1)
    KINPrintInfo(kin_mem, PRNT_ADJ, "KINSOL", "KINLineSearch", INFO_ADJ, nbktrk_l);

  /* scale sFdotJp and sJpnorm by rl * ratio for later use in KINForcingTerm */

  kin_mem->kin_sFdotJp = kin_mem->kin_sFdotJp * rl * ratio;
  kin_mem->kin_sJpnorm = kin_mem->kin_sJpnorm * rl * ratio;

  if ((rl * pnorm) > (POINT99 * kin_mem->kin_mxnewtstep)) *maxStepTaken = SUNTRUE;

  return(KIN_SUCCESS);
}

/*
 * Function : KINConstraint
 *
 * This routine checks if the proposed solution vector uu + pp
 * violates any constraints. If a constraint is violated, then the
 * scalar stepmul is determined such that uu + stepmul * pp does
 * not violate any constraints.
 *
 * Note: This routine is called by the functions
 *       KINLineSearch and KINFullNewton.
 */

static int KINConstraint(KINMem kin_mem)
{
  N_VLinearSum(ONE, kin_mem->kin_uu, ONE, kin_mem->kin_pp, kin_mem->kin_vtemp1);

  /* if vtemp1[i] violates constraint[i] then vtemp2[i] = 1
     else vtemp2[i] = 0 (vtemp2 is the mask vector) */

  if(N_VConstrMask(kin_mem->kin_constraints, kin_mem->kin_vtemp1, kin_mem->kin_vtemp2)) return(KIN_SUCCESS);

  /* vtemp1[i] = SUNRabs(pp[i]) */

  N_VAbs(kin_mem->kin_pp, kin_mem->kin_vtemp1);

  /* consider vtemp1[i] only if vtemp2[i] = 1 (constraint violated) */

  N_VProd(kin_mem->kin_vtemp2, kin_mem->kin_vtemp1, kin_mem->kin_vtemp1);

  N_VAbs(kin_mem->kin_uu, kin_mem->kin_vtemp2);
  kin_mem->kin_stepmul = POINT9 * N_VMinQuotient(kin_mem->kin_vtemp2, kin_mem->kin_vtemp1);

  return(CONSTR_VIOLATED);
}

/*
 * -----------------------------------------------------------------
 * Stopping tests
 * -----------------------------------------------------------------
 */

/*
 * KINStop
 *
 * This routine checks the current iterate unew to see if the
 * system func(unew) = 0 is satisfied by a variety of tests.
 *
 * strategy is one of KIN_NONE or KIN_LINESEARCH
 * sflag    is one of KIN_SUCCESS, STEP_TOO_SMALL
 */

static int KINStop(KINMem kin_mem, booleantype maxStepTaken, int sflag)
{
  realtype fmax, rlength, omexp;
  N_Vector delta;

  /* Check for too small a step */

  if (sflag == STEP_TOO_SMALL) {

    if ((kin_mem->kin_lsetup != NULL) && !(kin_mem->kin_jacCurrent)) {
      /* If the Jacobian is out of date, update it and retry */
      kin_mem->kin_sthrsh = TWO;
      return(RETRY_ITERATION);
    } else {
      /* Give up */
      if (kin_mem->kin_globalstrategy == KIN_NONE)  return(KIN_STEP_LT_STPTOL);
      else                       return(KIN_LINESEARCH_NONCONV);
    }

  }

  /* Check tolerance on scaled function norm at the current iterate */

  fmax = KINScFNorm(kin_mem, kin_mem->kin_fval, kin_mem->kin_fscale);

  if (kin_mem->kin_printfl > 1)
    KINPrintInfo(kin_mem, PRNT_FMAX, "KINSOL", "KINStop", INFO_FMAX, fmax);

  if (fmax <= kin_mem->kin_fnormtol) return(KIN_SUCCESS);

  /* Check if the scaled distance between the last two steps is too small */
  /* NOTE: pp used as work space to store this distance */

  delta = kin_mem->kin_pp;
  N_VLinearSum(ONE, kin_mem->kin_unew, -ONE, kin_mem->kin_uu, delta);
  rlength = KINScSNorm(kin_mem, delta, kin_mem->kin_unew);

  if (rlength <= kin_mem->kin_scsteptol) {

    if ((kin_mem->kin_lsetup != NULL) && !(kin_mem->kin_jacCurrent)) {
      /* If the Jacobian is out of date, update it and retry */
      kin_mem->kin_sthrsh = TWO;
      return(CONTINUE_ITERATIONS);
    } else {
      /* give up */
      return(KIN_STEP_LT_STPTOL);
    }

  }

  /* Check if the maximum number of iterations is reached */

  if (kin_mem->kin_nni >= kin_mem->kin_mxiter) return(KIN_MAXITER_REACHED);

  /* Check for consecutive number of steps taken of size mxnewtstep
     and if not maxStepTaken, then set ncscmx to 0 */

  if (maxStepTaken) kin_mem->kin_ncscmx++;
  else              kin_mem->kin_ncscmx = 0;

  if (kin_mem->kin_ncscmx == 5) return(KIN_MXNEWT_5X_EXCEEDED);

  /* Proceed according to the type of linear solver used */

  if (kin_mem->kin_inexact_ls) {

    /* We're doing inexact Newton.
       Load threshold for reevaluating the Jacobian. */

    kin_mem->kin_sthrsh = rlength;

  } else if (!(kin_mem->kin_noResMon)) {

    /* We're doing modified Newton and the user did not disable residual monitoring.
       Check if it is time to monitor residual. */

    if ((kin_mem->kin_nni - kin_mem->kin_nnilset_sub) >= kin_mem->kin_msbset_sub) {

      /* Residual monitoring needed */

      kin_mem->kin_nnilset_sub = kin_mem->kin_nni;

      /* If indicated, estimate new OMEGA value */
      if (kin_mem->kin_eval_omega) {
        omexp = SUNMAX(ZERO,((kin_mem->kin_fnorm)/(kin_mem->kin_fnormtol))-ONE);
        kin_mem->kin_omega = (omexp > TWELVE)? kin_mem->kin_omega_max : SUNMIN(kin_mem->kin_omega_min * SUNRexp(omexp), kin_mem->kin_omega_max);
      }
      /* Check if making satisfactory progress */

      if (kin_mem->kin_fnorm > kin_mem->kin_omega * kin_mem->kin_fnorm_sub) {
        /* Insufficient progress */
        if ((kin_mem->kin_lsetup != NULL) && !(kin_mem->kin_jacCurrent)) {
          /* If the Jacobian is out of date, update it and retry */
          kin_mem->kin_sthrsh = TWO;
          return(CONTINUE_ITERATIONS);
        } else {
          /* Otherwise, we cannot do anything, so just return. */
        }
      } else {
        /* Sufficient progress */
        kin_mem->kin_fnorm_sub = kin_mem->kin_fnorm;
        kin_mem->kin_sthrsh = ONE;
      }

    } else {

      /* Residual monitoring not needed */

      /* Reset sthrsh */
      if (kin_mem->kin_retry_nni || kin_mem->kin_update_fnorm_sub) kin_mem->kin_fnorm_sub = kin_mem->kin_fnorm;
      if (kin_mem->kin_update_fnorm_sub) kin_mem->kin_update_fnorm_sub = SUNFALSE;
      kin_mem->kin_sthrsh = ONE;

    }

  }

  /* if made it to here, then the iteration process is not finished
     so return CONTINUE_ITERATIONS flag */

  return(CONTINUE_ITERATIONS);
}

/*
 * KINForcingTerm
 *
 * This routine computes eta, the scaling factor in the linear
 * convergence stopping tolerance eps when choice #1 or choice #2
 * forcing terms are used. Eta is computed here for all but the
 * first iterative step, which is set to the default in routine
 * KINSolInit.
 *
 * This routine was written by Homer Walker of Utah State
 * University with subsequent modifications by Allan Taylor @ LLNL.
 *
 * It is based on the concepts of the paper 'Choosing the forcing
 * terms in an inexact Newton method', SIAM J Sci Comput, 17
 * (1996), pp 16 - 32, or Utah State University Research Report
 * 6/94/75 of the same title.
 */

static void KINForcingTerm(KINMem kin_mem, realtype fnormp)
{
  realtype eta_max, eta_min, eta_safe, linmodel_norm;

  eta_max  = POINT9;
  eta_min  = POINT0001;
  eta_safe = HALF;

  /* choice #1 forcing term */

  if (kin_mem->kin_etaflag == KIN_ETACHOICE1) {

    /* compute the norm of f + Jp , scaled L2 norm */

    linmodel_norm = SUNRsqrt((kin_mem->kin_fnorm * kin_mem->kin_fnorm) +
                             (TWO * kin_mem->kin_sFdotJp) +
                             (kin_mem->kin_sJpnorm * kin_mem->kin_sJpnorm));

    /* form the safeguarded for choice #1 */

    eta_safe = SUNRpowerR(kin_mem->kin_eta, kin_mem->kin_eta_alpha);
    kin_mem->kin_eta = SUNRabs(fnormp - linmodel_norm) / kin_mem->kin_fnorm;
  }

  /* choice #2 forcing term */

  if (kin_mem->kin_etaflag == KIN_ETACHOICE2) {
    eta_safe = kin_mem->kin_eta_gamma *
      SUNRpowerR(kin_mem->kin_eta, kin_mem->kin_eta_alpha);

    kin_mem->kin_eta = kin_mem->kin_eta_gamma *
      SUNRpowerR((fnormp / kin_mem->kin_fnorm), kin_mem->kin_eta_alpha);
  }

  /* apply safeguards */

  if(eta_safe < POINT1) eta_safe = ZERO;
  kin_mem->kin_eta = SUNMAX(kin_mem->kin_eta, eta_safe);
  kin_mem->kin_eta = SUNMAX(kin_mem->kin_eta, eta_min);
  kin_mem->kin_eta = SUNMIN(kin_mem->kin_eta, eta_max);

  return;
}


/*
 * -----------------------------------------------------------------
 * Norm functions
 * -----------------------------------------------------------------
 */

/*
 * Function : KINScFNorm
 *
 * This routine computes the max norm for scaled vectors. The
 * scaling vector is scale, and the vector of which the norm is to
 * be determined is vv. The returned value, fnormval, is the
 * resulting scaled vector norm. This routine uses N_Vector
 * functions from the vector module.
 */

static realtype KINScFNorm(KINMem kin_mem, N_Vector v, N_Vector scale)
{
  N_VProd(scale, v, kin_mem->kin_vtemp1);
  return(N_VMaxNorm(kin_mem->kin_vtemp1));
}

/*
 * Function : KINScSNorm
 *
 * This routine computes the max norm of the scaled steplength, ss.
 * Here ucur is the current step and usc is the u scale factor.
 */

static realtype KINScSNorm(KINMem kin_mem, N_Vector v, N_Vector u)
{
  realtype length;

  N_VInv(kin_mem->kin_uscale, kin_mem->kin_vtemp1);
  N_VAbs(u, kin_mem->kin_vtemp2);
  N_VLinearSum(ONE, kin_mem->kin_vtemp1, ONE, kin_mem->kin_vtemp2, kin_mem->kin_vtemp1);
  N_VDiv(v, kin_mem->kin_vtemp1, kin_mem->kin_vtemp1);

  length = N_VMaxNorm(kin_mem->kin_vtemp1);

  return(length);
}

/*
 * =================================================================
 * KINSOL Verbose output functions
 * =================================================================
 */

/*
 * KINPrintInfo
 *
 * KINPrintInfo is a high level error handling function
 * Based on the value info_code, it composes the info message and
 * passes it to the info handler function.
 */

void KINPrintInfo(KINMem kin_mem,
                  int info_code, const char *module, const char *fname,
                  const char *msgfmt, ...)
{
  va_list ap;
  char msg[256], msg1[40];
  char retstr[30];
  int ret;

  /* Initialize argument processing
   (msgfrmt is the last required argument) */

  va_start(ap, msgfmt);

  if (info_code == PRNT_RETVAL) {

    /* If info_code = PRNT_RETVAL, decode the numeric value */

    ret = va_arg(ap, int);

    switch(ret) {
    case KIN_SUCCESS:
      sprintf(retstr, "KIN_SUCCESS");
      break;
    case KIN_SYSFUNC_FAIL:
      sprintf(retstr, "KIN_SYSFUNC_FAIL");
      break;
    case KIN_REPTD_SYSFUNC_ERR:
      sprintf(retstr, "KIN_REPTD_SYSFUNC_ERR");
      break;
    case KIN_STEP_LT_STPTOL:
      sprintf(retstr, "KIN_STEP_LT_STPTOL");
      break;
    case KIN_LINESEARCH_NONCONV:
      sprintf(retstr, "KIN_LINESEARCH_NONCONV");
      break;
    case KIN_LINESEARCH_BCFAIL:
      sprintf(retstr, "KIN_LINESEARCH_BCFAIL");
      break;
    case KIN_MAXITER_REACHED:
      sprintf(retstr, "KIN_MAXITER_REACHED");
      break;
    case KIN_MXNEWT_5X_EXCEEDED:
      sprintf(retstr, "KIN_MXNEWT_5X_EXCEEDED");
      break;
    case KIN_LINSOLV_NO_RECOVERY:
      sprintf(retstr, "KIN_LINSOLV_NO_RECOVERY");
      break;
    case KIN_LSETUP_FAIL:
      sprintf(retstr, "KIN_PRECONDSET_FAILURE");
      break;
    case KIN_LSOLVE_FAIL:
      sprintf(retstr, "KIN_PRECONDSOLVE_FAILURE");
      break;
    }

    /* Compose the message */

    sprintf(msg1, msgfmt, ret);
    sprintf(msg,"%s (%s)",msg1,retstr);


  } else {

    /* Compose the message */

    vsprintf(msg, msgfmt, ap);

  }

  /* call the info message handler */

  kin_mem->kin_ihfun(module, fname, msg, kin_mem->kin_ih_data);

  /* finalize argument processing */

  va_end(ap);

  return;
}


/*
 * KINInfoHandler
 *
 * This is the default KINSOL info handling function.
 * It sends the info message to the stream pointed to by kin_infofp
 */

void KINInfoHandler(const char *module, const char *function,
                    char *msg, void *data)
{
  KINMem kin_mem;

  /* data points to kin_mem here */

  kin_mem = (KINMem) data;

#ifndef NO_FPRINTF_OUTPUT
  if (kin_mem->kin_infofp != NULL) {
    fprintf(kin_mem->kin_infofp,"\n[%s] %s\n",module, function);
    fprintf(kin_mem->kin_infofp,"   %s\n",msg);
  }
#endif

}

/*
 * =================================================================
 * KINSOL Error Handling functions
 * =================================================================
 */

/*
 * KINProcessError
 *
 * KINProcessError is a high level error handling function.
 * - If cv_mem==NULL it prints the error message to stderr.
 * - Otherwise, it sets up and calls the error handling function
 *   pointed to by cv_ehfun.
 */

void KINProcessError(KINMem kin_mem,
                    int error_code, const char *module, const char *fname,
                    const char *msgfmt, ...)
{
  va_list ap;
  char msg[256];

  /* Initialize the argument pointer variable
     (msgfmt is the last required argument to KINProcessError) */

  va_start(ap, msgfmt);

  /* Compose the message */

  vsprintf(msg, msgfmt, ap);

  if (kin_mem == NULL) {    /* We write to stderr */
#ifndef NO_FPRINTF_OUTPUT
    fprintf(stderr, "\n[%s ERROR]  %s\n  ", module, fname);
    fprintf(stderr, "%s\n\n", msg);
#endif

  } else {                 /* We can call ehfun */
    kin_mem->kin_ehfun(error_code, module, fname, msg, kin_mem->kin_eh_data);
  }

  /* Finalize argument processing */
  va_end(ap);

  return;
}

/*
 * KINErrHandler
 *
 * This is the default error handling function.
 * It sends the error message to the stream pointed to by kin_errfp
 */

void KINErrHandler(int error_code, const char *module,
                   const char *function, char *msg, void *data)
{
  KINMem kin_mem;
  char err_type[10];

  /* data points to kin_mem here */

  kin_mem = (KINMem) data;

  if (error_code == KIN_WARNING)
    sprintf(err_type,"WARNING");
  else
    sprintf(err_type,"ERROR");

#ifndef NO_FPRINTF_OUTPUT
  if (kin_mem->kin_errfp != NULL) {
    fprintf(kin_mem->kin_errfp,"\n[%s %s]  %s\n",module,err_type,function);
    fprintf(kin_mem->kin_errfp,"  %s\n\n",msg);
  }
#endif

  return;
}


/*
 * =======================================================================
 * Picard and fixed point solvers
 * =======================================================================
 */

/*
 * KINPicardAA
 *
 * This routine is the main driver for the Picard iteration with
 * acclerated fixed point.
 */

static int KINPicardAA(KINMem kin_mem, long int *iterp, realtype *R,
                       realtype *gamma, realtype *fmaxptr)
{
  int retval, ret;
  long int iter;
  realtype fmax, epsmin, fnormp;
  N_Vector delta, gval;

  delta = kin_mem->kin_vtemp1;
  gval = kin_mem->kin_gval;
  ret = CONTINUE_ITERATIONS;
  fmax = kin_mem->kin_fnormtol + ONE;
  iter = 0;
  epsmin = ZERO;
  fnormp = -ONE;

  N_VConst(ZERO, gval);

  /* if eps is to be bounded from below, set the bound */
  if (kin_mem->kin_inexact_ls && !(kin_mem->kin_noMinEps)) epsmin = POINT01 * kin_mem->kin_fnormtol;

  while (ret == CONTINUE_ITERATIONS) {

    iter++;

    /* Update the forcing term for the inexact linear solves */
    if (kin_mem->kin_inexact_ls) {
      kin_mem->kin_eps = (kin_mem->kin_eta + kin_mem->kin_uround) * kin_mem->kin_fnorm;
      if(!(kin_mem->kin_noMinEps)) kin_mem->kin_eps = SUNMAX(epsmin, kin_mem->kin_eps);
    }

    /* evaluate g = uu - L^{-1}func(uu) and return if failed.
       For Picard, assume that the fval vector has been filled
       with an eval of the nonlinear residual prior to this call. */
    retval = KINPicardFcnEval(kin_mem, gval, kin_mem->kin_uu, kin_mem->kin_fval);

    if (retval < 0) {
      ret = KIN_SYSFUNC_FAIL;
      break;
    }

    if (kin_mem->kin_m_aa == 0) {
      N_VScale(ONE, gval, kin_mem->kin_unew);
    }
    else {  /* use Anderson, if desired */
      N_VScale(ONE, kin_mem->kin_uu, kin_mem->kin_unew);
      AndersonAcc(kin_mem, gval, delta, kin_mem->kin_unew, kin_mem->kin_uu, iter-1, R, gamma);
    }

    /* Fill the Newton residual based on the new solution iterate */
    retval = kin_mem->kin_func(kin_mem->kin_unew, kin_mem->kin_fval, kin_mem->kin_user_data); kin_mem->kin_nfe++;

    if (retval < 0) {
      ret = KIN_SYSFUNC_FAIL;
      break;
    }

    /* Evaluate function norms */
    fnormp = N_VWL2Norm(kin_mem->kin_fval, kin_mem->kin_fscale);
    fmax = KINScFNorm(kin_mem, kin_mem->kin_fval, kin_mem->kin_fscale); /* measure  || F(x) ||_max */
    kin_mem->kin_fnorm = fmax;
    *fmaxptr = fmax;

    if (kin_mem->kin_printfl > 1)
      KINPrintInfo(kin_mem, PRNT_FMAX, "KINSOL", "KINPicardAA", INFO_FMAX, fmax);

    /* print the current iter, fnorm, and nfe values if printfl > 0 */
    if (kin_mem->kin_printfl > 0)
      KINPrintInfo(kin_mem, PRNT_NNI, "KINSOL", "KINPicardAA", INFO_NNI, iter, kin_mem->kin_nfe, kin_mem->kin_fnorm);

    /* Check if the maximum number of iterations is reached */
    if (iter >= kin_mem->kin_mxiter) {
      ret = KIN_MAXITER_REACHED;
    }
    if (fmax <= kin_mem->kin_fnormtol) {
      ret = KIN_SUCCESS;
    }

    /* Update with new iterate. */
    N_VScale(ONE, kin_mem->kin_unew, kin_mem->kin_uu);

    if (ret == CONTINUE_ITERATIONS) {
      /* evaluate eta by calling the forcing term routine */
      if (kin_mem->kin_callForcingTerm) KINForcingTerm(kin_mem, fnormp);
    }

    fflush(kin_mem->kin_errfp);

  }  /* end of loop; return */

  *iterp = iter;

  if (kin_mem->kin_printfl > 0)
    KINPrintInfo(kin_mem, PRNT_RETVAL, "KINSOL", "KINPicardAA", INFO_RETVAL, ret);

  return(ret);
}

/*
 * KINPicardFcnEval
 *
 * This routine evaluates the Picard fixed point function
 * using the linear solver, gval = u - L^{-1}F(u).
 * The function assumes the user has defined L either through
 * a user-supplied matvec if using a SPILS solver or through
 * a supplied matrix if using a dense solver.  This assumption is
 * tested by a check on the strategy and the requisite functionality
 * within the linear solve routines.
 *
 * This routine fills gval = uu - L^{-1}F(uu) given uu and fval = F(uu).
 */

static int KINPicardFcnEval(KINMem kin_mem, N_Vector gval, N_Vector uval, N_Vector fval1)
{
  int retval;

  if ((kin_mem->kin_nni - kin_mem->kin_nnilset) >= kin_mem->kin_msbset) {
    kin_mem->kin_sthrsh = TWO;
    kin_mem->kin_update_fnorm_sub = SUNTRUE;
  }

  for(;;){

    kin_mem->kin_jacCurrent = SUNFALSE;

    if ((kin_mem->kin_sthrsh > ONEPT5) && (kin_mem->kin_lsetup != NULL)) {
      retval = kin_mem->kin_lsetup(kin_mem);
      kin_mem->kin_jacCurrent = SUNTRUE;
      kin_mem->kin_nnilset = kin_mem->kin_nni;
      kin_mem->kin_nnilset_sub = kin_mem->kin_nni;
      if (retval != 0) return(KIN_LSETUP_FAIL);
    }

    /* call the generic 'lsolve' routine to solve the system Lx = -fval
       Note that we are using gval to hold x. */
    N_VScale(-ONE, fval1, fval1);
    retval = kin_mem->kin_lsolve(kin_mem, gval, fval1, &(kin_mem->kin_sJpnorm),
                                 &(kin_mem->kin_sFdotJp));

    if (retval == 0) {
      /* Update gval = uval + gval since gval = -L^{-1}F(uu)  */
      N_VLinearSum(ONE, uval, ONE, gval, gval);
      return(KIN_SUCCESS);
    }
    else if (retval < 0)                      return(KIN_LSOLVE_FAIL);
    else if ((kin_mem->kin_lsetup == NULL) || (kin_mem->kin_jacCurrent)) return(KIN_LINSOLV_NO_RECOVERY);

    /* loop back only if the linear solver setup is in use
       and matrix information is not current */

    kin_mem->kin_sthrsh = TWO;
  }

}


/*
 * KINFP
 *
 * This routine is the main driver for the fixed point iteration with
 * Anderson Acceleration.
 */

static int KINFP(KINMem kin_mem)
{
  int retval;     /* return value from user func */
  int ret;        /* iteration status            */
  realtype fmax;  /* max norm of residual func   */
  N_Vector delta; /* temporary workspace vector  */

  delta = kin_mem->kin_vtemp1;
  ret   = CONTINUE_ITERATIONS;
  fmax  = kin_mem->kin_fnormtol + ONE;

  /* initialize iteration count */
  kin_mem->kin_nni = 0;

  while (ret == CONTINUE_ITERATIONS) {

    /* update iteration count */
    kin_mem->kin_nni++;

    /* evaluate func(uu) and return if failed */
    retval = kin_mem->kin_func(kin_mem->kin_uu, kin_mem->kin_fval,
                               kin_mem->kin_user_data);
    kin_mem->kin_nfe++;

    if (retval < 0) {
      ret = KIN_SYSFUNC_FAIL;
      break;
    }

    /* compute new solution */
    if (kin_mem->kin_m_aa == 0) {
      /* standard fixed point */
      N_VScale(ONE, kin_mem->kin_fval, kin_mem->kin_unew);
    } else {
      /* apply Anderson acceleration */
      AndersonAcc(kin_mem, kin_mem->kin_fval, delta, kin_mem->kin_unew,
                  kin_mem->kin_uu, kin_mem->kin_nni - 1, kin_mem->kin_R_aa,
                  kin_mem->kin_gamma_aa);
    }

    /* compute change between iterations */
    N_VLinearSum(ONE, kin_mem->kin_unew, -ONE, kin_mem->kin_uu, delta);

    fmax = KINScFNorm(kin_mem, delta, kin_mem->kin_fscale); /* measure  || g(x)-x || */

    if (kin_mem->kin_printfl > 1)
      KINPrintInfo(kin_mem, PRNT_FMAX, "KINSOL", "KINFP", INFO_FMAX, fmax);

    kin_mem->kin_fnorm = fmax;

    /* print the current iter, fnorm, and nfe values if printfl > 0 */
    if (kin_mem->kin_printfl > 0)
      KINPrintInfo(kin_mem, PRNT_NNI, "KINSOL", "KINFP", INFO_NNI,
                   kin_mem->kin_nni, kin_mem->kin_nfe, kin_mem->kin_fnorm);

    /* Check if the maximum number of iterations is reached */
    if (kin_mem->kin_nni >= kin_mem->kin_mxiter) {
      ret = KIN_MAXITER_REACHED;
    }
    if (fmax <= kin_mem->kin_fnormtol) {
      ret = KIN_SUCCESS;
    }

    if (ret == CONTINUE_ITERATIONS) {
      /* Only update solution if taking a next iteration.  */
      /* CSW  Should put in a conditional to send back the newest iterate or
         the one consistent with the fval */
      N_VScale(ONE, kin_mem->kin_unew, kin_mem->kin_uu);
    }

    fflush(kin_mem->kin_errfp);

  }  /* end of loop; return */

  if (kin_mem->kin_printfl > 0)
    KINPrintInfo(kin_mem, PRNT_RETVAL, "KINSOL", "KINFP", INFO_RETVAL, ret);

  return(ret);
}


/* -----------------------------------------------------------------
 * Stopping tests
 * -----------------------------------------------------------------
 */


/*
 * ========================================================================
 * Anderson Acceleration
 * ========================================================================
 */

static int AndersonAcc(KINMem kin_mem, N_Vector gval, N_Vector fv,
                       N_Vector x, N_Vector xold,
                       long int iter, realtype *R, realtype *gamma)
{
  int retval;
  long int i_pt, i, j, lAA;
  long int *ipt_map;
  realtype alfa;
  realtype onembeta;
  realtype a, b, temp, c, s;

  /* local shortcuts for fused vector operation */
  int       nvec=0;
  realtype* cv=kin_mem->kin_cv;
  N_Vector* Xv=kin_mem->kin_Xv;

  ipt_map = kin_mem->kin_ipt_map;
  i_pt = iter-1 - ((iter-1) / kin_mem->kin_m_aa) * kin_mem->kin_m_aa;
  N_VLinearSum(ONE, gval, -ONE, xold, fv);
  if (iter > 0) {
    /* compute dg_new = gval - gval_old */
    N_VLinearSum(ONE, gval, -ONE, kin_mem->kin_gold_aa, kin_mem->kin_dg_aa[i_pt]);
    /* compute df_new = fval - fval_old */
    N_VLinearSum(ONE, fv, -ONE, kin_mem->kin_fold_aa, kin_mem->kin_df_aa[i_pt]);
  }

  N_VScale(ONE, gval, kin_mem->kin_gold_aa);
  N_VScale(ONE, fv, kin_mem->kin_fold_aa);

  /* on first iteration, just do basic fixed-point update */
  if (iter == 0) {
    N_VScale(ONE, gval, x);
    return(0);
  }

  /* update data structures based on current iteration index */

  if (iter == 1) {

    /* second iteration */
    R[0] = SUNRsqrt(N_VDotProd(kin_mem->kin_df_aa[i_pt], kin_mem->kin_df_aa[i_pt]));
    alfa = ONE/R[0];
    N_VScale(alfa, kin_mem->kin_df_aa[i_pt], kin_mem->kin_q_aa[i_pt]);
    ipt_map[0] = 0;

  } else if (iter <= kin_mem->kin_m_aa) {

    /* another iteration before we've reached maa */
    N_VScale(ONE, kin_mem->kin_df_aa[i_pt], kin_mem->kin_vtemp2);
    for (j=0; j < (iter-1); j++) {
      ipt_map[j] = j;
      R[(iter-1)*kin_mem->kin_m_aa+j] = N_VDotProd(kin_mem->kin_q_aa[j], kin_mem->kin_vtemp2);
      N_VLinearSum(ONE,kin_mem->kin_vtemp2, -R[(iter-1)*kin_mem->kin_m_aa+j], kin_mem->kin_q_aa[j], kin_mem->kin_vtemp2);
    }
    R[(iter-1)*kin_mem->kin_m_aa+iter-1] = SUNRsqrt(N_VDotProd(kin_mem->kin_vtemp2, kin_mem->kin_vtemp2));
    N_VScale((1/R[(iter-1)*kin_mem->kin_m_aa+iter-1]), kin_mem->kin_vtemp2, kin_mem->kin_q_aa[i_pt]);
    ipt_map[iter-1] = iter-1;

  } else {

    /* we've filled the acceleration subspace, so start recycling */

    /* Delete left-most column vector from QR factorization */
    for (i=0; i < kin_mem->kin_m_aa-1; i++) {
      a = R[(i+1)*kin_mem->kin_m_aa + i];
      b = R[(i+1)*kin_mem->kin_m_aa + i+1];
      temp = SUNRsqrt(a*a + b*b);
      c = a / temp;
      s = b / temp;
      R[(i+1)*kin_mem->kin_m_aa + i] = temp;
      R[(i+1)*kin_mem->kin_m_aa + i+1] = ZERO;
      /* OK to re-use temp */
      if (i < kin_mem->kin_m_aa-1) {
        for (j = i+2; j < kin_mem->kin_m_aa; j++) {
          a = R[j*kin_mem->kin_m_aa + i];
          b = R[j*kin_mem->kin_m_aa + i+1];
          temp = c * a + s * b;
          R[j*kin_mem->kin_m_aa + i+1] = -s*a + c*b;
          R[j*kin_mem->kin_m_aa + i] = temp;
        }
      }
      N_VLinearSum(c, kin_mem->kin_q_aa[i], s, kin_mem->kin_q_aa[i+1], kin_mem->kin_vtemp2);
      N_VLinearSum(-s, kin_mem->kin_q_aa[i], c, kin_mem->kin_q_aa[i+1], kin_mem->kin_q_aa[i+1]);
      N_VScale(ONE, kin_mem->kin_vtemp2, kin_mem->kin_q_aa[i]);
    }

    /* Shift R to the left by one. */
    for (i = 1; i < kin_mem->kin_m_aa; i++) {
      for (j = 0; j < kin_mem->kin_m_aa-1; j++) {
        R[(i-1)*kin_mem->kin_m_aa + j] = R[i*kin_mem->kin_m_aa + j];
      }
    }

    /* Add the new df vector */
    N_VScale(ONE, kin_mem->kin_df_aa[i_pt], kin_mem->kin_vtemp2);
    for (j=0; j < (kin_mem->kin_m_aa-1); j++) {
      R[(kin_mem->kin_m_aa-1)*kin_mem->kin_m_aa+j] = N_VDotProd(kin_mem->kin_q_aa[j], kin_mem->kin_vtemp2);
      N_VLinearSum(ONE, kin_mem->kin_vtemp2, -R[(kin_mem->kin_m_aa-1)*kin_mem->kin_m_aa+j], kin_mem->kin_q_aa[j],kin_mem->kin_vtemp2);
    }
    R[(kin_mem->kin_m_aa-1)*kin_mem->kin_m_aa+kin_mem->kin_m_aa-1] = SUNRsqrt(N_VDotProd(kin_mem->kin_vtemp2, kin_mem->kin_vtemp2));
    N_VScale((1/R[(kin_mem->kin_m_aa-1)*kin_mem->kin_m_aa+kin_mem->kin_m_aa-1]), kin_mem->kin_vtemp2, kin_mem->kin_q_aa[kin_mem->kin_m_aa-1]);

    /* Update the iteration map */
    j = 0;
    for (i=i_pt+1; i < kin_mem->kin_m_aa; i++)
      ipt_map[j++] = i;
    for (i=0; i < (i_pt+1); i++)
      ipt_map[j++] = i;
  }

  /* Solve least squares problem and update solution */
  lAA = iter;
  if (kin_mem->kin_m_aa < iter) lAA = kin_mem->kin_m_aa;

  retval = N_VDotProdMulti((int) lAA, fv, kin_mem->kin_q_aa, gamma);
  if (retval != KIN_SUCCESS) return(KIN_VECTOROP_ERR);

  /* set arrays for fused vector operation */
  cv[0] = ONE;
  Xv[0] = gval;
  nvec = 1;

  for (i=lAA-1; i > -1; i--) {
    for (j=i+1; j < lAA; j++) {
      gamma[i] = gamma[i]-R[j*kin_mem->kin_m_aa+i]*gamma[j];
    }
    gamma[i] = gamma[i]/R[i*kin_mem->kin_m_aa+i];

    cv[nvec] = -gamma[i];
    Xv[nvec] = kin_mem->kin_dg_aa[ipt_map[i]];
    nvec += 1;
  }

  /* if enabled, apply damping */
  if (kin_mem->kin_damping_aa) {
    onembeta = (ONE - kin_mem->kin_beta_aa);
    cv[nvec] = -onembeta;
    Xv[nvec] = fv;
    nvec += 1;
    for (i = lAA - 1; i > -1; i--) {
      cv[nvec] = onembeta * gamma[i];
      Xv[nvec] = kin_mem->kin_df_aa[ipt_map[i]];
      nvec += 1;
    }
  }

  /* update solution */
  retval = N_VLinearCombination(nvec, cv, Xv, x);
  if (retval != KIN_SUCCESS) return(KIN_VECTOROP_ERR);

  return 0;
}
