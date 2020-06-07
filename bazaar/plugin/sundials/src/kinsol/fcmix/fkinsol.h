/* -----------------------------------------------------------------
 * Programmer(s): Allan Taylor, Alan Hindmarsh, Radu Serban, and
 *                Aaron Collier @ LLNL
 *                Daniel R. Reynolds @ SMU
 *                David J. Gardner @ LLNL
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
 * This is the header file for the FKINSOL Interface Package.
 * See below for usage details.
 * -----------------------------------------------------------------*/

/***************************************************************************

                  FKINSOL Interface Package

 The FKINSOL Interface Package is a package of C functions which support the
 use of the KINSOL solver for the solution of nonlinear systems f(u) = 0, 
 in a mixed Fortran/C setting. While KINSOL is written in C, it is assumed 
 here that the user's calling program and user-supplied problem-defining 
 routines are written in Fortran. This package provides the necessary
 interface to KINSOL for the serial and parallel NVECTOR
 implementations.

 The user-callable functions, with the corresponding KINSOL functions,
 are as follows:

   FNVINITS, FNVINITP, FNVINITOMP, FNVINITPTS
          initialize serial, distributed memory parallel, or threaded 
          vector computations
   FKINMALLOC interfaces to KINInit
   FKINCREATE interfaces to KINCreate
   FKININIT interfaces to KINInit
   FKINSETIIN, FKINSETRIN, FKINSETVIN interface to KINSet* functions
   FKINSOL interfaces to KINSol and KINGet* functions
   FKINFREE interfaces to KINFree
   FKINLSINIT interface to KINSetLinearSolver
   FKINDENSESETJAC interface to KINSetJacFn
   FKINBANDSETJAC interface to KINSetJacFn
   FKINSPARSESETJAC interface to KINSetJacFn
   FKINLSSETJAC interface to KINSetJacTimes
   FKINLSSETPREC interface to KINSetPreconditioner

 The user-supplied functions, each with the corresponding interface function
 which calls it (and its type within KINSOL), are as follows:

   FKFUN    : called by the interface function FKINfunc of type KINSysFn
   FKDJAC   : called by the interface function FKINDenseJac of type
              KINLsJacFn
   FKBJAC   : called by the interface function FKINBandJac of type
              KINLsJacFn
   FKINSPJAC: called by the interface function FKINSparseJac of type 
              KINLsJacFn
   FKJTIMES : called by the interface function FKINJtimes of type
              KINLsJacTimesVecFn
   FKPSOL   : called by the interface function FKINPSol of type
              KINLsPrecSolveFn
   FKPSET   : called by the interface function FKINPSet of type
              KINLsPrecSetupFn

 In contrast to the case of direct use of KINSOL, the names of all 
 user-supplied routines here are fixed, in order to maximize portability for
 the resulting mixed-language program.

 Important note on portability:
 In this package, the names of the interface functions, and the names of
 the Fortran user routines called by them, appear as dummy names
 which are mapped to actual values by a series of definitions, in this
 and other header files.

 =========================================================================

                  Usage of the FKINSOL Interface Package

 The usage of FKINSOL requires calls to several interface functions, and 
 to a few user-supplied routines which define the problem to be solved.
 These function calls and user routines are summarized separately below.

 Some details are omitted, and the user is referred to the KINSOL manual
 for more complete documentation. Information on the arguments of any
 given user-callable interface routine, or of a given user-supplied
 function called by an interface function, can be found in the
 documentation on the corresponding function in the KINSOL package.

 The number labels on the instructions below end with "s" for instructions
 that apply to the serial version of KINSOL only, and end with "p" for
 those that apply to the parallel version only.

 (1) User-supplied system routine: FKFUN

     The user must in all cases supply the following Fortran routine:

       SUBROUTINE FKFUN (UU, FVAL, IER)
       DIMENSION UU(*), FVAL(*)

     It must set the FVAL array to f(u), the system function, as a
     function of the array UU = u. Here UU and FVAL are arrays representing
     vectors, which are distributed vectors in the parallel case.
     IER is a return flag, which should be 0 if FKFUN was successful.
     Return IER > 0 if a recoverable error occurred (and KINSOL is to try
     to recover).  Return IER < 0 if an unrecoverable error occurred.

 (2s) Optional user-supplied dense Jacobian approximation routine: FKDJAC
  
     As an option when using the DENSE linear solver, the user may supply a
     routine that computes a dense approximation of the system Jacobian 
     J = df/dy. If supplied, it must have the following form:
        
       SUBROUTINE FKDJAC(N, UU, FU, DJAC, WK1, WK2, IER)
       DIMENSION UU(*), FU(*), DJAC(N,*), WK1(*), WK2(*)

     This routine must compute the Jacobian and store it columnwise in DJAC.
     FKDJAC should return IER = 0 if successful, or a nonzero IER otherwise.

 (3s) Optional user-supplied band Jacobian approximation routine: FKBJAC
  
     As an option when using the BAND linear solver, the user may supply a
     routine that computes a band approximation of the system Jacobian 
     J = df/dy. If supplied, it must have the following form:
        
       SUBROUTINE FKBJAC(N, MU, ML, MDIM, UU, FU, BJAC, WK1, WK2, IER)
       DIMENSION UU(*), FU(*), BJAC(MDIM,*), WK1(*), WK2(*)

     This routine must load the MDIM by N array BJAC with the Jacobian matrix.
     FKBJAC should return IER = 0 if successful, or a nonzero IER otherwise.

 (4) Optional user-supplied Jacobian-vector product routine: FKJTIMES

     As an option, the user may supply a routine that computes the product
     of the system Jacobian and a given vector. This has the following form:

       SUBROUTINE FKJTIMES(V, Z, NEWU, UU, IER)
       DIMENSION V(*), Z(*), UU(*)

     This must set the array Z to the product J*V, where J is the Jacobian
     matrix J = dF/du, and V is a given array. Here UU is an array containing
     the current value of the unknown vector u. NEWU is an input integer 
     indicating whether UU has changed since FKJTIMES was last called 
     (1 = yes, 0 = no). If FKJTIMES computes and saves Jacobian data, then 
     no such computation is necessary when NEWU = 0. Here V, Z, and UU are 
     arrays of length NEQ, the problem size, or the local length of all 
     distributed vectors in the parallel case. FKJTIMES should return IER = 0 
     if successful, or a nonzero IER otherwise.

  (4.1s) User-supplied sparse Jacobian approximation routine: FKINSPJAC

     Required when using the KINKLU or KINSuperLUMT linear solvers, the 
     user must supply a routine that computes a compressed-sparse-column 
     [or compressed-sparse-row] approximation of the system Jacobian 
     J = dF(y)/dy.  If supplied, it must have the following form:

       SUBROUTINE FKINSPJAC(Y, FY, N, NNZ, JDATA, JRVALS, 
      &                     JCPTRS, WK1, WK2, IER)

     Typically this routine will use only N, NNZ, JDATA, JRVALS and 
     JCPTRS. It must load the N by N compressed sparse column [or compressed 
     sparse row] matrix with storage for NNZ nonzeros, stored in the arrays 
     JDATA (nonzero values), JRVALS (row [or column] indices for each nonzero), 
     JCOLPTRS (indices for start of each column [or row]), with the Jacobian 
     matrix at the current (y) in CSC [or CSR] form (see sunmatrix_sparse.h for
     more information).

     The arguments are:
         Y    -- array containing state variables [realtype, input]
         FY   -- array containing residual values [realtype, input]
         N    -- number of matrix rows/columns in Jacobian [int, input]
         NNZ  -- allocated length of nonzero storage [int, input]
        JDATA -- nonzero values in Jacobian
                 [realtype of length NNZ, output]
       JRVALS -- row [or column] indices for each nonzero in Jacobian
                  [int of length NNZ, output]
       JCPTRS -- pointers to each Jacobian column [or row] in preceding arrays
                 [int of length N+1, output]
         WK*  -- array containing temporary workspace of same size as Y 
                 [realtype, input]
         IER  -- return flag [int, output]:
                    0 if successful, 
                   >0 if a recoverable error occurred,
                   <0 if an unrecoverable error ocurred.

 (5) Initialization:  FNVINITS/FNVINITP/FNVINITOMP/FNVINITPTS and 
                      FKINCREATE and FKININIT 

 (5.1s) To initialize the serial machine environment, the user must make
        the following call:

          CALL FNVINITS (3, NEQ, IER)

        The arguments are:
          NEQ = size of vectors
          IER = return completion flag. Values are 0 = success, -1 = failure.

 (5.1p) To initialize the distributed memory parallel machine environment, 
        the user must make the following call:

          CALL FNVINITP (3, NLOCAL, NGLOBAL, IER)

        The arguments are:
          NLOCAL  = local size of vectors for this process
          NGLOBAL = the system size, and the global size of vectors
                    (the sum of all values of NLOCAL)
          IER     = return completion flag. Values are 0 = success,
                    -1 = failure.

 (5.1omp) To initialize the openMP threaded vector kernel, 
          the user must make the following call:

          CALL FNVINITOMP (3, NEQ, NUM_THREADS, IER)

        The arguments are:
          NEQ = size of vectors
          NUM_THREADS = number of threads
          IER = return completion flag. Values are 0 = success, -1 = failure.

 (5.1pts) To initialize the Pthreads threaded vector kernel, 
          the user must make the following call:

          CALL FNVINITOMP (3, NEQ, NUM_THREADS, IER)

        The arguments are:
          NEQ = size of vectors
          NUM_THREADS = number of threads
          IER = return completion flag. Values are 0 = success, -1 = failure.

 (5.2) To create the internal memory structure, make the following call:

         CALL FKINCREATE(IER)

       The arguments are:
         IER         = return completion flag. Values are 0 = success, and
                       -1 = failure.

       Note: See printed message for details in case of failure.

 (5.3) To set various integer optional inputs, make the folowing call:

          CALL FKINSETIIN(KEY, VALUE, IER)

       to set the optional input specified by the character key KEY to the 
       integer value VALUE.
       KEY is one of the following: 'PRNT_LEVEL', 'MAX_NITERS', 'ETA_FORM', 'MAA',
       'MAX_SETUPS', 'MAX_SP_SETUPS', 'NO_INIT_SETUP', 'NO_MIN_EPS', 'NO_RES_MON'.

       To set various real optional inputs, make the folowing call:

         CALL FKINSETRIN(KEY, VALUE, IER)

      to set the optional input specified by the character key KEY to the
      real value VALUE.
      KEY is one of the following: 'FNORM_TOL', 'SSTEP_TOL', 'MAX_STEP',
      'RERR_FUNC', 'ETA_CONST', 'ETA_PARAMS', 'RMON_CONST', 'RMON_PARAMS'.
      Note that if KEY is 'ETA_PARAMS' or 'RMON_PARAMS', then VALUE must be an
      array of dimension 2.

      To set the vector of constraints on the solution, make the following call:

        CALL FKINSETVIN(KEY, ARRAY, IER)

      where ARRAY is an array of reals and KEY is 'CONSTR_VEC'.

      FKINSETIIN, FKINSETRIN, and FKINSETVIN return IER=0 if successful and 
      IER<0 if an error occured.

 (5.4) To allocate and initialize the internal memory structure, 
       make the following call:

         CALL FKININIT(IOUT, ROUT, IER)

       The arguments are:
         IOUT        = array of length at least 16 for integer optional outputs
                       (declare as INTEGER*8)
         ROUT        = array of length at least 2 for real optional outputs
         IER         = return completion flag. Values are 0 = success, and
                       -1 = failure.

       Note: See printed message for details in case of failure.

 (6) Specification of linear system solution method:

     The solution method in KINSOL involves the solution of linear systems 
     related to the Jacobian J = dF/du of the nonlinear system.

 (6.1s) DENSE treatment of the linear systems (NVECTOR_SERIAL only):
  
        To initialize a dense matrix structure for storing the system Jacobian
        and for use within a direct linear solver, the user must call:
  
          CALL FSUNDENSEMATINIT(3, M, N, IER)
  
        The integer 3 is the KINSOL solver ID and the other arguments are:
          M   = the number of rows of the matrix [long int, input]
          N   = the number of columns of the matrix [long int, input]
          IER = return completion flag [int, output]:
                  0 = success, 
                 -1 = failure.
  
        To initialize a dense linear solver structure the user must call
        the following to use the SUNDIALS or LAPACK dense solvers:
          
          CALL FSUNDENSELINSOLINIT(3, IER)
  
          OR 
  
          CALL FSUNLAPACKDENSEINIT(3, IER)
  
        In the above routines, 3 is the KINSOL solver ID and IER is the return
        return completion flag (0 = success and -1 = failure).
  
        To attach the dense linear solver structure the user must call
        the following:

          CALL FKINLSINIT(IER)

        The arguments are:
            IER  = return completion flag [int, output]:
                    0 = SUCCESS,
                   -1 = failure (see printed message for failure details).

        If the user program includes the FKDJAC routine for the evaluation
        of the dense approximation to the system Jacobian, the following call
        must be made:
  
          CALL FKINDENSESETJAC(FLAG, IER)
  
        with FLAG = 1 to specify that FKDJAC is provided.  (FLAG = 0 specifies
        using the internal finite difference approximation to the Jacobian.)

 (6.2s) BAND treatment of the linear systems (NVECTOR_SERIAL only):
  
        To initialize a banded matrix structure for stroing the system Jacobian
        and for use within a banded linear solver, the user must call:
  
          CALL FSUNBANDMATINIT(3, N, MU, ML, SMU, IER)
  
        The integer 3 is the KINSOL solver ID and the other arguments are:
          N   = the number of columns of the matrix [long int, input]
          MU  = the number of upper bands (diagonal not included) in a banded 
                matrix [long int, input]
          ML  = the number of lower bands (diagonal not included) in a banded 
                matrix [long int, input]
          SMU = the number of upper bands to store (diagonal not included) 
                for factorization of a banded matrix [long int, input]
  
        To initialize a banded linear solver structure the user must call
        the following to use the SUNDIALS or LAPACK banded solvers:
  
          CALL FSUNBANDLINSOLINIT(3, IER)
  
          OR 
  
          CALL FSUNLAPACKBANDINIT(3, IER)
  
        In the above routines, 3 is the KINSOL solver ID and IER is the return
        return completion flag (0 = success and -1 = failure).
  
        To attach the banded linear solver structure the user must call
        the following:

          CALL FKINLSINIT(IER)

        The arguments are:
            IER  = return completion flag [int, output]:
                    0 = SUCCESS,
                   -1 = failure (see printed message for failure details).

        If the user program includes the FKBJAC routine for the evaluation
        of the band approximation to the system Jacobian, the following call
        must be made:
  
          CALL FKINBANDSETJAC(FLAG, IER)
  
        with FLAG = 1 to specify that FKBJAC is provided.  (FLAG = 0 specifies
        using the internal finite difference approximation to the Jacobian.)

 (6.3s) SPARSE treatment of the linear system using the KLU or SuperLU_MT solver.
  
        To initialize a sparse matrix structure for stroing the system Jacobian
        and for use within a sparse linear solver, the user must call:
  
          CALL FSUNSPARSEMATINIT(3, M, N, NNZ, SPARSETYPE, IER)
  
        The integer 3 is the KINSOL solver ID and the other arguments are:
          M   = the number of rows of the matrix [long int, input]
          N   = the number of columns of the matrix [long int, input]
          NNZ = the storage size (upper bound on the number of nonzeros) for 
                a sparse matrix [long int, input]
          SPARSETYPE = integer denoting use of CSC (0) vs CSR (1) storage 
                       for a sparse matrix [int, input]
          IER = return completion flag [int, output]:
                   0 = success, 
                  -1 = failure.
  
        To initialize a sparse linear solver structure the user must call
        the following to use the KLU or SuperLU_MT sparse solvers:
  
          CALL FSUNKLUINIT(3, IER)
  
          OR
  
          CALL FSUNSUPERLUMTINIT(3, NUM_THREADS, IER)
  
        In the above routines, 3 is the KINSOL solver ID, NUM_THREADS is the number
        of threads, and IER is the return completion flag (0 = success and
        -1 = failure).

        To attach the sparse linear solver structure the user must call
        the following:

          CALL FKINLSINIT(IER)

        The arguments are:
            IER  = return completion flag [int, output]:
                    0 = SUCCESS,
                   -1 = failure (see printed message for failure details).
  
        When using a sparse solver the user must provide the FKINSPJAC routine for the 
        evalution of the sparse approximation to the Jacobian. To indicate that this
        routine has been provided, after the call to FKINKLU, the following call must 
        be made    
  
          CALL FKINSPARSESETJAC(IER) 
  
        The int return flag IER=0 if successful, and nonzero otherwise.
  
        The KLU solver will reuse much of the factorization information from one
        nonlinear iteration to the next.  If at any time the user wants to force a full
        refactorization or if the number of nonzeros in the Jacobian matrix changes, the
        user should make the call:
  
          CALL FKINKLUREINIT(NEQ, NNZ, REINIT_TYPE)
  
        The arguments are:
          NEQ = the problem size [int; input]
          NNZ = the maximum number of nonzeros [int; input]
          REINIT_TYPE = 1 or 2.  For a value of 1, the matrix will be destroyed and 
            a new one will be allocated with NNZ nonzeros.  For a value of 2, 
            only symbolic and numeric factorizations will be completed. 

        At this time, there is no reinitialization capability for the SUNDIALS
        interface to the SuperLUMT solver.

        Once these the solvers have been initialized, their solver parameters may be
        modified via calls to the functions:

           CALL FSUNKLUSETORDERING(3, ORD_CHOICE, IER)
           CALL FSUNSUPERLUMTSETORDERING(3, ORD_CHOICE, IER)

        In the above routines, 3 is the KINSOL solver ID and ORD_CHOICE is an integer
        denoting ordering choice (see SUNKLUSetOrdering and SUNSuperLUMTSetOrdering
        documentation for details), and IER is the return completion flag (0 = success
        and -1 = failure).

 (6.4) Scaled Preconditioned Iterative linear Solvers (SPILS):

       To initialize a SPILS treatment of the linear system, the user must call one
       of the following:

         CALL FSUNPCGINIT(3, PRETYPE, MAXL, IER)
         CALL FSUNSPBCGSINIT(3, PRETYPE, MAXL, IER)
         CALL FSUNSPFGMRINIT(3, PRETYPE, MAXL, IER)
         CALL FSUNSPGMRINIT(3, PRETYPE, MAXL, IER)
         CALL FSUNSPTFQMRINIT(3, PRETYPE, MAXL, IER)

       The integer 3 is the KINSOL solver ID and the other arguments are:
         PRETYPE = type of preconditioning to perform (0=none, 1=left,
                   2=right, 3=both) [int, input]
         MAXL    = maximum Krylov subspace dimension [int, input]
         IER     = return completion flag [int, output]:
                    0 = success, 
                   -1 = failure.

        To attach the iterative linear solver structure the user must call
        the following:

          CALL FKINLSINIT(IER)

        The arguments are:
            IER  = return completion flag [int, output]:
                    0 = SUCCESS,
                   -1 = failure (see printed message for failure details).

       Once these the solvers have been initialized, their solver parameters may be
       modified via calls to the functions:

         CALL FSUNPCGSETPRECTYPE(3, PRETYPE, IER)
         CALL FSUNPCGSETMAXL(3, MAXL, IER)

         CALL FSUNSPBCGSSETPRECTYPE(3, PRETYPE, IER)
         CALL FSUNSPBCGSSETMAXL(3, MAXL, IER)

         CALL FSUNSPFGMRSETGSTYPE(3, GSTYPE, IER)
         CALL FSUNSPFGMRSETPRECTYPE(3, PRETYPE, IER)

         CALL FSUNSPGMRSETGSTYPE(3, GSTYPE, IER)
         CALL FSUNSPGMRSETPRECTYPE(3, PRETYPE, IER)

         CALL FSUNSPTFQMRSETPRECTYPE(3, PRETYPE, IER)
         CALL FSUNSPTFQMRSETMAXL(3, MAXL, IER)

       The integer 3 is the KINSOL solver ID and the other arguments are:
         PRETYPE = type of preconditioning to perform (0=none, 1=left, 
                   2=right, 3=both) [int, input]
         GSTYPE  = choice of Gram-Schmidt orthogonalization algorithm 
                   (0=modified, 1=classical) [int, input]
         IER     = return completion flag [int, output]:
                    0 = success, 
                   -1 = failure.       

 (6.5) Specifying user-provided functions for the iterative linear solvers (SPILS)

       If the user program includes the FKJTIMES routine for the evaluation
       of the Jacobian-vector product, the following call must be made:

         CALL FKINLSSETJAC(FLAG, IER)

       The argument FLAG = 0 specifies using the internal finite differences
       approximation to the Jacobian-vector product, while FLAG = 1 specifies
       that FKJTIMES is provided.

       Usage of the user-supplied routines FKPSET and FKPSOL for the setup and
       solution of the preconditioned linear system is specified by calling:

         CALL FKINLSSETPREC(FLAG, IER)

       where FLAG = 0 indicates no FKPSET or FKPSOL (default) and FLAG = 1
       specifies using FKPSET and FKPSOL. The user-supplied routines FKPSET
       and FKPSOL must be of the form:

         SUBROUTINE FKPSET (UU, USCALE, FVAL, FSCALE, IER)
         DIMENSION UU(*), USCALE(*), FVAL(*), FSCALE(*)

       It must perform any evaluation of Jacobian-related data and
       preprocessing needed for the solution of the preconditioned linear
       systems by FKPSOL. The variables UU through FSCALE are for use in the
       preconditioning setup process. Typically, the system function FKFUN is
       called, so that FVAL will have been updated. UU is the current solution
       iterate. If scaling is being used, USCALE and FSCALE are available for
       those operatins requiring scaling.

       On return, set IER = 0 if FKPSET was successful, set IER = 1 if
       an error occurred.

         SUBROUTINE FKPSOL (UU, USCALE, FVAL, FSCALE, VTEM, IER)
         DIMENSION UU(*), USCALE(*), FVAL(*), FSCALE(*), VTEM(*)

       Typically this routine will use only UU, FVAL, and VTEM.
       It must solve the preconditioned linear system Pz = r, where
       r = VTEM is input, and store the solution z in VTEM as well. Here
       P is the right preconditioner. If scaling is being used, the
       routine supplied must also account for scaling on either coordinate
       or function value.

 (7) The solver: FKINSOL

     Solving the nonlinear system is accomplished by making the following
     call:

       CALL FKINSOL (UU, GLOBALSTRAT, USCALE, FSCALE, IER)

     The arguments are:
       UU          = array containing the initial guess on input, and the
                     solution on return
       GLOBALSTRAT = (INTEGER) a number defining the global strategy choice:
                     0 = No globalization, 1 = LineSearch, 2 = Picard, 
                     3 = Fixed Point 
       USCALE      = array of scaling factors for the UU vector
       FSCALE      = array of scaling factors for the FVAL (function) vector
       IER         = INTEGER error flag as returned by KINSOL:
                     0 means success,
                     1 means initial guess satisfies f(u) = 0 (approx.),
                     2 means apparent stalling (small step),
                     a value < 0 means other error or failure.

     Note: See KINSOL documentation for detailed information.

 (8) Memory freeing: FKINFREE

     To the free the internal memory created by the calls to FKINCREATE and 
     FKININIT and any FNVINIT**, make the following call:

       CALL FKINFREE

 (9) Optional outputs: IOUT/ROUT

     The optional outputs available by way of IOUT and ROUT have the
     following names, locations, and descriptions. For further details see
     the KINSOL documentation.
 
       LENRW  = IOUT(1) = real workspace size
       LENRW  = IOUT(2) = real workspace size
       NNI    = IOUT(3) = number of Newton iterations
       NFE    = IOUT(4) = number of f evaluations
       NBCF   = IOUT(5) = number of line search beta condition failures
       NBKTRK = IOUT(6) = number of line search backtracks

       FNORM  = ROUT(1) = final scaled norm of f(u)
       STEPL  = ROUT(2) = scaled last step length

     The following optional outputs arise from the KINLS module:

       LRW    = IOUT( 7) = real workspace size for the linear solver module
       LIW    = IOUT( 8) = integer workspace size for the linear solver module
       LSTF   = IOUT( 9) = last flag returned by linear solver
       NFE    = IOUT(10) = number of f evaluations for DQ Jacobian or
                           Jacobian*vector approximation
       NJE    = IOUT(11) = number of Jacobian evaluations
       NJT    = IOUT(12) = number of Jacobian-vector product evaluations
       NPE    = IOUT(13) = number of preconditioner evaluations
       NPS    = IOUT(14) = number of preconditioner solves
       NLI    = IOUT(15) = number of linear (Krylov) iterations
       NCFL   = IOUT(16) = number of linear convergence failures

*******************************************************************************/

