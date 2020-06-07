/*-----------------------------------------------------------------
 * Programmer(s): Daniel R. Reynolds @ SMU
 *                Alan C. Hindmarsh and Radu Serban @ LLNL
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
 * Implementation file for IDA's linear solver interface.
 *-----------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ida_impl.h"
#include "ida_ls_impl.h"
#include <sundials/sundials_math.h>
#include <sundials/sundials_linearsolver.h>
#include <sunmatrix/sunmatrix_band.h>
#include <sunmatrix/sunmatrix_dense.h>
#include <sunmatrix/sunmatrix_sparse.h>

/* constants */
#define MAX_ITERS  3  /* max. number of attempts to recover in DQ J*v */
#define ZERO       RCONST(0.0)
#define PT25       RCONST(0.25)
#define PT05       RCONST(0.05)
#define PT9        RCONST(0.9)
#define ONE        RCONST(1.0)
#define TWO        RCONST(2.0)


/*===============================================================
  IDALS Exported functions -- Required
  ===============================================================*/

/*---------------------------------------------------------------
  IDASetLinearSolver specifies the linear solver
  ---------------------------------------------------------------*/
int IDASetLinearSolver(void *ida_mem, SUNLinearSolver LS, SUNMatrix A)
{
  IDAMem      IDA_mem;
  IDALsMem    idals_mem;
  int         retval, LSType;
  booleantype iterative;    /* is the solver iterative?    */
  booleantype matrixbased;  /* is a matrix structure used? */

  /* Return immediately if any input is NULL */
  if (ida_mem == NULL) {
    IDAProcessError(NULL, IDALS_MEM_NULL, "IDALS",
                    "IDASetLinearSolver", MSG_LS_IDAMEM_NULL);
    return(IDALS_MEM_NULL);
  }
  if (LS == NULL) {
    IDAProcessError(NULL, IDALS_ILL_INPUT, "IDALS",
                    "IDASetLinearSolver",
                    "LS must be non-NULL");
    return(IDALS_ILL_INPUT);
  }
  IDA_mem = (IDAMem) ida_mem;

  /* Test if solver is compatible with LS interface */
  if ( (LS->ops->gettype == NULL) || (LS->ops->solve == NULL) ) {
    IDAProcessError(IDA_mem, IDALS_ILL_INPUT, "IDALS",
                   "IDASetLinearSolver",
                   "LS object is missing a required operation");
    return(IDALS_ILL_INPUT);
  }

  /* Retrieve the LS type */
  LSType = SUNLinSolGetType(LS);

  /* Set flags based on LS type */
  iterative   = (LSType != SUNLINEARSOLVER_DIRECT);
  matrixbased = (LSType != SUNLINEARSOLVER_ITERATIVE);

  /* Test if vector is compatible with LS interface */
  if (IDA_mem->ida_tempv1->ops->nvconst == NULL ||
      IDA_mem->ida_tempv1->ops->nvwrmsnorm == NULL) {
    IDAProcessError(IDA_mem, IDALS_ILL_INPUT, "IDALS",
                    "IDASetLinearSolver", MSG_LS_BAD_NVECTOR);
    return(IDALS_ILL_INPUT);
  }

  /* Check for compatible LS type, matrix and "atimes" support */
  if (iterative) {

    if (IDA_mem->ida_tempv1->ops->nvgetlength == NULL) {
      IDAProcessError(IDA_mem, IDALS_ILL_INPUT, "IDALS",
                      "IDASetLinearSolver", MSG_LS_BAD_NVECTOR);
      return(IDALS_ILL_INPUT);
    }

    if (LS->ops->resid == NULL || LS->ops->numiters == NULL) {
      IDAProcessError(IDA_mem, IDALS_ILL_INPUT, "IDALS", "IDASetLinearSolver",
                      "Iterative LS object requires 'resid' and 'numiters' routines");
      return(IDALS_ILL_INPUT);
    }

    if (!matrixbased && LS->ops->setatimes == NULL) {
      IDAProcessError(IDA_mem, IDALS_ILL_INPUT, "IDALS", "IDASetLinearSolver",
                      "Incompatible inputs: iterative LS must support ATimes routine");
      return(IDALS_ILL_INPUT);
    }

    if (matrixbased && A == NULL) {
      IDAProcessError(IDA_mem, IDALS_ILL_INPUT, "IDALS", "IDASetLinearSolver",
                      "Incompatible inputs: matrix-iterative LS requires non-NULL matrix");
      return(IDALS_ILL_INPUT);
    }

  } else if (A == NULL) {

    IDAProcessError(IDA_mem, IDALS_ILL_INPUT, "IDALS", "IDASetLinearSolver",
                    "Incompatible inputs: direct LS requires non-NULL matrix");
    return(IDALS_ILL_INPUT);

  }

  /* free any existing system solver attached to IDA */
  if (IDA_mem->ida_lfree)  IDA_mem->ida_lfree(IDA_mem);

  /* Set four main system linear solver function fields in IDA_mem */
  IDA_mem->ida_linit  = idaLsInitialize;
  IDA_mem->ida_lsetup = idaLsSetup;
  IDA_mem->ida_lsolve = idaLsSolve;
  IDA_mem->ida_lfree  = idaLsFree;

  /* Set ida_lperf if using an iterative SUNLinearSolver object */
  IDA_mem->ida_lperf = (iterative) ? idaLsPerf : NULL;

  /* Allocate memory for IDALsMemRec */
  idals_mem = NULL;
  idals_mem = (IDALsMem) malloc(sizeof(struct IDALsMemRec));
  if (idals_mem == NULL) {
    IDAProcessError(IDA_mem, IDALS_MEM_FAIL, "IDALS",
                    "IDASetLinearSolver", MSG_LS_MEM_FAIL);
    return(IDALS_MEM_FAIL);
  }
  memset(idals_mem, 0, sizeof(struct IDALsMemRec));

  /* set SUNLinearSolver pointer */
  idals_mem->LS = LS;

  /* Linear solver type information */
  idals_mem->iterative   = iterative;
  idals_mem->matrixbased = matrixbased;

  /* Set defaults for Jacobian-related fields */
  idals_mem->J = A;
  if (A != NULL) {
    idals_mem->jacDQ     = SUNTRUE;
    idals_mem->jac       = idaLsDQJac;
    idals_mem->J_data    = IDA_mem;
  } else {
    idals_mem->jacDQ     = SUNFALSE;
    idals_mem->jac       = NULL;
    idals_mem->J_data    = NULL;
  }
  idals_mem->jtimesDQ = SUNTRUE;
  idals_mem->jtsetup  = NULL;
  idals_mem->jtimes   = idaLsDQJtimes;
  idals_mem->jt_data  = IDA_mem;

  /* Set defaults for preconditioner-related fields */
  idals_mem->pset   = NULL;
  idals_mem->psolve = NULL;
  idals_mem->pfree  = NULL;
  idals_mem->pdata  = IDA_mem->ida_user_data;

  /* Initialize counters */
  idaLsInitializeCounters(idals_mem);

  /* Set default values for the rest of the Ls parameters */
  idals_mem->eplifac   = PT05;
  idals_mem->dqincfac  = ONE;
  idals_mem->last_flag = IDALS_SUCCESS;

  /* If LS supports ATimes, attach IDALs routine */
  if (LS->ops->setatimes) {
    retval = SUNLinSolSetATimes(LS, IDA_mem, idaLsATimes);
    if (retval != SUNLS_SUCCESS) {
      IDAProcessError(IDA_mem, IDALS_SUNLS_FAIL, "IDALS",
                      "IDASetLinearSolver",
                      "Error in calling SUNLinSolSetATimes");
      free(idals_mem); idals_mem = NULL;
      return(IDALS_SUNLS_FAIL);
    }
  }

  /* If LS supports preconditioning, initialize pset/psol to NULL */
  if (LS->ops->setpreconditioner) {
    retval = SUNLinSolSetPreconditioner(LS, IDA_mem, NULL, NULL);
    if (retval != SUNLS_SUCCESS) {
      IDAProcessError(IDA_mem, IDALS_SUNLS_FAIL, "IDALS",
                      "IDASetLinearSolver",
                      "Error in calling SUNLinSolSetPreconditioner");
      free(idals_mem); idals_mem = NULL;
      return(IDALS_SUNLS_FAIL);
    }
  }

  /* Allocate memory for ytemp, yptemp and x */
  idals_mem->ytemp = N_VClone(IDA_mem->ida_tempv1);
  if (idals_mem->ytemp == NULL) {
    IDAProcessError(IDA_mem, IDALS_MEM_FAIL, "IDALS",
                    "IDASetLinearSolver", MSG_LS_MEM_FAIL);
    free(idals_mem); idals_mem = NULL;
    return(IDALS_MEM_FAIL);
  }

  idals_mem->yptemp = N_VClone(IDA_mem->ida_tempv1);
  if (idals_mem->yptemp == NULL) {
    IDAProcessError(IDA_mem, IDALS_MEM_FAIL, "IDALS",
                    "IDASetLinearSolver", MSG_LS_MEM_FAIL);
    N_VDestroy(idals_mem->ytemp);
    free(idals_mem); idals_mem = NULL;
    return(IDALS_MEM_FAIL);
  }

  idals_mem->x = N_VClone(IDA_mem->ida_tempv1);
  if (idals_mem->x == NULL) {
    IDAProcessError(IDA_mem, IDALS_MEM_FAIL, "IDALS",
                    "IDASetLinearSolver", MSG_LS_MEM_FAIL);
    N_VDestroy(idals_mem->ytemp);
    N_VDestroy(idals_mem->yptemp);
    free(idals_mem); idals_mem = NULL;
    return(IDALS_MEM_FAIL);
  }

  /* For iterative LS, compute sqrtN */
  if (iterative)
    idals_mem->sqrtN = SUNRsqrt( N_VGetLength(idals_mem->ytemp) );

  /* For matrix-based LS, enable solution scaling */
  if (matrixbased)
    idals_mem->scalesol = SUNTRUE;
  else
    idals_mem->scalesol = SUNFALSE;

  /* Attach linear solver memory to integrator memory */
  IDA_mem->ida_lmem = idals_mem;

  return(IDALS_SUCCESS);
}


