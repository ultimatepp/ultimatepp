/* -----------------------------------------------------------------
 * Programmer(s): David J. Gardner @ LLNL
 *                Allan Taylor, Alan Hindmarsh, Radu Serban, and
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
 * This file contains implementations of routines for a
 * band-block-diagonal preconditioner, i.e. a block-diagonal
 * matrix with banded blocks, for use with KINSol and the 
 * KINLS linear solver interface.
 *
 * Note: With only one process, a banded matrix results
 * rather than a b-b-d matrix with banded blocks. Diagonal
 * blocking occurs at the process level.
 * -----------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

#include "kinsol_impl.h"
#include "kinsol_ls_impl.h"
#include "kinsol_bbdpre_impl.h"

#include <sundials/sundials_math.h>
#include <nvector/nvector_serial.h>

#define ZERO RCONST(0.0)
#define ONE  RCONST(1.0)

/* Prototypes of functions KINBBDPrecSetup and KINBBDPrecSolve */
static int KINBBDPrecSetup(N_Vector uu, N_Vector uscale,
                           N_Vector fval, N_Vector fscale, 
                           void *pdata);

static int KINBBDPrecSolve(N_Vector uu, N_Vector uscale,
                           N_Vector fval, N_Vector fscale, 
                           N_Vector vv, void *pdata);

/* Prototype for KINBBDPrecFree */
static int KINBBDPrecFree(KINMem kin_mem);

/* Prototype for difference quotient jacobian calculation routine */
static int KBBDDQJac(KBBDPrecData pdata,
                     N_Vector uu, N_Vector uscale,
                     N_Vector gu, N_Vector gtemp, N_Vector utemp);

/*------------------------------------------------------------------
  user-callable functions
  ------------------------------------------------------------------*/

/*------------------------------------------------------------------
  KINBBDPrecInit
  ------------------------------------------------------------------*/
