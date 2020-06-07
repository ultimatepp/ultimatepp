/*
 * -----------------------------------------------------------------
 * $Revision$
 * $Date$
 * ----------------------------------------------------------------- 
 * Programmers: Alan C. Hindmarsh, and Radu Serban @ LLNL
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
 * This is the implementation file for the IC calculation for IDA.
 * It is independent of the linear solver in use.                  
 * -----------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>

#include "ida_impl.h"
#include <sundials/sundials_math.h>

/*
 * =================================================================
 * IDA Constants 
 * =================================================================
 */

/* Private Constants */

#define ZERO       RCONST(0.0)    /* real 0.0    */
#define HALF       RCONST(0.5)    /* real 0.5    */
#define ONE        RCONST(1.0)    /* real 1.0    */
#define TWO        RCONST(2.0)    /* real 2.0    */
#define PT99       RCONST(0.99)   /* real 0.99   */
#define PT1        RCONST(0.1)    /* real 0.1    */
#define PT001      RCONST(0.001)  /* real 0.001  */

/* IDACalcIC control constants */

#define ICRATEMAX  RCONST(0.9)    /* max. Newton conv. rate */
#define ALPHALS    RCONST(0.0001) /* alpha in linesearch conv. test */

/* Return values for lower level routines used by IDACalcIC */

#define IC_FAIL_RECOV       1
#define IC_CONSTR_FAILED    2
#define IC_LINESRCH_FAILED  3
#define IC_CONV_FAIL        4  
#define IC_SLOW_CONVRG      5

/*
 * =================================================================
 * Private Helper Functions Prototypes
 * =================================================================
 */

extern int IDAInitialSetup(IDAMem IDA_mem);
extern realtype IDAWrmsNorm(IDAMem IDA_mem, N_Vector x, N_Vector w, 
                            booleantype mask);

static int IDAnlsIC(IDAMem IDA_mem);
static int IDANewtonIC(IDAMem IDA_mem);
static int IDALineSrch(IDAMem IDA_mem, realtype *delnorm, realtype *fnorm);
static int IDAfnorm(IDAMem IDA_mem, realtype *fnorm);
static int IDANewyyp(IDAMem IDA_mem, realtype lambda);
static int IDANewy(IDAMem IDA_mem);
static int IDAICFailFlag(IDAMem IDA_mem, int retval);

/*
 * =================================================================
 * EXPORTED FUNCTIONS IMPLEMENTATION
 * =================================================================
 */

/*
 * -----------------------------------------------------------------
 * IDACalcIC
 * -----------------------------------------------------------------
 * IDACalcIC computes consistent initial conditions, given the 
 * user's initial guess for unknown components of yy0 and/or yp0.
 *
 * The return value is IDA_SUCCESS = 0 if no error occurred.
 *
 * The error return values (fully described in ida.h) are:
 *   IDA_MEM_NULL        ida_mem is NULL
 *   IDA_NO_MALLOC       ida_mem was not allocated
 *   IDA_ILL_INPUT       bad value for icopt, tout1, or id
 *   IDA_LINIT_FAIL      the linear solver linit routine failed
 *   IDA_BAD_EWT         zero value of some component of ewt
 *   IDA_RES_FAIL        res had a non-recoverable error
 *   IDA_FIRST_RES_FAIL  res failed recoverably on the first call
 *   IDA_LSETUP_FAIL     lsetup had a non-recoverable error
 *   IDA_LSOLVE_FAIL     lsolve had a non-recoverable error
 *   IDA_NO_RECOVERY     res, lsetup, or lsolve had a recoverable
 *                       error, but IDACalcIC could not recover
 *   IDA_CONSTR_FAIL     the inequality constraints could not be met
 *   IDA_LINESEARCH_FAIL the linesearch failed (either on steptol test
 *                       or on the maxbacks test)
 *   IDA_CONV_FAIL       the Newton iterations failed to converge
 * -----------------------------------------------------------------
 */