/*===============================================================
  Optional input/output routines
  ===============================================================*/


/* IDASetJacFn specifies the Jacobian function */
int IDASetJacFn(void *ida_mem, IDALsJacFn jac)
{
  IDAMem   IDA_mem;
  IDALsMem idals_mem;
  int      retval;

  /* access IDALsMem structure */
  retval = idaLs_AccessLMem(ida_mem, "IDALsSetJacFn",
                            &IDA_mem, &idals_mem);
  if (retval != IDALS_SUCCESS)  return(retval);

  /* return with failure if jac cannot be used */
  if ((jac != NULL) && (idals_mem->J == NULL)) {
    IDAProcessError(IDA_mem, IDALS_ILL_INPUT, "IDALS", "IDASetJacFn",
                    "Jacobian routine cannot be supplied for NULL SUNMatrix");
    return(IDALS_ILL_INPUT);
  }

  /* set Jacobian routine pointer, and update relevant flags */
  if (jac != NULL) {
    idals_mem->jacDQ  = SUNFALSE;
    idals_mem->jac    = jac;
    idals_mem->J_data = IDA_mem->ida_user_data;
  } else {
    idals_mem->jacDQ  = SUNTRUE;
    idals_mem->jac    = idaLsDQJac;
    idals_mem->J_data = IDA_mem;
  }

  return(IDALS_SUCCESS);
}


/* IDASetEpsLin specifies the nonlinear -> linear tolerance scale factor */
int IDASetEpsLin(void *ida_mem, realtype eplifac)
{
  IDAMem   IDA_mem;
  IDALsMem idals_mem;
  int      retval;

  /* access IDALsMem structure */
  retval = idaLs_AccessLMem(ida_mem, "IDASetEpsLin",
                            &IDA_mem, &idals_mem);
  if (retval != IDALS_SUCCESS)  return(retval);

  /* Check for legal eplifac */
  if (eplifac < ZERO) {
    IDAProcessError(IDA_mem, IDALS_ILL_INPUT, "IDALS",
                    "IDASetEpsLin", MSG_LS_NEG_EPLIFAC);
    return(IDALS_ILL_INPUT);
  }

  idals_mem->eplifac = (eplifac == ZERO) ? PT05 : eplifac;

  return(IDALS_SUCCESS);
}


/* IDASetLinearSolutionScaling enables or disables scaling the linear solver
   solution to account for changes in cj. */
int IDASetLinearSolutionScaling(void *ida_mem, booleantype onoff)
{
  IDAMem   IDA_mem;
  IDALsMem idals_mem;
  int      retval;

  /* access IDALsMem structure */
  retval = idaLs_AccessLMem(ida_mem, "IDASetLinearSolutionScaling",
                            &IDA_mem, &idals_mem);
  if (retval != IDALS_SUCCESS) return(retval);

  /* check for valid solver type */
  if (!(idals_mem->matrixbased)) return(IDALS_ILL_INPUT);

  /* set solution scaling flag */
  idals_mem->scalesol = onoff;

  return(IDALS_SUCCESS);
}


/* IDASetIncrementFactor specifies increment factor for DQ approximations to Jv */
int IDASetIncrementFactor(void *ida_mem, realtype dqincfac)
{
  IDAMem   IDA_mem;
  IDALsMem idals_mem;
  int      retval;

  /* access IDALsMem structure */
  retval = idaLs_AccessLMem(ida_mem, "IDASetIncrementFactor",
                            &IDA_mem, &idals_mem);
  if (retval != IDALS_SUCCESS)  return(retval);

  /* Check for legal dqincfac */
  if (dqincfac <= ZERO) {
    IDAProcessError(IDA_mem, IDALS_ILL_INPUT, "IDALS",
                    "IDASetIncrementFactor", MSG_LS_NEG_DQINCFAC);
    return(IDALS_ILL_INPUT);
  }

  idals_mem->dqincfac = dqincfac;

  return(IDALS_SUCCESS);
}


/* IDASetPreconditioner specifies the user-supplied psetup and psolve routines */
int IDASetPreconditioner(void *ida_mem,
                         IDALsPrecSetupFn psetup,
                         IDALsPrecSolveFn psolve)
{
  IDAMem   IDA_mem;
  IDALsMem idals_mem;
  PSetupFn idals_psetup;
  PSolveFn idals_psolve;
  int      retval;

  /* access IDALsMem structure */
  retval = idaLs_AccessLMem(ida_mem, "IDASetPreconditioner",
                            &IDA_mem, &idals_mem);
  if (retval != IDALS_SUCCESS)  return(retval);

  /* store function pointers for user-supplied routines in IDALs interface */
  idals_mem->pset   = psetup;
  idals_mem->psolve = psolve;

  /* issue error if LS object does not allow user-supplied preconditioning */
  if (idals_mem->LS->ops->setpreconditioner == NULL) {
    IDAProcessError(IDA_mem, IDALS_ILL_INPUT, "IDALS",
                   "IDASetPreconditioner",
                   "SUNLinearSolver object does not support user-supplied preconditioning");
    return(IDALS_ILL_INPUT);
  }

  /* notify iterative linear solver to call IDALs interface routines */
  idals_psetup = (psetup == NULL) ? NULL : idaLsPSetup;
  idals_psolve = (psolve == NULL) ? NULL : idaLsPSolve;
  retval = SUNLinSolSetPreconditioner(idals_mem->LS, IDA_mem,
                                      idals_psetup, idals_psolve);
  if (retval != SUNLS_SUCCESS) {
    IDAProcessError(IDA_mem, IDALS_SUNLS_FAIL, "IDALS",
                    "IDASetPreconditioner",
                    "Error in calling SUNLinSolSetPreconditioner");
    return(IDALS_SUNLS_FAIL);
  }

  return(IDALS_SUCCESS);
}


