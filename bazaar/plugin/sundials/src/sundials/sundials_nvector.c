/* -----------------------------------------------------------------
 * Programmer(s): Radu Serban, Aaron Collier, and
 *                David J. Gardner @ LLNL
 *                Daniel R. Reynolds @ SMU
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
 * This is the implementation file for a generic NVECTOR package.
 * It contains the implementation of the N_Vector operations listed
 * in nvector.h.
 * -----------------------------------------------------------------*/

#include <stdlib.h>
#include <sundials/sundials_nvector.h>

/* -----------------------------------------------------------------
 * Create an empty NVector object
 * -----------------------------------------------------------------*/

N_Vector N_VNewEmpty()
{
  N_Vector     v;
  N_Vector_Ops ops;

  /* create vector object */
  v = NULL;
  v = (N_Vector) malloc(sizeof *v);
  if (v == NULL) return(NULL);

  /* create vector ops structure */
  ops = NULL;
  ops = (N_Vector_Ops) malloc(sizeof *ops);
  if (ops == NULL) { free(v); return(NULL); }

  /* initialize operations to NULL */

  /* constructors, destructors, and utility operations */
  ops->nvgetvectorid     = NULL;
  ops->nvclone           = NULL;
  ops->nvcloneempty      = NULL;
  ops->nvdestroy         = NULL;
  ops->nvspace           = NULL;
  ops->nvgetarraypointer = NULL;
  ops->nvsetarraypointer = NULL;
  ops->nvgetcommunicator = NULL;
  ops->nvgetlength       = NULL;

  /* standard vector operations */
  ops->nvlinearsum    = NULL;
  ops->nvconst        = NULL;
  ops->nvprod         = NULL;
  ops->nvdiv          = NULL;
  ops->nvscale        = NULL;
  ops->nvabs          = NULL;
  ops->nvinv          = NULL;
  ops->nvaddconst     = NULL;
  ops->nvdotprod      = NULL;
  ops->nvmaxnorm      = NULL;
  ops->nvwrmsnormmask = NULL;
  ops->nvwrmsnorm     = NULL;
  ops->nvmin          = NULL;
  ops->nvwl2norm      = NULL;
  ops->nvl1norm       = NULL;
  ops->nvcompare      = NULL;
  ops->nvinvtest      = NULL;
  ops->nvconstrmask   = NULL;
  ops->nvminquotient  = NULL;

  /* fused vector operations (optional) */
  ops->nvlinearcombination = NULL;
  ops->nvscaleaddmulti     = NULL;
  ops->nvdotprodmulti      = NULL;

  /* vector array operations (optional) */
  ops->nvlinearsumvectorarray         = NULL;
  ops->nvscalevectorarray             = NULL;
  ops->nvconstvectorarray             = NULL;
  ops->nvwrmsnormvectorarray          = NULL;
  ops->nvwrmsnormmaskvectorarray      = NULL;
  ops->nvscaleaddmultivectorarray     = NULL;
  ops->nvlinearcombinationvectorarray = NULL;

  /* local reduction operations (optional) */
  ops->nvdotprodlocal     = NULL;
  ops->nvmaxnormlocal     = NULL;
  ops->nvminlocal         = NULL;
  ops->nvl1normlocal      = NULL;
  ops->nvinvtestlocal     = NULL;
  ops->nvconstrmasklocal  = NULL;
  ops->nvminquotientlocal = NULL;
  ops->nvwsqrsumlocal     = NULL;
  ops->nvwsqrsummasklocal = NULL;

  /* attach ops and initialize content to NULL */
  v->ops     = ops;
  v->content = NULL;

  return(v);
}


/* -----------------------------------------------------------------
 * Free a generic N_Vector (assumes content is already empty)
 * -----------------------------------------------------------------*/

void N_VFreeEmpty(N_Vector v)
{
  if (v == NULL)  return;

  /* free non-NULL ops structure */
  if (v->ops)  free(v->ops);
  v->ops = NULL;

  /* free overall N_Vector object and return */
  free(v);
  return;
}


