#include <Core/Core.h>

using namespace Upp;

#include <arrayfire.h>

using namespace af; 


static array A, B;

static array dist_naive(array a, array b) {
    array dist_mat = constant(0, a.dims(1), b.dims(1));

    for (int ii = 0; ii < a.dims(1); ii++) 			// Iterate through columns a
        for (int jj = 0; jj < b.dims(1); jj++) 		// Iterate through columns of b
            for (int kk = 0; kk < a.dims(0); kk++) 	// Get the sum of absolute differences
                dist_mat(ii, jj) += abs(a(kk, ii) - b(kk, jj));

    return dist_mat;
}

static array dist_vec(array a, array b) {
    array dist_mat = constant(0, a.dims(1), b.dims(1));

    for (int ii = 0; ii < a.dims(1); ii++) {		// Iterate through columns a
        array avec = a(span, ii);

        for (int jj = 0; jj < b.dims(1); jj++) {	// Iterate through columns of b
            array bvec = b(span, jj);

            dist_mat(ii, jj) = sum(abs(avec - bvec));// Get SAD using sum on the vector
        }
    }
    return dist_mat;
}

static array dist_gfor1(array a, array b) {
    array dist_mat = constant(0, a.dims(1), b.dims(1));

    gfor (seq ii, a.dims(1)) {						// GFOR along columns of a
        array avec = a(span, ii);

        for (int jj = 0; jj < b.dims(1); jj++) {	// Itere through columns of b
            array bvec = b(span, jj);

            dist_mat(ii, jj) = sum(abs(avec - bvec));// Get SAD using sum on the vector
        }
    }
    return dist_mat;
}

static array dist_gfor2(array a, array b) {
    array dist_mat = constant(0, a.dims(1), b.dims(1));

    gfor (seq jj, b.dims(1)) {						// GFOR along columns of b
        array bvec = b(span, jj);

        for (int ii = 0; ii < a.dims(1); ii++) {	// Iterate through columns of A
            array avec = a(span, ii);

            dist_mat(ii, jj) = sum(abs(avec - bvec));// Get SAD using sum on the vector
        }
    }
    return dist_mat;
}

static array dist_tile1(array a, array b) {
    int alen = a.dims(1);
    int blen = b.dims(1);

    array dist_mat = constant(0, alen, blen);

    for (int jj = 0; jj < blen; jj++) {    			// Iterate through columns of b
        											// Get the column vector of b
        array bvec = b(span, jj);					// shape of bvec is (feat_len, 1)
        											// Tile avec to be same size as a
        array bvec_tiled = tile(bvec, 1, alen);		// shape of bvec_tiled is (feat_len, alen)

        array sad = sum(abs(bvec_tiled - a));		// Get the sum of absolute differences

        											// sad is row vector, dist_mat needs column vector
        dist_mat(span, jj) = sad.T();				// transpose sad and fill in dist_mat
    }
    return dist_mat;
}

static array dist_tile2(array a, array b) {
    int feat_len = a.dims(0);
    int alen = a.dims(1);
    int blen = b.dims(1);
    
    array a_mod = a;								// Shape of a is (feat_len, alen, 1)

    array b_mod = moddims(b, feat_len, 1, blen);	// Reshape b from (feat_len, blen) to (feat_len, 1, blen)
    
    array a_tiled = tile(a_mod, 1, 1, blen);		// Tile both matrices to be (feat_len, alen, blen)
    array b_tiled = tile(b_mod, 1, alen, 1);
    												// Do The sum operation along first dimension
    array dist_mod = sum(abs(a_tiled - b_tiled));	// Output is of shape (1, alen, blen)

    array dist_mat = moddims(dist_mod, alen, blen);	// Reshape dist_mat from (1, alen, blen) to (alen, blen)
    return dist_mat;
}

static void bench_naive()	{dist_naive(A, B);}
static void bench_vec()		{dist_vec(A, B);}
static void bench_gfor1()	{dist_gfor1(A, B);}
static void bench_gfor2()	{dist_gfor2(A, B);}
static void bench_tile1()	{dist_tile1(A, B);}
static void bench_tile2()	{dist_tile2(A, B);}

void Vectorize_Bench() {
	printf("\nVectorize demo:");
    // Do not increase the sizes
    // dist_naive and dist_vec get too slow at large sizes
    A = randu(3, 200);
    B = randu(3, 300);

    array d1 = dist_naive(A, B);
    array d2 = dist_vec  (A, B);
    array d3 = dist_gfor1(A, B);
    array d4 = dist_gfor2(A, B);
    array d5 = dist_tile1(A, B);
    array d6 = dist_tile2(A, B);

    printf("\nMax. Error for dist_vec  : %f", max<float>(abs(d1 - d2)));
    printf("\nMax. Error for dist_gfor1: %f", max<float>(abs(d1 - d3)));
    printf("\nMax. Error for dist_gfor2: %f", max<float>(abs(d1 - d4)));
    printf("\nMax. Error for dist_tile1: %f", max<float>(abs(d1 - d5)));
    printf("\nMax. Error for dist_tile2: %f", max<float>(abs(d1 - d6)));
    printf("\n");
    printf("\nTime for dist_naive: %2.2fms", 1000 * timeit(bench_naive));
    printf("\nTime for dist_vec  : %2.2fms", 1000 * timeit(bench_vec  ));
    printf("\nTime for dist_gfor1: %2.2fms", 1000 * timeit(bench_gfor1));
    printf("\nTime for dist_gfor2: %2.2fms", 1000 * timeit(bench_gfor2));
    printf("\nTime for dist_tile1: %2.2fms", 1000 * timeit(bench_tile1));
    printf("\nTime for dist_tile2: %2.2fms", 1000 * timeit(bench_tile2));
}
