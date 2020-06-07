/*
 * -----------------------------------------------------------------
 * Programmer(s): Daniel Reynolds @ SMU
 *                David Gardner @ LLNL
 * Based on code sundials_sparse.c by: Carol Woodward and
 *     Slaven Peles @ LLNL, and Daniel R. Reynolds @ SMU
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
 * This is the implementation file for the sparse implementation of
 * the SUNMATRIX package.
 * -----------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>

#include <sunmatrix/sunmatrix_sparse.h>
#include <sundials/sundials_nvector.h>
#include <sundials/sundials_math.h>

#define ZERO RCONST(0.0)
#define ONE  RCONST(1.0)

/* Private function prototypes */
static booleantype SMCompatible_Sparse(SUNMatrix A, SUNMatrix B);
static booleantype SMCompatible2_Sparse(SUNMatrix A, N_Vector x, N_Vector y);
static int Matvec_SparseCSC(SUNMatrix A, N_Vector x, N_Vector y);
static int Matvec_SparseCSR(SUNMatrix A, N_Vector x, N_Vector y);
static int format_convert(const SUNMatrix A, SUNMatrix B);

/*
 * -----------------------------------------------------------------
 * exported functions
 * -----------------------------------------------------------------
 */

/*
 * ==================================================================
 * Private function prototypes (functions working on SlsMat)
 * ==================================================================
 */

/* ----------------------------------------------------------------------------
 * Function to create a new sparse matrix
 */

SUNMatrix SUNSparseMatrix(sunindextype M, sunindextype N,
                          sunindextype NNZ, int sparsetype)
{
  SUNMatrix A;
  SUNMatrixContent_Sparse content;

  /* return with NULL matrix on illegal input */
  if ( (M <= 0) || (N <= 0) || (NNZ < 0) ) return(NULL);
  if ( (sparsetype != CSC_MAT) && (sparsetype != CSR_MAT) ) return(NULL);

  /* Create an empty matrix object */
  A = NULL;
  A = SUNMatNewEmpty();
  if (A == NULL) return(NULL);

  /* Attach operations */
  A->ops->getid     = SUNMatGetID_Sparse;
  A->ops->clone     = SUNMatClone_Sparse;
  A->ops->destroy   = SUNMatDestroy_Sparse;
  A->ops->zero      = SUNMatZero_Sparse;
  A->ops->copy      = SUNMatCopy_Sparse;
  A->ops->scaleadd  = SUNMatScaleAdd_Sparse;
  A->ops->scaleaddi = SUNMatScaleAddI_Sparse;
  A->ops->matvec    = SUNMatMatvec_Sparse;
  A->ops->space     = SUNMatSpace_Sparse;

  /* Create content */
  content = NULL;
  content = (SUNMatrixContent_Sparse) malloc(sizeof *content);
  if (content == NULL) { SUNMatDestroy(A); return(NULL); }

  /* Attach content */
  A->content = content;

  /* Fill content */
  content->sparsetype = sparsetype;
  content->M = M;
  content->N = N;
  content->NNZ = NNZ;
  switch(sparsetype){
  case CSC_MAT:
    content->NP = N;
    content->rowvals = &(content->indexvals);
    content->colptrs = &(content->indexptrs);
    /* CSR indices */
    content->colvals = NULL;
    content->rowptrs = NULL;
    break;
  case CSR_MAT:
    content->NP = M;
    content->colvals = &(content->indexvals);
    content->rowptrs = &(content->indexptrs);
    /* CSC indices */
    content->rowvals = NULL;
    content->colptrs = NULL;
  }
  content->data      = NULL;
  content->indexvals = NULL;
  content->indexptrs = NULL;

  /* Allocate content */
  content->data = (realtype *) calloc(NNZ, sizeof(realtype));
  if (content->data == NULL) { SUNMatDestroy(A); return(NULL); }

  content->indexvals = (sunindextype *) calloc(NNZ, sizeof(sunindextype));
  if (content->indexvals == NULL) { SUNMatDestroy(A); return(NULL); }

  content->indexptrs = (sunindextype *) calloc((content->NP + 1), sizeof(sunindextype));
  if (content->indexptrs == NULL) { SUNMatDestroy(A); return(NULL); }
  content->indexptrs[content->NP] = 0;

  return(A);
}




/* ----------------------------------------------------------------------------
 * Function to create a new sparse matrix from an existing dense matrix
 * by copying all nonzero values into the sparse matrix structure.  Returns NULL
 * if the request for matrix storage cannot be satisfied.
 */

