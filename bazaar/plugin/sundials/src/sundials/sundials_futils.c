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
 * SUNDIALS Fortran 2003 interface utility implementations.
 * -----------------------------------------------------------------*/

#include <sundials/sundials_futils.h>

/* Create a file pointer with the given file name and mode. */
FILE* SUNDIALSFileOpen(const char* filename, const char* mode)
{
  return fopen(filename, mode);
}

/* Close a file pointer with the given file name. */
void SUNDIALSFileClose(FILE* fp)
{
  fclose(fp);
}
