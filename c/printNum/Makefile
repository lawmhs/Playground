CC=gcc
LIBB = /usr/local/lib/libtask/
INCLUDES = /usr/local/lib/libtask/ 
CFLAGS += -I$(INCLUDES)
LDFLAGS += -L$(LIBB)
LDFLAGS += -static -ltask

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: main.o sieve.o /usr/local/lib/libtask/libtask.a
	$(CC) -o main main.o sieve.o $(LDFLAGS)

clean:
	rm -f *.o
	rm -f main

run:
	make
	./main