/* IDASetJacTimes specifies the user-supplied Jacobian-vector product
   setup and multiply routines */
int IDASetJacTimes(void *ida_mem, IDALsJacTimesSetupFn jtsetup,
                   IDALsJacTimesVecFn jtimes)
{
  IDAMem   IDA_mem;
  IDALsMem idals_mem;
  int      retval;

  /* access IDALsMem structure */
  retval = idaLs_AccessLMem(ida_mem, "IDASetJacTimes",
                            &IDA_mem, &idals_mem);
  if (retval != IDALS_SUCCESS)  return(retval);

  /* issue error if LS object does not allow user-supplied ATimes */
  if (idals_mem->LS->ops->setatimes == NULL) {
    IDAProcessError(IDA_mem, IDALS_ILL_INPUT, "IDALS",
                    "IDASetJacTimes",
                    "SUNLinearSolver object does not support user-supplied ATimes routine");
    return(IDALS_ILL_INPUT);
  }

  /* store function pointers for user-supplied routines in IDALs
     interface (NULL jtimes implies use of DQ default) */
  if (jtimes != NULL) {
    idals_mem->jtimesDQ = SUNFALSE;
    idals_mem->jtsetup  = jtsetup;
    idals_mem->jtimes   = jtimes;
    idals_mem->jt_data  = IDA_mem->ida_user_data;
  } else {
    idals_mem->jtimesDQ = SUNTRUE;
    idals_mem->jtsetup  = NULL;
    idals_mem->jtimes   = idaLsDQJtimes;
    idals_mem->jt_data  = IDA_mem;
  }

  return(IDALS_SUCCESS);
}


/* IDAGetLinWorkSpace returns the length of workspace allocated
   for the IDALS linear solver interface */
int IDAGetLinWorkSpace(void *ida_mem, long int *lenrwLS,
                       long int *leniwLS)
{
  IDAMem       IDA_mem;
  IDALsMem     idals_mem;
  sunindextype lrw1, liw1;
  long int     lrw, liw;
  int          retval;

  /* access IDALsMem structure */
  retval = idaLs_AccessLMem(ida_mem, "IDAGetLinWorkSpace",
                            &IDA_mem, &idals_mem);
  if (retval != IDALS_SUCCESS)  return(retval);

  /* start with fixed sizes plus vector/matrix pointers */
  *lenrwLS = 3;
  *leniwLS = 33;

  /* add N_Vector sizes */
  if (IDA_mem->ida_tempv1->ops->nvspace) {
    N_VSpace(IDA_mem->ida_tempv1, &lrw1, &liw1);
    *lenrwLS += 3*lrw1;
    *leniwLS += 3*liw1;
  }

  /* add LS sizes */
  if (idals_mem->LS->ops->space) {
    retval = SUNLinSolSpace(idals_mem->LS, &lrw, &liw);
    if (retval == 0) {
      *lenrwLS += lrw;
      *leniwLS += liw;
    }
  }

  return(IDALS_SUCCESS);
}


/* IDAGetNumJacEvals returns the number of Jacobian evaluations */
int IDAGetNumJacEvals(void *ida_mem, long int *njevals)
{
  IDAMem   IDA_mem;
  IDALsMem idals_mem;
  int          retval;

  /* access IDALsMem structure; store output and return */
  retval = idaLs_AccessLMem(ida_mem, "IDAGetNumJacEvals",
                            &IDA_mem, &idals_mem);
  if (retval != IDALS_SUCCESS)  return(retval);
  *njevals = idals_mem->nje;
  return(IDALS_SUCCESS);
}


/* IDAGetNumPrecEvals returns the number of preconditioner evaluations */
int IDAGetNumPrecEvals(void *ida_mem, long int *npevals)
{
  IDAMem   IDA_mem;
  IDALsMem idals_mem;
  int      retval;

  /* access IDALsMem structure; store output and return */
  retval = idaLs_AccessLMem(ida_mem, "IDAGetNumPrecEvals",
                            &IDA_mem, &idals_mem);
  if (retval != IDALS_SUCCESS)  return(retval);
  *npevals = idals_mem->npe;
  return(IDALS_SUCCESS);
}


/* IDAGetNumPrecSolves returns the number of preconditioner solves */
int IDAGetNumPrecSolves(void *ida_mem, long int *npsolves)
{
  IDAMem   IDA_mem;
  IDALsMem idals_mem;
  int      retval;

  /* access IDALsMem structure; store output and return */
  retval = idaLs_AccessLMem(ida_mem, "IDAGetNumPrecSolves",
                            &IDA_mem, &idals_mem);
  if (retval != IDALS_SUCCESS)  return(retval);
  *npsolves = idals_mem->nps;
  return(IDALS_SUCCESS);
}


/* IDAGetNumLinIters returns the number of linear iterations */
int IDAGetNumLinIters(void *ida_mem, long int *nliters)
{
  IDAMem   IDA_mem;
  IDALsMem idals_mem;
  int      retval;

  /* access IDALsMem structure; store output and return */
  retval = idaLs_AccessLMem(ida_mem, "IDAGetNumLinIters",
                            &IDA_mem, &idals_mem);
  if (retval != IDALS_SUCCESS)  return(retval);
  *nliters = idals_mem->nli;
  return(IDALS_SUCCESS);
}


/* IDAGetNumLinConvFails returns the number of linear convergence failures */
int IDAGetNumLinConvFails(void *ida_mem, long int *nlcfails)
{
  IDAMem   IDA_mem;
  IDALsMem idals_mem;
  int      retval;

  /* access IDALsMem structure; store output and return */
  retval = idaLs_AccessLMem(ida_mem, "IDAGetNumLinConvFails",
                            &IDA_mem, &idals_mem);
  if (retval != IDALS_SUCCESS)  return(retval);
  *nlcfails = idals_mem->ncfl;
  return(IDALS_SUCCESS);
}


/* IDAGetNumJTSetupEvals returns the number of calls to the
   user-supplied Jacobian-vector product setup routine */
int IDAGetNumJTSetupEvals(void *ida_mem, long int *njtsetups)
{
  IDAMem   IDA_mem;
  IDALsMem idals_mem;
  int      retval;

  /* access IDALsMem structure; store output and return */
  retval = idaLs_AccessLMem(ida_mem, "IDAGetNumJTSetupEvals",
                            &IDA_mem, &idals_mem);
  if (retval != IDALS_SUCCESS)  return(retval);
  *njtsetups = idals_mem->njtsetup;
  return(IDALS_SUCCESS);
}


/* IDAGetNumJtimesEvals returns the number of calls to the
   Jacobian-vector product multiply routine */
int IDAGetNumJtimesEvals(void *ida_mem, long int *njvevals)
{
  IDAMem   IDA_mem;
  IDALsMem idals_mem;
  int      retval;

  /* access IDALsMem structure; store output and return */
  retval = idaLs_AccessLMem(ida_mem, "IDAGetNumJtimesEvals",
                            &IDA_mem, &idals_mem);
  if (retval != IDALS_SUCCESS)  return(retval);
  *njvevals = idals_mem->njtimes;
  return(IDALS_SUCCESS);
}


/* IDAGetNumLinResEvals returns the number of calls to the DAE
   residual needed for the DQ Jacobian approximation or J*v
   product approximation */
