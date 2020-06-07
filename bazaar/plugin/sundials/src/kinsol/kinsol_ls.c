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
 * Implementation file for KINSOL's linear solver interface.
 *-----------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "kinsol_impl.h"
#include "kinsol_ls_impl.h"

#include <sundials/sundials_math.h>
#include <sunmatrix/sunmatrix_band.h>
#include <sunmatrix/sunmatrix_dense.h>
#include <sunmatrix/sunmatrix_sparse.h>

/* constants */
#define MIN_INC_MULT RCONST(1000.0)
#define ZERO         RCONST(0.0)
#define ONE          RCONST(1.0)
#define TWO          RCONST(2.0)


/*==================================================================
  KINLS Exported functions -- Required
  ==================================================================*/

/*---------------------------------------------------------------
  KINSetLinearSolver specifies the linear solver
  ---------------------------------------------------------------*/
int KINSetLinearSolver(void *kinmem, SUNLinearSolver LS, SUNMatrix A)
{
  KINMem      kin_mem;
  KINLsMem    kinls_mem;
  int         retval, LSType;
  booleantype iterative;    /* is the solver iterative?    */
  booleantype matrixbased;  /* is a matrix structure used? */

  /* Return immediately if either kinmem or LS inputs are NULL */
  if (kinmem == NULL) {
    KINProcessError(NULL, KINLS_MEM_NULL, "KINLS",
                    "KINSetLinearSolver", MSG_LS_KINMEM_NULL);
    return(KINLS_MEM_NULL);
  }
  if (LS == NULL) {
    KINProcessError(NULL, KINLS_ILL_INPUT, "KINLS",
                    "KINSetLinearSolver",
                    "LS must be non-NULL");
    return(KINLS_ILL_INPUT);
  }
  kin_mem = (KINMem) kinmem;

  /* Test if solver is compatible with LS interface */
  if ( (LS->ops->gettype == NULL) || (LS->ops->solve == NULL) ) {
    KINProcessError(kin_mem, KINLS_ILL_INPUT, "KINLS",
                   "KINSetLinearSolver",
                   "LS object is missing a required operation");
    return(KINLS_ILL_INPUT);
  }

  /* Retrieve the LS type */
  LSType = SUNLinSolGetType(LS);

  /* Set flags based on LS type */
  iterative   = (LSType != SUNLINEARSOLVER_DIRECT);
  matrixbased = (LSType != SUNLINEARSOLVER_ITERATIVE);

  /* check for required vector operations for KINLS interface */
  if ( (kin_mem->kin_vtemp1->ops->nvconst == NULL) ||
       (kin_mem->kin_vtemp1->ops->nvdotprod == NULL) ) {
    KINProcessError(kin_mem, KINLS_ILL_INPUT, "KINLS",
                    "KINSetLinearSolver", MSG_LS_BAD_NVECTOR);
    return(KINLS_ILL_INPUT);
  }

  /* Check for compatible LS type, matrix and "atimes" support */
  if (iterative) {

    if ((LS->ops->setscalingvectors == NULL) &&
        (kin_mem->kin_vtemp1->ops->nvgetlength == NULL)) {
      KINProcessError(kin_mem, KINLS_ILL_INPUT, "KINLS",
                      "KINSetLinearSolver", MSG_LS_BAD_NVECTOR);
      return(KINLS_ILL_INPUT);
    }

    if (!matrixbased && (LS->ops->setatimes == NULL)) {
      KINProcessError(kin_mem, KINLS_ILL_INPUT, "KINLS", "KINSetLinearSolver",
                      "Incompatible inputs: iterative LS must support ATimes routine");
      return(KINLS_ILL_INPUT);
    }

    if (matrixbased && A == NULL) {
      KINProcessError(kin_mem, KINLS_ILL_INPUT, "KINLS", "KINSetLinearSolver",
                      "Incompatible inputs: matrix-iterative LS requires non-NULL matrix");
      return(KINLS_ILL_INPUT);
    }

  } else if (A == NULL) {

    KINProcessError(kin_mem, KINLS_ILL_INPUT, "KINLS", "KINSetLinearSolver",
                    "Incompatible inputs: direct LS requires non-NULL matrix");
    return(KINLS_ILL_INPUT);
  }

  /* free any existing system solver attached to KIN */
  if (kin_mem->kin_lfree) kin_mem->kin_lfree(kin_mem);

  /* Determine if this is an iterative linear solver */
  kin_mem->kin_inexact_ls = iterative;

  /* Set four main system linear solver function fields in kin_mem */
  kin_mem->kin_linit  = kinLsInitialize;
  kin_mem->kin_lsetup = kinLsSetup;
  kin_mem->kin_lsolve = kinLsSolve;
  kin_mem->kin_lfree  = kinLsFree;

  /* Get memory for KINLsMemRec */
  kinls_mem = NULL;
  kinls_mem = (KINLsMem) malloc(sizeof(struct KINLsMemRec));
  if (kinls_mem == NULL) {
    KINProcessError(kin_mem, KINLS_MEM_FAIL, "KINLS",
                    "KINSetLinearSolver", MSG_LS_MEM_FAIL);
    return(KINLS_MEM_FAIL);
  }
  memset(kinls_mem, 0, sizeof(struct KINLsMemRec));

  /* set SUNLinearSolver pointer */
  kinls_mem->LS = LS;

  /* Set defaults for Jacobian-related fields */
  if (A != NULL) {
    kinls_mem->jacDQ  = SUNTRUE;
    kinls_mem->jac    = kinLsDQJac;
    kinls_mem->J_data = kin_mem;
  } else {
    kinls_mem->jacDQ  = SUNFALSE;
    kinls_mem->jac    = NULL;
    kinls_mem->J_data = NULL;
  }
  kinls_mem->jtimesDQ = SUNTRUE;
  kinls_mem->jtimes   = kinLsDQJtimes;
  kinls_mem->jt_data  = kin_mem;

  /* Set defaults for preconditioner-related fields */
  kinls_mem->pset   = NULL;
  kinls_mem->psolve = NULL;
  kinls_mem->pfree  = NULL;
  kinls_mem->pdata  = kin_mem->kin_user_data;

  /* Initialize counters */
  kinLsInitializeCounters(kinls_mem);

  /* Set default values for the rest of the LS parameters */
  kinls_mem->last_flag = KINLS_SUCCESS;

  /* If LS supports ATimes, attach KINLs routine */
  if (LS->ops->setatimes) {
    retval = SUNLinSolSetATimes(LS, kin_mem, kinLsATimes);
    if (retval != SUNLS_SUCCESS) {
      KINProcessError(kin_mem, KINLS_SUNLS_FAIL, "KINLS",
                      "KINSetLinearSolver",
                      "Error in calling SUNLinSolSetATimes");
      free(kinls_mem); kinls_mem = NULL;
      return(KINLS_SUNLS_FAIL);
    }
  }

  /* If LS supports preconditioning, initialize pset/psol to NULL */
  if (LS->ops->setpreconditioner) {
    retval = SUNLinSolSetPreconditioner(LS, kin_mem, NULL, NULL);
    if (retval != SUNLS_SUCCESS) {
      KINProcessError(kin_mem, KINLS_SUNLS_FAIL, "KINLS",
                      "KINSetLinearSolver",
                      "Error in calling SUNLinSolSetPreconditioner");
      free(kinls_mem); kinls_mem = NULL;
      return(KINLS_SUNLS_FAIL);
    }
  }

  /* initialize tolerance scaling factor */
  kinls_mem->tol_fac = -ONE;

  /* set SUNMatrix pointer (can be NULL) */
  kinls_mem->J = A;

  /* Attach linear solver memory to integrator memory */
  kin_mem->kin_lmem = kinls_mem;

  return(KINLS_SUCCESS);
}