int KINBBDPrecInit(void *kinmem, sunindextype Nlocal, 
                   sunindextype mudq, sunindextype mldq,
                   sunindextype mukeep, sunindextype mlkeep,
                   realtype dq_rel_uu, 
                   KINBBDLocalFn gloc, KINBBDCommFn gcomm)
{
  KINMem kin_mem;
  KINLsMem kinls_mem;
  KBBDPrecData pdata;
  sunindextype muk, mlk, storage_mu, lrw1, liw1;
  long int lrw, liw;
  int flag;

  if (kinmem == NULL) {
    KINProcessError(NULL, KINLS_MEM_NULL, "KINBBDPRE",
                    "KINBBDPrecInit", MSGBBD_MEM_NULL);
    return(KINLS_MEM_NULL);
  }
  kin_mem = (KINMem) kinmem;

  /* Test if the LS linear solver interface has been created */
  if (kin_mem->kin_lmem == NULL) {
    KINProcessError(kin_mem, KINLS_LMEM_NULL, "KINBBDPRE",
                    "KINBBDPrecInit", MSGBBD_LMEM_NULL);
    return(KINLS_LMEM_NULL);
  }
  kinls_mem = (KINLsMem) kin_mem->kin_lmem;

  /* Test compatibility of NVECTOR package with the BBD preconditioner */
  /* Note: Do NOT need to check for N_VScale since has already been checked for in KINSOL */
  if (kin_mem->kin_vtemp1->ops->nvgetarraypointer == NULL) {
    KINProcessError(kin_mem, KINLS_ILL_INPUT, "KINBBDPRE",
                    "KINBBDPrecInit", MSGBBD_BAD_NVECTOR);
    return(KINLS_ILL_INPUT);
  }

  /* Allocate data memory */
  pdata = NULL;
  pdata = (KBBDPrecData) malloc(sizeof *pdata);
  if (pdata == NULL) {
    KINProcessError(kin_mem, KINLS_MEM_FAIL,
                    "KINBBDPRE", "KINBBDPrecInit", MSGBBD_MEM_FAIL);
    return(KINLS_MEM_FAIL);
  }

  /* Set pointers to gloc and gcomm; load half-bandwidths */
  pdata->kin_mem = kinmem;
  pdata->gloc = gloc;
  pdata->gcomm = gcomm;
  pdata->mudq = SUNMIN(Nlocal-1, SUNMAX(0, mudq));
  pdata->mldq = SUNMIN(Nlocal-1, SUNMAX(0, mldq));
  muk = SUNMIN(Nlocal-1, SUNMAX(0, mukeep));
  mlk = SUNMIN(Nlocal-1, SUNMAX(0, mlkeep));
  pdata->mukeep = muk;
  pdata->mlkeep = mlk;

  /* Set extended upper half-bandwidth for PP (required for pivoting) */
  storage_mu = SUNMIN(Nlocal-1, muk+mlk);

  /* Allocate memory for preconditioner matrix */
  pdata->PP = NULL;
  pdata->PP = SUNBandMatrixStorage(Nlocal, muk, mlk, storage_mu);
  if (pdata->PP == NULL) {
    free(pdata); pdata = NULL;
    KINProcessError(kin_mem, KINLS_MEM_FAIL, "KINBBDPRE",
                    "KINBBDPrecInit", MSGBBD_MEM_FAIL);
    return(KINLS_MEM_FAIL);
  }

  /* Allocate memory for temporary N_Vectors */
  pdata->zlocal = NULL;
  pdata->zlocal = N_VNew_Serial(Nlocal);
  if (pdata->zlocal == NULL) {
    SUNMatDestroy(pdata->PP);
    free(pdata); pdata = NULL;
    KINProcessError(kin_mem, KINLS_MEM_FAIL, "KINBBDPRE", 
                    "KINBBDPrecInit", MSGBBD_MEM_FAIL);
    return(KINLS_MEM_FAIL);
  }

  pdata->rlocal = NULL;
  pdata->rlocal = N_VNewEmpty_Serial(Nlocal); /* empty vector */
  if (pdata->rlocal == NULL) {
    N_VDestroy(pdata->zlocal);
    SUNMatDestroy(pdata->PP);
    free(pdata); pdata = NULL;
    KINProcessError(kin_mem, KINLS_MEM_FAIL, "KINBBDPRE",
                    "KINBBDPrecInit", MSGBBD_MEM_FAIL);
    return(KINLS_MEM_FAIL);
  }

  pdata->tempv1 = NULL;
  pdata->tempv1 = N_VClone(kin_mem->kin_vtemp1);
  if (pdata->tempv1 == NULL) {
    N_VDestroy(pdata->zlocal);
    N_VDestroy(pdata->rlocal);
    SUNMatDestroy(pdata->PP);
    free(pdata); pdata = NULL;
    KINProcessError(kin_mem, KINLS_MEM_FAIL, "KINBBDPRE",
                    "KINBBDPrecInit", MSGBBD_MEM_FAIL);
    return(KINLS_MEM_FAIL);
  }

  pdata->tempv2 = NULL;
  pdata->tempv2 = N_VClone(kin_mem->kin_vtemp1);
  if (pdata->tempv2 == NULL) {
    N_VDestroy(pdata->zlocal);
    N_VDestroy(pdata->rlocal);
    N_VDestroy(pdata->tempv1);
    SUNMatDestroy(pdata->PP);
    free(pdata); pdata = NULL;
    KINProcessError(kin_mem, KINLS_MEM_FAIL, "KINBBDPRE",
                    "KINBBDPrecInit", MSGBBD_MEM_FAIL);
    return(KINLS_MEM_FAIL);
  }

  pdata->tempv3 = NULL;
  pdata->tempv3 = N_VClone(kin_mem->kin_vtemp1);
  if (pdata->tempv3 == NULL) {
    N_VDestroy(pdata->zlocal);
    N_VDestroy(pdata->rlocal);
    N_VDestroy(pdata->tempv1);
    N_VDestroy(pdata->tempv2);
    SUNMatDestroy(pdata->PP);
    free(pdata); pdata = NULL;
    KINProcessError(kin_mem, KINLS_MEM_FAIL, "KINBBDPRE",
                    "KINBBDPrecInit", MSGBBD_MEM_FAIL);
    return(KINLS_MEM_FAIL);
  }

  /* Allocate memory for banded linear solver */
  pdata->LS = NULL;
  pdata->LS = SUNLinSol_Band(pdata->zlocal, pdata->PP);
  if (pdata->LS == NULL) {
    N_VDestroy(pdata->zlocal);
    N_VDestroy(pdata->rlocal);
    N_VDestroy(pdata->tempv1);
    N_VDestroy(pdata->tempv2);
    N_VDestroy(pdata->tempv3);
    SUNMatDestroy(pdata->PP);
    free(pdata); pdata = NULL;
    KINProcessError(kin_mem, KINLS_MEM_FAIL, "KINBBDPRE",
                    "KINBBDPrecInit", MSGBBD_MEM_FAIL);
    return(KINLS_MEM_FAIL);
  }

  /* initialize band linear solver object */
  flag = SUNLinSolInitialize(pdata->LS);
  if (flag != SUNLS_SUCCESS) {
    N_VDestroy(pdata->zlocal);
    N_VDestroy(pdata->rlocal);
    N_VDestroy(pdata->tempv1);
    N_VDestroy(pdata->tempv2);
    N_VDestroy(pdata->tempv3);
    SUNMatDestroy(pdata->PP);
    SUNLinSolFree(pdata->LS);
    free(pdata); pdata = NULL;
    KINProcessError(kin_mem, KINLS_SUNLS_FAIL, "KINBBDPRE",
                    "KINBBDPrecInit", MSGBBD_SUNLS_FAIL);
    return(KINLS_SUNLS_FAIL);
  }

  /* Set rel_uu based on input value dq_rel_uu (0 implies default) */
  pdata->rel_uu = (dq_rel_uu > ZERO) ? dq_rel_uu : SUNRsqrt(kin_mem->kin_uround);

  /* Store Nlocal to be used in KINBBDPrecSetup */
  pdata->n_local = Nlocal;

  /* Set work space sizes and initialize nge */
  pdata->rpwsize = 0;
  pdata->ipwsize = 0;
  if (kin_mem->kin_vtemp1->ops->nvspace) {
    N_VSpace(kin_mem->kin_vtemp1, &lrw1, &liw1);
    pdata->rpwsize += 3*lrw1;
    pdata->ipwsize += 3*liw1;
  }
  if (pdata->zlocal->ops->nvspace) {
    N_VSpace(pdata->zlocal, &lrw1, &liw1);
    pdata->rpwsize += lrw1;
    pdata->ipwsize += liw1;
  }
  if (pdata->rlocal->ops->nvspace) {
    N_VSpace(pdata->rlocal, &lrw1, &liw1);
    pdata->rpwsize += lrw1;
    pdata->ipwsize += liw1;
  }
  if (pdata->PP->ops->space) {
    flag = SUNMatSpace(pdata->PP, &lrw, &liw);
    pdata->rpwsize += lrw;
    pdata->ipwsize += liw;
  }
  if (pdata->LS->ops->space) {
    flag = SUNLinSolSpace(pdata->LS, &lrw, &liw);
    pdata->rpwsize += lrw;
    pdata->ipwsize += liw;
  }
  pdata->nge = 0;

  /* make sure pdata is free from any previous allocations */
  if (kinls_mem->pfree != NULL)
    kinls_mem->pfree(kin_mem);

  /* Point to the new pdata field in the LS memory */
  kinls_mem->pdata = pdata;

  /* Attach the pfree function */
  kinls_mem->pfree = KINBBDPrecFree;

  /* Attach preconditioner solve and setup functions */
  flag = KINSetPreconditioner(kinmem, KINBBDPrecSetup,
                              KINBBDPrecSolve);

  return(flag);
}