int IDAGetNumLinResEvals(void *ida_mem, long int *nrevalsLS)
{
  IDAMem   IDA_mem;
  IDALsMem idals_mem;
  int      retval;

  /* access IDALsMem structure; store output and return */
  retval = idaLs_AccessLMem(ida_mem, "IDAGetNumLinResEvals",
                            &IDA_mem, &idals_mem);
  if (retval != IDALS_SUCCESS)  return(retval);
  *nrevalsLS = idals_mem->nreDQ;
  return(IDALS_SUCCESS);
}


/* IDAGetLastLinFlag returns the last flag set in a IDALS function */
int IDAGetLastLinFlag(void *ida_mem, long int *flag)
{
  IDAMem   IDA_mem;
  IDALsMem idals_mem;
  int      retval;

  /* access IDALsMem structure; store output and return */
  retval = idaLs_AccessLMem(ida_mem, "IDAGetLastLinFlag",
                            &IDA_mem, &idals_mem);
  if (retval != IDALS_SUCCESS)  return(retval);
  *flag = idals_mem->last_flag;
  return(IDALS_SUCCESS);
}


/* IDAGetLinReturnFlagName translates from the integer error code
   returned by an IDALs routine to the corresponding string
   equivalent for that flag */
char *IDAGetLinReturnFlagName(long int flag)
{
  char *name = (char *)malloc(30*sizeof(char));

  switch(flag) {
  case IDALS_SUCCESS:
    sprintf(name,"IDALS_SUCCESS");
    break;
  case IDALS_MEM_NULL:
    sprintf(name,"IDALS_MEM_NULL");
    break;
  case IDALS_LMEM_NULL:
    sprintf(name,"IDALS_LMEM_NULL");
    break;
  case IDALS_ILL_INPUT:
    sprintf(name,"IDALS_ILL_INPUT");
    break;
  case IDALS_MEM_FAIL:
    sprintf(name,"IDALS_MEM_FAIL");
    break;
  case IDALS_PMEM_NULL:
    sprintf(name,"IDALS_PMEM_NULL");
    break;
  case IDALS_JACFUNC_UNRECVR:
    sprintf(name,"IDALS_JACFUNC_UNRECVR");
    break;
  case IDALS_JACFUNC_RECVR:
    sprintf(name,"IDALS_JACFUNC_RECVR");
    break;
  case IDALS_SUNMAT_FAIL:
    sprintf(name,"IDALS_SUNMAT_FAIL");
    break;
  case IDALS_SUNLS_FAIL:
    sprintf(name,"IDALS_SUNLS_FAIL");
    break;
  default:
    sprintf(name,"NONE");
  }

  return(name);
}


/*===============================================================
  IDALS Private functions
  ===============================================================*/

/*---------------------------------------------------------------
  idaLsATimes:

  This routine generates the matrix-vector product z = Jv, where
  J is the system Jacobian, by calling either the user provided
  routine or the internal DQ routine.  The return value is
  the same as the value returned by jtimes --
  0 if successful, nonzero otherwise.
  ---------------------------------------------------------------*/
int idaLsATimes(void *ida_mem, N_Vector v, N_Vector z)
{
  IDAMem   IDA_mem;
  IDALsMem idals_mem;
  int      retval;

  /* access IDALsMem structure */
  retval = idaLs_AccessLMem(ida_mem, "idaLsATimes",
                            &IDA_mem, &idals_mem);
  if (retval != IDALS_SUCCESS)  return(retval);

  /* call Jacobian-times-vector product routine
     (either user-supplied or internal DQ) */
  retval = idals_mem->jtimes(IDA_mem->ida_tn, idals_mem->ycur,
                             idals_mem->ypcur, idals_mem->rcur,
                             v, z, IDA_mem->ida_cj,
                             idals_mem->jt_data, idals_mem->ytemp,
                             idals_mem->yptemp);
  idals_mem->njtimes++;
  return(retval);
}


/*---------------------------------------------------------------
  idaLsPSetup:

  This routine interfaces between the generic iterative linear
  solvers and the user's psetup routine.  It passes to psetup all
  required state information from ida_mem.  Its return value
  is the same as that returned by psetup. Note that the generic
  iterative linear solvers guarantee that idaLsPSetup will only
  be called in the case that the user's psetup routine is non-NULL.
  ---------------------------------------------------------------*/
int idaLsPSetup(void *ida_mem)
{
  IDAMem   IDA_mem;
  IDALsMem idals_mem;
  int      retval;

  /* access IDALsMem structure */
  retval = idaLs_AccessLMem(ida_mem, "idaLsPSetup",
                            &IDA_mem, &idals_mem);
  if (retval != IDALS_SUCCESS)  return(retval);

  /* Call user pset routine to update preconditioner and possibly
     reset jcur (pass !jbad as update suggestion) */
  retval = idals_mem->pset(IDA_mem->ida_tn, idals_mem->ycur,
                           idals_mem->ypcur, idals_mem->rcur,
                           IDA_mem->ida_cj, idals_mem->pdata);
  idals_mem->npe++;
  return(retval);
}


/*---------------------------------------------------------------
  idaLsPSolve:

  This routine interfaces between the generic SUNLinSolSolve
  routine and the user's psolve routine.  It passes to psolve all
  required state information from ida_mem.  Its return value is
  the same as that returned by psolve.  Note that the generic
  SUNLinSol solver guarantees that IDASilsPSolve will not be
  called in the case in which preconditioning is not done. This
  is the only case in which the user's psolve routine is allowed
  to be NULL.
  ---------------------------------------------------------------*/
int idaLsPSolve(void *ida_mem, N_Vector r, N_Vector z, realtype tol, int lr)
{
  IDAMem   IDA_mem;
  IDALsMem idals_mem;
  int      retval;

  /* access IDALsMem structure */
  retval = idaLs_AccessLMem(ida_mem, "idaLsPSolve",
                            &IDA_mem, &idals_mem);
  if (retval != IDALS_SUCCESS)  return(retval);

  /* call the user-supplied psolve routine, and accumulate count */
  retval = idals_mem->psolve(IDA_mem->ida_tn, idals_mem->ycur,
                             idals_mem->ypcur, idals_mem->rcur,
                             r, z, IDA_mem->ida_cj, tol,
                             idals_mem->pdata);
  idals_mem->nps++;
  return(retval);
}


/*---------------------------------------------------------------
  idaLsDQJac:

  This routine is a wrapper for the Dense and Band
  implementations of the difference quotient Jacobian
  approximation routines.
---------------------------------------------------------------*/
int idaLsDQJac(realtype t, realtype c_j, N_Vector y, N_Vector yp,
               N_Vector r, SUNMatrix Jac, void *ida_mem,
               N_Vector tmp1, N_Vector tmp2, N_Vector tmp3)
{
  int    retval;
  IDAMem IDA_mem;
  IDA_mem = (IDAMem) ida_mem;

  /* access IDAMem structure */
  if (ida_mem == NULL) {
    IDAProcessError(NULL, IDALS_MEM_NULL, "IDALS",
                    "idaLsDQJac", MSG_LS_IDAMEM_NULL);
    return(IDALS_MEM_NULL);
  }

  /* verify that Jac is non-NULL */
  if (Jac == NULL) {
    IDAProcessError(IDA_mem, IDALS_LMEM_NULL, "IDALS",
                    "idaLsDQJac", MSG_LS_LMEM_NULL);
    return(IDALS_LMEM_NULL);
  }

  /* Verify that N_Vector supports required operations */
  if (IDA_mem->ida_tempv1->ops->nvcloneempty == NULL ||
      IDA_mem->ida_tempv1->ops->nvlinearsum == NULL ||
      IDA_mem->ida_tempv1->ops->nvdestroy == NULL ||
      IDA_mem->ida_tempv1->ops->nvscale == NULL ||
      IDA_mem->ida_tempv1->ops->nvgetarraypointer == NULL ||
      IDA_mem->ida_tempv1->ops->nvsetarraypointer == NULL) {
    IDAProcessError(IDA_mem, IDALS_ILL_INPUT, "IDALS",
                   "idaLsDQJac", MSG_LS_BAD_NVECTOR);
    return(IDALS_ILL_INPUT);
  }

  /* Call the matrix-structure-specific DQ approximation routine */
  if (SUNMatGetID(Jac) == SUNMATRIX_DENSE) {
    retval = idaLsDenseDQJac(t, c_j, y, yp, r, Jac, IDA_mem, tmp1);
  } else if (SUNMatGetID(Jac) == SUNMATRIX_BAND) {
    retval = idaLsBandDQJac(t, c_j, y, yp, r, Jac, IDA_mem, tmp1, tmp2, tmp3);
  } else {
    IDAProcessError(IDA_mem, IDA_ILL_INPUT, "IDALS",
                    "idaLsDQJac",
                    "unrecognized matrix type for idaLsDQJac");
    retval = IDA_ILL_INPUT;
  }
  return(retval);
}