/*==================================================================
  Optional input/output routines
  ==================================================================*/

/*------------------------------------------------------------------
  KINSetJacFn specifies the Jacobian function
  ------------------------------------------------------------------*/
int KINSetJacFn(void *kinmem, KINLsJacFn jac)
{
  KINMem   kin_mem;
  KINLsMem kinls_mem;
  int      retval;

  /* access KINLsMem structure */
  retval = kinLs_AccessLMem(kinmem, "KINSetJacFn",
                            &kin_mem, &kinls_mem);
  if (retval != KIN_SUCCESS)  return(retval);

  /* return with failure if jac cannot be used */
  if ((jac != NULL) && (kinls_mem->J == NULL)) {
    KINProcessError(kin_mem, KINLS_ILL_INPUT, "KINLS", "KINSetJacFn",
                    "Jacobian routine cannot be supplied for NULL SUNMatrix");
    return(KINLS_ILL_INPUT);
  }

  if (jac != NULL) {
    kinls_mem->jacDQ  = SUNFALSE;
    kinls_mem->jac    = jac;
    kinls_mem->J_data = kin_mem->kin_user_data;
  } else {
    kinls_mem->jacDQ  = SUNTRUE;
    kinls_mem->jac    = kinLsDQJac;
    kinls_mem->J_data = kin_mem;
  }

  return(KINLS_SUCCESS);
}


/*------------------------------------------------------------------
  KINSetPreconditioner sets the preconditioner setup and solve
  functions
  ------------------------------------------------------------------*/
int KINSetPreconditioner(void *kinmem,
                         KINLsPrecSetupFn psetup,
                         KINLsPrecSolveFn psolve)
{
  KINMem   kin_mem;
  KINLsMem kinls_mem;
  PSetupFn kinls_psetup;
  PSolveFn kinls_psolve;
  int      retval;

  /* access KINLsMem structure */
  retval = kinLs_AccessLMem(kinmem, "KINSetPreconditioner",
                            &kin_mem, &kinls_mem);
  if (retval != KIN_SUCCESS)  return(retval);

  /* store function pointers for user-supplied routines in KINLS interface */
  kinls_mem->pset   = psetup;
  kinls_mem->psolve = psolve;

  /* issue error if LS object does not support user-supplied preconditioning */
  if (kinls_mem->LS->ops->setpreconditioner == NULL) {
    KINProcessError(kin_mem, KINLS_ILL_INPUT, "KINLS", "KINSetPreconditioner",
                   "SUNLinearSolver object does not support user-supplied preconditioning");
    return(KINLS_ILL_INPUT);
  }

  /* notify iterative linear solver to call KINLs interface routines */
  kinls_psetup = (psetup == NULL) ? NULL : kinLsPSetup;
  kinls_psolve = (psolve == NULL) ? NULL : kinLsPSolve;
  retval = SUNLinSolSetPreconditioner(kinls_mem->LS, kin_mem,
                                      kinls_psetup, kinls_psolve);
  if (retval != SUNLS_SUCCESS) {
    KINProcessError(kin_mem, KINLS_SUNLS_FAIL, "KINLS",  "KINSetPreconditioner",
                    "Error in calling SUNLinSolSetPreconditioner");
    return(KINLS_SUNLS_FAIL);
  }

  return(KINLS_SUCCESS);
}

/*------------------------------------------------------------------
  KINSetJacTimesVecFn sets the matrix-vector product function
  ------------------------------------------------------------------*/
int KINSetJacTimesVecFn(void *kinmem, KINLsJacTimesVecFn jtv)
{
  int      retval;
  KINMem   kin_mem;
  KINLsMem kinls_mem;

  /* access KINLsMem structure */
  retval = kinLs_AccessLMem(kinmem, "KINSetJacTimesVecFn",
                            &kin_mem, &kinls_mem);
  if (retval != KIN_SUCCESS)  return(retval);

  /* issue error if LS object does not support user-supplied ATimes */
  if (kinls_mem->LS->ops->setatimes == NULL) {
    KINProcessError(kin_mem, KINLS_ILL_INPUT, "KINLS", "KINSetJacTimesVecFn",
                    "SUNLinearSolver object does not support user-supplied ATimes routine");
    return(KINLS_ILL_INPUT);
  }

  /* store function pointers for user-supplied routine in KINLs
     interface (NULL jtimes implies use of DQ default) */
  if (jtv != NULL) {
    kinls_mem->jtimesDQ = SUNFALSE;
    kinls_mem->jtimes   = jtv;
    kinls_mem->jt_data  = kin_mem->kin_user_data;
  } else {
    kinls_mem->jtimesDQ = SUNTRUE;
    kinls_mem->jtimes   = kinLsDQJtimes;
    kinls_mem->jt_data  = kin_mem;
  }

  return(KINLS_SUCCESS);
}

/*------------------------------------------------------------------
  KINGetLinWorkSpace returns the integer and real workspace size
  ------------------------------------------------------------------*/