/* -----------------------------------------------------------------
 * Copy a vector 'ops' structure
 * -----------------------------------------------------------------*/

int N_VCopyOps(N_Vector w, N_Vector v)
{
  /* Check that ops structures exist */
  if (w == NULL || v == NULL) return(-1);
  if (w->ops == NULL || v->ops == NULL) return(-1);

  /* Copy ops from w to v */

  /* constructors, destructors, and utility operations */
  v->ops->nvgetvectorid     = w->ops->nvgetvectorid;
  v->ops->nvclone           = w->ops->nvclone;
  v->ops->nvcloneempty      = w->ops->nvcloneempty;
  v->ops->nvdestroy         = w->ops->nvdestroy;
  v->ops->nvspace           = w->ops->nvspace;
  v->ops->nvgetarraypointer = w->ops->nvgetarraypointer;
  v->ops->nvsetarraypointer = w->ops->nvsetarraypointer;
  v->ops->nvgetcommunicator = w->ops->nvgetcommunicator;
  v->ops->nvgetlength       = w->ops->nvgetlength;

  /* standard vector operations */
  v->ops->nvlinearsum    = w->ops->nvlinearsum;
  v->ops->nvconst        = w->ops->nvconst;
  v->ops->nvprod         = w->ops->nvprod;
  v->ops->nvdiv          = w->ops->nvdiv;
  v->ops->nvscale        = w->ops->nvscale;
  v->ops->nvabs          = w->ops->nvabs;
  v->ops->nvinv          = w->ops->nvinv;
  v->ops->nvaddconst     = w->ops->nvaddconst;
  v->ops->nvdotprod      = w->ops->nvdotprod;
  v->ops->nvmaxnorm      = w->ops->nvmaxnorm;
  v->ops->nvwrmsnormmask = w->ops->nvwrmsnormmask;
  v->ops->nvwrmsnorm     = w->ops->nvwrmsnorm;
  v->ops->nvmin          = w->ops->nvmin;
  v->ops->nvwl2norm      = w->ops->nvwl2norm;
  v->ops->nvl1norm       = w->ops->nvl1norm;
  v->ops->nvcompare      = w->ops->nvcompare;
  v->ops->nvinvtest      = w->ops->nvinvtest;
  v->ops->nvconstrmask   = w->ops->nvconstrmask;
  v->ops->nvminquotient  = w->ops->nvminquotient;

  /* fused vector operations */
  v->ops->nvlinearcombination = w->ops->nvlinearcombination;
  v->ops->nvscaleaddmulti     = w->ops->nvscaleaddmulti;
  v->ops->nvdotprodmulti      = w->ops->nvdotprodmulti;

  /* vector array operations */
  v->ops->nvlinearsumvectorarray         = w->ops->nvlinearsumvectorarray;
  v->ops->nvscalevectorarray             = w->ops->nvscalevectorarray;
  v->ops->nvconstvectorarray             = w->ops->nvconstvectorarray;
  v->ops->nvwrmsnormvectorarray          = w->ops->nvwrmsnormvectorarray;
  v->ops->nvwrmsnormmaskvectorarray      = w->ops->nvwrmsnormmaskvectorarray;
  v->ops->nvscaleaddmultivectorarray     = w->ops->nvscaleaddmultivectorarray;
  v->ops->nvlinearcombinationvectorarray = w->ops->nvlinearcombinationvectorarray;

  /* local reduction operations */
  v->ops->nvdotprodlocal     = w->ops->nvdotprodlocal;
  v->ops->nvmaxnormlocal     = w->ops->nvmaxnormlocal;
  v->ops->nvminlocal         = w->ops->nvminlocal;
  v->ops->nvl1normlocal      = w->ops->nvl1normlocal;
  v->ops->nvinvtestlocal     = w->ops->nvinvtestlocal;
  v->ops->nvconstrmasklocal  = w->ops->nvconstrmasklocal;
  v->ops->nvminquotientlocal = w->ops->nvminquotientlocal;
  v->ops->nvwsqrsumlocal     = w->ops->nvwsqrsumlocal;
  v->ops->nvwsqrsummasklocal = w->ops->nvwsqrsummasklocal;

  return(0);
}