int IDACalcIC(void *ida_mem, int icopt, realtype tout1)
{
  int ewtsetOK;
  int ier, nwt, nh, mxnh, icret, retval=0;
  realtype tdist, troundoff, minid, hic, ypnorm;
  IDAMem IDA_mem;

  /* Check if IDA memory exists */

  if(ida_mem == NULL) {
    IDAProcessError(NULL, IDA_MEM_NULL, "IDA", "IDACalcIC", MSG_NO_MEM);
    return(IDA_MEM_NULL);
  }
  IDA_mem = (IDAMem) ida_mem;

  /* Check if problem was malloc'ed */
  
  if(IDA_mem->ida_MallocDone == SUNFALSE) {
    IDAProcessError(IDA_mem, IDA_NO_MALLOC, "IDA", "IDACalcIC", MSG_NO_MALLOC);
    return(IDA_NO_MALLOC);
  }

  /* Check inputs to IDA for correctness and consistency */

  ier = IDAInitialSetup(IDA_mem);
  if(ier != IDA_SUCCESS) return(IDA_ILL_INPUT);
  IDA_mem->ida_SetupDone = SUNTRUE;

  /* Check legality of input arguments, and set IDA memory copies. */

  if(icopt != IDA_YA_YDP_INIT && icopt != IDA_Y_INIT) {
    IDAProcessError(IDA_mem, IDA_ILL_INPUT, "IDA", "IDACalcIC", MSG_IC_BAD_ICOPT);
    return(IDA_ILL_INPUT);
  }
  IDA_mem->ida_icopt = icopt;

  if(icopt == IDA_YA_YDP_INIT && (IDA_mem->ida_id == NULL)) {
    IDAProcessError(IDA_mem, IDA_ILL_INPUT, "IDA", "IDACalcIC", MSG_IC_MISSING_ID);
    return(IDA_ILL_INPUT);
  }

  tdist = SUNRabs(tout1 - IDA_mem->ida_tn);
  troundoff = TWO * IDA_mem->ida_uround * (SUNRabs(IDA_mem->ida_tn) + SUNRabs(tout1));
  if(tdist < troundoff) {
    IDAProcessError(IDA_mem, IDA_ILL_INPUT, "IDA", "IDACalcIC", MSG_IC_TOO_CLOSE);
    return(IDA_ILL_INPUT);
  }

  /* Allocate space and initialize temporary vectors */

  IDA_mem->ida_yy0 = N_VClone(IDA_mem->ida_ee);
  IDA_mem->ida_yp0 = N_VClone(IDA_mem->ida_ee);
  IDA_mem->ida_t0  = IDA_mem->ida_tn;
  N_VScale(ONE, IDA_mem->ida_phi[0], IDA_mem->ida_yy0);
  N_VScale(ONE, IDA_mem->ida_phi[1], IDA_mem->ida_yp0);

  /* For use in the IDA_YA_YP_INIT case, set sysindex and tscale. */

  IDA_mem->ida_sysindex = 1;
  IDA_mem->ida_tscale   = tdist;
  if(icopt == IDA_YA_YDP_INIT) {
    minid = N_VMin(IDA_mem->ida_id);
    if(minid < ZERO) {
      IDAProcessError(IDA_mem, IDA_ILL_INPUT, "IDA", "IDACalcIC", MSG_IC_BAD_ID);
      return(IDA_ILL_INPUT);
    }
    if(minid > HALF) IDA_mem->ida_sysindex = 0;
  }

  /* Set the test constant in the Newton convergence test */

  IDA_mem->ida_epsNewt = IDA_mem->ida_epiccon;

  /* Initializations: 
     cjratio = 1 (for use in direct linear solvers); 
     set nbacktr = 0; */

  IDA_mem->ida_cjratio = ONE;
  IDA_mem->ida_nbacktr = 0;

  /* Set hic, hh, cj, and mxnh. */

  hic = PT001*tdist;
  ypnorm = IDAWrmsNorm(IDA_mem, IDA_mem->ida_yp0,
                       IDA_mem->ida_ewt, IDA_mem->ida_suppressalg);
  if(ypnorm > HALF/hic) hic = HALF/ypnorm;
  if(tout1 < IDA_mem->ida_tn) hic = -hic;
  IDA_mem->ida_hh = hic;
  if(icopt == IDA_YA_YDP_INIT) {
    IDA_mem->ida_cj = ONE/hic;
    mxnh = IDA_mem->ida_maxnh;
  }
  else {
    IDA_mem->ida_cj = ZERO;
    mxnh = 1;
  }

  /* Loop over nwt = number of evaluations of ewt vector. */

  for(nwt = 1; nwt <= 2; nwt++) {
 
    /* Loop over nh = number of h values. */
    for(nh = 1; nh <= mxnh; nh++) {

      /* Call the IC nonlinear solver function. */
      retval = IDAnlsIC(IDA_mem);

      /* Cut h and loop on recoverable IDA_YA_YDP_INIT failure; else break. */
      if(retval == IDA_SUCCESS) break;
      IDA_mem->ida_ncfn++;
      if(retval < 0) break;
      if(nh == mxnh) break;
      /* If looping to try again, reset yy0 and yp0 if not converging. */
      if(retval != IC_SLOW_CONVRG) {
        N_VScale(ONE, IDA_mem->ida_phi[0], IDA_mem->ida_yy0);
        N_VScale(ONE, IDA_mem->ida_phi[1], IDA_mem->ida_yp0);
      }
      hic *= PT1;
      IDA_mem->ida_cj = ONE/hic;
      IDA_mem->ida_hh = hic;
    }   /* End of nh loop */

    /* Break on failure; else reset ewt, save yy0, yp0 in phi, and loop. */
    if(retval != IDA_SUCCESS) break;
    ewtsetOK = IDA_mem->ida_efun(IDA_mem->ida_yy0, IDA_mem->ida_ewt,
                                 IDA_mem->ida_edata);
    if(ewtsetOK != 0) { 
      retval = IDA_BAD_EWT; 
      break; 
    }
    N_VScale(ONE, IDA_mem->ida_yy0, IDA_mem->ida_phi[0]);
    N_VScale(ONE, IDA_mem->ida_yp0, IDA_mem->ida_phi[1]);

  }   /* End of nwt loop */

  /* Free temporary space */

  N_VDestroy(IDA_mem->ida_yy0);
  N_VDestroy(IDA_mem->ida_yp0);

  /* Load the optional outputs. */

  if(icopt == IDA_YA_YDP_INIT)   IDA_mem->ida_hused = hic;

  /* On any failure, print message and return proper flag. */

  if(retval != IDA_SUCCESS) {
    icret = IDAICFailFlag(IDA_mem, retval);
    return(icret);
  }

  /* Otherwise return success flag. */

  return(IDA_SUCCESS);

}

