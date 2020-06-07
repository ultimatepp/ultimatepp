/* -----------------------------------------------------------------------------
 * Programmer(s): David J. Gardner @ LLNL
 * -----------------------------------------------------------------------------
 * SUNDIALS Copyright Start
 * Copyright (c) 2002-2020, Lawrence Livermore National Security
 * and Southern Methodist University.
 * All rights reserved.
 *
 * See the top-level LICENSE and NOTICE files for details.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * SUNDIALS Copyright End
 * -----------------------------------------------------------------------------
 * This is the implementation file for the SUNNonlinearSolver module
 * implementation of Newton's method.
 * ---------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sunnonlinsol/sunnonlinsol_newton.h>
#include <sundials/sundials_math.h>
#include <sundials/sundials_nvector_senswrapper.h>

/* Content structure accessibility macros  */
#define NEWTON_CONTENT(S) ( (SUNNonlinearSolverContent_Newton)(S->content) )

/* Constant macros */
#define ZERO RCONST(0.0) /* real 0.0 */
#define ONE  RCONST(1.0) /* real 1.0 */

/*==============================================================================
  Constructor to create a new Newton solver
  ============================================================================*/

SUNNonlinearSolver SUNNonlinSol_Newton(N_Vector y)
{
  SUNNonlinearSolver NLS;
  SUNNonlinearSolverContent_Newton content;

  /* Check that the supplied N_Vector is non-NULL */
  if (y == NULL) return(NULL);

  /* Check that the supplied N_Vector supports all required operations */
  if ( (y->ops->nvclone     == NULL) ||
       (y->ops->nvdestroy   == NULL) ||
       (y->ops->nvscale     == NULL) ||
       (y->ops->nvlinearsum == NULL) )
    return(NULL);

  /* Create an empty nonlinear linear solver object */
  NLS = NULL;
  NLS = SUNNonlinSolNewEmpty();
  if (NLS == NULL) return(NULL);

  /* Attach operations */
  NLS->ops->gettype         = SUNNonlinSolGetType_Newton;
  NLS->ops->initialize      = SUNNonlinSolInitialize_Newton;
  NLS->ops->solve           = SUNNonlinSolSolve_Newton;
  NLS->ops->free            = SUNNonlinSolFree_Newton;
  NLS->ops->setsysfn        = SUNNonlinSolSetSysFn_Newton;
  NLS->ops->setlsetupfn     = SUNNonlinSolSetLSetupFn_Newton;
  NLS->ops->setlsolvefn     = SUNNonlinSolSetLSolveFn_Newton;
  NLS->ops->setctestfn      = SUNNonlinSolSetConvTestFn_Newton;
  NLS->ops->setmaxiters     = SUNNonlinSolSetMaxIters_Newton;
  NLS->ops->getnumiters     = SUNNonlinSolGetNumIters_Newton;
  NLS->ops->getcuriter      = SUNNonlinSolGetCurIter_Newton;
  NLS->ops->getnumconvfails = SUNNonlinSolGetNumConvFails_Newton;

  /* Create content */
  content = NULL;
  content = (SUNNonlinearSolverContent_Newton) malloc(sizeof *content);
  if (content == NULL) { SUNNonlinSolFree(NLS); return(NULL); }

  /* Initialize all components of content to 0/NULL */
  memset(content, 0, sizeof(struct _SUNNonlinearSolverContent_Newton));

  /* Attach content */
  NLS->content = content;

  /* Fill general content */
  content->Sys        = NULL;
  content->LSetup     = NULL;
  content->LSolve     = NULL;
  content->CTest      = NULL;
  content->jcur       = SUNFALSE;
  content->curiter    = 0;
  content->maxiters   = 3;
  content->niters     = 0;
  content->nconvfails = 0;
  content->ctest_data = NULL;

  /* Fill allocatable content */
  content->delta = N_VClone(y);
  if (content->delta == NULL) { SUNNonlinSolFree(NLS); return(NULL); }

  return(NLS);
}


/*==============================================================================
  Constructor wrapper to create a new Newton solver for sensitivity solvers
  ============================================================================*/

