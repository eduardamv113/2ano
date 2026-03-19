#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
	int pd[2];
	pid_t pid;

	if (pipe(pd) == -1) {
		perror("pipe");
		return 1;
	}

	pid = fork();
	if (pid < 0) {
		perror("fork");
		close(pd[0]);
		close(pd[1]);
		return 1;
	}

	if (pid == 0) {
		int valor_recebido;
		ssize_t bytes_lidos;

		close(pd[1]);
		printf("Filho: a espera de dados no pipe...\n");

		bytes_lidos = read(pd[0], &valor_recebido, sizeof(valor_recebido));
		if (bytes_lidos == -1) {
			perror("read");
			close(pd[0]);
			return 1;
		}
		if (bytes_lidos != (ssize_t) sizeof(valor_recebido)) {
			fprintf(stderr, "Leitura incompleta no filho.\n");
			close(pd[0]);
			return 1;
		}

		printf("Filho recebeu: %d\n", valor_recebido);
		close(pd[0]);
		return 0;
	}

	{
		int valor_enviado = 42;
		ssize_t bytes_escritos;
/*o sleep vai fazer o pai epserar 5 segundos antes de escrever no pipe (fazer write), o filho chama o 
read primeiro e fica bloqueado a espera de dados no pipe. Quando o pai fizer o write, o filho vai ler os 
dados e imprimir o valor recebido. O sleep é usado para garantir que o filho esteja esperando no read antes do 
pai escrever, evitando assim uma condição de corrida onde o pai poderia escrever antes do filho estar pronto para ler.*/
		close(pd[0]);
		printf("Pai: vou esperar 5 segundos antes de escrever...\n");
		sleep(5);

		bytes_escritos = write(pd[1], &valor_enviado, sizeof(valor_enviado));
		if (bytes_escritos == -1) {
			perror("write");
			close(pd[1]);
			wait(NULL);
			return 1;
		}
		if (bytes_escritos != (ssize_t) sizeof(valor_enviado)) {
			fprintf(stderr, "Escrita incompleta no pai.\n");
			close(pd[1]);
			wait(NULL);
			return 1;
		}

		printf("Pai enviou: %d\n", valor_enviado);
		close(pd[1]);
		wait(NULL);
	}

	return 0;
}