SUNMatrix SUNSparseFromDenseMatrix(SUNMatrix Ad, realtype droptol, int sparsetype)
{
  sunindextype i, j, nnz;
  sunindextype M, N;
  SUNMatrix As;

  /* check for legal sparsetype, droptol and input matrix type */
  if ( (sparsetype != CSR_MAT) && (sparsetype != CSC_MAT) )
    return NULL;
  if ( droptol < ZERO )
    return NULL;
  if (SUNMatGetID(Ad) != SUNMATRIX_DENSE)
    return NULL;

  /* set size of new matrix */
  M = SM_ROWS_D(Ad);
  N = SM_COLUMNS_D(Ad);

  /* determine total number of nonzeros */
  nnz = 0;
  for (j=0; j<N; j++)
    for (i=0; i<M; i++)
      nnz += (SUNRabs(SM_ELEMENT_D(Ad,i,j)) > droptol);

  /* allocate sparse matrix */
  As = SUNSparseMatrix(M, N, nnz, sparsetype);
  if (As == NULL)  return NULL;

  /* copy nonzeros from Ad into As, based on CSR/CSC type */
  nnz = 0;
  if (sparsetype == CSC_MAT) {
    for (j=0; j<N; j++) {
      (SM_INDEXPTRS_S(As))[j] = nnz;
      for (i=0; i<M; i++) {
        if ( SUNRabs(SM_ELEMENT_D(Ad,i,j)) > droptol ) {
          (SM_INDEXVALS_S(As))[nnz] = i;
          (SM_DATA_S(As))[nnz++] = SM_ELEMENT_D(Ad,i,j);
        }
      }
    }
    (SM_INDEXPTRS_S(As))[N] = nnz;
  } else {       /* CSR_MAT */
    for (i=0; i<M; i++) {
      (SM_INDEXPTRS_S(As))[i] = nnz;
      for (j=0; j<N; j++) {
        if ( SUNRabs(SM_ELEMENT_D(Ad,i,j)) > droptol ) {
          (SM_INDEXVALS_S(As))[nnz] = j;
          (SM_DATA_S(As))[nnz++] = SM_ELEMENT_D(Ad,i,j);
        }
      }
    }
    (SM_INDEXPTRS_S(As))[M] = nnz;
  }

  return(As);
}


/* ----------------------------------------------------------------------------
 * Function to create a new sparse matrix from an existing band matrix
 * by copying all nonzero values into the sparse matrix structure.  Returns NULL
 * if the request for matrix storage cannot be satisfied.
 */

SUNMatrix SUNSparseFromBandMatrix(SUNMatrix Ad, realtype droptol, int sparsetype)
{
  sunindextype i, j, nnz;
  sunindextype M, N;
  SUNMatrix As;

  /* check for legal sparsetype, droptol and input matrix type */
  if ( (sparsetype != CSR_MAT) && (sparsetype != CSC_MAT) )
    return NULL;
  if ( droptol < ZERO )
    return NULL;
  if (SUNMatGetID(Ad) != SUNMATRIX_BAND)
    return NULL;

  /* set size of new matrix */
  M = SM_ROWS_B(Ad);
  N = SM_COLUMNS_B(Ad);

  /* determine total number of nonzeros */
  nnz = 0;
  for (j=0; j<N; j++)
    for (i=SUNMAX(0,j-SM_UBAND_B(Ad)); i<=SUNMIN(M-1,j+SM_LBAND_B(Ad)); i++)
      nnz += (SUNRabs(SM_ELEMENT_B(Ad,i,j)) > droptol);

  /* allocate sparse matrix */
  As = SUNSparseMatrix(M, N, nnz, sparsetype);
  if (As == NULL)  return NULL;

  /* copy nonzeros from Ad into As, based on CSR/CSC type */
  nnz = 0;
  if (sparsetype == CSC_MAT) {
    for (j=0; j<N; j++) {
      (SM_INDEXPTRS_S(As))[j] = nnz;
      for (i=SUNMAX(0,j-SM_UBAND_B(Ad)); i<=SUNMIN(M-1,j+SM_LBAND_B(Ad)); i++) {
        if ( SUNRabs(SM_ELEMENT_B(Ad,i,j)) > droptol ) {
          (SM_INDEXVALS_S(As))[nnz] = i;
          (SM_DATA_S(As))[nnz++] = SM_ELEMENT_B(Ad,i,j);
        }
      }
    }
    (SM_INDEXPTRS_S(As))[N] = nnz;
  } else {       /* CSR_MAT */
    for (i=0; i<M; i++) {
      (SM_INDEXPTRS_S(As))[i] = nnz;
      for (j=SUNMAX(0,i-SM_LBAND_B(Ad)); j<=SUNMIN(N-1,i+SM_UBAND_B(Ad)); j++) {
        if ( SUNRabs(SM_ELEMENT_B(Ad,i,j)) > droptol ) {
          (SM_INDEXVALS_S(As))[nnz] = j;
          (SM_DATA_S(As))[nnz++] = SM_ELEMENT_B(Ad,i,j);
        }
      }
    }
    (SM_INDEXPTRS_S(As))[M] = nnz;
  }

  return(As);
}


/* ----------------------------------------------------------------------------
 * Function to create a new CSR matrix from a CSC matrix.
 */
int SUNSparseMatrix_ToCSR(const SUNMatrix A, SUNMatrix* Bout)
{
    if (A == NULL) return(SUNMAT_ILL_INPUT);
    if (SM_SPARSETYPE_S(A) != CSC_MAT) return(SUNMAT_ILL_INPUT);

    *Bout = SUNSparseMatrix(SM_ROWS_S(A), SM_COLUMNS_S(A), SM_NNZ_S(A), CSR_MAT);
    if (*Bout == NULL) return(SUNMAT_MEM_FAIL);

    return format_convert(A, *Bout);
}


/* ----------------------------------------------------------------------------
 * Function to create a new CSC matrix from a CSR matrix.
 */
int SUNSparseMatrix_ToCSC(const SUNMatrix A, SUNMatrix* Bout)
{
    if (A == NULL) return(SUNMAT_ILL_INPUT);
    if (SM_SPARSETYPE_S(A) != CSR_MAT) return(SUNMAT_ILL_INPUT);

    *Bout = SUNSparseMatrix(SM_ROWS_S(A), SM_COLUMNS_S(A), SM_NNZ_S(A), CSC_MAT);
    if (*Bout == NULL) return(SUNMAT_MEM_FAIL);

    return format_convert(A, *Bout);
}


