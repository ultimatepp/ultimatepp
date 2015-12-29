#include <Core/Core.h>

using namespace Upp;

#include <arrayfire.h>

using namespace af; 

static array A, B; // populated before each timing

static void MatMult() {
    array C = matmul(A, B);  // matrix multiply
    C.eval();                // ensure evaluated
}

void MatMult_Bench() {
    printf("\nBenchmark N-by-N matrix multiply");
    
  	double peak = 0;
    for (int n = 200; n <= 2000; n += 200) {
        printf("\n%4d x %4d: ", n, n);
        A = randu(n, n, f32);
        B = randu(n, n, f32);
        double time = timeit(MatMult); // time in seconds
        double gflops = 2.0 * pow(n, 3) / (time * 1e9);
        if (gflops > peak)
            peak = gflops;

        printf(" %f secs, %4.0f Gflops", time, gflops);
        fflush(stdout);
    }
    printf("\nPeak %g GFLOPS\n", peak);
}