/*------------------------------------------------------------------
  KINBBDPrecGetWorkSpace
  ------------------------------------------------------------------*/
int KINBBDPrecGetWorkSpace(void *kinmem,
                           long int *lenrwBBDP,
                           long int *leniwBBDP)
{
  KINMem kin_mem;
  KINLsMem kinls_mem;
  KBBDPrecData pdata;

  if (kinmem == NULL) {
    KINProcessError(NULL, KINLS_MEM_NULL, "KINBBDPRE",
                    "KINBBDPrecGetWorkSpace", MSGBBD_MEM_NULL);
    return(KINLS_MEM_NULL);
  }
  kin_mem = (KINMem) kinmem;

  if (kin_mem->kin_lmem == NULL) {
    KINProcessError(kin_mem, KINLS_LMEM_NULL, "KINBBDPRE",
                    "KINBBDPrecGetWorkSpace", MSGBBD_LMEM_NULL);
    return(KINLS_LMEM_NULL);
  }
  kinls_mem = (KINLsMem) kin_mem->kin_lmem;

  if (kinls_mem->pdata == NULL) {
    KINProcessError(kin_mem, KINLS_PMEM_NULL, "KINBBDPRE",
                    "KINBBDPrecGetWorkSpace", MSGBBD_PMEM_NULL);
    return(KINLS_PMEM_NULL);
  } 
  pdata = (KBBDPrecData) kinls_mem->pdata;

  *lenrwBBDP = pdata->rpwsize;
  *leniwBBDP = pdata->ipwsize;

  return(KINLS_SUCCESS);
}