/*
 * =================================================================
 * PRIVATE FUNCTIONS IMPLEMENTATION
 * =================================================================
 */

/*
 * -----------------------------------------------------------------
 * IDAnlsIC
 * -----------------------------------------------------------------
 * IDAnlsIC solves a nonlinear system for consistent initial 
 * conditions.  It calls IDANewtonIC to do most of the work.
 *
 * The return value is IDA_SUCCESS = 0 if no error occurred.
 * The error return values (positive) considered recoverable are:
 *  IC_FAIL_RECOV      if res, lsetup, or lsolve failed recoverably
 *  IC_CONSTR_FAILED   if the constraints could not be met
 *  IC_LINESRCH_FAILED if the linesearch failed (either on steptol test
 *                     or on maxbacks test)
 *  IC_CONV_FAIL       if the Newton iterations failed to converge
 *  IC_SLOW_CONVRG     if the iterations are converging slowly
 *                     (failed the convergence test, but showed
 *                     norm reduction or convergence rate < 1)
 * The error return values (negative) considered non-recoverable are:
 *  IDA_RES_FAIL       if res had a non-recoverable error
 *  IDA_FIRST_RES_FAIL if res failed recoverably on the first call
 *  IDA_LSETUP_FAIL    if lsetup had a non-recoverable error
 *  IDA_LSOLVE_FAIL    if lsolve had a non-recoverable error
 * -----------------------------------------------------------------
 */

