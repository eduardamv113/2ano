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
	    close(fds[0]);
	    int x = 123;
	    write(fds[1], &x, sizeof(int));
	    x=456;
	    write(fds[1], &x, sizeof(int));
	    x=789;
	    write(fds[1], &x, sizeof(int));
	    close(fds[1]);
	    exit(0);
	} else{
	    close(fds[1]);
	    int x;
	    while (read(fds[0], &x, sizeof(int)) != 0) {
	        printf("Pai recebeu: %d\n", x);
	    }
	    close(fds[0]);
	    int status;
	    wait(&status);
	    return 0;
	}
}