#ifndef _FKINSOL_H
#define _FKINSOL_H

/*------------------------------------------------------------------
  header files
  ------------------------------------------------------------------*/

#include <kinsol/kinsol.h>
#include <sundials/sundials_linearsolver.h> /* definition of SUNLinearSolver */
#include <sundials/sundials_matrix.h>       /* definition of SUNMatrix       */
#include <sundials/sundials_nvector.h>      /* definition of type N_Vector   */
#include <sundials/sundials_types.h>        /* definition of type realtype   */

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif

/*------------------------------------------------------------------
  generic names are translated through the define statements below
  ------------------------------------------------------------------*/

#if defined(SUNDIALS_F77_FUNC)

#define FKIN_MALLOC         SUNDIALS_F77_FUNC(fkinmalloc, FKINMALLOC)
#define FKIN_CREATE         SUNDIALS_F77_FUNC(fkincreate, FKINCREATE)
#define FKIN_INIT           SUNDIALS_F77_FUNC(fkininit,   FKININIT)
#define FKIN_SETIIN         SUNDIALS_F77_FUNC(fkinsetiin, FKINSETIIN)
#define FKIN_SETRIN         SUNDIALS_F77_FUNC(fkinsetrin, FKINSETRIN)
#define FKIN_SETVIN         SUNDIALS_F77_FUNC(fkinsetvin, FKINSETVIN)
#define FKIN_SOL            SUNDIALS_F77_FUNC(fkinsol, FKINSOL)
#define FKIN_FREE           SUNDIALS_F77_FUNC(fkinfree, FKINFREE)
#define FKIN_LSINIT         SUNDIALS_F77_FUNC(fkinlsinit, FKINLSINIT)
#define FKIN_LSSETJAC       SUNDIALS_F77_FUNC(fkinlssetjac, FKINLSSETJAC)
#define FKIN_LSSETPREC      SUNDIALS_F77_FUNC(fkinlssetprec, FKINLSSETPREC)
#define FK_PSET             SUNDIALS_F77_FUNC(fkpset, FKPSET)
#define FK_PSOL             SUNDIALS_F77_FUNC(fkpsol, FKPSOL)
#define FKIN_DENSESETJAC    SUNDIALS_F77_FUNC(fkindensesetjac, FKINDENSESETJAC)
#define FK_DJAC             SUNDIALS_F77_FUNC(fkdjac, FKDJAC)
#define FKIN_BANDSETJAC     SUNDIALS_F77_FUNC(fkinbandsetjac, FKINBANDSETJAC)
#define FK_BJAC             SUNDIALS_F77_FUNC(fkbjac, FKBJAC)
#define FKIN_SPARSESETJAC   SUNDIALS_F77_FUNC(fkinsparsesetjac, FKINSPARSESETJAC)  
#define FKIN_SPJAC          SUNDIALS_F77_FUNC(fkinspjac, FKINSPJAC)
#define FK_JTIMES           SUNDIALS_F77_FUNC(fkjtimes, FKJTIMES)
#define FK_FUN              SUNDIALS_F77_FUNC(fkfun, FKFUN)