/*------------------------------------------------------------------
 KINBBDPrecGetNumGfnEvals
 -------------------------------------------------------------------*/
int KINBBDPrecGetNumGfnEvals(void *kinmem,
                             long int *ngevalsBBDP)
{
  KINMem kin_mem;
  KINLsMem kinls_mem;
  KBBDPrecData pdata;

  if (kinmem == NULL) {
    KINProcessError(NULL, KINLS_MEM_NULL, "KINBBDPRE",
                    "KINBBDPrecGetNumGfnEvals", MSGBBD_MEM_NULL);
    return(KINLS_MEM_NULL);
  }
  kin_mem = (KINMem) kinmem;

  if (kin_mem->kin_lmem == NULL) {
    KINProcessError(kin_mem, KINLS_LMEM_NULL, "KINBBDPRE",
                    "KINBBDPrecGetNumGfnEvals", MSGBBD_LMEM_NULL);
    return(KINLS_LMEM_NULL);
  }
  kinls_mem = (KINLsMem) kin_mem->kin_lmem;

  if (kinls_mem->pdata == NULL) {
    KINProcessError(kin_mem, KINLS_PMEM_NULL, "KINBBDPRE",
                    "KINBBDPrecGetNumGfnEvals", MSGBBD_PMEM_NULL);
    return(KINLS_PMEM_NULL);
  } 
  pdata = (KBBDPrecData) kinls_mem->pdata;

  *ngevalsBBDP = pdata->nge;

  return(KINLS_SUCCESS);
}


/*------------------------------------------------------------------
  KINBBDPrecSetup

  KINBBDPrecSetup generates and factors a banded block of the
  preconditioner matrix on each processor, via calls to the
  user-supplied gloc and gcomm functions. It uses difference
  quotient approximations to the Jacobian elements.
 
  KINBBDPrecSetup calculates a new Jacobian, stored in banded
  matrix PP and does an LU factorization of P in place in PP.
 
  The parameters of KINBBDPrecSetup are as follows:
 
  uu      is the current value of the dependent variable vector,
          namely the solutin to func(uu)=0
 
  uscale  is the dependent variable scaling vector (i.e. uu)
 
  fval    is the vector f(u)
 
  fscale  is the function scaling vector
 
  bbd_data is the pointer to BBD data set by KINBBDInit.
  
  Note: The value to be returned by the KINBBDPrecSetup function
  is a flag indicating whether it was successful. This value is:
    0 if successful,
    > 0 for a recoverable error - step will be retried.
  ------------------------------------------------------------------*/