/*---------------------------------------------------------------
  idaLsDenseDQJac

  This routine generates a dense difference quotient approximation
  to the Jacobian F_y + c_j*F_y'. It assumes a dense SUNmatrix
  input (stored column-wise, and that elements within each column
  are contiguous). The address of the jth column of J is obtained
  via the function SUNDenseMatrix_Column() and this pointer is
  associated with an N_Vector using the
  N_VGetArrayPointer/N_VSetArrayPointer functions.  Finally, the
  actual computation of the jth column of the Jacobian is
  done with a call to N_VLinearSum.
---------------------------------------------------------------*/
int idaLsDenseDQJac(realtype tt, realtype c_j, N_Vector yy,
                    N_Vector yp, N_Vector rr, SUNMatrix Jac,
                    IDAMem IDA_mem, N_Vector tmp1)
{
  realtype inc, inc_inv, yj, ypj, srur, conj;
  realtype *y_data, *yp_data, *ewt_data, *cns_data = NULL;
  N_Vector rtemp, jthCol;
  sunindextype j, N;
  IDALsMem idals_mem;
  int retval = 0;

  /* access LsMem interface structure */
  idals_mem = (IDALsMem) IDA_mem->ida_lmem;

  /* access matrix dimension */
  N = SUNDenseMatrix_Rows(Jac);

  /* Rename work vectors for readibility */
  rtemp = tmp1;

  /* Create an empty vector for matrix column calculations */
  jthCol = N_VCloneEmpty(tmp1);

  /* Obtain pointers to the data for ewt, yy, yp. */
  ewt_data = N_VGetArrayPointer(IDA_mem->ida_ewt);
  y_data   = N_VGetArrayPointer(yy);
  yp_data  = N_VGetArrayPointer(yp);
  if(IDA_mem->ida_constraintsSet)
    cns_data = N_VGetArrayPointer(IDA_mem->ida_constraints);

  srur = SUNRsqrt(IDA_mem->ida_uround);

  for (j=0; j < N; j++) {

    /* Generate the jth col of J(tt,yy,yp) as delta(F)/delta(y_j). */

    /* Set data address of jthCol, and save y_j and yp_j values. */
    N_VSetArrayPointer(SUNDenseMatrix_Column(Jac,j), jthCol);
    yj = y_data[j];
    ypj = yp_data[j];

    /* Set increment inc to y_j based on sqrt(uround)*abs(y_j), with
    adjustments using yp_j and ewt_j if this is small, and a further
    adjustment to give it the same sign as hh*yp_j. */

    inc = SUNMAX( srur * SUNMAX( SUNRabs(yj), SUNRabs(IDA_mem->ida_hh*ypj) ),
                  ONE/ewt_data[j] );

    if (IDA_mem->ida_hh*ypj < ZERO) inc = -inc;
    inc = (yj + inc) - yj;

    /* Adjust sign(inc) again if y_j has an inequality constraint. */
    if (IDA_mem->ida_constraintsSet) {
      conj = cns_data[j];
      if (SUNRabs(conj) == ONE)      {if((yj+inc)*conj <  ZERO) inc = -inc;}
      else if (SUNRabs(conj) == TWO) {if((yj+inc)*conj <= ZERO) inc = -inc;}
    }

    /* Increment y_j and yp_j, call res, and break on error return. */
    y_data[j] += inc;
    yp_data[j] += c_j*inc;

    retval = IDA_mem->ida_res(tt, yy, yp, rtemp, IDA_mem->ida_user_data);
    idals_mem->nreDQ++;
    if (retval != 0) break;

    /* Construct difference quotient in jthCol */
    inc_inv = ONE/inc;
    N_VLinearSum(inc_inv, rtemp, -inc_inv, rr, jthCol);

    /*  reset y_j, yp_j */
    y_data[j] = yj;
    yp_data[j] = ypj;
  }

  /* Destroy jthCol vector */
  N_VSetArrayPointer(NULL, jthCol);  /* SHOULDN'T BE NEEDED */
  N_VDestroy(jthCol);

  return(retval);
}


/*---------------------------------------------------------------
  idaLsBandDQJac

  This routine generates a banded difference quotient approximation
  JJ to the DAE system Jacobian J.  It assumes a band SUNMatrix
  input (stored column-wise, and that elements within each column
  are contiguous).  This makes it possible to get the address
  of a column of JJ via the function SUNBandMatrix_Column(). The
  columns of the Jacobian are constructed using mupper + mlower + 1
  calls to the res routine, and appropriate differencing.
  The return value is either IDABAND_SUCCESS = 0, or the nonzero
  value returned by the res routine, if any.
  ---------------------------------------------------------------*/