/*---DEPRECATED---*/
#define FKIN_DLSINIT        SUNDIALS_F77_FUNC(fkindlsinit, FKINDLSINIT)
#define FKIN_SPILSINIT      SUNDIALS_F77_FUNC(fkinspilsinit, FKINSPILSINIT)
#define FKIN_SPILSSETJAC    SUNDIALS_F77_FUNC(fkinspilssetjac, FKINSPILSSETJAC)
#define FKIN_SPILSSETPREC   SUNDIALS_F77_FUNC(fkinspilssetprec, FKINSPILSSETPREC)
/*----------------*/

#else

#define FKIN_MALLOC         fkinmalloc_
#define FKIN_CREATE         fkincreate_
#define FKIN_INIT           fkininit_
#define FKIN_SETIIN         fkinsetiin_
#define FKIN_SETRIN         fkinsetrin_
#define FKIN_SETVIN         fkinsetvin_
#define FKIN_SOL            fkinsol_
#define FKIN_FREE           fkinfree_
#define FKIN_LSINIT         fkinlsinit_
#define FKIN_LSSETJAC       fkinlssetjac_
#define FK_JTIMES           fkjtimes_
#define FKIN_LSSETPREC      fkinlssetprec_
#define FKIN_DENSESETJAC    fkindensesetjac_
#define FK_DJAC             fkdjac_
#define FKIN_BANDSETJAC     fkinbandsetjac_
#define FK_BJAC             fkbjac_
#define FKIN_SPARSESETJAC   fkinsparsesetjac_
#define FKIN_SPJAC          fkinspjac_
#define FK_PSET             fkpset_
#define FK_PSOL             fkpsol_
#define FK_FUN              fkfun_

