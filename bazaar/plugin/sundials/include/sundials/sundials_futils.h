/* -----------------------------------------------------------------
 * Programmer(s): Cody J. Balos
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
 * SUNDIALS Fortran 2003 interface utility definitions.
 * -----------------------------------------------------------------*/

#ifndef _SUNDIALS_FUTILS_H
#define _SUNDIALS_FUTILS_H

#include <stdio.h>
#include <sundials/sundials_config.h>

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif

/* Create a file pointer with the given file name and mode. */
SUNDIALS_EXPORT FILE* SUNDIALSFileOpen(const char* filename, const char* modes);

/* Close a file pointer with the given file name. */
SUNDIALS_EXPORT void SUNDIALSFileClose(FILE* fp);


#ifdef __cplusplus
}
#endif

#endif
