CC = gcc
CFLAGS = -Wall -g

all: server client create_fifo write_fifo read_fifo

server: server.o vector.o

client: client.o

server.o: server.c defs.h vector.h

client.o: client.c defs.h

vector.o: vector.c vector.h

create_fifo: create_fifo.c
	gcc -o create_fifo create_fifo.c

write_fifo: write_fifo.c
	gcc -o write_fifo write_fifo.c

read_fifo: read_fifo.c
	gcc -o read_fifo read_fifo.c

clean:
	rm -f -r *.dSYM fifo* server client *.o create_fifo write_fifo read_fifo