int idaLsBandDQJac(realtype tt, realtype c_j, N_Vector yy,
                   N_Vector yp, N_Vector rr, SUNMatrix Jac,
                   IDAMem IDA_mem, N_Vector tmp1, N_Vector tmp2,
                   N_Vector tmp3)
{
  realtype inc, inc_inv, yj, ypj, srur, conj, ewtj;
  realtype *y_data, *yp_data, *ewt_data, *cns_data = NULL;
  realtype *ytemp_data, *yptemp_data, *rtemp_data, *r_data, *col_j;
  N_Vector rtemp, ytemp, yptemp;
  sunindextype i, j, i1, i2, width, ngroups, group;
  sunindextype N, mupper, mlower;
  IDALsMem idals_mem;
  int retval = 0;

  /* access LsMem interface structure */
  idals_mem = (IDALsMem) IDA_mem->ida_lmem;

  /* access matrix dimensions */
  N = SUNBandMatrix_Columns(Jac);
  mupper = SUNBandMatrix_UpperBandwidth(Jac);
  mlower = SUNBandMatrix_LowerBandwidth(Jac);

  /* Rename work vectors for use as temporary values of r, y and yp */
  rtemp = tmp1;
  ytemp = tmp2;
  yptemp= tmp3;

  /* Obtain pointers to the data for all eight vectors used.  */
  ewt_data    = N_VGetArrayPointer(IDA_mem->ida_ewt);
  r_data      = N_VGetArrayPointer(rr);
  y_data      = N_VGetArrayPointer(yy);
  yp_data     = N_VGetArrayPointer(yp);
  rtemp_data  = N_VGetArrayPointer(rtemp);
  ytemp_data  = N_VGetArrayPointer(ytemp);
  yptemp_data = N_VGetArrayPointer(yptemp);
  if (IDA_mem->ida_constraintsSet)
    cns_data = N_VGetArrayPointer(IDA_mem->ida_constraints);

  /* Initialize ytemp and yptemp. */
  N_VScale(ONE, yy, ytemp);
  N_VScale(ONE, yp, yptemp);

  /* Compute miscellaneous values for the Jacobian computation. */
  srur = SUNRsqrt(IDA_mem->ida_uround);
  width = mlower + mupper + 1;
  ngroups = SUNMIN(width, N);

  /* Loop over column groups. */
  for (group=1; group <= ngroups; group++) {

    /* Increment all yy[j] and yp[j] for j in this group. */
    for (j=group-1; j<N; j+=width) {
        yj = y_data[j];
        ypj = yp_data[j];
        ewtj = ewt_data[j];

        /* Set increment inc to yj based on sqrt(uround)*abs(yj), with
        adjustments using ypj and ewtj if this is small, and a further
        adjustment to give it the same sign as hh*ypj. */
        inc = SUNMAX( srur * SUNMAX( SUNRabs(yj), SUNRabs(IDA_mem->ida_hh*ypj) ),
                      ONE/ewtj );
        if (IDA_mem->ida_hh*ypj < ZERO)  inc = -inc;
        inc = (yj + inc) - yj;

        /* Adjust sign(inc) again if yj has an inequality constraint. */
        if (IDA_mem->ida_constraintsSet) {
          conj = cns_data[j];
          if (SUNRabs(conj) == ONE)      {if((yj+inc)*conj <  ZERO) inc = -inc;}
          else if (SUNRabs(conj) == TWO) {if((yj+inc)*conj <= ZERO) inc = -inc;}
        }

        /* Increment yj and ypj. */
        ytemp_data[j] += inc;
        yptemp_data[j] += IDA_mem->ida_cj*inc;
    }

    /* Call res routine with incremented arguments. */
    retval = IDA_mem->ida_res(tt, ytemp, yptemp, rtemp, IDA_mem->ida_user_data);
    idals_mem->nreDQ++;
    if (retval != 0) break;

    /* Loop over the indices j in this group again. */
    for (j=group-1; j<N; j+=width) {

      /* Reset ytemp and yptemp components that were perturbed. */
      yj = ytemp_data[j]  = y_data[j];
      ypj = yptemp_data[j] = yp_data[j];
      col_j = SUNBandMatrix_Column(Jac, j);
      ewtj = ewt_data[j];

      /* Set increment inc exactly as above. */
      inc = SUNMAX( srur * SUNMAX( SUNRabs(yj), SUNRabs(IDA_mem->ida_hh*ypj) ),
                    ONE/ewtj );
      if (IDA_mem->ida_hh*ypj < ZERO)  inc = -inc;
      inc = (yj + inc) - yj;
      if (IDA_mem->ida_constraintsSet) {
        conj = cns_data[j];
        if (SUNRabs(conj) == ONE)      {if((yj+inc)*conj <  ZERO) inc = -inc;}
        else if (SUNRabs(conj) == TWO) {if((yj+inc)*conj <= ZERO) inc = -inc;}
      }

      /* Load the difference quotient Jacobian elements for column j */
      inc_inv = ONE/inc;
      i1 = SUNMAX(0, j-mupper);
      i2 = SUNMIN(j+mlower,N-1);
      for (i=i1; i<=i2; i++)
        SM_COLUMN_ELEMENT_B(col_j,i,j) = inc_inv * (rtemp_data[i]-r_data[i]);
    }
  }

  return(retval);
}


/*---------------------------------------------------------------
  idaLsDQJtimes

  This routine generates a difference quotient approximation to
  the matrix-vector product z = Jv, where J is the system
  Jacobian. The approximation is
       Jv = [F(t,y1,yp1) - F(t,y,yp)]/sigma,
  where
       y1 = y + sigma*v,  yp1 = yp + cj*sigma*v,
       sigma = sqrt(Neq)*dqincfac.
  The return value from the call to res is saved in order to set
  the return flag from idaLsSolve.
  ---------------------------------------------------------------*/
int idaLsDQJtimes(realtype tt, N_Vector yy, N_Vector yp, N_Vector rr,
                  N_Vector v, N_Vector Jv, realtype c_j,
                  void *ida_mem, N_Vector work1, N_Vector work2)
{
  IDAMem   IDA_mem;
  IDALsMem idals_mem;
  N_Vector y_tmp, yp_tmp;
  realtype sig, siginv;
  int      iter, retval;
  SUNLinearSolver_ID LSID;

  /* access IDALsMem structure */
  retval = idaLs_AccessLMem(ida_mem, "idaLsDQJtimes",
                            &IDA_mem, &idals_mem);
  if (retval != IDALS_SUCCESS)  return(retval);

  LSID = SUNLinSolGetID(idals_mem->LS);
  if (LSID == SUNLINEARSOLVER_SPGMR || LSID == SUNLINEARSOLVER_SPFGMR)
    sig = idals_mem->sqrtN * idals_mem->dqincfac;
  else
    sig = idals_mem->dqincfac / N_VWrmsNorm(v, IDA_mem->ida_ewt);

  /* Rename work1 and work2 for readibility */
  y_tmp  = work1;
  yp_tmp = work2;

  for (iter=0; iter<MAX_ITERS; iter++) {

    /* Set y_tmp = yy + sig*v, yp_tmp = yp + cj*sig*v. */
    N_VLinearSum(sig, v, ONE, yy, y_tmp);
    N_VLinearSum(c_j*sig, v, ONE, yp, yp_tmp);

    /* Call res for Jv = F(t, y_tmp, yp_tmp), and return if it failed. */
    retval = IDA_mem->ida_res(tt, y_tmp, yp_tmp, Jv, IDA_mem->ida_user_data);
    idals_mem->nreDQ++;
    if (retval == 0) break;
    if (retval < 0)  return(-1);

    sig *= PT25;
  }

  if (retval > 0) return(+1);

  /* Set Jv to [Jv - rr]/sig and return. */
  siginv = ONE/sig;
  N_VLinearSum(siginv, Jv, -siginv, rr, Jv);

  return(0);
}


/*---------------------------------------------------------------
 idaLsInitialize

 This routine performs remaining initializations specific
 to the iterative linear solver interface (and solver itself)
---------------------------------------------------------------*/
int idaLsInitialize(IDAMem IDA_mem)
{
  IDALsMem idals_mem;
  int      retval;

  /* access IDALsMem structure */
  if (IDA_mem->ida_lmem == NULL) {
    IDAProcessError(IDA_mem, IDALS_LMEM_NULL, "IDALS",
                    "idaLsInitialize", MSG_LS_LMEM_NULL);
    return(IDALS_LMEM_NULL);
  }
  idals_mem = (IDALsMem) IDA_mem->ida_lmem;


  /* Test for valid combinations of matrix & Jacobian routines: */
  if (idals_mem->J == NULL) {

    /* If SUNMatrix A is NULL: ensure 'jac' function pointer is NULL */
    idals_mem->jacDQ  = SUNFALSE;
    idals_mem->jac    = NULL;
    idals_mem->J_data = NULL;

  } else if (idals_mem->jacDQ) {

    /* If J is non-NULL, and 'jac' is not user-supplied:
       - if J is dense or band, ensure that our DQ approx. is used
       - otherwise => error */
    retval = 0;
    if (idals_mem->J->ops->getid) {

      if ( (SUNMatGetID(idals_mem->J) == SUNMATRIX_DENSE) ||
           (SUNMatGetID(idals_mem->J) == SUNMATRIX_BAND) ) {
        idals_mem->jac    = idaLsDQJac;
        idals_mem->J_data = IDA_mem;
      } else {
        retval++;
      }

    } else {
      retval++;
    }
    if (retval) {
      IDAProcessError(IDA_mem, IDALS_ILL_INPUT, "IDALS", "idaLsInitialize",
                     "No Jacobian constructor available for SUNMatrix type");
      idals_mem->last_flag = IDALS_ILL_INPUT;
      return(IDALS_ILL_INPUT);
    }

  } else {

    /* If J is non-NULL, and 'jac' is user-supplied,
       reset J_data pointer (just in case) */
    idals_mem->J_data = IDA_mem->ida_user_data;
  }

  /* reset counters */
  idaLsInitializeCounters(idals_mem);

  /* Set Jacobian-related fields, based on jtimesDQ */
  if (idals_mem->jtimesDQ) {
    idals_mem->jtsetup = NULL;
    idals_mem->jtimes  = idaLsDQJtimes;
    idals_mem->jt_data = IDA_mem;
  } else {
    idals_mem->jt_data = IDA_mem->ida_user_data;
  }

  /* if J is NULL and psetup is not present, then idaLsSetup does
     not need to be called, so set the lsetup function to NULL */
  if ( (idals_mem->J == NULL) && (idals_mem->pset == NULL) )
    IDA_mem->ida_lsetup = NULL;

  /* Call LS initialize routine */
  idals_mem->last_flag = SUNLinSolInitialize(idals_mem->LS);
  return(idals_mem->last_flag);
}


