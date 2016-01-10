#include <OpenMP_demo/OpenMP_demo.h>

static long samples = 100000000;

static double pi_device() {
	double x, y;
	long count = 0, i;
	
	// Parallel loop with reduction for calculating PI  
	#pragma omp parallel for private(i, x, y) shared (samples) reduction(+:count) 
    for (i = 0; i < samples; ++i) {
        x = Random(1000000)/1000000.;
        y = Random(1000000)/1000000.;
        if (sqrt(x*x + y*y) < 1)
            count++;
    }
    return 4.0 * count / samples;
    
}

static double pi_host() {
	double x, y;
    long count = 0;
    for (long i = 0; i < samples; ++i) {
        x = Random(1000000)/1000000.;
        y = Random(1000000)/1000000.;
        if (sqrt(x*x + y*y) < 1)
            count++;
    }
    return 4.0 * count / samples;
}

// void wrappers for timeit()
static void device_wrapper() 	{ pi_device(); }
static void host_wrapper() 		{ pi_host(); }

void Pi_Bench(int nThreads) {
	omp_set_num_threads(nThreads);
	double t_device = timeIt(device_wrapper);
	double t_host = timeIt(host_wrapper);
	printf("\nPI number benchmark\n");
	omp_set_num_threads(nThreads);
  	printf("OpenMP: %.5f seconds to estimate pi = %.8f\n", t_device, pi_device());
   	printf("  host: %.5f seconds to estimate pi = %.8f\n", t_host, pi_host());
   	printf("%d threads is %.2f times faster than 1 thread\n", nThreads, t_host/t_device);
}