static int KINBBDPrecSetup(N_Vector uu, N_Vector uscale,
                           N_Vector fval, N_Vector fscale, 
                           void *bbd_data)
{
  KBBDPrecData pdata;
  KINMem kin_mem;
  int retval;

  pdata = (KBBDPrecData) bbd_data;

  kin_mem = (KINMem) pdata->kin_mem;

  /* Call KBBDDQJac for a new Jacobian calculation and store in PP */
  retval = SUNMatZero(pdata->PP);
  if (retval != 0) {
    KINProcessError(kin_mem, -1, "KINBBDPRE", "KINBBDPrecSetup",
                    MSGBBD_SUNMAT_FAIL);
    return(-1);
  }

  retval = KBBDDQJac(pdata, uu, uscale,
                     pdata->tempv1, pdata->tempv2, pdata->tempv3);
  if (retval != 0) {
    KINProcessError(kin_mem, -1, "KINBBDPRE", "KINBBDPrecSetup",
                    MSGBBD_FUNC_FAILED);
    return(-1);
  }

  /* Do LU factorization of P and return error flag */
  retval = SUNLinSolSetup_Band(pdata->LS, pdata->PP);
  return(retval);
}

/*------------------------------------------------------------------
  INBBDPrecSolve

  KINBBDPrecSolve solves a linear system P z = r, with the
  banded blocked preconditioner matrix P generated and factored
  by KINBBDPrecSetup. Here, r comes in as vv and z is
  returned in vv as well.
 
  The parameters for KINBBDPrecSolve are as follows:
 
  uu     an N_Vector giving the current iterate for the system
 
  uscale an N_Vector giving the diagonal entries of the
         uu scaling matrix
 
  fval   an N_Vector giving the current function value
 
  fscale an N_Vector giving the diagonal entries of the
         function scaling matrix

   vv  vector initially set to the right-hand side vector r, but
       which upon return contains a solution of the linear system
       P*z = r
 
  bbd_data is the pointer to BBD data set by KINBBDInit.
  
  Note: The value returned by the KINBBDPrecSolve function is a
  flag returned from the lienar solver object.
  ------------------------------------------------------------------*/

static int KINBBDPrecSolve(N_Vector uu, N_Vector uscale,
                           N_Vector fval, N_Vector fscale, 
                           N_Vector vv, void *bbd_data)
{
  KBBDPrecData pdata;
  realtype *vd;
  realtype *zd;
  int i, retval;

  pdata = (KBBDPrecData) bbd_data;

  /* Get data pointers */
  vd = N_VGetArrayPointer(vv);
  zd = N_VGetArrayPointer(pdata->zlocal);

  /* Attach local data array for vv to rlocal */
  N_VSetArrayPointer(vd, pdata->rlocal);

  /* Call banded solver object to do the work */
  retval = SUNLinSolSolve(pdata->LS, pdata->PP, pdata->zlocal, 
                          pdata->rlocal, ZERO);

  /* Copy result into vv */
  for (i=0; i<pdata->n_local; i++)
    vd[i] = zd[i]; 

  return(retval);
}


/*------------------------------------------------------------------
  KINBBDPrecFree
  ------------------------------------------------------------------*/
