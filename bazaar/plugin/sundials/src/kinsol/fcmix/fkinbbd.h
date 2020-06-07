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
 * This is the Fortran interface include file for the BBD
 * preconditioner module KINBBDPRE.
 * -----------------------------------------------------------------*/

/*******************************************************************************

                  FKINBBD Interface Package

 The FKINBBD Interface Package is a package of C functions which support the
 use of the KINSOL solver and MPI-parallel N_Vector module, along with the 
 KINBBDPRE preconditioner module, for the solution of nonlinear systems in a
 mixed Fortran/C setting. The combination of KINSOL and KINBBDPRE solves systems
 linear system arising from the solution of f(u) = 0 using a Krylov iterative
 linear solver via the KINSPILS interface, and with a preconditioner that is
 block-diagonal with banded blocks. While KINSOL and KINBBDPRE are written in C,
 it is assumed here that the user's calling program and user-supplied
 problem-defining routines are written in Fortran.

 The user-callable functions in this package, with the corresponding KINSOL and
 KINBBDPRE functions, are as follows:

   FKINBBDINIT : interfaces to KINBBDPrecInit
   FKINBBDOPT  : accesses optional outputs
   FKINBBDFREE : interfaces to KINBBDPrecFree

 In addition to the Fortran system function FKFUN, and optional Jacobian vector
 product routine FKJTIMES, the following are the user-supplied functions
 required by this package, each with the corresponding interface function which
 calls it (and its type within KINBBDPRE):

   FKLOCFN  : called by the interface function FKINgloc of type KINBBDLocalFn
   FKCOMMFN : called by the interface function FKINgcomm of type KINBBDCommFn

 Note: The names of all user-supplied routines here are fixed, in order to
 maximize portability for the resulting mixed-language program.

 Note: The names used within this interface package make use of the preprocessor
 to expand them appropriately for different machines/platforms. Later in this
 file, each name is expanded appropriately. For example, FKIN_BBDINIT is
 replaced with either fkinbbdinit, fkinbbdinit_, or fkinbbdinit__ depending
 upon the platform.

 ==============================================================================

              Usage of the FKINSOL/FKINBBD Interface Packages

 The usage of combined interface packages FKINSOL and FKINBBD requires calls
 to several interface functions, and a few user-supplied routines which define
 the problem to be solved and indirectly define the preconditioner. These
 function calls and user routines are summarized separately below. 

 Some details have been omitted, and the user is referred to the KINSOL User
 Guide for more complete information.

 (1) User-supplied system function routine: FKFUN

     The user must in all cases supply the following Fortran routine:

       SUBROUTINE FKFUN (UU, FVAL, IER)
       DIMENSION UU(*), FVAL(*)

     It must set the FVAL array to f(u), the system function, as a function
     of the array UU = u. Here UU and FVAL are vectors (distributed in the
     parallel case). IER is a return flag (currently not used).

 (2) Optional user-supplied Jacobian-vector product routine: FKJTIMES

     As an option, the user may supply a routine that computes the product
     of the system Jacobian and a given vector. The user-supplied function
     must have the following form:

       SUBROUTINE FKJTIMES (V, Z, NEWU, UU, IER)
       DIMENSION V(*), Z(*), UU(*)

     This must set the array Z to the product J*V, where J is the Jacobian
     matrix J = dF/du, and V is a given array. Here UU is an array containing
     the current value of the unknown vector u, and NEWU is an input integer
     indicating whether UU has changed since FKJTIMES was last called
     (1 = yes, 0 = no). If FKJTIMES computes and saves Jacobian data, then
     no such computation is necessary when NEWU = 0. Here V, Z, and UU are
     arrays of length NLOC - the local length of all distributed vectors.
     FKJTIMES should return IER = 0 if successful, or a nonzero IER otherwise.

 (3) User-supplied routines to define preconditoner: FKLOCFN and FKCOMMFN

     The routines in the KINBBDPRE (kinbbdpre.c) module provide a preconditioner 
     matrix for KINSOL that is block-diagonal with banded blocks. The blocking
     corresponds to the distribution of the dependent variable vector u
     amongst the processes. Each preconditioner block is generated from the
     Jacobian of the local part (associated with the current process) of a given
     function g(u) approximating f(u). The blocks are generated by a difference
     quotient scheme (independently by each process), utilizing the assumed
     banded structure with given half-bandwidths.

 (3.1) Local approximate function: FKLOCFN

       The user must supply a subroutine of the following form:

         SUBROUTINE FKLOCFN (NLOC, ULOC, GLOC, IER)
         DIMENSION ULOC(*), GLOC(*) 

       The routine is used to compute the function g(u) which approximates the
       system function f(u). This function is to be computed locally, i.e.
       without  inter-process communication. Note: The case where g is
       mathematically identical to f is allowed. It takes as input the local
       vector length (NLOC) and the local real solution array ULOC. It is to
       compute the local part of g(u) and store the result in the realtype
       array GLOC. IER is a return flag (currently not used).

 (3.2) Communication function: FKCOMMFN

       The user must also supply a subroutine of the following form:

         SUBROUTINE FKCOMMFN (NLOC, ULOC, IER)
         DIMENSION ULOC(*)

       The routine is used to perform all inter-process communication necessary
       to evaluate the approximate system function g described above. This
       function takes as input the local vector length (NLOC), and the local real
       dependent variable array ULOC. It is expected to save communicated data in 
       work space defined by the user, and made available to FKLOCFN. Each call
       to the FKCOMMFN function is preceded by a call to FKFUN with the same
       arguments. Thus FKCOMMFN can omit any communications done by FKFUN if
       relevant to the evaluation of g. IER is a return flag (currently not
       used).

 (4) Initialization: FNVINITP, FKINMALLOC, FKINBBDINIT, and FKINBBDSP*

 (4.1) To initialize the parallel machine environment, the user must make the
       following call:

         CALL FNVINITP (5, NLOCAL, NGLOBAL, IER)

       The arguments are:
         NLOCAL  = local size of vectors associated with process
         NGLOBAL = the system size, and the global size of vectors (the sum 
                   of all values of NLOCAL)
         IER     = return completion flag. Values are 0 = success, and
                   -1 = failure.

 (4.2) To allocate internal memory for KINSOL, make the following call:

         CALL FKINMALLOC (MSBPRE, FNORMTOL, SCSTEPTOL, CONSTRAINTS,
                          OPTIN, IOPT, ROPT, IER)

       The arguments are:
         MSBPRE      = maximum number of preconditioning solve calls without
                       calling the preconditioning setup routine
                       Note: 0 indicates default (10).
         FNORMTOL    = tolerance on the norm of f(u) to accept convergence
         SCSTEPTOL   = tolerance on minimum scaled step size
         CONSTRAINTS = array of constraint values on components of the
                       solution vector UU
         INOPT       = integer used as a flag to indicate whether possible
                       input values in IOPT[] array are to be used for
                       input: 0 = no and 1 = yes.
         IOPT        = array for integer optional inputs and outputs (declare
                       as INTEGER*8
         ROPT        = array of real optional inputs and outputs
         IER         = return completion flag. Values are 0 = success, and
                       -1 = failure.

       Note: See printed message for details in case of failure.

 (4.3) Initialize and attach one of the SPILS linear solvers. Make one of the 
       following calls to initialize a solver (see fkinsol.h for more details):

         CALL FSUNPCGINIT(3, PRETYPE, MAXL, IER)
         CALL FSUNSPBCGSINIT(3, PRETYPE, MAXL, IER)
         CALL FSUNSPFGMRINIT(3, PRETYPE, MAXL, IER)
         CALL FSUNSPGMRINIT(3, PRETYPE, MAXL, IER)
         CALL FSUNSPTFQMRINIT(3, PRETYPE, MAXL, IER)

       Then to attach the iterative linear solver structure the user must call:

         CALL FKINSPILSINIT(IER)

 (4.4) To allocate memory and initialize data associated with the BBD
       preconditioner, make the following call:

        CALL FKINBBDINIT(NLOCAL, MUDQ, MLDQ, MU, ML, IER)

      The arguments are:
        NLOCAL = local vector size on this process [long int, input]
        MUDQ   = upper half-bandwidth to be used in the computation
                 of the local Jacobian blocks by difference 
                 quotients.  These may be smaller than the true 
                 half-bandwidths of the Jacobian of the local block 
                 of g, when smaller values may provide greater 
                 efficiency [long int, input]
        MLDQ   = lower half-bandwidth to be used in the computation
                 of the local Jacobian blocks by difference 
                 quotients [long int, input]
        MU     = upper half-bandwidth of the band matrix that is
                 retained as an approximation of the local Jacobian
                 block (may be smaller than MUDQ) [long int, input]
        ML     = lower half-bandwidth of the band matrix that is
                 retained as an approximation of the local Jacobian
                 block (may be smaller than MLDQ) [long int, input]
        IER    = return completion flag [int, output]:
                    0 = success
                   <0 = an error occurred
 
 (5) To solve the system, make the following call:

       CALL FKINSOL (UU, GLOBALSTRAT, USCALE, FSCALE, IER)

     The arguments are:
       UU          = array containing the initial guess when called and the
                     solution upon termination
       GLOBALSTRAT = (INTEGER) a number defining the global strategy choice:
                     1 = inexact Newton, 2 = line search.
       USCALE      = array of scaling factors for the UU vector
       FSCALE      = array of scaling factors for the FVAL (function) vector
       IER         = integer error flag as returned by KINSOL.

     Note: See the KINSOL documentation for further information.

 (6) Optional outputs: FKINBBDOPT

     In addition to the optional inputs and outputs available with the FKINSOL
     interface package, there are optional outputs specific to the KINBBDPRE
     module. These are accessed by making the following call:

       CALL FKINBBDOPT (LENRPW, LENIPW, NGE)

     The arguments returned are:
       LENRPW = length of real preconditioner work space, in realtype words
                Note: This size is local to the current process.
       LENIPW = length of integer preconditioner work space, in integer words
                Note: This size is local to the current process.
       NGE    = number of g(u) evaluations (calls to FKLOCFN)

 (7) Memory freeing: FKINFREE

     To the free the internal memory created by the calls to FNVINITP
     and FKINMALLOC, make the following call:

       CALL FKINFREE

*******************************************************************************/