/*---DEPRECATED---*/
#define FKIN_DLSINIT        fkindlsinit_
#define FKIN_SPILSINIT      fkinspilsinit_
#define FKIN_SPILSSETJAC    fkinspilssetjac_
#define FKIN_SPILSSETPREC   fkinspilssetprec_
/*----------------*/
  
#endif

/*------------------------------------------------------------------
  Prototypes : exported functions
  ------------------------------------------------------------------*/

void FKIN_MALLOC(long int *iout, realtype *rout, int *ier);
void FKIN_CREATE(int *ier);
void FKIN_INIT(long int *iout, realtype *rout, int *ier);

void FKIN_SETIIN(char key_name[], long int *ival, int *ier);
void FKIN_SETRIN(char key_name[], realtype *rval, int *ier);
void FKIN_SETVIN(char key_name[], realtype *vval, int *ier);

void FKIN_LSINIT(int *ier);
void FKIN_LSSETJAC(int *flag, int *ier);
void FKIN_LSSETPREC(int *flag, int *ier);
void FKIN_DENSESETJAC(int *flag, int *ier);
void FKIN_BANDSETJAC(int *flag, int *ier);
void FKIN_SPARSESETJAC(int *ier);

/*---DEPRECATED---*/
void FKIN_DLSINIT(int *ier);
void FKIN_SPILSINIT(int *ier);
void FKIN_SPILSSETJAC(int *flag, int *ier);
void FKIN_SPILSSETPREC(int *flag, int *ier);
/*----------------*/
  