int KINGetLinWorkSpace(void *kinmem, long int *lenrwLS, long int *leniwLS)
{
  KINMem       kin_mem;
  KINLsMem     kinls_mem;
  sunindextype lrw1, liw1;
  long int     lrw, liw;
  int          retval;

  /* access KINLsMem structure */
  retval = kinLs_AccessLMem(kinmem, "KINGetLinWorkSpace",
                            &kin_mem, &kinls_mem);
  if (retval != KIN_SUCCESS)  return(retval);

  /* start with fixed sizes plus vector/matrix pointers */
  *lenrwLS = 1;
  *leniwLS = 21;

  /* add N_Vector sizes */
  if (kin_mem->kin_vtemp1->ops->nvspace) {
    N_VSpace(kin_mem->kin_vtemp1, &lrw1, &liw1);
    *lenrwLS += lrw1;
    *leniwLS += liw1;
  }

  /* add LS sizes */
  if (kinls_mem->LS->ops->space) {
    retval = SUNLinSolSpace(kinls_mem->LS, &lrw, &liw);
    if (retval == 0) {
      *lenrwLS += lrw;
      *leniwLS += liw;
    }
  }

  return(KINLS_SUCCESS);
}

/*------------------------------------------------------------------
  KINGetNumJacEvals returns the number of Jacobian evaluations
  ------------------------------------------------------------------*/
int KINGetNumJacEvals(void *kinmem, long int *njevals)
{
  KINMem   kin_mem;
  KINLsMem kinls_mem;
  int      retval;

  /* access KINLsMem structure; set output value and return */
  retval = kinLs_AccessLMem(kinmem, "KINGetNumJacEvals",
                            &kin_mem, &kinls_mem);
  if (retval != KIN_SUCCESS)  return(retval);
  *njevals = kinls_mem->nje;
  return(KINLS_SUCCESS);
}

/*------------------------------------------------------------------
  KINGetNumPrecEvals returns the total number of preconditioner
  evaluations
  ------------------------------------------------------------------*/
int KINGetNumPrecEvals(void *kinmem, long int *npevals)
{
  KINMem   kin_mem;
  KINLsMem kinls_mem;
  int      retval;

  /* access KINLsMem structure; set output value and return */
  retval = kinLs_AccessLMem(kinmem, "KINGetNumPrecEvals",
                            &kin_mem, &kinls_mem);
  if (retval != KIN_SUCCESS)  return(retval);
  *npevals = kinls_mem->npe;
  return(KINLS_SUCCESS);
}

/*------------------------------------------------------------------
  KINGetNumPrecSolves returns the total number of times the
  preconditioner was applied
  ------------------------------------------------------------------*/
int KINGetNumPrecSolves(void *kinmem, long int *npsolves)
{
  KINMem   kin_mem;
  KINLsMem kinls_mem;
  int      retval;

  /* access KINLsMem structure; set output value and return */
  retval = kinLs_AccessLMem(kinmem, "KINGetNumPrecSolves",
                            &kin_mem, &kinls_mem);
  if (retval != KIN_SUCCESS)  return(retval);
  *npsolves = kinls_mem->nps;
  return(KINLS_SUCCESS);
}

/*------------------------------------------------------------------
  KINGetNumLinIters returns the total number of linear
  iterations
  ------------------------------------------------------------------*/
int KINGetNumLinIters(void *kinmem, long int *nliters)
{
  KINMem   kin_mem;
  KINLsMem kinls_mem;
  int      retval;

  /* access KINLsMem structure; set output value and return */
  retval = kinLs_AccessLMem(kinmem, "KINGetNumLinIters",
                            &kin_mem, &kinls_mem);
  if (retval != KIN_SUCCESS)  return(retval);
  *nliters = kinls_mem->nli;
  return(KINLS_SUCCESS);
}

/*------------------------------------------------------------------
  KINGetNumLinConvFails returns the total numbe of convergence
  failures
  ------------------------------------------------------------------*/
int KINGetNumLinConvFails(void *kinmem, long int *nlcfails)
{
  KINMem   kin_mem;
  KINLsMem kinls_mem;
  int      retval;

  /* access KINLsMem structure; set output value and return */
  retval = kinLs_AccessLMem(kinmem, "KINGetNumLinConvFails",
                            &kin_mem, &kinls_mem);
  if (retval != KIN_SUCCESS)  return(retval);
  *nlcfails = kinls_mem->ncfl;
  return(KINLS_SUCCESS);
}

/*------------------------------------------------------------------
  KINGetNumJtimesEvals returns the number of times the matrix
  vector product was computed
  ------------------------------------------------------------------*/
int KINGetNumJtimesEvals(void *kinmem, long int *njvevals)
{
  KINMem   kin_mem;
  KINLsMem kinls_mem;
  int      retval;

  /* access KINLsMem structure; set output value and return */
  retval = kinLs_AccessLMem(kinmem, "KINGetNumJtimesEvals",
                            &kin_mem, &kinls_mem);
  if (retval != KIN_SUCCESS)  return(retval);
  *njvevals = kinls_mem->njtimes;
  return(KINLS_SUCCESS);
}

/*------------------------------------------------------------------
  KINGetNumLinFuncEvals returns the number of calls to the user's
  F routine by the linear solver module
  ------------------------------------------------------------------*/
int KINGetNumLinFuncEvals(void *kinmem, long int *nfevals)
{
  KINMem   kin_mem;
  KINLsMem kinls_mem;
  int      retval;

  /* access KINLsMem structure; set output value and return */
  retval = kinLs_AccessLMem(kinmem, "KINGetNumLinFuncEvals",
                            &kin_mem, &kinls_mem);
  if (retval != KIN_SUCCESS)  return(retval);
  *nfevals = kinls_mem->nfeDQ;
  return(KINLS_SUCCESS);
}

/*------------------------------------------------------------------
  KINGetLastLinFlag returns the last flag set in the KINLS
  function
  ------------------------------------------------------------------*/
int KINGetLastLinFlag(void *kinmem, long int *flag)
{
  KINMem   kin_mem;
  KINLsMem kinls_mem;
  int      retval;

  /* access KINLsMem structure; set output value and return */
  retval = kinLs_AccessLMem(kinmem, "KINGetLastLinFlag",
                            &kin_mem, &kinls_mem);
  if (retval != KIN_SUCCESS)  return(retval);
  *flag = kinls_mem->last_flag;
  return(KINLS_SUCCESS);
}

/*------------------------------------------------------------------
  KINGetLinReturnFlagName
  ------------------------------------------------------------------*/
char *KINGetLinReturnFlagName(long int flag)
{
  char *name;

  name = (char *)malloc(30*sizeof(char));

  switch(flag) {
  case KINLS_SUCCESS:
    sprintf(name, "KINLS_SUCCESS");
    break;
  case KINLS_MEM_NULL:
    sprintf(name, "KINLS_MEM_NULL");
    break;
  case KINLS_LMEM_NULL:
    sprintf(name, "KINLS_LMEM_NULL");
    break;
  case KINLS_ILL_INPUT:
    sprintf(name, "KINLS_ILL_INPUT");
    break;
  case KINLS_MEM_FAIL:
    sprintf(name, "KINLS_MEM_FAIL");
    break;
  case KINLS_PMEM_NULL:
    sprintf(name, "KINLS_PMEM_NULL");
    break;
  case KINLS_JACFUNC_ERR:
    sprintf(name,"KINLS_JACFUNC_ERR");
    break;
  case KINLS_SUNMAT_FAIL:
    sprintf(name,"KINLS_SUNMAT_FAIL");
    break;
  case KINLS_SUNLS_FAIL:
    sprintf(name,"KINLS_SUNLS_FAIL");
    break;
  default:
    sprintf(name, "NONE");
  }

  return(name);
}