static int IDAnlsIC (IDAMem IDA_mem)
{
  int retval, nj;
  N_Vector tv1, tv2, tv3;

  tv1 = IDA_mem->ida_ee;
  tv2 = IDA_mem->ida_tempv2;
  tv3 = IDA_mem->ida_phi[2];

  retval = IDA_mem->ida_res(IDA_mem->ida_t0, IDA_mem->ida_yy0,
                            IDA_mem->ida_yp0, IDA_mem->ida_delta,
                            IDA_mem->ida_user_data);
  IDA_mem->ida_nre++;
  if(retval < 0) return(IDA_RES_FAIL);
  if(retval > 0) return(IDA_FIRST_RES_FAIL);

  N_VScale(ONE, IDA_mem->ida_delta, IDA_mem->ida_savres);

  /* Loop over nj = number of linear solve Jacobian setups. */

  for(nj = 1; nj <= IDA_mem->ida_maxnj; nj++) {

    /* If there is a setup routine, call it. */
    if(IDA_mem->ida_lsetup) {
      IDA_mem->ida_nsetups++;
      retval = IDA_mem->ida_lsetup(IDA_mem, IDA_mem->ida_yy0,
                                   IDA_mem->ida_yp0, IDA_mem->ida_delta,
                                   tv1, tv2, tv3);
      if(retval < 0) return(IDA_LSETUP_FAIL);
      if(retval > 0) return(IC_FAIL_RECOV);
    }

    /* Call the Newton iteration routine, and return if successful.  */
    retval = IDANewtonIC(IDA_mem);
    if(retval == IDA_SUCCESS) return(IDA_SUCCESS);

    /* If converging slowly and lsetup is nontrivial, retry. */
    if(retval == IC_SLOW_CONVRG && IDA_mem->ida_lsetup) {
      N_VScale(ONE, IDA_mem->ida_savres, IDA_mem->ida_delta);
      continue;
    } else {
      return(retval);
    }

  }   /* End of nj loop */

  /* No convergence after maxnj tries; return with retval=IC_SLOW_CONVRG */
  return(retval);

}

/*
 * -----------------------------------------------------------------
 * IDANewtonIC
 * -----------------------------------------------------------------
 * IDANewtonIC performs the Newton iteration to solve for consistent
 * initial conditions.  It calls IDALineSrch within each iteration.
 * On return, savres contains the current residual vector.
 *
 * The return value is IDA_SUCCESS = 0 if no error occurred.
 * The error return values (positive) considered recoverable are:
 *  IC_FAIL_RECOV      if res or lsolve failed recoverably
 *  IC_CONSTR_FAILED   if the constraints could not be met
 *  IC_LINESRCH_FAILED if the linesearch failed (either on steptol test
 *                     or on maxbacks test)
 *  IC_CONV_FAIL       if the Newton iterations failed to converge
 *  IC_SLOW_CONVRG     if the iterations appear to be converging slowly.
 *                     They failed the convergence test, but showed 
 *                     an overall norm reduction (by a factor of < 0.1)
 *                     or a convergence rate <= ICRATEMAX).
 * The error return values (negative) considered non-recoverable are:
 *  IDA_RES_FAIL   if res had a non-recoverable error
 *  IDA_LSOLVE_FAIL      if lsolve had a non-recoverable error
 * -----------------------------------------------------------------
 */

