all: cl.so

CC=gcc
CL_HEADERS_PATH=
LIBOPENCL_PATH=

cl.so: host.o
	$(CC) -shared -o cl.so host.o -I $(CL_HEADERS_PATH) -L $(LIBOPENCL_PATH) -lOpenCL

host.o: host.c
	$(CC) -fpic -c host.c

clean:
	rm cl.so host.o
