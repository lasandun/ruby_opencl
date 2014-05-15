all: cl.so

cl.so: host.o
	gcc -shared -o cl.so host.o -lOpenCL

host.o: host.c
	gcc -fpic -c host.c

clean:
	rm cl.so host.o