/* ----------------------------------------------------------------------------
 * Function to reallocate internal sparse matrix storage arrays so that the
 * resulting sparse matrix holds indexptrs[NP] nonzeros.  Returns 0 on success
 * and 1 on failure (e.g. if A does not have sparse type, or if nnz is negative)
 */

int SUNSparseMatrix_Realloc(SUNMatrix A)
{
  sunindextype nzmax;

  /* check for valid matrix type */
  if (SUNMatGetID(A) != SUNMATRIX_SPARSE)
    return SUNMAT_ILL_INPUT;

  /* get total number of nonzeros (return with failure if illegal) */
  nzmax = (SM_INDEXPTRS_S(A))[SM_NP_S(A)];
  if (nzmax < 0) return SUNMAT_ILL_INPUT;

  /* perform reallocation */
  SM_INDEXVALS_S(A) = (sunindextype *) realloc(SM_INDEXVALS_S(A), nzmax*sizeof(sunindextype));
  SM_DATA_S(A) = (realtype *) realloc(SM_DATA_S(A), nzmax*sizeof(realtype));
  SM_NNZ_S(A) = nzmax;

  return SUNMAT_SUCCESS;
}


/* ----------------------------------------------------------------------------
 * Function to reallocate internal sparse matrix storage arrays so that the
 * resulting sparse matrix has storage for a specified number of nonzeros.
 * Returns 0 on success and 1 on failure (e.g. if A does not have sparse type,
 * or if nnz is negative)
 */

int SUNSparseMatrix_Reallocate(SUNMatrix A, sunindextype NNZ)
{
  /* check for valid matrix type */
  if (SUNMatGetID(A) != SUNMATRIX_SPARSE)  return SUNMAT_ILL_INPUT;

  /* check for valid nnz */
  if (NNZ < 0)  return SUNMAT_ILL_INPUT;

  /* perform reallocation */
  SM_INDEXVALS_S(A) = (sunindextype *) realloc(SM_INDEXVALS_S(A), NNZ*sizeof(sunindextype));
  SM_DATA_S(A) = (realtype *) realloc(SM_DATA_S(A), NNZ*sizeof(realtype));
  SM_NNZ_S(A) = NNZ;

  return SUNMAT_SUCCESS;
}


/* ----------------------------------------------------------------------------
 * Function to print the sparse matrix
 */

void SUNSparseMatrix_Print(SUNMatrix A, FILE* outfile)
{
  sunindextype i, j;
  char *matrixtype;
  char *indexname;

  /* should not be called unless A is a sparse matrix;
     otherwise return immediately */
  if (SUNMatGetID(A) != SUNMATRIX_SPARSE)
    return;

  /* perform operation */
  if (SM_SPARSETYPE_S(A) == CSC_MAT) {
    indexname = (char*) "col";
    matrixtype = (char*) "CSC";
  } else {
    indexname = (char*) "row";
    matrixtype = (char*) "CSR";
  }
  fprintf(outfile, "\n");
  fprintf(outfile, "%ld by %ld %s matrix, NNZ: %ld \n",
          (long int) SM_ROWS_S(A), (long int) SM_COLUMNS_S(A),
          matrixtype, (long int) SM_NNZ_S(A));
  for (j=0; j<SM_NP_S(A); j++) {
    fprintf(outfile, "%s %ld : locations %ld to %ld\n", indexname,
            (long int) j, (long int) (SM_INDEXPTRS_S(A))[j],
            (long int) (SM_INDEXPTRS_S(A))[j+1]-1);
    fprintf(outfile, "  ");
    for (i=(SM_INDEXPTRS_S(A))[j]; i<(SM_INDEXPTRS_S(A))[j+1]; i++) {
#if defined(SUNDIALS_EXTENDED_PRECISION)
      fprintf(outfile, "%ld: %.32Lg   ", (long int) (SM_INDEXVALS_S(A))[i],
              (SM_DATA_S(A))[i]);
#elif defined(SUNDIALS_DOUBLE_PRECISION)
      fprintf(outfile, "%ld: %.16g   ", (long int) (SM_INDEXVALS_S(A))[i],
              (SM_DATA_S(A))[i]);
#else
      fprintf(outfile, "%ld: %.8g   ", (long int) (SM_INDEXVALS_S(A))[i],
              (SM_DATA_S(A))[i]);
#endif
    }
    fprintf(outfile, "\n");
  }
  fprintf(outfile, "\n");
  return;
}


/* ----------------------------------------------------------------------------
 * Functions to access the contents of the sparse matrix structure
 */

sunindextype SUNSparseMatrix_Rows(SUNMatrix A)
{
  if (SUNMatGetID(A) == SUNMATRIX_SPARSE)
    return SM_ROWS_S(A);
  else
    return SUNMAT_ILL_INPUT;
}

sunindextype SUNSparseMatrix_Columns(SUNMatrix A)
{
  if (SUNMatGetID(A) == SUNMATRIX_SPARSE)
    return SM_COLUMNS_S(A);
  else
    return SUNMAT_ILL_INPUT;
}

sunindextype SUNSparseMatrix_NNZ(SUNMatrix A)
{
  if (SUNMatGetID(A) == SUNMATRIX_SPARSE)
    return SM_NNZ_S(A);
  else
    return SUNMAT_ILL_INPUT;
}

sunindextype SUNSparseMatrix_NP(SUNMatrix A)
{
  if (SUNMatGetID(A) == SUNMATRIX_SPARSE)
    return SM_NP_S(A);
  else
    return SUNMAT_ILL_INPUT;
}

int SUNSparseMatrix_SparseType(SUNMatrix A)
{
  if (SUNMatGetID(A) == SUNMATRIX_SPARSE)
    return SM_SPARSETYPE_S(A);
  else
    return SUNMAT_ILL_INPUT;
}