/* -----------------------------------------------------------------
 * Functions in the 'ops' structure
 * -----------------------------------------------------------------*/

N_Vector_ID N_VGetVectorID(N_Vector w)
{
  return(w->ops->nvgetvectorid(w));
}

N_Vector N_VClone(N_Vector w)
{
  return(w->ops->nvclone(w));
}

N_Vector N_VCloneEmpty(N_Vector w)
{
  return(w->ops->nvcloneempty(w));
}

void N_VDestroy(N_Vector v)
{
  if (v == NULL) return;

  /* if the destroy operation exists use it */
  if (v->ops)
    if (v->ops->nvdestroy) { v->ops->nvdestroy(v); return; }

  /* if we reach this point, either ops == NULL or nvdestroy == NULL,
     try to cleanup by freeing the content, ops, and vector */
  if (v->content) { free(v->content); v->content = NULL; }
  if (v->ops) { free(v->ops); v->ops = NULL; }
  free(v); v = NULL;

  return;
}

void N_VSpace(N_Vector v, sunindextype *lrw, sunindextype *liw)
{
  v->ops->nvspace(v, lrw, liw);
  return;
}

realtype *N_VGetArrayPointer(N_Vector v)
{
  return((realtype *) v->ops->nvgetarraypointer(v));
}

void N_VSetArrayPointer(realtype *v_data, N_Vector v)
{
  v->ops->nvsetarraypointer(v_data, v);
  return;
}

void *N_VGetCommunicator(N_Vector v)
{
  if (v->ops->nvgetcommunicator)
    return(v->ops->nvgetcommunicator(v));
  else
    return(NULL);
}

sunindextype N_VGetLength(N_Vector v)
{
  return((sunindextype) v->ops->nvgetlength(v));
}

/* -----------------------------------------------------------------
 * standard vector operations
 * -----------------------------------------------------------------*/

void N_VLinearSum(realtype a, N_Vector x, realtype b, N_Vector y, N_Vector z)
{
  z->ops->nvlinearsum(a, x, b, y, z);
  return;
}

void N_VConst(realtype c, N_Vector z)
{
  z->ops->nvconst(c, z);
  return;
}

void N_VProd(N_Vector x, N_Vector y, N_Vector z)
{
  z->ops->nvprod(x, y, z);
  return;
}

void N_VDiv(N_Vector x, N_Vector y, N_Vector z)
{
  z->ops->nvdiv(x, y, z);
  return;
}

void N_VScale(realtype c, N_Vector x, N_Vector z)
{
  z->ops->nvscale(c, x, z);
  return;
}

void N_VAbs(N_Vector x, N_Vector z)
{
  z->ops->nvabs(x, z);
  return;
}

void N_VInv(N_Vector x, N_Vector z)
{
  z->ops->nvinv(x, z);
  return;
}

void N_VAddConst(N_Vector x, realtype b, N_Vector z)
{
  z->ops->nvaddconst(x, b, z);
  return;
}

realtype N_VDotProd(N_Vector x, N_Vector y)
{
  return((realtype) y->ops->nvdotprod(x, y));
}

realtype N_VMaxNorm(N_Vector x)
{
  return((realtype) x->ops->nvmaxnorm(x));
}

realtype N_VWrmsNorm(N_Vector x, N_Vector w)
{
  return((realtype) x->ops->nvwrmsnorm(x, w));
}

realtype N_VWrmsNormMask(N_Vector x, N_Vector w, N_Vector id)
{
  return((realtype) x->ops->nvwrmsnormmask(x, w, id));
}

realtype N_VMin(N_Vector x)
{
  return((realtype) x->ops->nvmin(x));
}

