CC=gcc
LIBB = python3.6m
INCLUDES = /usr/include/python3.6m
CFLAGS += -I$(INCLUDES)
LDFLAGS += -l$(LIBB)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: main.o functions.o caller.o helloworld.o
	$(CC) -o main main.c functions.c caller.c helloworld.c $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o
	rm -f main

cythonize:
	python3 setup.py build_ext --inplace

run:
	make
	./main

run_cythonize:
	python3 setup.py build_ext --inplace
	make
	./main