realtype* SUNSparseMatrix_Data(SUNMatrix A)
{
  if (SUNMatGetID(A) == SUNMATRIX_SPARSE)
    return SM_DATA_S(A);
  else
    return NULL;
}

sunindextype* SUNSparseMatrix_IndexValues(SUNMatrix A)
{
  if (SUNMatGetID(A) == SUNMATRIX_SPARSE)
    return SM_INDEXVALS_S(A);
  else
    return NULL;
}

sunindextype* SUNSparseMatrix_IndexPointers(SUNMatrix A)
{
  if (SUNMatGetID(A) == SUNMATRIX_SPARSE)
    return SM_INDEXPTRS_S(A);
  else
    return NULL;
}


/*
 * -----------------------------------------------------------------
 * implementation of matrix operations
 * -----------------------------------------------------------------
 */

SUNMatrix_ID SUNMatGetID_Sparse(SUNMatrix A)
{
  return SUNMATRIX_SPARSE;
}

SUNMatrix SUNMatClone_Sparse(SUNMatrix A)
{
  SUNMatrix B = SUNSparseMatrix(SM_ROWS_S(A), SM_COLUMNS_S(A),
                                SM_NNZ_S(A), SM_SPARSETYPE_S(A));
  return(B);
}

void SUNMatDestroy_Sparse(SUNMatrix A)
{
  if (A == NULL) return;

  /* free content */
  if (A->content != NULL) {
    /* free data array */
    if (SM_DATA_S(A)) {
      free(SM_DATA_S(A));
      SM_DATA_S(A) = NULL;
    }
    /* free index values array */
    if (SM_INDEXVALS_S(A)) {
      free(SM_INDEXVALS_S(A));
      SM_INDEXVALS_S(A) = NULL;
      SM_CONTENT_S(A)->rowvals = NULL;
      SM_CONTENT_S(A)->colvals = NULL;
    }
    /* free index pointers array */
    if (SM_INDEXPTRS_S(A)) {
      free(SM_INDEXPTRS_S(A));
      SM_INDEXPTRS_S(A) = NULL;
      SM_CONTENT_S(A)->colptrs = NULL;
      SM_CONTENT_S(A)->rowptrs = NULL;
    }
    /* free content struct */
    free(A->content);
    A->content = NULL;
  }

  /* free ops and matrix */
  if (A->ops) { free(A->ops); A->ops = NULL; }
  free(A); A = NULL;

  return;
}

int SUNMatZero_Sparse(SUNMatrix A)
{
  sunindextype i;

  /* Perform operation */
  for (i=0; i<SM_NNZ_S(A); i++) {
    (SM_DATA_S(A))[i] = ZERO;
    (SM_INDEXVALS_S(A))[i] = 0;
  }
  for (i=0; i<SM_NP_S(A); i++)
    (SM_INDEXPTRS_S(A))[i] = 0;
  (SM_INDEXPTRS_S(A))[SM_NP_S(A)] = 0;
  return SUNMAT_SUCCESS;
}

int SUNMatCopy_Sparse(SUNMatrix A, SUNMatrix B)
{
  sunindextype i, A_nz;

  /* Verify that A and B are compatible */
  if (!SMCompatible_Sparse(A, B))
    return SUNMAT_ILL_INPUT;

  /* Perform operation */
  A_nz = (SM_INDEXPTRS_S(A))[SM_NP_S(A)];

  /* ensure that B is allocated with at least as
     much memory as we have nonzeros in A */
  if (SM_NNZ_S(B) < A_nz) {
    SM_INDEXVALS_S(B) = (sunindextype *) realloc(SM_INDEXVALS_S(B), A_nz*sizeof(sunindextype));
    SM_DATA_S(B) = (realtype *) realloc(SM_DATA_S(B), A_nz*sizeof(realtype));
    SM_NNZ_S(B) = A_nz;
  }

  /* zero out B so that copy works correctly */
  if (SUNMatZero_Sparse(B) != SUNMAT_SUCCESS)
    return SUNMAT_OPERATION_FAIL;

  /* copy the data and row indices over */
  for (i=0; i<A_nz; i++){
    (SM_DATA_S(B))[i] = (SM_DATA_S(A))[i];
    (SM_INDEXVALS_S(B))[i] = (SM_INDEXVALS_S(A))[i];
  }

  /* copy the column pointers over */
  for (i=0; i<SM_NP_S(A); i++) {
    (SM_INDEXPTRS_S(B))[i] = (SM_INDEXPTRS_S(A))[i];
  }
  (SM_INDEXPTRS_S(B))[SM_NP_S(A)] = A_nz;

  return SUNMAT_SUCCESS;
}

