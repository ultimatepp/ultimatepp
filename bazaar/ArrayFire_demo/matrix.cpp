#include <Core/Core.h>

using namespace Upp;

#include <arrayfire.h>

using namespace af; 

static array A, B; 			// populated before each timing

static void MatMult_device() {
    array C = matmul(A, B); // matrix multiply
    C.eval();               // ensure evaluated
}

static Buffer<float> Ah, Bh;
static int hsize;

void MatMult_host() {
	Buffer<float> c(hsize*hsize);
	
	for (int row = 0; row < hsize; ++row) {
 		for (int col = 0; col < hsize; ++col) {
			float sum = 0;
			for (int inner = 0; inner < hsize; ++inner) 
				sum += Ah[row*hsize + inner] * Bh[inner*hsize + col];
			c[row*hsize + col] = sum;
		}
	}
}

void MatMult_Bench() {
    printf("\nBenchmark N-by-N matrix multiply");
    
  	double peak = 0;
    for (int n = 200; n <= 2000; n += 200) {
        printf("\n%4d x %4d: ", n, n);
        A = randu(n, n, f32);
        B = randu(n, n, f32);
        double time = timeit(MatMult_device); // time in seconds
        double gflops = 2.0 * pow(n, 3) / (time * 1e9);
        if (gflops > peak)
            peak = gflops;

        printf(" %.5f secs, %4.0f Gflops", time, gflops);

		hsize = n;
		Ah.Alloc(n*n);
		Bh.Alloc(n*n);
		for (int i = 0; i < hsize; ++i) {
			Ah[i] = (float)Randomf();
			Bh[i] = (float)Randomf();
		}
		double timeh = timeit(MatMult_host); 
		printf(". Host: %.5f secs, %3.0f Host/Device", timeh, timeh/time);
        fflush(stdout);
    }
    printf("\nPeak %g GFLOPS\n", peak);
}