/*---------------------------------------------------------------
 idaLsSetup

 This calls the Jacobian evaluation routine (if using a SUNMatrix
 object), updates counters, and calls the LS 'setup' routine to
 prepare for subsequent calls to the LS 'solve' routine.
---------------------------------------------------------------*/
int idaLsSetup(IDAMem IDA_mem, N_Vector y, N_Vector yp, N_Vector r,
               N_Vector vt1, N_Vector vt2, N_Vector vt3)
{
  IDALsMem idals_mem;
  int      retval;

  /* access IDALsMem structure */
  if (IDA_mem->ida_lmem == NULL) {
    IDAProcessError(IDA_mem, IDALS_LMEM_NULL, "IDALS",
                    "idaLsSetup", MSG_LS_LMEM_NULL);
    return(IDALS_LMEM_NULL);
  }
  idals_mem = (IDALsMem) IDA_mem->ida_lmem;

  /* Set IDALs N_Vector pointers to inputs */
  idals_mem->ycur  = y;
  idals_mem->ypcur = yp;
  idals_mem->rcur  = r;

  /* recompute if J if it is non-NULL */
  if (idals_mem->J) {

    /* Increment nje counter. */
    idals_mem->nje++;

    /* Clear the linear system matrix if necessary */
    if (SUNLinSolGetType(idals_mem->LS) == SUNLINEARSOLVER_DIRECT) {
      retval = SUNMatZero(idals_mem->J);
      if (retval != 0) {
        IDAProcessError(IDA_mem, IDALS_SUNMAT_FAIL, "IDALS",
                        "idaLsSetup", MSG_LS_MATZERO_FAILED);
        idals_mem->last_flag = IDALS_SUNMAT_FAIL;
        return(idals_mem->last_flag);
      }
    }

    /* Call Jacobian routine */
    retval = idals_mem->jac(IDA_mem->ida_tn, IDA_mem->ida_cj, y,
                            yp, r, idals_mem->J,
                            idals_mem->J_data, vt1, vt2, vt3);
    if (retval < 0) {
      IDAProcessError(IDA_mem, IDALS_JACFUNC_UNRECVR, "IDALS",
                      "idaLsSetup", MSG_LS_JACFUNC_FAILED);
      idals_mem->last_flag = IDALS_JACFUNC_UNRECVR;
      return(-1);
    }
    if (retval > 0) {
      idals_mem->last_flag = IDALS_JACFUNC_RECVR;
      return(1);
    }

  }

  /* Call LS setup routine -- the LS will call idaLsPSetup if applicable */
  idals_mem->last_flag = SUNLinSolSetup(idals_mem->LS, idals_mem->J);
  return(idals_mem->last_flag);
}


/*---------------------------------------------------------------
 idaLsSolve

 This routine interfaces between IDA and the generic
 SUNLinearSolver object LS, by setting the appropriate tolerance
 and scaling vectors, calling the solver, accumulating
 statistics from the solve for use/reporting by IDA, and scaling
 the result if using a non-NULL SUNMatrix and cjratio does not
 equal one.
---------------------------------------------------------------*/
int idaLsSolve(IDAMem IDA_mem, N_Vector b, N_Vector weight,
               N_Vector ycur, N_Vector ypcur, N_Vector rescur)
{
  IDALsMem idals_mem;
  int      nli_inc, retval;
  realtype tol, w_mean;

  /* access IDALsMem structure */
  if (IDA_mem->ida_lmem == NULL) {
    IDAProcessError(IDA_mem, IDALS_LMEM_NULL, "IDALS",
                    "idaLsSolve", MSG_LS_LMEM_NULL);
    return(IDALS_LMEM_NULL);
  }
  idals_mem = (IDALsMem) IDA_mem->ida_lmem;

  /* If the linear solver is iterative: set convergence test constant tol,
     in terms of the Newton convergence test constant epsNewt and safety
     factors. The factor sqrt(Neq) assures that the convergence test is
     applied to the WRMS norm of the residual vector, rather than the
     weighted L2 norm. */
  if (idals_mem->iterative) {
    tol = idals_mem->sqrtN * idals_mem->eplifac * IDA_mem->ida_epsNewt;
  } else {
    tol = ZERO;
  }

  /* Set vectors ycur, ypcur and rcur for use by the Atimes and
     Psolve interface routines */
  idals_mem->ycur  = ycur;
  idals_mem->ypcur = ypcur;
  idals_mem->rcur  = rescur;

  /* Set scaling vectors for LS to use (if applicable) */
  if (idals_mem->LS->ops->setscalingvectors) {
    retval = SUNLinSolSetScalingVectors(idals_mem->LS, weight, weight);
    if (retval != SUNLS_SUCCESS) {
      IDAProcessError(IDA_mem, IDALS_SUNLS_FAIL, "IDALS", "idaLsSolve",
                      "Error in calling SUNLinSolSetScalingVectors");
      idals_mem->last_flag = IDALS_SUNLS_FAIL;
      return(idals_mem->last_flag);
    }

  /* If solver is iterative and does not support scaling vectors, update the
     tolerance in an attempt to account for weight vector.  We make the
     following assumptions:
       1. w_i = w_mean, for i=0,...,n-1 (i.e. the weights are homogeneous)
       2. the linear solver uses a basic 2-norm to measure convergence
     Hence (using the notation from sunlinsol_spgmr.h, with S = diag(w)),
           || bbar - Abar xbar ||_2 < tol
       <=> || S b - S A x ||_2 < tol
       <=> || S (b - A x) ||_2 < tol
       <=> \sum_{i=0}^{n-1} (w_i (b - A x)_i)^2 < tol^2
       <=> w_mean^2 \sum_{i=0}^{n-1} (b - A x_i)^2 < tol^2
       <=> \sum_{i=0}^{n-1} (b - A x_i)^2 < tol^2 / w_mean^2
       <=> || b - A x ||_2 < tol / w_mean
     So we compute w_mean = ||w||_RMS and scale the desired tolerance accordingly. */
  } else if (idals_mem->iterative) {

    N_VConst(ONE, idals_mem->x);
    w_mean = N_VWrmsNorm(weight, idals_mem->x);
    tol /= w_mean;

  }

  /* Set initial guess x = 0 to LS */
  N_VConst(ZERO, idals_mem->x);

  /* If a user-provided jtsetup routine is supplied, call that here */
  if (idals_mem->jtsetup) {
    idals_mem->last_flag = idals_mem->jtsetup(IDA_mem->ida_tn, ycur, ypcur, rescur,
                                              IDA_mem->ida_cj, idals_mem->jt_data);
    idals_mem->njtsetup++;
    if (idals_mem->last_flag != 0) {
      IDAProcessError(IDA_mem, retval, "IDALS",
                      "idaLsSolve", MSG_LS_JTSETUP_FAILED);
      return(idals_mem->last_flag);
    }
  }

  /* Call solver */
  retval = SUNLinSolSolve(idals_mem->LS, idals_mem->J,
                          idals_mem->x, b, tol);

  /* Copy appropriate result to b (depending on solver type) */
  if (idals_mem->iterative) {

    /* Retrieve solver statistics */
    nli_inc = SUNLinSolNumIters(idals_mem->LS);

    /* Copy x (or preconditioned residual vector if no iterations required) to b */
    if (nli_inc == 0) N_VScale(ONE, SUNLinSolResid(idals_mem->LS), b);
    else N_VScale(ONE, idals_mem->x, b);

    /* Increment nli counter */
    idals_mem->nli += nli_inc;

  } else {

    /* Copy x to b */
    N_VScale(ONE, idals_mem->x, b);

  }

  /* If using a direct or matrix-iterative solver, scale the correction to
     account for change in cj */
  if (idals_mem->scalesol && (IDA_mem->ida_cjratio != ONE))
    N_VScale(TWO/(ONE + IDA_mem->ida_cjratio), b, b);

  /* Increment ncfl counter */
  if (retval != SUNLS_SUCCESS) idals_mem->ncfl++;

  /* Interpret solver return value  */
  idals_mem->last_flag = retval;

  switch(retval) {

  case SUNLS_SUCCESS:
    return(0);
    break;
  case SUNLS_RES_REDUCED:
  case SUNLS_CONV_FAIL:
  case SUNLS_PSOLVE_FAIL_REC:
  case SUNLS_PACKAGE_FAIL_REC:
  case SUNLS_QRFACT_FAIL:
  case SUNLS_LUFACT_FAIL:
    return(1);
    break;
  case SUNLS_MEM_NULL:
  case SUNLS_ILL_INPUT:
  case SUNLS_MEM_FAIL:
  case SUNLS_GS_FAIL:
  case SUNLS_QRSOL_FAIL:
    return(-1);
    break;
  case SUNLS_PACKAGE_FAIL_UNREC:
    IDAProcessError(IDA_mem, SUNLS_PACKAGE_FAIL_UNREC, "IDALS",
                    "idaLsSolve",
                    "Failure in SUNLinSol external package");
    return(-1);
    break;
  case SUNLS_PSOLVE_FAIL_UNREC:
    IDAProcessError(IDA_mem, SUNLS_PSOLVE_FAIL_UNREC, "IDALS",
                    "idaLsSolve", MSG_LS_PSOLVE_FAILED);
    return(-1);
    break;
  }

  return(0);
}


