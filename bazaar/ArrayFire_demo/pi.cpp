#include <Core/Core.h>

using namespace Upp;

#include <arrayfire.h>

using namespace af; 

// generate millions of random samples
static int samples = 40000000;

/* Self-contained code to run host and device estimates of PI.  Note that
   each is generating its own random values, so the estimates of PI
   will differ. */

static double pi_device() {
    array x = randu(samples, f32), 
    	  y = randu(samples, f32);
    return 4.0 * sum<float>(sqrt(x*x + y*y) < 1) / samples;
}

static double pi_host() {
    int count = 0;
    for (int i = 0; i < samples; ++i) {
        float x = float(rand()) / RAND_MAX;
        float y = float(rand()) / RAND_MAX;
        if (sqrt(x*x + y*y) < 1)
            count++;
    }
    return 4.0 * count / samples;
}

// void wrappers for timeit()
static void device_wrapper() 	{ pi_device(); }
static void host_wrapper() 		{ pi_host(); }

void Pi_Bench() {
	double t_device = timeit(device_wrapper);
	double t_host = timeit(host_wrapper);
	printf("\nPI number benchmark\n");
  	printf("device:  %.5f seconds to estimate  pi = %.8f\n", t_device, pi_device());
   	printf("  host:  %.5f seconds to estimate  pi = %.8f\n", t_host, pi_host());
   	printf("GPU/accelerated device is %.2f times faster than CPU host\n", t_host/t_device);
}