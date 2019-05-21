CC=gcc
INCLUDES = /usr/local/lib/libaco/
CFLAGS += -I$(INCLUDES)
LDFLAGS += -lmill -luv

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

main: main.o
	$(CC) -o main main.o $(LDFLAGS)

clean:
	rm -f *.o
	rm -f main

run:
	make
	./main