static int IDANewtonIC(IDAMem IDA_mem)
{
  int retval, mnewt;
  realtype delnorm, fnorm, fnorm0, oldfnrm, rate;

  /* Set pointer for vector delnew */
  IDA_mem->ida_delnew = IDA_mem->ida_phi[2];

  /* Call the linear solve function to get the Newton step, delta. */
  retval = IDA_mem->ida_lsolve(IDA_mem, IDA_mem->ida_delta,
                               IDA_mem->ida_ewt, IDA_mem->ida_yy0,
                               IDA_mem->ida_yp0, IDA_mem->ida_savres);
  if(retval < 0) return(IDA_LSOLVE_FAIL);
  if(retval > 0) return(IC_FAIL_RECOV);

  /* Compute the norm of the step; return now if this is small. */
  fnorm = IDAWrmsNorm(IDA_mem, IDA_mem->ida_delta, IDA_mem->ida_ewt, SUNFALSE);
  if(IDA_mem->ida_sysindex == 0)
    fnorm *= IDA_mem->ida_tscale * SUNRabs(IDA_mem->ida_cj);
  if(fnorm <= IDA_mem->ida_epsNewt)
    return(IDA_SUCCESS);
  fnorm0 = fnorm;

  /* Initialize rate to avoid compiler warning message */
  rate = ZERO;

  /* Newton iteration loop */

  for(mnewt = 0; mnewt < IDA_mem->ida_maxnit; mnewt++) {

    IDA_mem->ida_nni++;
    delnorm = fnorm;
    oldfnrm = fnorm;

    /* Call the Linesearch function and return if it failed. */
    retval = IDALineSrch(IDA_mem, &delnorm, &fnorm);
    if(retval != IDA_SUCCESS) return(retval);

    /* Set the observed convergence rate and test for convergence. */
    rate = fnorm/oldfnrm;
    if(fnorm <= IDA_mem->ida_epsNewt) return(IDA_SUCCESS);

    /* If not converged, copy new step vector, and loop. */
    N_VScale(ONE, IDA_mem->ida_delnew, IDA_mem->ida_delta);

  }   /* End of Newton iteration loop */

  /* Return either IC_SLOW_CONVRG or recoverable fail flag. */
  if(rate <= ICRATEMAX || fnorm < PT1*fnorm0) return(IC_SLOW_CONVRG);
  return(IC_CONV_FAIL);

}


/*
 * -----------------------------------------------------------------
 * IDALineSrch
 * -----------------------------------------------------------------
 * IDALineSrch performs the Linesearch algorithm with the 
 * calculation of consistent initial conditions.
 *
 * On entry, yy0 and yp0 are the current values of y and y', the 
 * Newton step is delta, the current residual vector F is savres,
 * delnorm is WRMS-norm(delta), and fnorm is the norm of the vector
 * J-inverse F.
 *
 * On a successful return, yy0, yp0, and savres have been updated, 
 * delnew contains the current value of J-inverse F, and fnorm is
 * WRMS-norm(delnew).
 *
 * The return value is IDA_SUCCESS = 0 if no error occurred.
 * The error return values (positive) considered recoverable are:
 *  IC_FAIL_RECOV      if res or lsolve failed recoverably
 *  IC_CONSTR_FAILED   if the constraints could not be met
 *  IC_LINESRCH_FAILED if the linesearch failed (either on steptol test
 *                     or on maxbacks test)
 * The error return values (negative) considered non-recoverable are:
 *  IDA_RES_FAIL   if res had a non-recoverable error
 *  IDA_LSOLVE_FAIL      if lsolve had a non-recoverable error
 * -----------------------------------------------------------------
 */

