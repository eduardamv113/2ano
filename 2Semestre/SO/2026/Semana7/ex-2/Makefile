all: server client

server: server.c vector.c vector.h
	gcc -o server server.c vector.c

client: client.c
	gcc -o client client.c

clean:
	rm -f server client *.o fifo_request fifo_response