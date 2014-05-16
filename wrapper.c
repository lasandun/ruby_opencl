#include "ruby.h"
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

VALUE ClModule = Qnil;

void Init_Opencl_integration();

VALUE method_integrate(VALUE self, VALUE a, VALUE b, VALUE n, VALUE f);

// The initialization method for this module
void Init_Opencl_integration() {
	ClModule = rb_define_module("Opencl_integration");
	rb_define_method(ClModule, "integrate", method_integrate, 4);	
}

VALUE method_integrate(VALUE self, VALUE a, VALUE b, VALUE n, VALUE f) {
    void    *handle;
    float (*fptr)(float, float, float, char*);

    // open the needed object
    handle = dlopen("./cl/cl.so", RTLD_LOCAL | RTLD_LAZY);
    if(handle == NULL) {
        printf("error opening shared library!\n");
        exit(0);
	}

    // find the address of function and data objects
    *(void **)(&fptr) = dlsym(handle, "util_integrate");
    if(fptr == NULL) {
        printf("function not found in shared library!\n");
        exit(0);
	}

    // calculate integration and return result
    double aa, bb, nn;
    char* ff = StringValuePtr(f);
    aa = NUM2DBL(a);
    bb = NUM2DBL(b);
    nn = NUM2DBL(n);
    double res = (*fptr)(aa, bb, nn, ff);
	return DBL2NUM(res);
}