realtype N_VWL2Norm(N_Vector x, N_Vector w)
{
  return((realtype) x->ops->nvwl2norm(x, w));
}

realtype N_VL1Norm(N_Vector x)
{
  return((realtype) x->ops->nvl1norm(x));
}

void N_VCompare(realtype c, N_Vector x, N_Vector z)
{
  z->ops->nvcompare(c, x, z);
  return;
}

booleantype N_VInvTest(N_Vector x, N_Vector z)
{
  return((booleantype) z->ops->nvinvtest(x, z));
}

booleantype N_VConstrMask(N_Vector c, N_Vector x, N_Vector m)
{
  return((booleantype) x->ops->nvconstrmask(c, x, m));
}

realtype N_VMinQuotient(N_Vector num, N_Vector denom)
{
  return((realtype) num->ops->nvminquotient(num, denom));
}

/* -----------------------------------------------------------------
 * OPTIONAL fused vector operations
 * -----------------------------------------------------------------*/

int N_VLinearCombination(int nvec, realtype* c, N_Vector* X, N_Vector z)
{
  int i;
  realtype ONE=RCONST(1.0);

  if (z->ops->nvlinearcombination != NULL) {

    return(z->ops->nvlinearcombination(nvec, c, X, z));

  } else {

    z->ops->nvscale(c[0], X[0], z);
    for (i=1; i<nvec; i++) {
      z->ops->nvlinearsum(c[i], X[i], ONE, z, z);
    }
    return(0);

  }
}

int N_VScaleAddMulti(int nvec, realtype* a, N_Vector x, N_Vector* Y, N_Vector* Z)
{
  int i;
  realtype ONE=RCONST(1.0);

  if (x->ops->nvscaleaddmulti != NULL) {

    return(x->ops->nvscaleaddmulti(nvec, a, x, Y, Z));

  } else {

    for (i=0; i<nvec; i++) {
      x->ops->nvlinearsum(a[i], x, ONE, Y[i], Z[i]);
    }
    return(0);

  }
}

int N_VDotProdMulti(int nvec, N_Vector x, N_Vector* Y, realtype* dotprods)
{
  int i;

  if (x->ops->nvdotprodmulti != NULL) {

    return(x->ops->nvdotprodmulti(nvec, x, Y, dotprods));

  } else {

    for (i=0; i<nvec; i++) {
      dotprods[i] = x->ops->nvdotprod(x, Y[i]);
    }
    return(0);

  }
}

/* -----------------------------------------------------------------
 * OPTIONAL vector array operations
 * -----------------------------------------------------------------*/

int N_VLinearSumVectorArray(int nvec, realtype a, N_Vector* X,
                            realtype b, N_Vector* Y, N_Vector* Z)
{
  int i;

  if (Z[0]->ops->nvlinearsumvectorarray != NULL) {

    return(Z[0]->ops->nvlinearsumvectorarray(nvec, a, X, b, Y, Z));

  } else {

    for (i=0; i<nvec; i++) {
      Z[0]->ops->nvlinearsum(a, X[i], b, Y[i], Z[i]);
    }
    return(0);

  }
}

int N_VScaleVectorArray(int nvec, realtype* c, N_Vector* X, N_Vector* Z)
{
  int i;

  if (Z[0]->ops->nvscalevectorarray != NULL) {

    return(Z[0]->ops->nvscalevectorarray(nvec, c, X, Z));

  } else {

    for (i=0; i<nvec; i++) {
      Z[0]->ops->nvscale(c[i], X[i], Z[i]);
    }
    return(0);

  }
}

int N_VConstVectorArray(int nvec, realtype c, N_Vector* Z)
{
  int i, ier;

  if (Z[0]->ops->nvconstvectorarray != NULL) {

    ier = Z[0]->ops->nvconstvectorarray(nvec, c, Z);
    return(ier);

  } else {

    for (i=0; i<nvec; i++) {
      Z[0]->ops->nvconst(c, Z[i]);
    }
    return(0);

  }
}

