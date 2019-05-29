MPICC?=mpicc
INCLUDES = /usr/include/mpich/
CFLAGS += -I$(INCLUDES)

all: main pingpong

%.o: %.c
	$(MPICC) -c -o $@ $< $(CFLAGS)

main: main.o
	$(MPICC) -o main main.o

pingpong: pingpong.o
	$(MPICC) -o pingpong pingpong.o


clean:
	rm -f *.o
	rm -f main
	rm -f pingpong

run:
	make
	mpiexec -n 12 ./main

run_pingpong:
	make
	mpiexec -n 2 ./pingpong