CC=gcc
LIBB = /usr/local/lib/libmill/
INCLUDES = /usr/local/lib/libmill/
CFLAGS += -I$(INCLUDES)
LDFLAGS += -L$(LIBB)
LDFLAGS += -lmill

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