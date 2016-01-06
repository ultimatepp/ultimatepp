/*******************************************************
 * Copyright (c) 2014, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#include <arrayfire.h> 

using namespace af; 
 
void Demo()  { 
	printf("\n\nBasic operations in ArrayFire");
	
    printf("Create a 5-by-3 matrix of random floats on the GPU\n");
    array A = randu(5,3, f32);
    af_print(A);

    printf("Element-wise arithmetic\n");
    array B = sin(A) + 1.5;
    af_print(B);

    printf("Negate the first three elements of second column\n");
    B(seq(0, 2), 1) = B(seq(0, 2), 1) * -1;
    af_print(B);

    printf("Fourier transform the result\n");
    array C = fft(B);
    af_print(C);

    printf("Grab last row\n");
    array c = C.row(end);
    af_print(c);

    printf("Create 2-by-3 matrix from host data\n");
    float d[] = { 1, 2, 3, 4, 5, 6 };
    array D(2, 3, d, af_source::afHost);
    af_print(D);

    printf("Copy last column onto first\n");
    D.col(0) = D.col(end);
    af_print(D);

    // Sort A
    printf("Sort A and print sorted array and corresponding indices\n");
    array vals, inds;
    sort(vals, inds, A);
    af_print(vals);
    af_print(inds);
}
