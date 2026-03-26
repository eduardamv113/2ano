all: create_fifo write_fifo read_fifo

create_fifo: create_fifo.c
	gcc -o create_fifo create_fifo.c

write_fifo: write_fifo.c
	gcc -o write_fifo write_fifo.c

read_fifo: read_fifo.c
	gcc -o read_fifo read_fifo.c

clean:
	rm -f create_fifo write_fifo read_fifo *.o fifo