SUNNonlinearSolver SUNNonlinSol_NewtonSens(int count, N_Vector y)
{
  SUNNonlinearSolver NLS;
  N_Vector w;

  /* create sensitivity vector wrapper */
  w = N_VNew_SensWrapper(count, y);

  /* create nonlinear solver using sensitivity vector wrapper */
  NLS = SUNNonlinSol_Newton(w);

  /* free sensitivity vector wrapper */
  N_VDestroy(w);

  /* return NLS object */
  return(NLS);
}


/*==============================================================================
  GetType, Initialize, Setup, Solve, and Free operations
  ============================================================================*/

SUNNonlinearSolver_Type SUNNonlinSolGetType_Newton(SUNNonlinearSolver NLS)
{
  return(SUNNONLINEARSOLVER_ROOTFIND);
}


int SUNNonlinSolInitialize_Newton(SUNNonlinearSolver NLS)
{
  /* check that the nonlinear solver is non-null */
  if (NLS == NULL) return(SUN_NLS_MEM_NULL);

  /* check that all required function pointers have been set */
  if ( (NEWTON_CONTENT(NLS)->Sys    == NULL) ||
       (NEWTON_CONTENT(NLS)->LSolve == NULL) ||
       (NEWTON_CONTENT(NLS)->CTest  == NULL) ) {
    return(SUN_NLS_MEM_NULL);
  }

  /* reset the total number of iterations and convergence failures */
  NEWTON_CONTENT(NLS)->niters     = 0;
  NEWTON_CONTENT(NLS)->nconvfails = 0;

  /* reset the Jacobian status */
  NEWTON_CONTENT(NLS)->jcur = SUNFALSE;

  return(SUN_NLS_SUCCESS);
}


/*------------------------------------------------------------------------------
  SUNNonlinSolSolve_Newton: Performs the nonlinear solve F(y) = 0

  Successful solve return code:
    SUN_NLS_SUCCESS = 0

  Recoverable failure return codes (positive):
    SUN_NLS_CONV_RECVR
    *_RHSFUNC_RECVR (ODEs) or *_RES_RECVR (DAEs)
    *_LSETUP_RECVR
    *_LSOLVE_RECVR

  Unrecoverable failure return codes (negative):
    *_MEM_NULL
    *_RHSFUNC_FAIL (ODEs) or *_RES_FAIL (DAEs)
    *_LSETUP_FAIL
    *_LSOLVE_FAIL

  Note return values beginning with * are package specific values returned by
  the Sys, LSetup, and LSolve functions provided to the nonlinear solver.
  ----------------------------------------------------------------------------*/