int SUNMatScaleAddI_Sparse(realtype c, SUNMatrix A)
{
  sunindextype j, i, p, nz, newvals, M, N, cend;
  booleantype newmat, found;
  sunindextype *w, *Ap, *Ai, *Cp, *Ci;
  realtype *x, *Ax, *Cx;
  SUNMatrix C;

  /* store shortcuts to matrix dimensions (M is inner dimension, N is outer) */
  if (SM_SPARSETYPE_S(A) == CSC_MAT) {
    M = SM_ROWS_S(A);
    N = SM_COLUMNS_S(A);
  }
  else {
    M = SM_COLUMNS_S(A);
    N = SM_ROWS_S(A);
  }

  /* access data arrays from A (return if failure) */
  Ap = Ai = NULL;
  Ax = NULL;
  if (SM_INDEXPTRS_S(A))  Ap = SM_INDEXPTRS_S(A);
  else  return (SUNMAT_MEM_FAIL);
  if (SM_INDEXVALS_S(A))  Ai = SM_INDEXVALS_S(A);
  else  return (SUNMAT_MEM_FAIL);
  if (SM_DATA_S(A))       Ax = SM_DATA_S(A);
  else  return (SUNMAT_MEM_FAIL);


  /* determine if A: contains values on the diagonal (so I can just be added in);
     if not, then increment counter for extra storage that should be required. */
  newvals = 0;
  for (j=0; j < SUNMIN(M,N); j++) {
    /* scan column (row if CSR) of A, searching for diagonal value */
    found = SUNFALSE;
    for (i=Ap[j]; i<Ap[j+1]; i++) {
      if (Ai[i] == j) {
        found = SUNTRUE;
        break;
      }
    }
    /* if no diagonal found, increment necessary storage counter */
    if (!found)  newvals += 1;
  }

  /* If extra nonzeros required, check whether matrix has sufficient storage space
     for new nonzero entries  (so I can be inserted into existing storage) */
  newmat = SUNFALSE;   /* no reallocation needed */
  if (newvals > (SM_NNZ_S(A) - Ap[N]))
    newmat = SUNTRUE;


  /* perform operation based on existing/necessary structure */

  /*   case 1: A already contains a diagonal */
  if (newvals == 0) {

    /* iterate through columns, adding 1.0 to diagonal */
    for (j=0; j < SUNMIN(M,N); j++)
      for (i=Ap[j]; i<Ap[j+1]; i++)
        if (Ai[i] == j) {
          Ax[i] = ONE + c*Ax[i];
        } else {
          Ax[i] = c*Ax[i];
        }


  /*   case 2: A has sufficient storage, but does not already contain a diagonal */
  } else if (!newmat) {


    /* create work arrays for nonzero indices and values in a single column (row) */
    w = (sunindextype *) malloc(M * sizeof(sunindextype));
    x = (realtype *) malloc(M * sizeof(realtype));

    /* determine storage location where last column (row) should end */
    nz = Ap[N] + newvals;

    /* store pointer past last column (row) from original A,
       and store updated value in revised A */
    cend = Ap[N];
    Ap[N] = nz;

    /* iterate through columns (rows) backwards */
    for (j=N-1; j>=0; j--) {

      /* clear out temporary arrays for this column (row) */
      for (i=0; i<M; i++) {
        w[i] = 0;
        x[i] = RCONST(0.0);
      }

      /* iterate down column (row) of A, collecting nonzeros */
      for (p=Ap[j]; p<cend; p++) {
        w[Ai[p]] += 1;         /* indicate that row (column) is filled */
        x[Ai[p]] = c*Ax[p];    /* collect/scale value */
      }

      /* add identity to this column (row) */
      if (j < M) {
        w[j] += 1;     /* indicate that row (column) is filled */
        x[j] += ONE;   /* update value */
      }

      /* fill entries of A with this column's (row's) data */
      for (i=M-1; i>=0; i--) {
        if ( w[i] > 0 ) {
          Ai[--nz] = i;
          Ax[nz] = x[i];
        }
      }

      /* store ptr past this col (row) from orig A, update value for new A */
      cend = Ap[j];
      Ap[j] = nz;

    }

    /* clean up */
    free(w);
    free(x);


  /*   case 3: A must be reallocated with sufficient storage */
  } else {

    /* create work arrays for nonzero indices and values */
    w = (sunindextype *) malloc(M * sizeof(sunindextype));
    x = (realtype *) malloc(M * sizeof(realtype));

    /* create new matrix for sum */
    C = SUNSparseMatrix(SM_ROWS_S(A), SM_COLUMNS_S(A),
                        Ap[N] + newvals,
                        SM_SPARSETYPE_S(A));

    /* access data from CSR structures (return if failure) */
    Cp = Ci = NULL;
    Cx = NULL;
    if (SM_INDEXPTRS_S(C))  Cp = SM_INDEXPTRS_S(C);
    else  return (SUNMAT_MEM_FAIL);
    if (SM_INDEXVALS_S(C))  Ci = SM_INDEXVALS_S(C);
    else  return (SUNMAT_MEM_FAIL);
    if (SM_DATA_S(C))       Cx = SM_DATA_S(C);
    else  return (SUNMAT_MEM_FAIL);

    /* initialize total nonzero count */
    nz = 0;

    /* iterate through columns (rows for CSR) */
    for (j=0; j<N; j++) {

      /* set current column (row) pointer to current # nonzeros */
      Cp[j] = nz;

      /* clear out temporary arrays for this column (row) */
      for (i=0; i<M; i++) {
        w[i] = 0;
        x[i] = 0.0;
      }

      /* iterate down column (along row) of A, collecting nonzeros */
      for (p=Ap[j]; p<Ap[j+1]; p++) {
        w[Ai[p]] += 1;         /* indicate that row is filled */
        x[Ai[p]] = c*Ax[p];    /* collect/scale value */
      }

      /* add identity to this column (row) */
      if (j < M) {
        w[j] += 1;     /* indicate that row is filled */
        x[j] += ONE;   /* update value */
      }

      /* fill entries of C with this column's (row's) data */
      for (i=0; i<M; i++) {
        if ( w[i] > 0 ) {
          Ci[nz] = i;
          Cx[nz++] = x[i];
        }
      }
    }

    /* indicate end of data */
    Cp[N] = nz;

    /* update A's structure with C's values; nullify C's pointers */
    SM_NNZ_S(A) = SM_NNZ_S(C);

    if (SM_DATA_S(A))
      free(SM_DATA_S(A));
    SM_DATA_S(A) = SM_DATA_S(C);
    SM_DATA_S(C) = NULL;

    if (SM_INDEXVALS_S(A))
      free(SM_INDEXVALS_S(A));
    SM_INDEXVALS_S(A) = SM_INDEXVALS_S(C);
    SM_INDEXVALS_S(C) = NULL;

    if (SM_INDEXPTRS_S(A))
      free(SM_INDEXPTRS_S(A));
    SM_INDEXPTRS_S(A) = SM_INDEXPTRS_S(C);
    SM_INDEXPTRS_S(C) = NULL;

    /* clean up */
    SUNMatDestroy_Sparse(C);
    free(w);
    free(x);

  }
  return SUNMAT_SUCCESS;

}

