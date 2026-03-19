CC = gcc
CFLAGS = -Wall -g

all: ex1 ex2 ex3 controlador minishell auxiliares

ex1: ex1.o

ex1.o: ex1.c

ex2: ex2.o

ex2.o: ex2.c

mysystem.o: mysystem.c mysystem.h

ex3: ex3.o mysystem.o

ex3.o: ex3.c mysystem.h

controlador: controlador.o mysystem.o

controlador.o: controlador.c mysystem.h

minishell: minishell.o

minishell.o: minishell.c

auxiliares: auxiliar.c
	$(CC) $(CFLAGS) auxiliar.c -o a.out
	$(CC) $(CFLAGS) auxiliar.c -o b.out
	$(CC) $(CFLAGS) auxiliar.c -o c.out

clean:
	rm -rf *.dSYM *.o ex1 ex2 ex3 controlador minishell a.out b.out c.out

