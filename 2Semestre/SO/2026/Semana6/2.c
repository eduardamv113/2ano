#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
	int fds[2];

	if (pipe(fds) == -1) {
		perror("pipe");
		return 1;
	}

	pid_t pid = fork();
	if (pid < 0) {
		perror("fork");
		close(fds[0]);
		close(fds[1]);
		return 1;
	}

	if(pid==0){
	    /* FILHO: envia inteiros */
	    close(fds[0]);
	    printf("Filho: a enviar inteiros...\n");
	    int x = 123;
	    write(fds[1], &x, sizeof(int));
	    printf("Filho: enviou 123\n");
	    x=456;
	    write(fds[1], &x, sizeof(int));
	    printf("Filho: enviou 456\n");
	    x=789;
	    write(fds[1], &x, sizeof(int));
	    printf("Filho: enviou 789\n");
	    close(fds[1]);
	    printf("Filho: fechou pipe, vai terminar\n");
	    exit(0);
	} else{
	    /* PAI: recebe inteiros com atraso */
	    close(fds[1]);
	    printf("Pai: vou esperar 5 segundos antes de ler...\n");
	    printf("(O filho vai ficar bloqueado no write enquanto o buffer enche)\n");
	    sleep(5);
	    printf("Pai: agora vou ler inteiros do pipe...\n");
	    int x;
	    while (read(fds[0], &x, sizeof(int)) != 0) {
	        printf("Pai recebeu: %d\n", x);
	    }
	    printf("Pai: detectou EOF no pipe (nenhum processo tem escrita aberta)\n");
	    close(fds[0]);
	    int status;
	    wait(&status);
	    printf("Pai: filho terminou\n");
	    return 0;
	}
}


