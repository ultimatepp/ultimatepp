/* -----------------------------------------------------------------
 * Programmer: Radu Serban @ LLNL
 *             Daniel Reynolds @ SMU
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
 * This is the header file for a generic package of direct matrix
 * operations for use with BLAS/LAPACK.
 * -----------------------------------------------------------------*/

#ifndef _SUNDIALS_LAPACK_H
#define _SUNDIALS_LAPACK_H

#include <sundials/sundials_types.h>

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif

/*
 * ==================================================================
 * Blas and Lapack functions
 * ==================================================================
 */

#if defined(SUNDIALS_F77_FUNC)

#define dcopy_f77       SUNDIALS_F77_FUNC(dcopy, DCOPY)
#define dscal_f77       SUNDIALS_F77_FUNC(dscal, DSCAL)
#define dgemv_f77       SUNDIALS_F77_FUNC(dgemv, DGEMV)
#define dtrsv_f77       SUNDIALS_F77_FUNC(dtrsv, DTRSV)
#define dsyrk_f77       SUNDIALS_F77_FUNC(dsyrk, DSKYR)

#define dgbtrf_f77      SUNDIALS_F77_FUNC(dgbtrf, DGBTRF)
#define dgbtrs_f77      SUNDIALS_F77_FUNC(dgbtrs, DGBTRS)
#define dgetrf_f77      SUNDIALS_F77_FUNC(dgetrf, DGETRF)
#define dgetrs_f77      SUNDIALS_F77_FUNC(dgetrs, DGETRS)
#define dgeqp3_f77      SUNDIALS_F77_FUNC(dgeqp3, DGEQP3)
#define dgeqrf_f77      SUNDIALS_F77_FUNC(dgeqrf, DGEQRF)
#define dormqr_f77      SUNDIALS_F77_FUNC(dormqr, DORMQR)
#define dpotrf_f77      SUNDIALS_F77_FUNC(dpotrf, DPOTRF)
#define dpotrs_f77      SUNDIALS_F77_FUNC(dpotrs, DPOTRS)

#define scopy_f77       SUNDIALS_F77_FUNC(scopy, SCOPY)
#define sscal_f77       SUNDIALS_F77_FUNC(sscal, SSCAL)
#define sgemv_f77       SUNDIALS_F77_FUNC(sgemv, SGEMV)
#define strsv_f77       SUNDIALS_F77_FUNC(strsv, STRSV)
#define ssyrk_f77       SUNDIALS_F77_FUNC(ssyrk, SSKYR)

#define sgbtrf_f77      SUNDIALS_F77_FUNC(sgbtrf, SGBTRF)
#define sgbtrs_f77      SUNDIALS_F77_FUNC(sgbtrs, SGBTRS)
#define sgetrf_f77      SUNDIALS_F77_FUNC(sgetrf, SGETRF)
#define sgetrs_f77      SUNDIALS_F77_FUNC(sgetrs, SGETRS)
#define sgeqp3_f77      SUNDIALS_F77_FUNC(sgeqp3, SGEQP3)
#define sgeqrf_f77      SUNDIALS_F77_FUNC(sgeqrf, SGEQRF)
#define sormqr_f77      SUNDIALS_F77_FUNC(sormqr, SORMQR)
#define spotrf_f77      SUNDIALS_F77_FUNC(spotrf, SPOTRF)
#define spotrs_f77      SUNDIALS_F77_FUNC(spotrs, SPOTRS)

#else

#define dcopy_f77       dcopy_
#define dscal_f77       dscal_
#define dgemv_f77       dgemv_
#define dtrsv_f77       dtrsv_
#define dsyrk_f77       dsyrk_

#define dgbtrf_f77      dgbtrf_
#define dgbtrs_f77      dgbtrs_
#define dgeqp3_f77      dgeqp3_
#define dgeqrf_f77      dgeqrf_
#define dgetrf_f77      dgetrf_
#define dgetrs_f77      dgetrs_
#define dormqr_f77      dormqr_
#define dpotrf_f77      dpotrf_
#define dpotrs_f77      dpotrs_

#define scopy_f77       scopy_
#define sscal_f77       sscal_
#define sgemv_f77       sgemv_
#define strsv_f77       strsv_
#define ssyrk_f77       ssyrk_

#define sgbtrf_f77      sgbtrf_
#define sgbtrs_f77      sgbtrs_
#define sgeqp3_f77      sgeqp3_
#define sgeqrf_f77      sgeqrf_
#define sgetrf_f77      sgetrf_
#define sgetrs_f77      sgetrs_
#define sormqr_f77      sormqr_
#define spotrf_f77      spotrf_
#define spotrs_f77      spotrs_

#endif

/* Level-1 BLAS */

extern void dcopy_f77(sunindextype *n, const double *x,
                      const sunindextype *inc_x, double *y,
                      const sunindextype *inc_y);

extern void dscal_f77(sunindextype *n, const double *alpha, double *x,
                      const sunindextype *inc_x);

extern void scopy_f77(sunindextype *n, const float *x,
                      const sunindextype *inc_x, float *y,
                      const sunindextype *inc_y);

extern void sscal_f77(sunindextype *n, const float *alpha, float *x,
                      const sunindextype *inc_x);

/* Level-2 BLAS */

extern void dgemv_f77(const char *trans, sunindextype *m, sunindextype *n,
                      const double *alpha, const double *a, sunindextype *lda,
                      const double *x, sunindextype *inc_x, const double *beta,
                      double *y, sunindextype *inc_y);

