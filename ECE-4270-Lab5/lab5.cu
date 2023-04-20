#include <stdio.h>

__global__ void add_matrix(float *d_out, float *d_a, float * d_b){
	int idx = threadIdx.x;
	float a = d_a[idx];
	float b = d_b[idx];
	d_out[idx] = a + b;
}

int main(int argc, char ** argv){
	const int ARRAY_SIZE = 4;
	const int ARRAY_BYTES = ARRAY_SIZE * sizeof(float);
	
	float h_in[ARRAY_SIZE];
	for (int i=0; i < ARRAY_SIZE; i++){
		h_in[i] = float(i);
	}
    float h_in2[ARRAY_SIZE];
	for (int i=0; i < ARRAY_SIZE; i++){
		h_in2[i] = float(i + 4);
	}
	float h_out[ARRAY_SIZE];
    printf("Input matrix 1:\n");
    for (int i =0; i<ARRAY_SIZE; i++){
		printf("%.2f", h_in[i]);
		printf(((i % 2) !=1)? "\t": "\n");
	}
    printf("\nInput matrix 2:\n");
    for (int i =0; i<ARRAY_SIZE; i++){
		printf("%.2f", h_in2[i]);
		printf(((i % 2) !=1)? "\t": "\n");
	}

	float *d_in;
	float *d_in2;
	float *d_out;

	cudaMalloc((void**) &d_in, ARRAY_BYTES); 
	cudaMalloc((void**) &d_in2, ARRAY_BYTES); 
	cudaMalloc((void**) &d_out, ARRAY_BYTES); 

	cudaMemcpy(d_in, h_in, ARRAY_BYTES, cudaMemcpyHostToDevice);
	cudaMemcpy(d_in2, h_in2, ARRAY_BYTES, cudaMemcpyHostToDevice);

	add_matrix<<<1, ARRAY_SIZE>>>(d_out, d_in, d_in2);

	cudaMemcpy(h_out, d_out, ARRAY_BYTES, cudaMemcpyDeviceToHost);

    printf("\nOutput matrix:\n");
	for (int i =0; i<ARRAY_SIZE; i++){
		printf("%.2f", h_out[i]);
		printf(((i % 2) !=1)? "\t": "\n");
	}
	
	cudaFree(d_in);
	cudaFree(d_in2);
	cudaFree(d_out);

	return 0;
}