static int IDALineSrch(IDAMem IDA_mem, realtype *delnorm, realtype *fnorm)
{
  booleantype conOK;
  int retval, nbacks;
  realtype f1norm, fnormp, f1normp, ratio, lambda, minlam, slpi;
  N_Vector mc;

  /* Initialize work space pointers, f1norm, ratio.
     (Use of mc in constraint check does not conflict with ypnew.) */
  mc = IDA_mem->ida_ee;
  IDA_mem->ida_dtemp = IDA_mem->ida_phi[3];
  IDA_mem->ida_ynew = IDA_mem->ida_tempv2;
  IDA_mem->ida_ypnew = IDA_mem->ida_ee;
  f1norm = (*fnorm)*(*fnorm)*HALF;
  ratio = ONE;

  /* If there are constraints, check and reduce step if necessary. */
  if(IDA_mem->ida_constraintsSet) {

    /* Update y and check constraints. */
    IDANewy(IDA_mem);
    conOK = N_VConstrMask(IDA_mem->ida_constraints, IDA_mem->ida_ynew, mc);

    if(!conOK) {
      /* Not satisfied.  Compute scaled step to satisfy constraints. */
      N_VProd(mc, IDA_mem->ida_delta, IDA_mem->ida_dtemp);
      ratio = PT99*N_VMinQuotient(IDA_mem->ida_yy0, IDA_mem->ida_dtemp);
      (*delnorm) *= ratio;
      if((*delnorm) <= IDA_mem->ida_steptol) return(IC_CONSTR_FAILED);
      N_VScale(ratio, IDA_mem->ida_delta, IDA_mem->ida_delta);
    }

  } /* End of constraints check */

  slpi = -TWO*f1norm*ratio;
  minlam = IDA_mem->ida_steptol / (*delnorm);
  lambda = ONE;
  nbacks = 0;

  /* In IDA_Y_INIT case, set ypnew = yp0 (fixed) for linesearch. */
  if(IDA_mem->ida_icopt == IDA_Y_INIT)
    N_VScale(ONE, IDA_mem->ida_yp0, IDA_mem->ida_ypnew);

  /* Loop on linesearch variable lambda. */

  for(;;) {

    if (nbacks == IDA_mem->ida_maxbacks) return(IC_LINESRCH_FAILED);
    /* Get new (y,y') = (ynew,ypnew) and norm of new function value. */
    IDANewyyp(IDA_mem, lambda);
    retval = IDAfnorm(IDA_mem, &fnormp);
    if(retval != IDA_SUCCESS) return(retval);

    /* If lsoff option is on, break out. */
    if(IDA_mem->ida_lsoff) break;

    /* Do alpha-condition test. */
    f1normp = fnormp*fnormp*HALF;
    if(f1normp <= f1norm + ALPHALS*slpi*lambda) break;
    if(lambda < minlam) return(IC_LINESRCH_FAILED);
    lambda /= TWO;
    IDA_mem->ida_nbacktr++; nbacks++;

  }  /* End of breakout linesearch loop */

  /* Update yy0, yp0, and fnorm, then return. */
  N_VScale(ONE, IDA_mem->ida_ynew,  IDA_mem->ida_yy0);
  if(IDA_mem->ida_icopt == IDA_YA_YDP_INIT)
    N_VScale(ONE, IDA_mem->ida_ypnew, IDA_mem->ida_yp0);
  *fnorm = fnormp;
  return(IDA_SUCCESS);

}

/*
 * -----------------------------------------------------------------
 * IDAfnorm
 * -----------------------------------------------------------------
 * IDAfnorm computes the norm of the current function value, by
 * evaluating the DAE residual function, calling the linear 
 * system solver, and computing a WRMS-norm.
 *
 * On return, savres contains the current residual vector F, and
 * delnew contains J-inverse F.
 *
 * The return value is IDA_SUCCESS = 0 if no error occurred, or
 *  IC_FAIL_RECOV    if res or lsolve failed recoverably, or
 *  IDA_RES_FAIL     if res had a non-recoverable error, or
 *  IDA_LSOLVE_FAIL  if lsolve had a non-recoverable error.
 * -----------------------------------------------------------------
 */