int SUNMatScaleAdd_Sparse(realtype c, SUNMatrix A, SUNMatrix B)
{
  sunindextype j, i, p, nz, newvals, M, N, cend;
  booleantype newmat;
  sunindextype *w, *Ap, *Ai, *Bp, *Bi, *Cp, *Ci;
  realtype *x, *Ax, *Bx, *Cx;
  SUNMatrix C;

  /* Verify that A and B are compatible */
  if (!SMCompatible_Sparse(A, B))
    return SUNMAT_ILL_INPUT;

  /* store shortcuts to matrix dimensions (M is inner dimension, N is outer) */
  if (SM_SPARSETYPE_S(A) == CSC_MAT) {
    M = SM_ROWS_S(A);
    N = SM_COLUMNS_S(A);
  }
  else {
    M = SM_COLUMNS_S(A);
    N = SM_ROWS_S(A);
  }

  /* access data arrays from A and B (return if failure) */
  Ap = Ai = Bp = Bi = NULL;
  Ax = Bx = NULL;
  if (SM_INDEXPTRS_S(A))  Ap = SM_INDEXPTRS_S(A);
  else  return(SUNMAT_MEM_FAIL);
  if (SM_INDEXVALS_S(A))  Ai = SM_INDEXVALS_S(A);
  else  return(SUNMAT_MEM_FAIL);
  if (SM_DATA_S(A))       Ax = SM_DATA_S(A);
  else  return(SUNMAT_MEM_FAIL);
  if (SM_INDEXPTRS_S(B))  Bp = SM_INDEXPTRS_S(B);
  else  return(SUNMAT_MEM_FAIL);
  if (SM_INDEXVALS_S(B))  Bi = SM_INDEXVALS_S(B);
  else  return(SUNMAT_MEM_FAIL);
  if (SM_DATA_S(B))       Bx = SM_DATA_S(B);
  else  return(SUNMAT_MEM_FAIL);

  /* create work arrays for row indices and nonzero column values */
  w = (sunindextype *) malloc(M * sizeof(sunindextype));
  x = (realtype *) malloc(M * sizeof(realtype));

  /* determine if A already contains the sparsity pattern of B */
  newvals = 0;
  for (j=0; j<N; j++) {

    /* clear work array */
    for (i=0; i<M; i++)  w[i] = 0;

    /* scan column of A, incrementing w by one */
    for (i=Ap[j]; i<Ap[j+1]; i++)
      w[Ai[i]] += 1;

    /* scan column of B, decrementing w by one */
    for (i=Bp[j]; i<Bp[j+1]; i++)
      w[Bi[i]] -= 1;

    /* if any entry of w is negative, A doesn't contain B's sparsity,
       so increment necessary storage counter */
    for (i=0; i<M; i++)
      if (w[i] < 0)  newvals += 1;
  }

  /* If extra nonzeros required, check whether A has sufficient storage space
     for new nonzero entries (so B can be inserted into existing storage) */
  newmat = SUNFALSE;   /* no reallocation needed */
  if (newvals > (SM_NNZ_S(A) - Ap[N]))
    newmat = SUNTRUE;

  /* perform operation based on existing/necessary structure */

  /*   case 1: A already contains sparsity pattern of B */
  if (newvals == 0) {

    /* iterate through columns, adding matrices */
    for (j=0; j<N; j++) {

      /* clear work array */
      for (i=0; i<M; i++)
        x[i] = ZERO;

      /* scan column of B, updating work array */
      for (i = Bp[j]; i < Bp[j+1]; i++)
        x[Bi[i]] = Bx[i];

      /* scan column of A, updating array entries appropriately */
      for (i = Ap[j]; i < Ap[j+1]; i++)
        Ax[i] = c*Ax[i] + x[Ai[i]];

    }


  /*   case 2: A has sufficient storage, but does not already contain B's sparsity */
  } else if (!newmat) {

    /* determine storage location where last column (row) should end */
    nz = Ap[N] + newvals;

    /* store pointer past last column (row) from original A,
       and store updated value in revised A */
    cend = Ap[N];
    Ap[N] = nz;

    /* iterate through columns (rows) backwards */
    for (j=N-1; j>=0; j--) {

      /* clear out temporary arrays for this column (row) */
      for (i=0; i<M; i++) {
        w[i] = 0;
        x[i] = RCONST(0.0);
      }

      /* iterate down column (row) of A, collecting nonzeros */
      for (p=Ap[j]; p<cend; p++) {
        w[Ai[p]] += 1;         /* indicate that row (column) is filled */
        x[Ai[p]] = c*Ax[p];    /* collect/scale value */
      }

      /* iterate down column of B, collecting nonzeros */
      for (p=Bp[j]; p<Bp[j+1]; p++) {
        w[Bi[p]] += 1;       /* indicate that row is filled */
        x[Bi[p]] += Bx[p];   /* collect value */
      }

      /* fill entries of A with this column's (row's) data */
      for (i=M-1; i>=0; i--) {
        if ( w[i] > 0 ) {
          Ai[--nz] = i;
          Ax[nz] = x[i];
        }
      }

      /* store ptr past this col (row) from orig A, update value for new A */
      cend = Ap[j];
      Ap[j] = nz;

    }


  /*   case 3: A must be reallocated with sufficient storage */
  } else {


    /* create new matrix for sum */
    C = SUNSparseMatrix(SM_ROWS_S(A), SM_COLUMNS_S(A),
                        Ap[N] + newvals, SM_SPARSETYPE_S(A));

    /* access data from CSR structures (return if failure) */
    Cp = Ci = NULL;
    Cx = NULL;
    if (SM_INDEXPTRS_S(C))  Cp = SM_INDEXPTRS_S(C);
    else  return(SUNMAT_MEM_FAIL);
    if (SM_INDEXVALS_S(C))  Ci = SM_INDEXVALS_S(C);
    else  return(SUNMAT_MEM_FAIL);
    if (SM_DATA_S(C))       Cx = SM_DATA_S(C);
    else  return(SUNMAT_MEM_FAIL);

    /* initialize total nonzero count */
    nz = 0;

    /* iterate through columns (rows) */
    for (j=0; j<N; j++) {

      /* set current column (row) pointer to current # nonzeros */
      Cp[j] = nz;

      /* clear out temporary arrays for this column (row) */
      for (i=0; i<M; i++) {
        w[i] = 0;
        x[i] = RCONST(0.0);
      }

      /* iterate down column of A, collecting nonzeros */
      for (p=Ap[j]; p<Ap[j+1]; p++) {
        w[Ai[p]] += 1;         /* indicate that row is filled */
        x[Ai[p]] = c*Ax[p];    /* collect/scale value */
      }

      /* iterate down column of B, collecting nonzeros */
      for (p=Bp[j]; p<Bp[j+1]; p++) {
        w[Bi[p]] += 1;       /* indicate that row is filled */
        x[Bi[p]] += Bx[p];   /* collect value */
      }

      /* fill entries of C with this column's data */
      for (i=0; i<M; i++) {
        if ( w[i] > 0 ) {
          Ci[nz] = i;
          Cx[nz++] = x[i];
        }
      }
    }

    /* indicate end of data */
    Cp[N] = nz;

    /* update A's structure with C's values; nullify C's pointers */
    SM_NNZ_S(A) = SM_NNZ_S(C);

    free(SM_DATA_S(A));
    SM_DATA_S(A) = SM_DATA_S(C);
    SM_DATA_S(C) = NULL;

    free(SM_INDEXVALS_S(A));
    SM_INDEXVALS_S(A) = SM_INDEXVALS_S(C);
    SM_INDEXVALS_S(C) = NULL;

    free(SM_INDEXPTRS_S(A));
    SM_INDEXPTRS_S(A) = SM_INDEXPTRS_S(C);
    SM_INDEXPTRS_S(C) = NULL;

    /* clean up */
    SUNMatDestroy_Sparse(C);

  }

  /* clean up */
  free(w);
  free(x);

  /* return success */
  return(0);

}

