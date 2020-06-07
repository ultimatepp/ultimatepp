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
 * This is the implementation file for a generic SUNNonlinerSolver package. It
 * contains the implementation of the SUNNonlinearSolver operations listed in
 * the 'ops' structure in sundials_nonlinearsolver.h
 * ---------------------------------------------------------------------------*/

#include <stdlib.h>
#include <sundials/sundials_nonlinearsolver.h>

/* -----------------------------------------------------------------------------
 * Create a new empty SUNLinearSolver object
 * ---------------------------------------------------------------------------*/

SUNNonlinearSolver SUNNonlinSolNewEmpty()
{
  SUNNonlinearSolver     NLS;
  SUNNonlinearSolver_Ops ops;

  /* create nonlinear solver object */
  NLS = NULL;
  NLS = (SUNNonlinearSolver) malloc(sizeof *NLS);
  if (NLS == NULL) return(NULL);

  /* create nonlinear solver ops structure */
  ops = NULL;
  ops = (SUNNonlinearSolver_Ops) malloc(sizeof *ops);
  if (ops == NULL) { free(NLS); return(NULL); }

  /* initialize operations to NULL */
  ops->gettype         = NULL;
  ops->initialize      = NULL;
  ops->setup           = NULL;
  ops->solve           = NULL;
  ops->free            = NULL;
  ops->setsysfn        = NULL;
  ops->setlsetupfn     = NULL;
  ops->setlsolvefn     = NULL;
  ops->setctestfn      = NULL;
  ops->setmaxiters     = NULL;
  ops->getnumiters     = NULL;
  ops->getcuriter      = NULL;
  ops->getnumconvfails = NULL;

  /* attach ops and initialize content to NULL */
  NLS->ops     = ops;
  NLS->content = NULL;

  return(NLS);
}

/* -----------------------------------------------------------------------------
 * Free a generic SUNNonlinearSolver (assumes content is already empty)
 * ---------------------------------------------------------------------------*/

void SUNNonlinSolFreeEmpty(SUNNonlinearSolver NLS)
{
  if (NLS == NULL)  return;

  /* free non-NULL ops structure */
  if (NLS->ops)  free(NLS->ops);
  NLS->ops = NULL;

  /* free overall N_Vector object and return */
  free(NLS);
  return;
}

/* -----------------------------------------------------------------------------
 * core functions
 * ---------------------------------------------------------------------------*/

SUNNonlinearSolver_Type SUNNonlinSolGetType(SUNNonlinearSolver NLS)
{
  return(NLS->ops->gettype(NLS));
}

int SUNNonlinSolInitialize(SUNNonlinearSolver NLS)
{
  if (NLS->ops->initialize)
    return((int) NLS->ops->initialize(NLS));
  else
    return(SUN_NLS_SUCCESS);
}

int SUNNonlinSolSetup(SUNNonlinearSolver NLS, N_Vector y, void* mem)
{
  if (NLS->ops->setup)
    return((int) NLS->ops->setup(NLS, y, mem));
  else
    return(SUN_NLS_SUCCESS);
}

int SUNNonlinSolSolve(SUNNonlinearSolver NLS,
                      N_Vector y0, N_Vector y,
                      N_Vector w, realtype tol,
                      booleantype callLSetup, void* mem)
{
  return((int) NLS->ops->solve(NLS, y0, y, w, tol, callLSetup, mem));
}

int SUNNonlinSolFree(SUNNonlinearSolver NLS)
{
  if (NLS == NULL) return(SUN_NLS_SUCCESS);

  /* if the free operation exists use it */
  if (NLS->ops)
    if (NLS->ops->free) return(NLS->ops->free(NLS));

  /* if we reach this point, either ops == NULL or free == NULL,
     try to cleanup by freeing the content, ops, and solver */
  if (NLS->content) { free(NLS->content); NLS->content = NULL; }
  if (NLS->ops) { free(NLS->ops); NLS->ops = NULL; }
  free(NLS); NLS = NULL;

  return(SUN_NLS_SUCCESS);
}

/* -----------------------------------------------------------------------------
 * set functions
 * ---------------------------------------------------------------------------*/

/* set the nonlinear system function (required) */
int SUNNonlinSolSetSysFn(SUNNonlinearSolver NLS, SUNNonlinSolSysFn SysFn)
{
  return((int) NLS->ops->setsysfn(NLS, SysFn));
}

/* set the linear solver setup function (optional) */
int SUNNonlinSolSetLSetupFn(SUNNonlinearSolver NLS, SUNNonlinSolLSetupFn LSetupFn)
{
  if (NLS->ops->setlsetupfn)
    return((int) NLS->ops->setlsetupfn(NLS, LSetupFn));
  else
    return(SUN_NLS_SUCCESS);
}

/* set the linear solver solve function (optional) */
int SUNNonlinSolSetLSolveFn(SUNNonlinearSolver NLS, SUNNonlinSolLSolveFn LSolveFn)
{
  if (NLS->ops->setlsolvefn)
    return((int) NLS->ops->setlsolvefn(NLS, LSolveFn));
  else
    return(SUN_NLS_SUCCESS);
}

/* set the convergence test function (optional) */
int SUNNonlinSolSetConvTestFn(SUNNonlinearSolver NLS,
                              SUNNonlinSolConvTestFn CTestFn,
                              void* ctest_data)
{
  if (NLS->ops->setctestfn)
    return((int) NLS->ops->setctestfn(NLS, CTestFn, ctest_data));
  else
    return(SUN_NLS_SUCCESS);
}

int SUNNonlinSolSetMaxIters(SUNNonlinearSolver NLS, int maxiters)
{
  if (NLS->ops->setmaxiters)
    return((int) NLS->ops->setmaxiters(NLS, maxiters));
  else
    return(SUN_NLS_SUCCESS);
}

/* -----------------------------------------------------------------------------
 * get functions
 * ---------------------------------------------------------------------------*/

/* get the total number on nonlinear iterations (optional) */
int SUNNonlinSolGetNumIters(SUNNonlinearSolver NLS, long int *niters)
{
  if (NLS->ops->getnumiters) {
    return((int) NLS->ops->getnumiters(NLS, niters));
  } else {
    *niters = 0;
    return(SUN_NLS_SUCCESS);
  }
}


/* get the iteration count for the current nonlinear solve */
int SUNNonlinSolGetCurIter(SUNNonlinearSolver NLS, int *iter)
{
  if (NLS->ops->getcuriter) {
    return((int) NLS->ops->getcuriter(NLS, iter));
  } else {
    *iter = -1;
    return(SUN_NLS_SUCCESS);
  }
}


/* get the total number on nonlinear solve convergence failures (optional) */
int SUNNonlinSolGetNumConvFails(SUNNonlinearSolver NLS, long int *nconvfails)
{
  if (NLS->ops->getnumconvfails) {
    return((int) NLS->ops->getnumconvfails(NLS, nconvfails));
  } else {
    *nconvfails = 0;
    return(SUN_NLS_SUCCESS);
  }
}