void FKIN_SOL(realtype *uu, int *globalstrategy, 
              realtype *uscale , realtype *fscale, int *ier);

void FKIN_FREE(void);

/*------------------------------------------------------------------
  Prototypes : functions called by the solver
  ------------------------------------------------------------------*/

int FKINfunc(N_Vector uu, N_Vector fval, void *user_data);

int FKINDenseJac(N_Vector uu, N_Vector fval, SUNMatrix J,
                 void *user_data, N_Vector vtemp1, N_Vector vtemp2);

int FKINBandJac(N_Vector uu, N_Vector fval, SUNMatrix J,
                void *user_data, N_Vector vtemp1, N_Vector vtemp2);

int FKINSparseJac(N_Vector uu, N_Vector fval, SUNMatrix J,
		  void *user_data, N_Vector vtemp1, N_Vector vtemp2);

int FKINJtimes(N_Vector v, N_Vector Jv, N_Vector uu,
               booleantype *new_uu, void *user_data);

int FKINPSet(N_Vector uu, N_Vector uscale,
             N_Vector fval, N_Vector fscale,
             void *user_data);

int FKINPSol(N_Vector uu, N_Vector uscale, 
             N_Vector fval, N_Vector fscale, 
             N_Vector vv, void *user_data);

void FKINNullMatrix();
void FKINNullLinsol();
  
/*------------------------------------------------------------------
  declarations for global variables shared amongst various routines
  ------------------------------------------------------------------*/

extern N_Vector F2C_KINSOL_vec;           /* defined in FNVECTOR module   */
extern SUNMatrix F2C_KINSOL_matrix;       /* defined in FSUNMATRIX module */
extern SUNLinearSolver F2C_KINSOL_linsol; /* defined in FSUNLINSOL module */
extern void *KIN_kinmem;                  /* defined in fkinsol.c         */
extern long int *KIN_iout;                /* defined in fkinsol.c         */
extern realtype *KIN_rout;                /* defined in fkinsol.c         */

#ifdef __cplusplus
}
#endif

#endif
