MPICC=mpicc
LIBB = python3.6m
INCLUDES = /usr/include/mpich/ -I/usr/include/python3.6/ -I/usr/local/lib/python3.5/dist-packages/numpy/core/include/
CFLAGS += -I$(INCLUDES)
LDFLAGS += -l$(LIBB)

all: main

%.o: %.c $(DEPS)
	$(MPICC) -c -o $@ $< $(CFLAGS)

main: main.o caller.o
	$(MPICC) -o main main.o caller.c $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o
	rm -f main
	rm -f sink

cythonize:
	python3 setup.py build_ext --inplace

run:
	make
	mpiexec -n 4 ./main

run_cythonize:
	python3 setup.py build_ext --inplace
	make
	mpiexec -n 4 ./main