extern void dtrsv_f77(const char *uplo, const char *trans, const char *diag,
                      const sunindextype *n, const double *a,
                      const sunindextype *lda, double *x,
                      const sunindextype *inc_x);

extern void sgemv_f77(const char *trans, sunindextype *m, sunindextype *n,
                      const float *alpha, const float *a, sunindextype *lda,
                      const float *x, sunindextype *inc_x, const float *beta,
                      float *y, sunindextype *inc_y);

extern void strsv_f77(const char *uplo, const char *trans, const char *diag,
                      const sunindextype *n, const float *a,
                      const sunindextype *lda, float *x,
                      const sunindextype *inc_x);

/* Level-3 BLAS */

extern void dsyrk_f77(const char *uplo, const char *trans,
                      const sunindextype *n, const sunindextype *k,
                      const double *alpha, const double *a,
                      const sunindextype *lda, const double *beta,
                      const double *c, const sunindextype *ldc);

extern void ssyrk_f77(const char *uplo, const char *trans,
                      const sunindextype *n, const sunindextype *k,
                      const float *alpha, const float *a,
                      const sunindextype *lda, const float *beta,
                      const float *c, const sunindextype *ldc);

/* LAPACK */

extern void dgbtrf_f77(const sunindextype *m, const sunindextype *n,
                       const sunindextype *kl, const sunindextype *ku,
                       double *ab, sunindextype *ldab, sunindextype *ipiv,
                       sunindextype *info);

extern void dgbtrs_f77(const char *trans, const sunindextype *n,
                       const sunindextype *kl, const sunindextype *ku,
                       const sunindextype *nrhs, double *ab,
                       const sunindextype *ldab, sunindextype *ipiv,
                       double *b, const sunindextype *ldb, sunindextype *info);


extern void dgeqp3_f77(const sunindextype *m, const sunindextype *n, double *a,
                       const sunindextype *lda, sunindextype *jpvt, double *tau,
                       double *work, const sunindextype *lwork,
                       sunindextype *info);

extern void dgeqrf_f77(const sunindextype *m, const sunindextype *n, double *a,
                       const sunindextype *lda, double *tau, double *work,
                       const sunindextype *lwork, sunindextype *info);

extern void dgetrf_f77(const sunindextype *m, const sunindextype *n, double *a,
                       sunindextype *lda, sunindextype *ipiv,
                       sunindextype *info);

extern void dgetrs_f77(const char *trans, const sunindextype *n,
                       const sunindextype *nrhs, double *a,
                       const sunindextype *lda, sunindextype *ipiv, double *b,
                       const sunindextype *ldb, sunindextype *info);


extern void dormqr_f77(const char *side, const char *trans,
                       const sunindextype *m, const sunindextype *n,
                       const sunindextype *k, double *a,
                       const sunindextype *lda, double *tau, double *c,
                       const sunindextype *ldc, double *work,
                       const sunindextype *lwork, sunindextype *info);

extern void dpotrf_f77(const char *uplo, const sunindextype *n, double *a,
                       sunindextype *lda, sunindextype *info);

extern void dpotrs_f77(const char *uplo, const sunindextype *n,
                       const sunindextype *nrhs, double *a,
                       const sunindextype *lda, double *b,
                       const sunindextype *ldb, sunindextype *info);


extern void sgbtrf_f77(const sunindextype *m, const sunindextype *n,
                       const sunindextype *kl, const sunindextype *ku,
                       float *ab, sunindextype *ldab, sunindextype *ipiv,
                       sunindextype *info);

extern void sgbtrs_f77(const char *trans, const sunindextype *n,
                       const sunindextype *kl, const sunindextype *ku,
                       const sunindextype *nrhs, float *ab,
                       const sunindextype *ldab, sunindextype *ipiv,
                       float *b, const sunindextype *ldb, sunindextype *info);


extern void sgeqp3_f77(const sunindextype *m, const sunindextype *n, float *a,
                       const sunindextype *lda, sunindextype *jpvt, float *tau,
                       float *work, const sunindextype *lwork,
                       sunindextype *info);

extern void sgeqrf_f77(const sunindextype *m, const sunindextype *n, float *a,
                       const sunindextype *lda, float *tau, float *work,
                       const sunindextype *lwork, sunindextype *info);

extern void sgetrf_f77(const sunindextype *m, const sunindextype *n, float *a,
                       sunindextype *lda, sunindextype *ipiv,
                       sunindextype *info);

extern void sgetrs_f77(const char *trans, const sunindextype *n,
                       const sunindextype *nrhs, float *a,
                       const sunindextype *lda, sunindextype *ipiv,
                       float *b, const sunindextype *ldb, sunindextype *info);


extern void sormqr_f77(const char *side, const char *trans,
                       const sunindextype *m, const sunindextype *n,
                       const sunindextype *k, float *a, const sunindextype *lda,
                       float *tau, float *c, const sunindextype *ldc,
                       float *work, const sunindextype *lwork,
                       sunindextype *info);

extern void spotrf_f77(const char *uplo, const sunindextype *n, float *a,
                       sunindextype *lda, sunindextype *info);

extern void spotrs_f77(const char *uplo, const sunindextype *n,
                       const sunindextype *nrhs, float *a,
                       const sunindextype *lda, float *b,
                       const sunindextype *ldb, sunindextype *info);


#ifdef __cplusplus
}
#endif

#endif