static int KINBBDPrecFree(KINMem kin_mem)
{
  KINLsMem kinls_mem;
  KBBDPrecData pdata;
  
  if (kin_mem->kin_lmem == NULL) return(0);
  kinls_mem = (KINLsMem) kin_mem->kin_lmem;
  
  if (kinls_mem->pdata == NULL) return(0);
  pdata = (KBBDPrecData) kinls_mem->pdata;

  SUNLinSolFree(pdata->LS);
  N_VDestroy(pdata->zlocal);
  N_VDestroy(pdata->rlocal);
  N_VDestroy(pdata->tempv1);
  N_VDestroy(pdata->tempv2);
  N_VDestroy(pdata->tempv3);
  SUNMatDestroy(pdata->PP);

  free(pdata);
  pdata = NULL;

  return(0);
}


/*------------------------------------------------------------------
  KBBDDQJac

  This routine generates a banded difference quotient
  approximation to the Jacobian of f(u). It assumes that a band
  matrix of type SUNMatrix is stored column-wise, and that elements
  within each column are contiguous. All matrix elements are
  generated as difference quotients, by way of calls to the user
  routine gloc. By virtue of the band structure, the number of
  these calls is bandwidth + 1, where bandwidth = ml + mu + 1.
  This routine also assumes that the local elements of a vector
  are stored contiguously.
  ------------------------------------------------------------------*/
static int KBBDDQJac(KBBDPrecData pdata,
                     N_Vector uu, N_Vector uscale,
                     N_Vector gu, N_Vector gtemp, N_Vector utemp)
{
  KINMem kin_mem;
  realtype inc, inc_inv;
  int retval;
  sunindextype group, i, j, width, ngroups, i1, i2;
  realtype *udata, *uscdata, *gudata, *gtempdata, *utempdata, *col_j;

  kin_mem = (KINMem) pdata->kin_mem;

  /* load utemp with uu = predicted solution vector */
  N_VScale(ONE, uu, utemp);

  /* set pointers to the data for all vectors */
  udata     = N_VGetArrayPointer(uu);
  uscdata   = N_VGetArrayPointer(uscale);
  gudata    = N_VGetArrayPointer(gu);
  gtempdata = N_VGetArrayPointer(gtemp);
  utempdata = N_VGetArrayPointer(utemp);

  /* Call gcomm and gloc to get base value of g(uu) */
  if (pdata->gcomm != NULL) {
    retval = pdata->gcomm(pdata->n_local, uu, kin_mem->kin_user_data);
    if (retval != 0) return(retval);
  }

  retval = pdata->gloc(pdata->n_local, uu, gu, kin_mem->kin_user_data);
  pdata->nge++;
  if (retval != 0) return(retval);

  /* Set bandwidth and number of column groups for band differencing */
  width = pdata->mldq + pdata->mudq + 1;
  ngroups = SUNMIN(width, pdata->n_local);

  /* Loop over groups */
  for(group = 1; group <= ngroups; group++) {

    /* increment all u_j in group */
    for(j = group - 1; j < pdata->n_local; j += width) {
      inc = pdata->rel_uu * SUNMAX(SUNRabs(udata[j]), (ONE / uscdata[j]));
      utempdata[j] += inc;
    }

    /* Evaluate g with incremented u */
    retval = pdata->gloc(pdata->n_local, utemp, gtemp, kin_mem->kin_user_data);
    pdata->nge++;
    if (retval != 0) return(retval);

    /* restore utemp, then form and load difference quotients */
    for (j = group - 1; j < pdata->n_local; j += width) {
      utempdata[j] = udata[j];
      col_j = SUNBandMatrix_Column(pdata->PP,j);
      inc = pdata->rel_uu * SUNMAX(SUNRabs(udata[j]) , (ONE / uscdata[j]));
      inc_inv = ONE / inc;
      i1 = SUNMAX(0, (j - pdata->mukeep));
      i2 = SUNMIN((j + pdata->mlkeep), (pdata->n_local - 1));
      for (i = i1; i <= i2; i++)
        SM_COLUMN_ELEMENT_B(col_j, i, j) = inc_inv * (gtempdata[i] - gudata[i]);
    }
  }

  return(0);
}