static int IDAfnorm(IDAMem IDA_mem, realtype *fnorm)
{

  int retval;

  /* Get residual vector F, return if failed, and save F in savres. */
  retval = IDA_mem->ida_res(IDA_mem->ida_t0, IDA_mem->ida_ynew,
                            IDA_mem->ida_ypnew, IDA_mem->ida_delnew,
                            IDA_mem->ida_user_data);
  IDA_mem->ida_nre++;
  if(retval < 0) return(IDA_RES_FAIL);
  if(retval > 0) return(IC_FAIL_RECOV);

  N_VScale(ONE, IDA_mem->ida_delnew, IDA_mem->ida_savres);

  /* Call the linear solve function to get J-inverse F; return if failed. */
  retval = IDA_mem->ida_lsolve(IDA_mem, IDA_mem->ida_delnew,
                               IDA_mem->ida_ewt, IDA_mem->ida_ynew,
                               IDA_mem->ida_ypnew, IDA_mem->ida_savres);
  if(retval < 0) return(IDA_LSOLVE_FAIL);
  if(retval > 0) return(IC_FAIL_RECOV);

  /* Compute the WRMS-norm; rescale if index = 0. */
  *fnorm = IDAWrmsNorm(IDA_mem, IDA_mem->ida_delnew, IDA_mem->ida_ewt, SUNFALSE);
  if(IDA_mem->ida_sysindex == 0)
    (*fnorm) *= IDA_mem->ida_tscale * SUNRabs(IDA_mem->ida_cj);

  return(IDA_SUCCESS);

}

/*
 * -----------------------------------------------------------------
 * IDANewyyp
 * -----------------------------------------------------------------
 * IDANewyyp updates the vectors ynew and ypnew from yy0 and yp0,
 * using the current step vector lambda*delta, in a manner
 * depending on icopt and the input id vector.
 *
 * The return value is always IDA_SUCCESS = 0.
 * -----------------------------------------------------------------
 */

static int IDANewyyp(IDAMem IDA_mem, realtype lambda)
{
  
  /* IDA_YA_YDP_INIT case: ynew  = yy0 - lambda*delta    where id_i = 0
                           ypnew = yp0 - cj*lambda*delta where id_i = 1. */
  if(IDA_mem->ida_icopt == IDA_YA_YDP_INIT) {
    N_VProd(IDA_mem->ida_id, IDA_mem->ida_delta, IDA_mem->ida_dtemp);
    N_VLinearSum(ONE, IDA_mem->ida_yp0, -IDA_mem->ida_cj*lambda,
                 IDA_mem->ida_dtemp, IDA_mem->ida_ypnew);
    N_VLinearSum(ONE, IDA_mem->ida_delta, -ONE,
                 IDA_mem->ida_dtemp, IDA_mem->ida_dtemp);
    N_VLinearSum(ONE, IDA_mem->ida_yy0, -lambda,
                 IDA_mem->ida_dtemp, IDA_mem->ida_ynew);
    return(IDA_SUCCESS);
  }

  /* IDA_Y_INIT case: ynew = yy0 - lambda*delta. (ypnew = yp0 preset.) */
  N_VLinearSum(ONE, IDA_mem->ida_yy0, -lambda,
               IDA_mem->ida_delta, IDA_mem->ida_ynew);
  return(IDA_SUCCESS);

}

/*
 * -----------------------------------------------------------------
 * IDANewy
 * -----------------------------------------------------------------
 * IDANewy updates the vector ynew from yy0,
 * using the current step vector delta, in a manner
 * depending on icopt and the input id vector.
 *
 * The return value is always IDA_SUCCESS = 0.
 * -----------------------------------------------------------------
 */

