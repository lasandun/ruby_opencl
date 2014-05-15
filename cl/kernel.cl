float f(float x){
    return (x*2 + 2*x + 1);
}

__kernel void vector_add(__global const float *a, __global const float *dx, __global float *results) {
 
    // Get the index of the current element to be processed
    int i = get_global_id(0);
 
    // Do the operation
    results[i] = (f((*a) + (*dx) * i) + f((*a) + (*dx) * (i + 1))) * (*dx) / 2;
}