int N_VWrmsNormVectorArray(int nvec, N_Vector* X, N_Vector* W, realtype* nrm)
{
  int i, ier;

  if (X[0]->ops->nvwrmsnormvectorarray != NULL) {

    ier = X[0]->ops->nvwrmsnormvectorarray(nvec, X, W, nrm);
    return(ier);

  } else {

    for (i=0; i<nvec; i++) {
      nrm[i] = X[0]->ops->nvwrmsnorm(X[i], W[i]);
    }
    return(0);

  }
}

int N_VWrmsNormMaskVectorArray(int nvec, N_Vector* X, N_Vector* W, N_Vector id,
                               realtype* nrm)
{
  int i, ier;

  if (id->ops->nvwrmsnormmaskvectorarray != NULL) {

    ier = id->ops->nvwrmsnormmaskvectorarray(nvec, X, W, id, nrm);
    return(ier);

  } else {

    for (i=0; i<nvec; i++) {
      nrm[i] = id->ops->nvwrmsnormmask(X[i], W[i], id);
    }
    return(0);

  }
}

int N_VScaleAddMultiVectorArray(int nvec, int nsum, realtype* a, N_Vector* X,
                                 N_Vector** Y, N_Vector** Z)
{
  int        i, j;
  int        ier=0;
  realtype   ONE=RCONST(1.0);
  N_Vector* YY=NULL;
  N_Vector* ZZ=NULL;

  if (X[0]->ops->nvscaleaddmultivectorarray != NULL) {

    ier = X[0]->ops->nvscaleaddmultivectorarray(nvec, nsum, a, X, Y, Z);
    return(ier);

  } else if (X[0]->ops->nvscaleaddmulti != NULL ) {

    /* allocate arrays of vectors */
    YY = (N_Vector*) malloc(nsum * sizeof(N_Vector));
    ZZ = (N_Vector*) malloc(nsum * sizeof(N_Vector));

    for (i=0; i<nvec; i++) {

      for (j=0; j<nsum; j++) {
        YY[j] = Y[j][i];
        ZZ[j] = Z[j][i];
      }

      ier = X[0]->ops->nvscaleaddmulti(nsum, a, X[i], YY, ZZ);
      if (ier != 0) break;
    }

    /* free array of vectors */
    free(YY);
    free(ZZ);

    return(ier);

  } else {

    for (i=0; i<nvec; i++) {
      for (j=0; j<nsum; j++) {
        X[0]->ops->nvlinearsum(a[j], X[i], ONE, Y[j][i], Z[j][i]);
      }
    }
    return(0);
  }
}

int N_VLinearCombinationVectorArray(int nvec, int nsum, realtype* c,
                                    N_Vector** X, N_Vector* Z)
{
  int        i, j;
  int        ier=0;
  realtype   ONE=RCONST(1.0);
  N_Vector* Y=NULL;

  if (Z[0]->ops->nvlinearcombinationvectorarray != NULL) {

    ier = Z[0]->ops->nvlinearcombinationvectorarray(nvec, nsum, c, X, Z);
    return(ier);

  } else if (Z[0]->ops->nvlinearcombination != NULL ) {

    /* allocate array of vectors */
    Y = (N_Vector* ) malloc(nsum * sizeof(N_Vector));

    for (i=0; i<nvec; i++) {

      for (j=0; j<nsum; j++) {
        Y[j] = X[j][i];
      }

      ier = Z[0]->ops->nvlinearcombination(nsum, c, Y, Z[i]);
      if (ier != 0) break;
    }

    /* free array of vectors */
    free(Y);

    return(ier);

  } else {

    for (i=0; i<nvec; i++) {
      Z[0]->ops->nvscale(c[0], X[0][i], Z[i]);
      for (j=1; j<nsum; j++) {
        Z[0]->ops->nvlinearsum(c[j], X[j][i], ONE, Z[i], Z[i]);
      }
    }
    return(0);
  }
}