/*==================================================================
  KINLS Private functions
  ==================================================================*/

/*------------------------------------------------------------------
  kinLsATimes

  This routine coordinates the generation of the matrix-vector
  product z = J*v by calling either kinLsDQJtimes, which uses
  a difference quotient approximation for J*v, or by calling the
  user-supplied routine KINLsJacTimesVecFn if it is non-null.
  ------------------------------------------------------------------*/
int kinLsATimes(void *kinmem, N_Vector v, N_Vector z)
{
  KINMem   kin_mem;
  KINLsMem kinls_mem;
  int      retval;

  /* access KINLsMem structure */
  retval = kinLs_AccessLMem(kinmem, "kinLsATimes",
                            &kin_mem, &kinls_mem);
  if (retval != KIN_SUCCESS)  return(retval);

  /* call Jacobian-times-vector product routine
     (either user-supplied or internal DQ) */
  retval = kinls_mem->jtimes(v, z, kin_mem->kin_uu,
                             &(kinls_mem->new_uu),
                             kinls_mem->jt_data);
  kinls_mem->njtimes++;
  return(retval);
}


/*---------------------------------------------------------------
  kinLsPSetup:

  This routine interfaces between the generic iterative linear
  solvers and the user's psetup routine. It passes to psetup all
  required state information from kin_mem. Its return value
  is the same as that returned by psetup. Note that the generic
  iterative linear solvers guarantee that kinLsPSetup will only
  be called in the case that the user's psetup routine is non-NULL.
  ---------------------------------------------------------------*/
int kinLsPSetup(void *kinmem)
{
  KINMem   kin_mem;
  KINLsMem kinls_mem;
  int      retval;

  /* access KINLsMem structure */
  retval = kinLs_AccessLMem(kinmem, "kinLsPSetup",
                            &kin_mem, &kinls_mem);
  if (retval != KIN_SUCCESS)  return(retval);

  /* Call user pset routine to update preconditioner */
  retval = kinls_mem->pset(kin_mem->kin_uu, kin_mem->kin_uscale,
                           kin_mem->kin_fval, kin_mem->kin_fscale,
                           kinls_mem->pdata);
  kinls_mem->npe++;
  return(retval);
}


/*------------------------------------------------------------------
  kinLsPSolve

  This routine interfaces between the generic iterative linear
  solvers and the user's psolve routine. It passes to psolve all
  required state information from kinsol_mem. Its return value is
  the same as that returned by psolve. Note that the generic
  SUNLinSol solver guarantees that kinLsPSolve will not be called
  in the case in which preconditioning is not done. This is the only
  case in which the user's psolve routine is allowed to be NULL.
  ------------------------------------------------------------------*/
int kinLsPSolve(void *kinmem, N_Vector r, N_Vector z, realtype tol, int lr)
{
  KINMem   kin_mem;
  KINLsMem kinls_mem;
  int      retval;

  /* access KINLsMem structure */
  retval = kinLs_AccessLMem(kinmem, "kinLsPSolve",
                            &kin_mem, &kinls_mem);
  if (retval != KIN_SUCCESS)  return(retval);

  /* copy the rhs into z before the psolve call */
  /* Note: z returns with the solution */
  N_VScale(ONE, r, z);

  /* note: user-supplied preconditioning with KINSOL does not
     support either the 'tol' or 'lr' inputs */
  retval = kinls_mem->psolve(kin_mem->kin_uu, kin_mem->kin_uscale,
                             kin_mem->kin_fval, kin_mem->kin_fscale,
                             z, kinls_mem->pdata);
  kinls_mem->nps++;
  return(retval);
}


/*------------------------------------------------------------------
  kinLsDQJac

  This routine is a wrapper for the Dense and Band implementations
  of the difference quotient Jacobian approximation routines.
  ------------------------------------------------------------------*/
int kinLsDQJac(N_Vector u, N_Vector fu, SUNMatrix Jac,
               void *kinmem, N_Vector tmp1, N_Vector tmp2)
{
  KINMem kin_mem;
  int    retval;

  /* access KINMem structure */
  if (kinmem == NULL) {
    KINProcessError(NULL, KINLS_MEM_NULL, "KINLS",
                    "kinLsDQJac", MSG_LS_KINMEM_NULL);
    return(KINLS_MEM_NULL);
  }
  kin_mem = (KINMem) kinmem;

  /* verify that Jac is non-NULL */
  if (Jac == NULL) {
    KINProcessError(kin_mem, KINLS_LMEM_NULL, "KINLS",
                    "kinLsDQJac", MSG_LS_LMEM_NULL);
    return(KINLS_LMEM_NULL);
  }

  /* Call the matrix-structure-specific DQ approximation routine */
  if (SUNMatGetID(Jac) == SUNMATRIX_DENSE) {
    retval = kinLsDenseDQJac(u, fu, Jac, kin_mem, tmp1, tmp2);
  } else if (SUNMatGetID(Jac) == SUNMATRIX_BAND) {
    retval = kinLsBandDQJac(u, fu, Jac, kin_mem, tmp1, tmp2);
  } else {
    KINProcessError(kin_mem, KIN_ILL_INPUT, "KINLS", "kinLsDQJac",
                    "unrecognized matrix type for kinLsDQJac");
    retval = KIN_ILL_INPUT;
  }
  return(retval);
}


/*------------------------------------------------------------------
  kinLsDenseDQJac

  This routine generates a dense difference quotient approximation
  to the Jacobian of F(u). It assumes a dense SUNMatrix input
  stored column-wise, and that elements within each column are
  contiguous. The address of the jth column of J is obtained via
  the function SUNDenseMatrix_Column() and this pointer is
  associated with an N_Vector using the N_VGetArrayPointer and
  N_VSetArrayPointer functions. Finally, the actual computation of
  the jth column of the Jacobian is done with a call to N_VLinearSum.

  The increment used in the finite-difference approximation
    J_ij = ( F_i(u+sigma_j * e_j) - F_i(u)  ) / sigma_j
  is
   sigma_j = max{|u_j|, |1/uscale_j|} * sqrt(uround)

  Note: uscale_j = 1/typ(u_j)

  NOTE: Any type of failure of the system function here leads to an
        unrecoverable failure of the Jacobian function and thus of
        the linear solver setup function, stopping KINSOL.
  ------------------------------------------------------------------*/
