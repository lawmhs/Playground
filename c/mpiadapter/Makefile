# On gnu-linux need to explicitly add in the pthread library, you don't on the mac and mpicc does it automatically
GCC = gcc -DNOMPI
CC = mpicc

OSTYPE = $(shell uname -s)
ifeq (${OSTYPE},Darwin)
        AR = libtool -o
        CFLAGS = -g -Wall -Qunused-arguments -Wno-implicit-function-declaration -Wno-implicit-int -Wno-unused-function -Wno-parentheses-equality -Wno-tautological-compare -Wno-self-assign
        OPTFLAGS  =   -g -Wpointer-arith
else
        CFLAGS =  -g -Wall -Wpointer-arith -Wno-return-type -Wno-implicit-function-declaration -Wno-implicit-int -Wno-unused-function
        OPTFLAGS  =  -g -Wpointer-arith
        AR = ar rcs
endif

# added in flags for THREAD_SAVE and REENTRANT
OPTFLAGS  =   -g -Wpointer-arith 
# add in optimization flags -02
INCLUDES = -I. -I..
LIB	= -lpthread

# unix commands
RM        = /bin/rm -f     

.PREFIXES = .c .o         
.c.o: 
	${CC} -c ${CFLAGS} ${INCLUDES} ${LIB} $<

PROGS = chan.o queue.o iochild.o connectionmanager grequest.o netutils.o libadapter.a

all: ${PROGS}

connectionmanager: connectionmanager.c
	${CC}  -c ${CFLAGS} ${INCLUDES} ${LIB} connectionmanager.c

libadapter.a: grequest.o connectionmanager.o netutils.o iochild.o chan.o queue.o 
	${AR} $@ $^
	ranlib $@

clean:                      
	${RM} *.o 
	${RM} *.a 

py: 
	${CC} mpitest.c debug.c -ladapter -L . -o mpitest -lpthread -lpython3.6m -lm -I/usr/include/python3.6m

run:
	${CC} mpitest.c debug.c -ladapter -L . -o mpitest -lpthread -lpython3.6m -lm -I/usr/include/python3.6m
	./mpitest
