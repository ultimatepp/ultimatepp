#include <stdio.h>

__global__ void mul_a(float a, float* x)
{
	int i = threadIdx.x + blockIdx.x * blockDim.x;
	x[i] *= a;
}

void cuda_mul_a(float a, float *x, int count)
{
	// Copy data to device.
	float* device_x;

	cudaMalloc(&device_x, count * sizeof(float));
	cudaMemcpy(device_x, x, count * sizeof(float), cudaMemcpyHostToDevice);
	
	// Launch the kernel.
	mul_a<<<((count + 255) >> 8), 256>>>(a, device_x);
	
	// Copy data back to host.
	cudaDeviceSynchronize();
	cudaMemcpy(x, device_x, count * sizeof(float), cudaMemcpyDeviceToHost);
	
	cudaDeviceReset();
}