int kinLsDenseDQJac(N_Vector u, N_Vector fu, SUNMatrix Jac,
                    KINMem kin_mem, N_Vector tmp1, N_Vector tmp2)
{
  realtype inc, inc_inv, ujsaved, ujscale, sign;
  realtype *tmp2_data, *u_data, *uscale_data;
  N_Vector ftemp, jthCol;
  sunindextype j, N;
  KINLsMem kinls_mem;
  int retval = 0;

  /* access LsMem interface structure */
  kinls_mem = (KINLsMem) kin_mem->kin_lmem;

  /* access matrix dimension */
  N = SUNDenseMatrix_Rows(Jac);

  /* Save pointer to the array in tmp2 */
  tmp2_data = N_VGetArrayPointer(tmp2);

  /* Rename work vectors for readibility */
  ftemp  = tmp1;
  jthCol = tmp2;

  /* Obtain pointers to the data for u and uscale */
  u_data      = N_VGetArrayPointer(u);
  uscale_data = N_VGetArrayPointer(kin_mem->kin_uscale);

  /* This is the only for loop for 0..N-1 in KINSOL */

  for (j = 0; j < N; j++) {

    /* Generate the jth col of J(u) */

    /* Set data address of jthCol, and save u_j values and scaling */
    N_VSetArrayPointer(SUNDenseMatrix_Column(Jac,j), jthCol);
    ujsaved = u_data[j];
    ujscale = ONE/uscale_data[j];

    /* Compute increment */
    sign = (ujsaved >= ZERO) ? ONE : -ONE;
    inc = kin_mem->kin_sqrt_relfunc*SUNMAX(SUNRabs(ujsaved), ujscale)*sign;

    /* Increment u_j, call F(u), and return if error occurs */
    u_data[j] += inc;

    retval = kin_mem->kin_func(u, ftemp, kin_mem->kin_user_data);
    kinls_mem->nfeDQ++;
    if (retval != 0) break;

    /* reset u_j */
    u_data[j] = ujsaved;

    /* Construct difference quotient in jthCol */
    inc_inv = ONE/inc;
    N_VLinearSum(inc_inv, ftemp, -inc_inv, fu, jthCol);
  }

  /* Restore original array pointer in tmp2 */
  N_VSetArrayPointer(tmp2_data, tmp2);

  return(retval);
}


/*------------------------------------------------------------------
  kinLsBandDQJac

  This routine generates a banded difference quotient approximation
  to the Jacobian of F(u).  It assumes a SUNBandMatrix input stored
  column-wise, and that elements within each column are contiguous.
  This makes it possible to get the address of a column of J via the
  function SUNBandMatrix_Column() and to write a simple for loop to
  set each of the elements of a column in succession.

  NOTE: Any type of failure of the system function her leads to an
        unrecoverable failure of the Jacobian function and thus of
        the linear solver setup function, stopping KINSOL.
  ------------------------------------------------------------------*/
int kinLsBandDQJac(N_Vector u, N_Vector fu, SUNMatrix Jac,
                   KINMem kin_mem, N_Vector tmp1, N_Vector tmp2)
{
  realtype inc, inc_inv;
  N_Vector futemp, utemp;
  sunindextype group, i, j, width, ngroups, i1, i2;
  sunindextype N, mupper, mlower;
  realtype *col_j, *fu_data, *futemp_data, *u_data, *utemp_data, *uscale_data;
  KINLsMem kinls_mem;
  int retval = 0;

  /* access LsMem interface structure */
  kinls_mem = (KINLsMem) kin_mem->kin_lmem;

  /* access matrix dimensions */
  N = SUNBandMatrix_Columns(Jac);
  mupper = SUNBandMatrix_UpperBandwidth(Jac);
  mlower = SUNBandMatrix_LowerBandwidth(Jac);

  /* Rename work vectors for use as temporary values of u and fu */
  futemp = tmp1;
  utemp  = tmp2;

  /* Obtain pointers to the data for ewt, fy, futemp, y, ytemp */
  fu_data     = N_VGetArrayPointer(fu);
  futemp_data = N_VGetArrayPointer(futemp);
  u_data      = N_VGetArrayPointer(u);
  uscale_data = N_VGetArrayPointer(kin_mem->kin_uscale);
  utemp_data  = N_VGetArrayPointer(utemp);

  /* Load utemp with u */
  N_VScale(ONE, u, utemp);

  /* Set bandwidth and number of column groups for band differencing */
  width   = mlower + mupper + 1;
  ngroups = SUNMIN(width, N);

  for (group=1; group <= ngroups; group++) {

    /* Increment all utemp components in group */
    for(j=group-1; j < N; j+=width) {
      inc = kin_mem->kin_sqrt_relfunc*SUNMAX(SUNRabs(u_data[j]),
                                             ONE/SUNRabs(uscale_data[j]));
      utemp_data[j] += inc;
    }

    /* Evaluate f with incremented u */
    retval = kin_mem->kin_func(utemp, futemp, kin_mem->kin_user_data);
    if (retval != 0) return(retval);

    /* Restore utemp components, then form and load difference quotients */
    for (j=group-1; j < N; j+=width) {
      utemp_data[j] = u_data[j];
      col_j = SUNBandMatrix_Column(Jac, j);
      inc = kin_mem->kin_sqrt_relfunc*SUNMAX(SUNRabs(u_data[j]),
                                             ONE/SUNRabs(uscale_data[j]));
      inc_inv = ONE/inc;
      i1 = SUNMAX(0, j-mupper);
      i2 = SUNMIN(j+mlower, N-1);
      for (i=i1; i <= i2; i++)
        SM_COLUMN_ELEMENT_B(col_j,i,j) = inc_inv * (futemp_data[i] - fu_data[i]);
    }
  }

  /* Increment counter nfeDQ */
  kinls_mem->nfeDQ += ngroups;

  return(0);
}


/*------------------------------------------------------------------
  kinLsDQJtimes

  This routine generates the matrix-vector product z = J*v using a
  difference quotient approximation. The approximation is
  J*v = [func(uu + sigma*v) - func(uu)]/sigma. Here sigma is based
  on the dot products (uscale*uu, uscale*v) and
  (uscale*v, uscale*v), the L1Norm(uscale*v), and on sqrt_relfunc
  (the square root of the relative error in the function). Note
  that v in the argument list has already been both preconditioned
  and unscaled.

  NOTE: Unlike the DQ Jacobian functions for direct linear solvers
        (which are called from within the lsetup function), this
        function is called from within the lsolve function and thus
        a recovery may still be possible even if the system function
        fails (recoverably).
  ------------------------------------------------------------------*/
