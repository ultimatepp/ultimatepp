#include <OpenMP_demo/OpenMP_demo.h>

static Buffer<float> A, B;
static Buffer<float> Ah, Bh;
static int hsize;


static void MatMult_device() {
	Buffer<float> c(hsize*hsize);
	
	int	row, col, inner;
	int chunk = 10;                    /* set loop iteration chunk size */
	
	// Spawn a parallel region explicitly scoping all variables 
	#pragma omp parallel shared(hsize, chunk) private(row, col, inner)
	{
		// Do matrix multiply sharing iterations on outer loop 
		#pragma omp for schedule (static, chunk)
		for (row = 0; row < hsize; ++row) {
			for (col = 0; col < hsize; ++col) {
				float sum = 0;      
				for (inner = 0; inner < hsize; ++inner) 
					sum += A[row*hsize + inner] * B[inner*hsize + col];
				c[row*hsize + col] = sum;
			}
		}
	}
}

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

void MatMult_Bench(int nThreads) {
    printf("\nBenchmark N-by-N matrix multiply");
    
  	double peak = 0;
    for (int n = 200; n <= 2000; n += 200) {
        hsize = n;
        
        A.Alloc(n*n);
		B.Alloc(n*n);
        Ah.Alloc(n*n);
		Bh.Alloc(n*n);
		for (int i = 0; i < hsize; ++i) {
			A[i] = (float)Randomf();
			B[i] = (float)Randomf();
			Ah[i] = (float)Randomf();
			Bh[i] = (float)Randomf();
		}
		
        printf("\n%4d x %4d: ", n, n);
        omp_set_num_threads(nThreads);
        double time = timeIt(MatMult_device); 
        double timeh = timeIt(MatMult_host);
        printf("OpenMP %.4f s, 1 thread: %.4f s, %2.1f faster %d/1 threads", time, timeh, timeh/time, nThreads);
    }
    printf("\nPeak %g GFLOPS\n", peak);
}