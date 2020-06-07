/* -----------------------------------------------------------------
 * Programmer(s): Daniel Reynolds @ SMU
 *                David J. Gardner, Carol S. Woodward, and
 *                Slaven Peles @ LLNL
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
 * This is the implementation file for a generic SUNLINEARSOLVER
 * package.  It contains the implementation of the SUNLinearSolver
 * operations listed in sundials_linearsolver.h
 * -----------------------------------------------------------------*/

#include <stdlib.h>
#include <sundials/sundials_linearsolver.h>

/* -----------------------------------------------------------------
 * Create a new empty SUNLinearSolver object
 * ----------------------------------------------------------------- */

SUNLinearSolver SUNLinSolNewEmpty()
{
  SUNLinearSolver     LS;
  SUNLinearSolver_Ops ops;

  /* create linear solver object */
  LS = NULL;
  LS = (SUNLinearSolver) malloc(sizeof *LS);
  if (LS == NULL) return(NULL);

  /* create linear solver ops structure */
  ops = NULL;
  ops = (SUNLinearSolver_Ops) malloc(sizeof *ops);
  if (ops == NULL) { free(LS); return(NULL); }

  /* initialize operations to NULL */
  ops->gettype           = NULL;
  ops->getid             = NULL;
  ops->setatimes         = NULL;
  ops->setpreconditioner = NULL;
  ops->setscalingvectors = NULL;
  ops->initialize        = NULL;
  ops->setup             = NULL;
  ops->solve             = NULL;
  ops->numiters          = NULL;
  ops->resnorm           = NULL;
  ops->resid             = NULL;
  ops->lastflag          = NULL;
  ops->space             = NULL;
  ops->free              = NULL;

  /* attach ops and initialize content to NULL */
  LS->ops     = ops;
  LS->content = NULL;

  return(LS);
}

/* -----------------------------------------------------------------
 * Free a generic SUNLinearSolver (assumes content is already empty)
 * ----------------------------------------------------------------- */

void SUNLinSolFreeEmpty(SUNLinearSolver S)
{
  if (S == NULL)  return;

  /* free non-NULL ops structure */
  if (S->ops)  free(S->ops);
  S->ops = NULL;

  /* free overall N_Vector object and return */
  free(S);
  return;
}

/* -----------------------------------------------------------------
 * Functions in the 'ops' structure
 * -----------------------------------------------------------------*/

SUNLinearSolver_Type SUNLinSolGetType(SUNLinearSolver S)
{
  return(S->ops->gettype(S));
}

SUNLinearSolver_ID SUNLinSolGetID(SUNLinearSolver S)
{
  if (S->ops->getid)
    return(S->ops->getid(S));
  else
    return(SUNLINEARSOLVER_CUSTOM);
}

int SUNLinSolSetATimes(SUNLinearSolver S, void* A_data,
                       ATimesFn ATimes)
{
  if (S->ops->setatimes)
    return ((int) S->ops->setatimes(S, A_data, ATimes));
  else
    return SUNLS_SUCCESS;
}


int SUNLinSolSetPreconditioner(SUNLinearSolver S, void* P_data,
                               PSetupFn Pset, PSolveFn Psol)
{
  if (S->ops->setpreconditioner)
    return ((int) S->ops->setpreconditioner(S, P_data, Pset, Psol));
  else
    return SUNLS_SUCCESS;
}

int SUNLinSolSetScalingVectors(SUNLinearSolver S,
                               N_Vector s1, N_Vector s2)
{
  if (S->ops->setscalingvectors)
    return ((int) S->ops->setscalingvectors(S, s1, s2));
  else
    return SUNLS_SUCCESS;
}

int SUNLinSolInitialize(SUNLinearSolver S)
{
  if (S->ops->initialize)
    return ((int) S->ops->initialize(S));
  else
    return SUNLS_SUCCESS;
}

int SUNLinSolSetup(SUNLinearSolver S, SUNMatrix A)
{
  if (S->ops->setup)
    return ((int) S->ops->setup(S, A));
  else
    return SUNLS_SUCCESS;
}

int SUNLinSolSolve(SUNLinearSolver S, SUNMatrix A, N_Vector x,
                   N_Vector b, realtype tol)
{
  return ((int) S->ops->solve(S, A, x, b, tol));
}

int SUNLinSolNumIters(SUNLinearSolver S)
{
  if (S->ops->numiters)
    return ((int) S->ops->numiters(S));
  else
    return 0;
}

realtype SUNLinSolResNorm(SUNLinearSolver S)
{
  if (S->ops->resnorm)
    return ((realtype) S->ops->resnorm(S));
  else
    return RCONST(0.0);
}

N_Vector SUNLinSolResid(SUNLinearSolver S)
{
  if (S->ops->resid)
    return ((N_Vector) S->ops->resid(S));
  else
    return NULL;
}

sunindextype SUNLinSolLastFlag(SUNLinearSolver S)
{
  if (S->ops->lastflag)
    return ((sunindextype) S->ops->lastflag(S));
  else
    return SUNLS_SUCCESS;
}

int SUNLinSolSpace(SUNLinearSolver S, long int *lenrwLS,
                   long int *leniwLS)
{
  if (S->ops->space)
    return ((int) S->ops->space(S, lenrwLS, leniwLS));
  else {
    *lenrwLS = 0;
    *leniwLS = 0;
    return SUNLS_SUCCESS;
  }
}

int SUNLinSolFree(SUNLinearSolver S)
{
  if (S == NULL) return SUNLS_SUCCESS;

  /* if the free operation exists use it */
  if (S->ops)
    if (S->ops->free) return(S->ops->free(S));

  /* if we reach this point, either ops == NULL or free == NULL,
     try to cleanup by freeing the content, ops, and solver */
  if (S->content) { free(S->content); S->content = NULL; }
  if (S->ops) { free(S->ops); S->ops = NULL; }
  free(S); S = NULL;

  return(SUNLS_SUCCESS);
}