int kinLsDQJtimes(N_Vector v, N_Vector Jv, N_Vector u,
                  booleantype *new_u, void *kinmem)
{
  realtype sigma, sigma_inv, sutsv, sq1norm, sign, vtv;
  KINMem   kin_mem;
  KINLsMem kinls_mem;
  int      retval;

  /* access KINLsMem structure */
  retval = kinLs_AccessLMem(kinmem, "kinLsDQJtimes",
                            &kin_mem, &kinls_mem);
  if (retval != KIN_SUCCESS)  return(retval);

  /* ensure that NVector supplies requisite routines */
  if ( (v->ops->nvprod == NULL) || (v->ops->nvdotprod == NULL) ||
       (v->ops->nvl1norm == NULL) || (v->ops->nvlinearsum == NULL) ){
    KINProcessError(kin_mem, KINLS_ILL_INPUT, "KINLS",
                    "kinLsDQJtimes", MSG_LS_BAD_NVECTOR);
    return(KINLS_ILL_INPUT);
  }

  /* scale the vector v and put Du*v into vtemp1 */
  N_VProd(v, kin_mem->kin_uscale, kin_mem->kin_vtemp1);

  /* scale u and put into Jv (used as a temporary storage) */
  N_VProd(u, kin_mem->kin_uscale, Jv);

  /* compute dot product (Du*u).(Du*v) */
  sutsv = N_VDotProd(Jv, kin_mem->kin_vtemp1);

  /* compute dot product (Du*v).(Du*v) */
  vtv = N_VDotProd(kin_mem->kin_vtemp1, kin_mem->kin_vtemp1);

  /* compute differencing factor -- this is from p. 469, Brown and Saad paper */
  sq1norm = N_VL1Norm(kin_mem->kin_vtemp1);
  sign = (sutsv >= ZERO) ? ONE : -ONE ;
  sigma = sign*(kin_mem->kin_sqrt_relfunc)*SUNMAX(SUNRabs(sutsv),sq1norm)/vtv;
  sigma_inv = ONE/sigma;

  /* compute the u-prime at which to evaluate the function func */
  N_VLinearSum(ONE, u, sigma, v, kin_mem->kin_vtemp1);

  /* call the system function to calculate func(u+sigma*v) */
  retval = kin_mem->kin_func(kin_mem->kin_vtemp1, kin_mem->kin_vtemp2,
                             kin_mem->kin_user_data);
  kinls_mem->nfeDQ++;
  if (retval != 0) return(retval);

  /* finish the computation of the difference quotient */
  N_VLinearSum(sigma_inv, kin_mem->kin_vtemp2, -sigma_inv, kin_mem->kin_fval, Jv);

  return(0);
}


/*------------------------------------------------------------------
  kinLsInitialize performs remaining initializations specific
  to the iterative linear solver interface (and solver itself)
  ------------------------------------------------------------------*/
int kinLsInitialize(KINMem kin_mem)
{
  KINLsMem kinls_mem;
  int      retval;

  /* Access KINLsMem structure */
  if (kin_mem->kin_lmem == NULL) {
    KINProcessError(kin_mem, KINLS_LMEM_NULL, "KINLS",
                    "kinLsInitialize", MSG_LS_LMEM_NULL);
    return(KINLS_LMEM_NULL);
  }
  kinls_mem = (KINLsMem) kin_mem->kin_lmem;

  /* Test for valid combinations of matrix & Jacobian routines: */
  if (kinls_mem->J == NULL) {

    /* If SUNMatrix A is NULL: ensure 'jac' function pointer is NULL */
    kinls_mem->jacDQ  = SUNFALSE;
    kinls_mem->jac    = NULL;
    kinls_mem->J_data = NULL;

  } else if (kinls_mem->jacDQ) {

    /* If J is non-NULL, and 'jac' is not user-supplied:
       - if A is dense or band, ensure that our DQ approx. is used
       - otherwise => error */
    retval = 0;
    if (kinls_mem->J->ops->getid) {

      if ( (SUNMatGetID(kinls_mem->J) == SUNMATRIX_DENSE) ||
           (SUNMatGetID(kinls_mem->J) == SUNMATRIX_BAND) ) {
        kinls_mem->jac    = kinLsDQJac;
        kinls_mem->J_data = kin_mem;
      } else {
        retval++;
      }

    } else {
      retval++;
    }
    if (retval) {
      KINProcessError(kin_mem, KINLS_ILL_INPUT, "KINLS", "kinLsInitialize",
                     "No Jacobian constructor available for SUNMatrix type");
      kinls_mem->last_flag = KINLS_ILL_INPUT;
      return(KINLS_ILL_INPUT);
    }

    /* check for required vector operations for kinLsDQJac routine */
    if ( (kin_mem->kin_vtemp1->ops->nvlinearsum == NULL) ||
         (kin_mem->kin_vtemp1->ops->nvscale == NULL) ||
         (kin_mem->kin_vtemp1->ops->nvgetarraypointer == NULL) ||
         (kin_mem->kin_vtemp1->ops->nvsetarraypointer == NULL) ) {
      KINProcessError(kin_mem, KINLS_ILL_INPUT, "KINLS",
                      "kinLsInitialize", MSG_LS_BAD_NVECTOR);
      return(KINLS_ILL_INPUT);
    }

  } else {

    /* If J is non-NULL, and 'jac' is user-supplied,
       reset J_data pointer (just in case) */
    kinls_mem->J_data = kin_mem->kin_user_data;
  }

  /* Prohibit Picard iteration with DQ Jacobian approximation or difference-quotient J*v */
  if ( (kin_mem->kin_globalstrategy == KIN_PICARD) &&
       kinls_mem->jacDQ && kinls_mem->jtimesDQ ) {
    KINProcessError(kin_mem, KINLS_ILL_INPUT, "KINLS",
                    "kinLsInitialize", MSG_NOL_FAIL);
    return(KINLS_ILL_INPUT);
  }


  /** error-checking is complete, begin initializtions **/

  /* Initialize counters */
  kinLsInitializeCounters(kinls_mem);

  /* Set Jacobian-related fields, based on jtimesDQ */
  if (kinls_mem->jtimesDQ) {
    kinls_mem->jtimes  = kinLsDQJtimes;
    kinls_mem->jt_data = kin_mem;
  } else {
    kinls_mem->jt_data = kin_mem->kin_user_data;
  }

  /* if J is NULL and: NOT preconditioning or do NOT need to setup the
     preconditioner, then set the lsetup function to NULL */
  if (kinls_mem->J == NULL)
    if ((kinls_mem->psolve == NULL) || (kinls_mem->pset == NULL))
      kin_mem->kin_lsetup = NULL;

  /* Set scaling vectors assuming RIGHT preconditioning */
  /* NOTE: retval is non-zero only if LS == NULL        */
  if (kinls_mem->LS->ops->setscalingvectors) {
    retval = SUNLinSolSetScalingVectors(kinls_mem->LS,
                                        kin_mem->kin_fscale,
                                        kin_mem->kin_fscale);
    if (retval != SUNLS_SUCCESS) {
      KINProcessError(kin_mem, KINLS_SUNLS_FAIL, "KINLS", "kinLsInitialize",
                      "Error in calling SUNLinSolSetScalingVectors");
      return(KINLS_SUNLS_FAIL);
    }
  }

  /* If the linear solver is iterative or matrix-iterative, and if left/right
     scaling are not supported, we must update linear solver tolerances in an
     attempt to account for the fscale vector.  We make the following assumptions:
       1. fscale_i = fs_mean, for i=0,...,n-1 (i.e. the weights are homogeneous)
       2. the linear solver uses a basic 2-norm to measure convergence
     Hence (using the notation from sunlinsol_spgmr.h, with S = diag(fscale)),
           || bbar - Abar xbar ||_2 < tol
       <=> || S b - S A x ||_2 < tol
       <=> || S (b - A x) ||_2 < tol
       <=> \sum_{i=0}^{n-1} (fscale_i (b - A x)_i)^2 < tol^2
       <=> fs_mean^2 \sum_{i=0}^{n-1} (b - A x_i)^2 < tol^2
       <=> \sum_{i=0}^{n-1} (b - A x_i)^2 < tol^2 / fs_mean^2
       <=> || b - A x ||_2 < tol / fs_mean
       <=> || b - A x ||_2 < tol * tol_fac
     So we compute tol_fac = sqrt(N) / ||fscale||_L2 for scaling desired tolerances */
  if (kinls_mem->iterative && kinls_mem->LS->ops->setscalingvectors == NULL) {
    N_VConst(ONE, kin_mem->kin_vtemp1);
    kinls_mem->tol_fac = SUNRsqrt(N_VGetLength(kin_mem->kin_vtemp1))
                       / N_VWL2Norm(kin_mem->kin_fscale, kin_mem->kin_vtemp1);
  } else {
    kinls_mem->tol_fac = ONE;
  }

  /* Call LS initialize routine, and return result */
  kinls_mem->last_flag = SUNLinSolInitialize(kinls_mem->LS);
  return(kinls_mem->last_flag);
}


