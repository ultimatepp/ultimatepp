/* -----------------------------------------------------------------
 * Programmer(s): Radu Serban and Aaron Collier @ LLNL
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
 * This file (companion of nvector.h) contains definitions
 * needed for the initialization of vector operations in Fortran.
 * -----------------------------------------------------------------*/


#ifndef _FNVECTOR_H
#define _FNVECTOR_H

#ifndef _SUNDIALS_CONFIG_H
#define _SUNDIALS_CONFIG_H
#include <sundials/sundials_config.h>
#endif

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif

/* SUNDIALS solver IDs */

#define FCMIX_CVODE   1
#define FCMIX_IDA     2
#define FCMIX_KINSOL  3
#define FCMIX_ARKODE  4

#ifdef __cplusplus
}
#endif

#endif