int SUNMatMatvec_Sparse(SUNMatrix A, N_Vector x, N_Vector y)
{
  /* Verify that A, x and y are compatible */
  if (!SMCompatible2_Sparse(A, x, y))
    return SUNMAT_ILL_INPUT;

  /* Perform operation */
  if(SM_SPARSETYPE_S(A) == CSC_MAT)
    return Matvec_SparseCSC(A, x, y);
  else
    return Matvec_SparseCSR(A, x, y);
}

int SUNMatSpace_Sparse(SUNMatrix A, long int *lenrw, long int *leniw)
{
  *lenrw = SM_NNZ_S(A);
  *leniw = 10 + SM_NP_S(A) + SM_NNZ_S(A);
  return SUNMAT_SUCCESS;
}


/*
 * =================================================================
 * private functions
 * =================================================================
 */

/* -----------------------------------------------------------------
 * Function to check compatibility of two sparse SUNMatrix objects
 */

static booleantype SMCompatible_Sparse(SUNMatrix A, SUNMatrix B)
{
  /* both matrices must be sparse */
  if ( (SUNMatGetID(A) != SUNMATRIX_SPARSE) ||
       (SUNMatGetID(B) != SUNMATRIX_SPARSE) )
    return SUNFALSE;

  /* both matrices must have the same shape and sparsity type */
  if (SUNSparseMatrix_Rows(A) != SUNSparseMatrix_Rows(B))
    return SUNFALSE;
  if (SUNSparseMatrix_Columns(A) != SUNSparseMatrix_Columns(B))
    return SUNFALSE;
  if (SM_SPARSETYPE_S(A) != SM_SPARSETYPE_S(B))
    return SUNFALSE;

  return SUNTRUE;
}


/* -----------------------------------------------------------------
 * Function to check compatibility of a SUNMatrix object with two
 * N_Vectors (A*x = b)
 */

static booleantype SMCompatible2_Sparse(SUNMatrix A, N_Vector x, N_Vector y)
{
  /* vectors must implement N_VGetArrayPointer */
  if ( (x->ops->nvgetarraypointer == NULL) ||
       (y->ops->nvgetarraypointer == NULL) )
    return SUNFALSE;

  /* Verify that the dimensions of A, x, and y agree */
  if ( (SUNSparseMatrix_Columns(A) != N_VGetLength(x)) ||
       (SUNSparseMatrix_Rows(A) != N_VGetLength(y)) )
    return SUNFALSE;

  return SUNTRUE;
}