/*------------------------------------------------------------------
  kinLsSetup call the LS setup routine
  ------------------------------------------------------------------*/
int kinLsSetup(KINMem kin_mem)
{
  KINLsMem kinls_mem;
  int      retval;

  /* Access KINLsMem structure */
  if (kin_mem->kin_lmem == NULL) {
    KINProcessError(kin_mem, KINLS_LMEM_NULL, "KINLS",
                    "kinLsSetup", MSG_LS_LMEM_NULL);
    return(KINLS_LMEM_NULL);
  }
  kinls_mem = (KINLsMem) kin_mem->kin_lmem;

  /* recompute if J if it is non-NULL */
  if (kinls_mem->J) {

    /* Increment nje counter. */
    kinls_mem->nje++;

    /* Clear the linear system matrix if necessary */
    if (SUNLinSolGetType(kinls_mem->LS) == SUNLINEARSOLVER_DIRECT) {
      retval = SUNMatZero(kinls_mem->J);
      if (retval != 0) {
        KINProcessError(kin_mem, KINLS_SUNMAT_FAIL, "KINLS",
                        "kinLsSetup", MSG_LS_MATZERO_FAILED);
        kinls_mem->last_flag = KINLS_SUNMAT_FAIL;
        return(kinls_mem->last_flag);
      }
    }

    /* Call Jacobian routine */
    retval = kinls_mem->jac(kin_mem->kin_uu, kin_mem->kin_fval,
                            kinls_mem->J, kinls_mem->J_data,
                            kin_mem->kin_vtemp1, kin_mem->kin_vtemp2);
    if (retval != 0) {
      KINProcessError(kin_mem, KINLS_JACFUNC_ERR, "KINLS",
                      "kinLsSetup", MSG_LS_JACFUNC_FAILED);
      kinls_mem->last_flag = KINLS_JACFUNC_ERR;
      return(kinls_mem->last_flag);
    }

  }

  /* Call LS setup routine -- the LS will call kinLsPSetup (if applicable) */
  kinls_mem->last_flag = SUNLinSolSetup(kinls_mem->LS, kinls_mem->J);

  /* save nni value from most recent lsetup call */
  kin_mem->kin_nnilset = kin_mem->kin_nni;

  return(kinls_mem->last_flag);
}


/*------------------------------------------------------------------
  kinLsSolve interfaces between KINSOL and the generic
  SUNLinearSolver object
  ------------------------------------------------------------------*/