int SUNNonlinSolSolve_Newton(SUNNonlinearSolver NLS,
                             N_Vector y0, N_Vector ycor,
                             N_Vector w, realtype tol,
                             booleantype callLSetup, void* mem)
{
  /* local variables */
  int retval;
  booleantype jbad;
  N_Vector delta;

  /* check that the inputs are non-null */
  if ( (NLS  == NULL) ||
       (y0   == NULL) ||
       (ycor == NULL) ||
       (w    == NULL) ||
       (mem  == NULL) )
    return(SUN_NLS_MEM_NULL);

  /* check that all required function pointers have been set */
  if ( (NEWTON_CONTENT(NLS)->Sys    == NULL) ||
       (NEWTON_CONTENT(NLS)->LSolve == NULL) ||
       (callLSetup && (NEWTON_CONTENT(NLS)->LSetup == NULL)) ||
       (NEWTON_CONTENT(NLS)->CTest  == NULL) ) {
    return(SUN_NLS_MEM_NULL);
  }

  /* set local shortcut variables */
  delta = NEWTON_CONTENT(NLS)->delta;

  /* assume the Jacobian is good */
  jbad = SUNFALSE;

  /* looping point for attempts at solution of the nonlinear system:
       Evaluate the nonlinear residual function (store in delta)
       Setup the linear solver if necessary
       Preform Newton iteraion */
  for(;;) {

    /* compute the nonlinear residual, store in delta */
    retval = NEWTON_CONTENT(NLS)->Sys(ycor, delta, mem);
    if (retval != SUN_NLS_SUCCESS) break;

    /* if indicated, setup the linear system */
    if (callLSetup) {
      retval = NEWTON_CONTENT(NLS)->LSetup(jbad,
                                           &(NEWTON_CONTENT(NLS)->jcur),
                                           mem);
      if (retval != SUN_NLS_SUCCESS) break;
    }

    /* initialize counter curiter */
    NEWTON_CONTENT(NLS)->curiter = 0;

    /* looping point for Newton iteration. Break out on any error. */
    for(;;) {

      /* increment nonlinear solver iteration counter */
      NEWTON_CONTENT(NLS)->niters++;

      /* compute the negative of the residual for the linear system rhs */
      N_VScale(-ONE, delta, delta);

      /* solve the linear system to get Newton update delta */
      retval = NEWTON_CONTENT(NLS)->LSolve(delta, mem);
      if (retval != SUN_NLS_SUCCESS) break;

      /* update the Newton iterate */
      N_VLinearSum(ONE, ycor, ONE, delta, ycor);

      /* test for convergence */
      retval = NEWTON_CONTENT(NLS)->CTest(NLS, ycor, delta, tol, w,
                                          NEWTON_CONTENT(NLS)->ctest_data);

      /* if successful update Jacobian status and return */
      if (retval == SUN_NLS_SUCCESS) {
        NEWTON_CONTENT(NLS)->jcur = SUNFALSE;
        return(SUN_NLS_SUCCESS);
      }

      /* check if the iteration should continue; otherwise exit Newton loop */
      if (retval != SUN_NLS_CONTINUE) break;

      /* not yet converged. Increment curiter and test for max allowed. */
      NEWTON_CONTENT(NLS)->curiter++;
      if (NEWTON_CONTENT(NLS)->curiter >= NEWTON_CONTENT(NLS)->maxiters) {
        retval = SUN_NLS_CONV_RECVR;
        break;
      }

      /* compute the nonlinear residual, store in delta */
      retval = NEWTON_CONTENT(NLS)->Sys(ycor, delta, mem);
      if (retval != SUN_NLS_SUCCESS) break;

    } /* end of Newton iteration loop */

    /* all errors go here */

    /* If there is a recoverable convergence failure and the Jacobian-related
       data appears not to be current, increment the convergence failure count,
       reset the initial correction to zero, and loop again with a call to
       lsetup in which jbad is TRUE. Otherwise break out and return. */
    if ((retval > 0) && !(NEWTON_CONTENT(NLS)->jcur) && (NEWTON_CONTENT(NLS)->LSetup)) {
      NEWTON_CONTENT(NLS)->nconvfails++;
      callLSetup = SUNTRUE;
      jbad = SUNTRUE;
      N_VConst(ZERO, ycor);
      continue;
    } else {
      break;
    }

  } /* end of setup loop */

  /* increment number of convergence failures */
  NEWTON_CONTENT(NLS)->nconvfails++;

  /* all error returns exit here */
  return(retval);
}


int SUNNonlinSolFree_Newton(SUNNonlinearSolver NLS)
{
  /* return if NLS is already free */
  if (NLS == NULL)
    return(SUN_NLS_SUCCESS);

  /* free items from contents, then the generic structure */
  if (NLS->content) {

    if (NEWTON_CONTENT(NLS)->delta)
      N_VDestroy(NEWTON_CONTENT(NLS)->delta);
    NEWTON_CONTENT(NLS)->delta = NULL;

    free(NLS->content);
    NLS->content = NULL;
  }

  /* free the ops structure */
  if (NLS->ops) {
    free(NLS->ops);
    NLS->ops = NULL;
  }

  /* free the nonlinear solver */
  free(NLS);

  return(SUN_NLS_SUCCESS);
}


/*==============================================================================
  Set functions
  ============================================================================*/

int SUNNonlinSolSetSysFn_Newton(SUNNonlinearSolver NLS, SUNNonlinSolSysFn SysFn)
{
  /* check that the nonlinear solver is non-null */
  if (NLS == NULL)
    return(SUN_NLS_MEM_NULL);

  /* check that the nonlinear system function is non-null */
  if (SysFn == NULL)
    return(SUN_NLS_ILL_INPUT);

  NEWTON_CONTENT(NLS)->Sys = SysFn;
  return(SUN_NLS_SUCCESS);
}