/* -----------------------------------------------------------------
 * Computes y=A*x, where A is a CSC SUNMatrix_Sparse of dimension MxN, x is a
 * compatible N_Vector object of length N, and y is a compatible
 * N_Vector object of length M.
 *
 * Returns 0 if successful, 1 if unsuccessful (failed memory access, or both
 * x and y are the same vector).
 */
int Matvec_SparseCSC(SUNMatrix A, N_Vector x, N_Vector y)
{
  sunindextype i, j;
  sunindextype *Ap, *Ai;
  realtype *Ax, *xd, *yd;

  /* access data from CSC structure (return if failure) */
  Ap = SM_INDEXPTRS_S(A);
  Ai = SM_INDEXVALS_S(A);
  Ax = SM_DATA_S(A);
  if ((Ap == NULL) || (Ai == NULL) || (Ax == NULL))
    return SUNMAT_MEM_FAIL;

  /* access vector data (return if failure) */
  xd = N_VGetArrayPointer(x);
  yd = N_VGetArrayPointer(y);
  if ((xd == NULL) || (yd == NULL) || (xd == yd) )
    return SUNMAT_MEM_FAIL;

  /* initialize result */
  for (i=0; i<SM_ROWS_S(A); i++)
    yd[i] = 0.0;

  /* iterate through matrix columns */
  for (j=0; j<SM_COLUMNS_S(A); j++) {

    /* iterate down column of A, performing product */
    for (i=Ap[j]; i<Ap[j+1]; i++)
      yd[Ai[i]] += Ax[i]*xd[j];

  }

  return SUNMAT_SUCCESS;
}


/* -----------------------------------------------------------------
 * Computes y=A*x, where A is a CSR SUNMatrix_Sparse of dimension MxN, x is a
 * compatible N_Vector object of length N, and y is a compatible
 * N_Vector object of length M.
 *
 * Returns 0 if successful, -1 if unsuccessful (failed memory access).
 */
int Matvec_SparseCSR(SUNMatrix A, N_Vector x, N_Vector y)
{
  sunindextype i, j;
  sunindextype *Ap, *Aj;
  realtype *Ax, *xd, *yd;

  /* access data from CSR structure (return if failure) */
  Ap = SM_INDEXPTRS_S(A);
  Aj = SM_INDEXVALS_S(A);
  Ax = SM_DATA_S(A);
  if ((Ap == NULL) || (Aj == NULL) || (Ax == NULL))
    return SUNMAT_MEM_FAIL;

  /* access vector data (return if failure) */
  xd = N_VGetArrayPointer(x);
  yd = N_VGetArrayPointer(y);
  if ((xd == NULL) || (yd == NULL) || (xd == yd))
    return SUNMAT_MEM_FAIL;

  /* initialize result */
  for (i=0; i<SM_ROWS_S(A); i++)
    yd[i] = 0.0;

  /* iterate through matrix rows */
  for (i=0; i<SM_ROWS_S(A); i++) {

    /* iterate along row of A, performing product */
    for (j=Ap[i]; j<Ap[i+1]; j++)
      yd[i] += Ax[j]*xd[Aj[j]];

  }

  return(0);
}


/* -----------------------------------------------------------------
 * Copies A into a matrix B in the opposite format of A.
 * Returns 0 if successful, nonzero if unsuccessful.
 */
int format_convert(const SUNMatrix A, SUNMatrix B)
{
    realtype *Ax, *Bx;
    sunindextype *Ap, *Aj;
    sunindextype *Bp, *Bi;
    sunindextype n_row, n_col, nnz;
    sunindextype n, col, csum, row, last;

    if (SM_SPARSETYPE_S(A) == SM_SPARSETYPE_S(B))
      return SUNMatCopy_Sparse(A, B);

    Ap = SM_INDEXPTRS_S(A);
    Aj = SM_INDEXVALS_S(A);
    Ax = SM_DATA_S(A);
    
    n_row = (SM_SPARSETYPE_S(A) == CSR_MAT) ? SM_ROWS_S(A) : SM_COLUMNS_S(A);
    n_col = (SM_SPARSETYPE_S(A) == CSR_MAT) ? SM_COLUMNS_S(A) : SM_ROWS_S(A);

    Bp = SM_INDEXPTRS_S(B);
    Bi = SM_INDEXVALS_S(B);
    Bx = SM_DATA_S(B);

    nnz = Ap[n_row];

    SUNMatZero_Sparse(B);

    /* compute number of non-zero entries per column (if CSR) or per row (if CSC) of A */
    for (n = 0; n < nnz; n++)
    {
        Bp[Aj[n]]++;
    }

    /* cumualtive sum the nnz per column to get Bp[] */
    for (col = 0, csum = 0; col < n_col; col++)
    {
        sunindextype temp  = Bp[col];
        Bp[col] = csum;
        csum += temp;
    }
    Bp[n_col] = nnz;

    for (row = 0; row < n_row; row++)
    {
        sunindextype jj;
        for (jj = Ap[row]; jj < Ap[row+1]; jj++)
        {
            sunindextype col  = Aj[jj];
            sunindextype dest = Bp[col];

            Bi[dest] = row;
            Bx[dest] = Ax[jj];

            Bp[col]++;
        }
    }

    for (col = 0, last = 0; col <= n_col; col++)
    {
        sunindextype temp  = Bp[col];
        Bp[col] = last;
        last    = temp;
    }

    return 0;
}