/* -----------------------------------------------------------------
 * OPTIONAL local reduction kernels (no parallel communication)
 * -----------------------------------------------------------------*/

realtype N_VDotProdLocal(N_Vector x, N_Vector y)
{
  return((realtype) y->ops->nvdotprodlocal(x, y));
}

realtype N_VMaxNormLocal(N_Vector x)
{
  return((realtype) x->ops->nvmaxnormlocal(x));
}

realtype N_VMinLocal(N_Vector x)
{
  return((realtype) x->ops->nvminlocal(x));
}

realtype N_VL1NormLocal(N_Vector x)
{
  return((realtype) x->ops->nvl1normlocal(x));
}

realtype N_VWSqrSumLocal(N_Vector x, N_Vector w)
{
  return((realtype) x->ops->nvwsqrsumlocal(x,w));
}

realtype N_VWSqrSumMaskLocal(N_Vector x, N_Vector w, N_Vector id)
{
  return((realtype) x->ops->nvwsqrsummasklocal(x,w,id));
}

booleantype N_VInvTestLocal(N_Vector x, N_Vector z)
{
  return((booleantype) z->ops->nvinvtestlocal(x,z));
}

booleantype N_VConstrMaskLocal(N_Vector c, N_Vector x, N_Vector m)
{
  return((booleantype) x->ops->nvconstrmasklocal(c,x,m));
}

realtype N_VMinQuotientLocal(N_Vector num, N_Vector denom)
{
  return((realtype) num->ops->nvminquotientlocal(num,denom));
}

/* -----------------------------------------------------------------
 * Additional functions exported by the generic NVECTOR:
 *   N_VNewVectorArray
 *   N_VCloneEmptyVectorArray
 *   N_VCloneVectorArray
 *   N_VDestroyVectorArray
 * -----------------------------------------------------------------*/
N_Vector* N_VNewVectorArray(int count)
{
  N_Vector* vs = NULL;

  if (count <= 0) return(NULL);

  vs = (N_Vector* ) malloc(count * sizeof(N_Vector));
  if(vs == NULL) return(NULL);

  return(vs);
}

N_Vector* N_VCloneEmptyVectorArray(int count, N_Vector w)
{
  N_Vector* vs = NULL;
  int j;

  if (count <= 0) return(NULL);

  vs = (N_Vector* ) malloc(count * sizeof(N_Vector));
  if(vs == NULL) return(NULL);

  for (j = 0; j < count; j++) {
    vs[j] = N_VCloneEmpty(w);
    if (vs[j] == NULL) {
      N_VDestroyVectorArray(vs, j-1);
      return(NULL);
    }
  }

  return(vs);
}

N_Vector* N_VCloneVectorArray(int count, N_Vector w)
{
  N_Vector* vs = NULL;
  int j;

  if (count <= 0) return(NULL);

  vs = (N_Vector* ) malloc(count * sizeof(N_Vector));
  if(vs == NULL) return(NULL);

  for (j = 0; j < count; j++) {
    vs[j] = N_VClone(w);
    if (vs[j] == NULL) {
      N_VDestroyVectorArray(vs, j-1);
      return(NULL);
    }
  }

  return(vs);
}

void N_VDestroyVectorArray(N_Vector* vs, int count)
{
  int j;

  if (vs==NULL) return;

  for (j = 0; j < count; j++) N_VDestroy(vs[j]);

  free(vs); vs = NULL;

  return;
}

/* These function are really only for users of the Fortran interface */
N_Vector N_VGetVecAtIndexVectorArray(N_Vector* vs, int index)
{
  if (vs==NULL)       return NULL;
  else if (index < 0) return NULL;
  else                return vs[index];
}

void N_VSetVecAtIndexVectorArray(N_Vector* vs, int index, N_Vector w)
{
  if (vs==NULL)       return;
  else if (index < 0) return;
  else                vs[index] = w;
}
