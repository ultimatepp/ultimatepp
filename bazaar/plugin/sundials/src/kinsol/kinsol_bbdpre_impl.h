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
 * KINBBDPRE module header file (private version)
 * -----------------------------------------------------------------*/

#ifndef _KINBBDPRE_IMPL_H
#define _KINBBDPRE_IMPL_H

#include <kinsol/kinsol_bbdpre.h>
#include <sunmatrix/sunmatrix_band.h>
#include <sunlinsol/sunlinsol_band.h>

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif

/*------------------------------------------------------------------
  Definition of KBBDData
  ------------------------------------------------------------------*/

typedef struct KBBDPrecDataRec {

  /* passed by user to KINBBDPrecAlloc, used by pset/psolve functions */
  sunindextype mudq, mldq, mukeep, mlkeep;
  realtype rel_uu;  /* relative error for the Jacobian DQ routine */
  KINBBDLocalFn gloc;
  KINBBDCommFn gcomm;

  /* set by KINBBDPrecSetup and used by KINBBDPrecSetup and 
     KINBBDPrecSolve functions */
  sunindextype n_local;
  SUNMatrix PP;
  SUNLinearSolver LS;
  N_Vector rlocal;
  N_Vector zlocal;
  N_Vector tempv1;
  N_Vector tempv2;
  N_Vector tempv3;

  /* available for optional output */
  long int rpwsize;
  long int ipwsize;
  long int nge;

  /* pointer to KINSol memory */
  void *kin_mem;

} *KBBDPrecData;

/*
 *-----------------------------------------------------------------
 * KINBBDPRE error messages
 *-----------------------------------------------------------------
 */

#define MSGBBD_MEM_NULL    "KINSOL Memory is NULL."
#define MSGBBD_LMEM_NULL   "Linear solver memory is NULL. One of the SPILS linear solvers must be attached."
#define MSGBBD_MEM_FAIL    "A memory request failed."
#define MSGBBD_BAD_NVECTOR "A required vector operation is not implemented."
#define MSGBBD_SUNMAT_FAIL "An error arose from a SUNBandMatrix routine."
#define MSGBBD_SUNLS_FAIL  "An error arose from a SUNBandLinearSolver routine."
#define MSGBBD_PMEM_NULL   "BBD peconditioner memory is NULL. IDABBDPrecInit must be called."
#define MSGBBD_FUNC_FAILED "The gloc or gcomm routine failed in an unrecoverable manner."

#ifdef __cplusplus
}
#endif

#endif
