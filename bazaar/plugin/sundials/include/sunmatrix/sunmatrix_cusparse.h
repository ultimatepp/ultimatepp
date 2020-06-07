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
 * This is the header file is for the cuSPARSE implementation of the
 * SUNMATRIX module.
 * -----------------------------------------------------------------
 */

#ifndef _SUNMATRIX_CUSPARSE_H
#define _SUNMATRIX_CUSPARSE_H

#include <stdio.h>

#include <cuda_runtime.h>
#include <cusparse.h>

#include <sundials/sundials_matrix.h>

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif

/* ------------------------------------------
 * Implementation of SUNMATRIX_CUSPARSE
 * ------------------------------------------ */

/* storage formats */
#define SUNMAT_CUSPARSE_CSR 0
#define SUNMAT_CUSPARSE_BCSR 1

struct _SUNMatrix_Content_cuSparse {
  int M;
  int N;
  int NNZ;
  int nblocks;
  int blockrows;
  int blockcols;
  int blocknnz;
  int sparse_type;
  booleantype own_data;
  booleantype fixed_pattern;
  int* colind;
  int* rowptrs;
  realtype* data;
  cusparseMatDescr_t mat_descr;
  cusparseHandle_t cusp_handle;
};

typedef struct _SUNMatrix_Content_cuSparse *SUNMatrix_Content_cuSparse;


/* ------------------------------------------------------------------
 * Constructors.
 * ------------------------------------------------------------------ */

SUNDIALS_EXPORT SUNMatrix SUNMatrix_cuSparse_NewCSR(int M, int N, int NNZ, cusparseHandle_t cusp);
SUNDIALS_EXPORT SUNMatrix SUNMatrix_cuSparse_MakeCSR(cusparseMatDescr_t mat_descr, int M, int N, int NNZ,
                                                     int *rowptrs , int *colind , realtype *data,
                                                     cusparseHandle_t cusp);

/* Creates a CSR block-diagonal matrix where each block shares the same sparsity structure.
   Reduces memory usage by only storing the row pointers and column indices for one block. */
SUNDIALS_EXPORT SUNMatrix SUNMatrix_cuSparse_NewBlockCSR(int nblocks, int blockrows, int blockcols,
                                                         int blocknnz, cusparseHandle_t cusp);


/* ------------------------------------------------------------------
 * Implementation specific routines.
 * ------------------------------------------------------------------ */

SUNDIALS_EXPORT int SUNMatrix_cuSparse_SparseType(SUNMatrix A);
SUNDIALS_EXPORT int SUNMatrix_cuSparse_Rows(SUNMatrix A);
SUNDIALS_EXPORT int SUNMatrix_cuSparse_Columns(SUNMatrix A);
SUNDIALS_EXPORT int SUNMatrix_cuSparse_NNZ(SUNMatrix A);
SUNDIALS_EXPORT int* SUNMatrix_cuSparse_IndexPointers(SUNMatrix A);
SUNDIALS_EXPORT int* SUNMatrix_cuSparse_IndexValues(SUNMatrix A);
SUNDIALS_EXPORT realtype* SUNMatrix_cuSparse_Data(SUNMatrix A);

SUNDIALS_EXPORT int SUNMatrix_cuSparse_SetFixedPattern(SUNMatrix A, booleantype yesno);
SUNDIALS_EXPORT int SUNMatrix_cuSparse_NumBlocks(SUNMatrix A);
SUNDIALS_EXPORT int SUNMatrix_cuSparse_BlockRows(SUNMatrix A);
SUNDIALS_EXPORT int SUNMatrix_cuSparse_BlockColumns(SUNMatrix A);
SUNDIALS_EXPORT int SUNMatrix_cuSparse_BlockNNZ(SUNMatrix A);
SUNDIALS_EXPORT realtype* SUNMatrix_cuSparse_BlockData(SUNMatrix A, int blockidx);
SUNDIALS_EXPORT cusparseMatDescr_t SUNMatrix_cuSparse_MatDescr(SUNMatrix A);
SUNDIALS_EXPORT int SUNMatrix_cuSparse_CopyToDevice(SUNMatrix device, realtype* h_data,
                                                    int* h_idxptrs, int* h_idxvals);
SUNDIALS_EXPORT int SUNMatrix_cuSparse_CopyFromDevice(SUNMatrix device, realtype* h_data,
                                                      int* h_idxptrs, int* h_idxvals);


/* ------------------------------------------------------------------
 * SUNMatrix API routines.
 * ------------------------------------------------------------------ */

SUNDIALS_EXPORT SUNMatrix_ID SUNMatGetID_cuSparse(SUNMatrix A);
SUNDIALS_EXPORT SUNMatrix SUNMatClone_cuSparse(SUNMatrix A);
SUNDIALS_EXPORT void SUNMatDestroy_cuSparse(SUNMatrix A);
SUNDIALS_EXPORT int SUNMatZero_cuSparse(SUNMatrix A);
SUNDIALS_EXPORT int SUNMatCopy_cuSparse(SUNMatrix A, SUNMatrix B);
SUNDIALS_EXPORT int SUNMatScaleAdd_cuSparse(realtype c, SUNMatrix A, SUNMatrix B);
SUNDIALS_EXPORT int SUNMatScaleAddI_cuSparse(realtype c, SUNMatrix A);
SUNDIALS_EXPORT int SUNMatMatvec_cuSparse(SUNMatrix A, N_Vector x, N_Vector y);


#ifdef __cplusplus
}
#endif

#endif