int SUNNonlinSolSetLSetupFn_Newton(SUNNonlinearSolver NLS, SUNNonlinSolLSetupFn LSetupFn)
{
  /* check that the nonlinear solver is non-null */
  if (NLS == NULL)
    return(SUN_NLS_MEM_NULL);

  NEWTON_CONTENT(NLS)->LSetup = LSetupFn;
  return(SUN_NLS_SUCCESS);
}


int SUNNonlinSolSetLSolveFn_Newton(SUNNonlinearSolver NLS, SUNNonlinSolLSolveFn LSolveFn)
{
  /* check that the nonlinear solver is non-null */
  if (NLS == NULL)
    return(SUN_NLS_MEM_NULL);

  /* check that the linear solver solve function is non-null */
  if (LSolveFn == NULL)
    return(SUN_NLS_ILL_INPUT);

  NEWTON_CONTENT(NLS)->LSolve = LSolveFn;
  return(SUN_NLS_SUCCESS);
}


int SUNNonlinSolSetConvTestFn_Newton(SUNNonlinearSolver NLS,
                                     SUNNonlinSolConvTestFn CTestFn,
                                     void* ctest_data)
{
  /* check that the nonlinear solver is non-null */
  if (NLS == NULL)
    return(SUN_NLS_MEM_NULL);

  /* check that the convergence test function is non-null */
  if (CTestFn == NULL)
    return(SUN_NLS_ILL_INPUT);

  NEWTON_CONTENT(NLS)->CTest = CTestFn;

  /* attach convergence test data */
  NEWTON_CONTENT(NLS)->ctest_data = ctest_data;

  return(SUN_NLS_SUCCESS);
}


int SUNNonlinSolSetMaxIters_Newton(SUNNonlinearSolver NLS, int maxiters)
{
  /* check that the nonlinear solver is non-null */
  if (NLS == NULL)
    return(SUN_NLS_MEM_NULL);

  /* check that maxiters is a vaild */
  if (maxiters < 1)
    return(SUN_NLS_ILL_INPUT);

  NEWTON_CONTENT(NLS)->maxiters = maxiters;
  return(SUN_NLS_SUCCESS);
}


/*==============================================================================
  Get functions
  ============================================================================*/

int SUNNonlinSolGetNumIters_Newton(SUNNonlinearSolver NLS, long int *niters)
{
  /* check that the nonlinear solver is non-null */
  if (NLS == NULL)
    return(SUN_NLS_MEM_NULL);

  /* return the total number of nonlinear iterations */
  *niters = NEWTON_CONTENT(NLS)->niters;
  return(SUN_NLS_SUCCESS);
}


int SUNNonlinSolGetCurIter_Newton(SUNNonlinearSolver NLS, int *iter)
{
  /* check that the nonlinear solver is non-null */
  if (NLS == NULL)
    return(SUN_NLS_MEM_NULL);

  /* return the current nonlinear solver iteration count */
  *iter = NEWTON_CONTENT(NLS)->curiter;
  return(SUN_NLS_SUCCESS);
}


int SUNNonlinSolGetNumConvFails_Newton(SUNNonlinearSolver NLS, long int *nconvfails)
{
  /* check that the nonlinear solver is non-null */
  if (NLS == NULL)
    return(SUN_NLS_MEM_NULL);

  /* return the total number of nonlinear convergence failures */
  *nconvfails = NEWTON_CONTENT(NLS)->nconvfails;
  return(SUN_NLS_SUCCESS);
}


int SUNNonlinSolGetSysFn_Newton(SUNNonlinearSolver NLS, SUNNonlinSolSysFn *SysFn)
{
  /* check that the nonlinear solver is non-null */
  if (NLS == NULL)
    return(SUN_NLS_MEM_NULL);

  /* return the nonlinear system defining function */
  *SysFn = NEWTON_CONTENT(NLS)->Sys;
  return(SUN_NLS_SUCCESS);
}
