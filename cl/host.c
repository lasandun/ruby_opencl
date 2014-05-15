// gcc -I . test.cpp -lOpenCL
#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_SOURCE_SIZE (0x100000)

//int main() {
//    printf("%f \n", util_integrate(0, 4, 100));
//    return 0;
//}

float util_integrate(float a, float b, float n) {
    char* source_str;
    size_t source_size;
    int i = 0;
        
    float dx = (b-a) / (n*1.0);
    float *results = (float*) malloc(n * sizeof(float));
    for(i=0; i < n; ++i)    results[i] = 0;

    FILE* fp = fopen("cl/kernel.cl", "r");
    if(fp == 0){
        printf("kernel file not found");
        exit(0);
    }

    source_str = (char*) malloc(sizeof(char) * MAX_SOURCE_SIZE);
    source_size = fread( source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose( fp );

    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;   
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);

    // create kernel
    cl_context context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret);
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);    

    cl_mem a_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float), NULL, &ret);
    cl_mem dx_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float), NULL, &ret);
    cl_mem result_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, n * sizeof(float), NULL, &ret);

    ret = clEnqueueWriteBuffer(command_queue, a_obj, CL_TRUE, 0, sizeof(int), &a, 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(command_queue, dx_obj, CL_TRUE, 0, sizeof(int), &dx, 0, NULL, NULL);

    cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

    cl_kernel kernel = clCreateKernel(program, "vector_add", &ret);
    
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_obj);    // set arguments of kernel
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&dx_obj);    // set arguments of kernel
    ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&result_obj);    // set arguments of kernel

    // execute kernel
    size_t global_item_size = n;
    size_t local_item_size = n;
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);
    
    // retrieve values from array
    ret = clEnqueueReadBuffer(command_queue, result_obj, CL_TRUE, 0, n * sizeof(int), results, 0, NULL, NULL);
    
    float res = 0;
    for(i=0; i < n; ++i) {
        res += results[i];
    }


    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(a_obj);
    ret = clReleaseMemObject(dx_obj);
    ret = clReleaseMemObject(result_obj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
    free(results);

    return res;

}
