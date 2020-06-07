/*
 * -----------------------------------------------------------------
 * Programmer(s): Cody J. Balos @ LLNL
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
 * This header files defines internal utility functions and macros
 * for SUNDIALS debugging.
 * -----------------------------------------------------------------
 */

#ifndef _SUNDIALS_DEBUG_H
#define _SUNDIALS_DEBUG_H

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif

/*
 * Macro which prints to stderr when in debug mode 
 */
#ifdef SUNDIALS_DEBUG
#define SUNDIALS_DEBUG_PRINT(str) fprintf(stderr, str)
#else
#define SUNDIALS_DEBUG_PRINT(str)
#endif

#ifdef __cplusplus  /* wrapper to enable C++ usage */
}
#endif

#endif /* _SUNDIALS_DEBUG_H */