int kinLsSolve(KINMem kin_mem, N_Vector xx, N_Vector bb,
               realtype *sJpnorm, realtype *sFdotJp)
{
  KINLsMem kinls_mem;
  int      nli_inc, retval;
  realtype res_norm, tol;

  /* Access KINLsMem structure */
  if (kin_mem->kin_lmem == NULL) {
    KINProcessError(kin_mem, KINLS_LMEM_NULL, "KINLS",
                    "kinLsSolve", MSG_LS_LMEM_NULL);
    return(KINLS_LMEM_NULL);
  }
  kinls_mem = (KINLsMem) kin_mem->kin_lmem;

  /* Set linear solver tolerance as input value times scaling factor
     (to account for possible lack of support for left/right scaling
     vectors in SUNLinSol object) */
  tol = kin_mem->kin_eps * kinls_mem->tol_fac;

  /* Set initial guess x = 0 to LS */
  N_VConst(ZERO, xx);

  /* set flag required for user-supplied J*v routine */
  kinls_mem->new_uu = SUNTRUE;

  /* Call solver */
  retval = SUNLinSolSolve(kinls_mem->LS, kinls_mem->J, xx, bb, tol);

  /* Retrieve solver statistics */
  res_norm = ZERO;
  if (kinls_mem->LS->ops->resnorm)
    res_norm = SUNLinSolResNorm(kinls_mem->LS);
  nli_inc = 0;
  if (kinls_mem->LS->ops->numiters)
    nli_inc = SUNLinSolNumIters(kinls_mem->LS);

  if (kinls_mem->iterative && kin_mem->kin_printfl > 2)
    KINPrintInfo(kin_mem, PRNT_NLI, "KINLS", "kinLsSolve",
                 INFO_NLI, nli_inc);

  /* Increment counters nli and ncfl */
  kinls_mem->nli += nli_inc;
  if (retval != SUNLS_SUCCESS) kinls_mem->ncfl++;

  /* Interpret solver return value */
  kinls_mem->last_flag = retval;

  if ( (retval != 0) && (retval != SUNLS_RES_REDUCED) ) {

    switch(retval) {
    case SUNLS_ATIMES_FAIL_REC:
    case SUNLS_PSOLVE_FAIL_REC:
      return(1);
      break;
    case SUNLS_MEM_NULL:
    case SUNLS_ILL_INPUT:
    case SUNLS_MEM_FAIL:
    case SUNLS_GS_FAIL:
    case SUNLS_CONV_FAIL:
    case SUNLS_QRFACT_FAIL:
    case SUNLS_LUFACT_FAIL:
    case SUNLS_QRSOL_FAIL:
      break;
    case SUNLS_PACKAGE_FAIL_REC:
      KINProcessError(kin_mem, SUNLS_PACKAGE_FAIL_REC, "KINLS",
                      "kinLsSolve",
                      "Failure in SUNLinSol external package");
      break;
    case SUNLS_PACKAGE_FAIL_UNREC:
      KINProcessError(kin_mem, SUNLS_PACKAGE_FAIL_UNREC, "KINLS",
                      "kinLsSolve",
                      "Failure in SUNLinSol external package");
      break;
    case SUNLS_ATIMES_FAIL_UNREC:
      KINProcessError(kin_mem, SUNLS_ATIMES_FAIL_UNREC, "KINLS",
                      "kinLsSolve", MSG_LS_JTIMES_FAILED);
      break;
    case SUNLS_PSOLVE_FAIL_UNREC:
      KINProcessError(kin_mem, SUNLS_PSOLVE_FAIL_UNREC, "KINLS",
                      "kinLsSolve", MSG_LS_PSOLVE_FAILED);
      break;
    }
    return(retval);
  }

  /* SUNLinSolSolve returned SUNLS_SUCCESS or SUNLS_RES_REDUCED */

  /* Compute auxiliary values for use in the linesearch and in KINForcingTerm.
     These will be subsequently corrected if the step is reduced by constraints
     or the linesearch. */
  if (kin_mem->kin_globalstrategy != KIN_FP) {

    /* sJpnorm is the norm of the scaled product (scaled by fscale) of the
       current Jacobian matrix J and the step vector p (= solution vector xx) */
    if (kin_mem->kin_inexact_ls && kin_mem->kin_etaflag == KIN_ETACHOICE1) {
      retval = kinLsATimes(kin_mem, xx, bb);
      if (retval > 0) {
        kinls_mem->last_flag = SUNLS_ATIMES_FAIL_REC;
        return(1);
      }
      else if (retval < 0) {
        kinls_mem->last_flag = SUNLS_ATIMES_FAIL_UNREC;
        return(-1);
      }
      *sJpnorm = N_VWL2Norm(bb, kin_mem->kin_fscale);
    }

    /* sFdotJp is the dot product of the scaled f vector and the scaled
       vector J*p, where the scaling uses fscale */
    if ((kin_mem->kin_inexact_ls && kin_mem->kin_etaflag == KIN_ETACHOICE1) ||
        kin_mem->kin_globalstrategy == KIN_LINESEARCH) {
      N_VProd(bb, kin_mem->kin_fscale, bb);
      N_VProd(bb, kin_mem->kin_fscale, bb);
      *sFdotJp = N_VDotProd(kin_mem->kin_fval, bb);
    }
  }

  if (kin_mem->kin_inexact_ls && kin_mem->kin_printfl > 2)
    KINPrintInfo(kin_mem, PRNT_EPS, "KINLS", "kinLsSolve",
                 INFO_EPS, res_norm, kin_mem->kin_eps);

  return(0);
}


/*------------------------------------------------------------------
  kinLsFree frees memory associated with the KINLs system
  solver interface
  ------------------------------------------------------------------*/
int kinLsFree(KINMem kin_mem)
{
  KINLsMem kinls_mem;

  /* Return immediately if kin_mem or kin_mem->kin_lmem are NULL */
  if (kin_mem == NULL) return (KINLS_SUCCESS);
  if (kin_mem->kin_lmem == NULL) return(KINLS_SUCCESS);
  kinls_mem = (KINLsMem) kin_mem->kin_lmem;

  /* Nullify SUNMatrix pointer */
  kinls_mem->J = NULL;

  /* Free preconditioner memory (if applicable) */
  if (kinls_mem->pfree) kinls_mem->pfree(kin_mem);

  /* free KINLs interface structure */
  free(kin_mem->kin_lmem);

  return(KINLS_SUCCESS);
}


/*------------------------------------------------------------------
  kinLsInitializeCounters resets counters for the LS interface
  ------------------------------------------------------------------*/
int kinLsInitializeCounters(KINLsMem kinls_mem)
{
  kinls_mem->nje     = 0;
  kinls_mem->nfeDQ   = 0;
  kinls_mem->npe     = 0;
  kinls_mem->nli     = 0;
  kinls_mem->nps     = 0;
  kinls_mem->ncfl    = 0;
  kinls_mem->njtimes = 0;
  return(0);
}


/*---------------------------------------------------------------
  kinLs_AccessLMem

  This routine unpacks the kin_mem and ls_mem structures from
  void* pointer.  If either is missing it returns KINLS_MEM_NULL
  or KINLS_LMEM_NULL.
  ---------------------------------------------------------------*/
int kinLs_AccessLMem(void* kinmem, const char *fname,
                     KINMem *kin_mem, KINLsMem *kinls_mem)
{
  if (kinmem==NULL) {
    KINProcessError(NULL, KINLS_MEM_NULL, "KINLS",
                    fname, MSG_LS_KINMEM_NULL);
    return(KINLS_MEM_NULL);
  }
  *kin_mem = (KINMem) kinmem;
  if ((*kin_mem)->kin_lmem==NULL) {
    KINProcessError(*kin_mem, KINLS_LMEM_NULL, "KINLS",
                    fname, MSG_LS_LMEM_NULL);
    return(KINLS_LMEM_NULL);
  }
  *kinls_mem = (KINLsMem) (*kin_mem)->kin_lmem;
  return(KINLS_SUCCESS);
}


/*---------------------------------------------------------------
  EOF
  ---------------------------------------------------------------*/