static int IDANewy(IDAMem IDA_mem)
{
  
  /* IDA_YA_YDP_INIT case: ynew = yy0 - delta    where id_i = 0. */
  if(IDA_mem->ida_icopt == IDA_YA_YDP_INIT) {
    N_VProd(IDA_mem->ida_id, IDA_mem->ida_delta, IDA_mem->ida_dtemp);
    N_VLinearSum(ONE, IDA_mem->ida_delta, -ONE,
                 IDA_mem->ida_dtemp, IDA_mem->ida_dtemp);
    N_VLinearSum(ONE, IDA_mem->ida_yy0, -ONE,
                 IDA_mem->ida_dtemp, IDA_mem->ida_ynew);
    return(IDA_SUCCESS);
  }

  /* IDA_Y_INIT case: ynew = yy0 - delta. */
  N_VLinearSum(ONE, IDA_mem->ida_yy0, -ONE,
               IDA_mem->ida_delta, IDA_mem->ida_ynew);
  return(IDA_SUCCESS);

}

/*
 * -----------------------------------------------------------------
 * IDAICFailFlag
 * -----------------------------------------------------------------
 * IDAICFailFlag prints a message and sets the IDACalcIC return
 * value appropriate to the flag retval returned by IDAnlsIC.
 * -----------------------------------------------------------------
 */

static int IDAICFailFlag(IDAMem IDA_mem, int retval)
{

  /* Depending on retval, print error message and return error flag. */
  switch(retval) {

    case IDA_RES_FAIL:
      IDAProcessError(IDA_mem, IDA_RES_FAIL, "IDA", "IDACalcIC", MSG_IC_RES_NONREC);
      return(IDA_RES_FAIL);

    case IDA_FIRST_RES_FAIL:
      IDAProcessError(IDA_mem, IDA_FIRST_RES_FAIL, "IDA", "IDACalcIC", MSG_IC_RES_FAIL);
      return(IDA_FIRST_RES_FAIL);

    case IDA_LSETUP_FAIL:
      IDAProcessError(IDA_mem, IDA_LSETUP_FAIL, "IDA", "IDACalcIC", MSG_IC_SETUP_FAIL);
      return(IDA_LSETUP_FAIL);

    case IDA_LSOLVE_FAIL:  
      IDAProcessError(IDA_mem, IDA_LSOLVE_FAIL, "IDA", "IDACalcIC", MSG_IC_SOLVE_FAIL);
      return(IDA_LSOLVE_FAIL);

    case IC_FAIL_RECOV:
      IDAProcessError(IDA_mem, IDA_NO_RECOVERY, "IDA", "IDACalcIC", MSG_IC_NO_RECOVERY);
      return(IDA_NO_RECOVERY);

    case IC_CONSTR_FAILED: 
      IDAProcessError(IDA_mem, IDA_CONSTR_FAIL, "IDA", "IDACalcIC", MSG_IC_FAIL_CONSTR);
      return(IDA_CONSTR_FAIL);

    case IC_LINESRCH_FAILED:  
      IDAProcessError(IDA_mem, IDA_LINESEARCH_FAIL, "IDA", "IDACalcIC", MSG_IC_FAILED_LINS);
      return(IDA_LINESEARCH_FAIL);

    case IC_CONV_FAIL:
      IDAProcessError(IDA_mem, IDA_CONV_FAIL, "IDA", "IDACalcIC", MSG_IC_CONV_FAILED);
      return(IDA_CONV_FAIL);

    case IC_SLOW_CONVRG: 
      IDAProcessError(IDA_mem, IDA_CONV_FAIL, "IDA", "IDACalcIC", MSG_IC_CONV_FAILED);
      return(IDA_CONV_FAIL);

    case IDA_BAD_EWT:
      IDAProcessError(IDA_mem, IDA_BAD_EWT, "IDA", "IDACalcIC", MSG_IC_BAD_EWT);
      return(IDA_BAD_EWT);

  }
  return -99;
}