#ifndef _FKINBBD_H
#define _FKINBBD_H

/*
 * -----------------------------------------------------------------
 * header files
 * -----------------------------------------------------------------
 */
#include <sundials/sundials_nvector.h> /* definition of type N_Vector */
#include <sundials/sundials_types.h>   /* definition of type realtype */

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif

/*
 * -----------------------------------------------------------------
 * generic names are translated through the define statements below
 * -----------------------------------------------------------------
 */

#if defined(SUNDIALS_F77_FUNC)

#define FKIN_BBDINIT    SUNDIALS_F77_FUNC(fkinbbdinit, FKINBBDINIT)
#define FKIN_BBDOPT     SUNDIALS_F77_FUNC(fkinbbdopt, FKINBBDOPT)
#define FK_COMMFN       SUNDIALS_F77_FUNC(fkcommfn, FKCOMMFN)
#define FK_LOCFN        SUNDIALS_F77_FUNC(fklocfn, FKLOCFN)

#else

#define FKIN_BBDINIT    fkinbbdinit_
#define FKIN_BBDOPT     fkinbbdopt_
#define FK_COMMFN       fkcommfn_
#define FK_LOCFN        fklocfn_

#endif

/*
 * -----------------------------------------------------------------
 * Prototypes: exported functions
 * -----------------------------------------------------------------
 */

void FKIN_BBDINIT(long int *nlocal, long int *mudq, long int *mldq,
		  long int *mu, long int *ml, int *ier);
void FKIN_BBDOPT(long int *lenrpw, long int *lenipw, long int *nge);

/*
 * -----------------------------------------------------------------
 * Prototypes: FKINgloc and FKINgcomm
 * -----------------------------------------------------------------
 */

int FKINgloc(long int Nloc, N_Vector uu, N_Vector gval, void *user_data);
int FKINgcomm(long int Nloc, N_Vector uu, void *user_data);

#ifdef __cplusplus
}
#endif

#endif