/*---------------------------------------------------------------
 idaLsPerf: accumulates performance statistics information
 for IDA
---------------------------------------------------------------*/
int idaLsPerf(IDAMem IDA_mem, int perftask)
{
  IDALsMem    idals_mem;
  realtype    rcfn, rcfl;
  long int    nstd, nnid;
  booleantype lcfn, lcfl;

  /* access IDALsMem structure */
  if (IDA_mem->ida_lmem == NULL) {
    IDAProcessError(IDA_mem, IDALS_LMEM_NULL, "IDALS",
                    "idaLsPerf", MSG_LS_LMEM_NULL);
    return(IDALS_LMEM_NULL);
  }
  idals_mem = (IDALsMem) IDA_mem->ida_lmem;

  /* when perftask == 0, store current performance statistics */
  if (perftask == 0) {
    idals_mem->nst0  = IDA_mem->ida_nst;
    idals_mem->nni0  = IDA_mem->ida_nni;
    idals_mem->ncfn0 = IDA_mem->ida_ncfn;
    idals_mem->ncfl0 = idals_mem->ncfl;
    idals_mem->nwarn = 0;
    return(0);
  }

  /* Compute statistics since last call

     Note: the performance monitor that checked whether the average
       number of linear iterations was too close to maxl has been
       removed, since the 'maxl' value is no longer owned by the
       IDALs interface.
   */
  nstd = IDA_mem->ida_nst - idals_mem->nst0;
  nnid = IDA_mem->ida_nni - idals_mem->nni0;
  if (nstd == 0 || nnid == 0) return(0);

  rcfn = ((realtype) (IDA_mem->ida_ncfn - idals_mem->ncfn0)) / ((realtype) nstd);
  rcfl = ((realtype) (idals_mem->ncfl - idals_mem->ncfl0)) / ((realtype) nnid);
  lcfn = (rcfn > PT9);
  lcfl = (rcfl > PT9);
  if (!(lcfn || lcfl)) return(0);
  idals_mem->nwarn++;
  if (idals_mem->nwarn > 10) return(1);
  if (lcfn)
    IDAProcessError(IDA_mem, IDA_WARNING, "IDALS", "idaLsPerf",
                    MSG_LS_CFN_WARN, IDA_mem->ida_tn, rcfn);
  if (lcfl)
    IDAProcessError(IDA_mem, IDA_WARNING, "IDALS", "idaLsPerf",
                    MSG_LS_CFL_WARN, IDA_mem->ida_tn, rcfl);
  return(0);
}


/*---------------------------------------------------------------
 idaLsFree frees memory associates with the IDALs system
 solver interface.
---------------------------------------------------------------*/
int idaLsFree(IDAMem IDA_mem)
{
  IDALsMem idals_mem;

  /* Return immediately if IDA_mem or IDA_mem->ida_lmem are NULL */
  if (IDA_mem == NULL)  return (IDALS_SUCCESS);
  if (IDA_mem->ida_lmem == NULL)  return(IDALS_SUCCESS);
  idals_mem = (IDALsMem) IDA_mem->ida_lmem;

  /* Free N_Vector memory */
  if (idals_mem->ytemp) {
    N_VDestroy(idals_mem->ytemp);
    idals_mem->ytemp = NULL;
  }
  if (idals_mem->yptemp) {
    N_VDestroy(idals_mem->yptemp);
    idals_mem->yptemp = NULL;
  }
  if (idals_mem->x) {
    N_VDestroy(idals_mem->x);
    idals_mem->x = NULL;
  }

  /* Nullify other N_Vector pointers */
  idals_mem->ycur  = NULL;
  idals_mem->ypcur = NULL;
  idals_mem->rcur  = NULL;

  /* Nullify SUNMatrix pointer */
  idals_mem->J = NULL;

  /* Free preconditioner memory (if applicable) */
  if (idals_mem->pfree)  idals_mem->pfree(IDA_mem);

  /* free IDALs interface structure */
  free(IDA_mem->ida_lmem);

  return(IDALS_SUCCESS);
}


/*---------------------------------------------------------------
 idaLsInitializeCounters resets all counters from an
 IDALsMem structure.
---------------------------------------------------------------*/
int idaLsInitializeCounters(IDALsMem idals_mem)
{
  idals_mem->nje      = 0;
  idals_mem->nreDQ    = 0;
  idals_mem->npe      = 0;
  idals_mem->nli      = 0;
  idals_mem->nps      = 0;
  idals_mem->ncfl     = 0;
  idals_mem->njtsetup = 0;
  idals_mem->njtimes  = 0;
  return(0);
}


/*---------------------------------------------------------------
  idaLs_AccessLMem

  This routine unpacks the IDA_mem and idals_mem structures from
  the void* ida_mem pointer.  If either is missing it returns
  IDALS_MEM_NULL or IDALS_LMEM_NULL.
  ---------------------------------------------------------------*/
int idaLs_AccessLMem(void* ida_mem, const char* fname,
                     IDAMem* IDA_mem, IDALsMem* idals_mem)
{
  if (ida_mem==NULL) {
    IDAProcessError(NULL, IDALS_MEM_NULL, "IDALS",
                    fname, MSG_LS_IDAMEM_NULL);
    return(IDALS_MEM_NULL);
  }
  *IDA_mem = (IDAMem) ida_mem;
  if ((*IDA_mem)->ida_lmem==NULL) {
    IDAProcessError(*IDA_mem, IDALS_LMEM_NULL, "IDALS",
                   fname, MSG_LS_LMEM_NULL);
    return(IDALS_LMEM_NULL);
  }
  *idals_mem = (IDALsMem) (*IDA_mem)->ida_lmem;
  return(IDALS_SUCCESS);
}


/*---------------------------------------------------------------
  EOF
  ---------------------------------------------------------------*/
