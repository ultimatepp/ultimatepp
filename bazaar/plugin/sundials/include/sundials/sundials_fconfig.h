!
! ----------------------------------------------------------------- 
! Programmer(s): Daniel R. Reynolds @ SMU
!-----------------------------------------------------------------
! LLNS/SMU Copyright Start
! Copyright (c) 2002-2018, Southern Methodist University and
! Lawrence Livermore National Security
!
! This work was performed under the auspices of the U.S. Department
! of Energy by Southern Methodist University and Lawrence Livermore
! National Laboratory under Contract DE-AC52-07NA27344.
! Produced at Southern Methodist University and the Lawrence
! Livermore National Laboratory.
!
! All rights reserved.
! For details, see the LICENSE file.
! LLNS/SMU Copyright End
! ------------------------------------------------------------------
! SUNDIALS fortran configuration input
! ------------------------------------------------------------------

!     Define precision of SUNDIALS data type 'realtype' as Fortran 
!     parameter "REALTYPE"
!
!     Depending on the precision level, this value will be one of
!          4  (SUNDIALS_SINGLE_PRECISION)
!          8  (SUNDIALS_DOUBLE_PRECISION)
!         16  (SUNDIALS_EXTENDED_PRECISION)
!
integer REALTYPE
parameter (REALTYPE=8)

!     Define type of vector indices in SUNDIALS 'sunindextype' as 
!     the Fortran parameter "SUNINDEXTYPE"
!
!     Depending on the user choice of indextype, this will be one of
!          4  (32BIT)
!          8  (64BIT)
!
integer SUNINDEXTYPE
parameter (SUNINDEXTYPE=8)

!     If building with MPI enabled, define the logical flag 
!     "SUNDIALS_MPI_COMM_F2C" indicating whether the user can specify
!     a different MPI communicator than MPI_COMM_WORLD to FNVInitP
!
!          .true.   (communicator can differ from MPI_COMM_WORLD)
!          .false.  (communicator must be MPI_COMM_WORLD)
!
logical SUNDIALS_MPI_COMM_F2C
parameter (SUNDIALS_MPI_COMM_F2C=.false.)
