#include <stdio.h>
#include <unistd.h>

int main(void) {
    printf("A executar ls -l com execlp...\n");
    fflush(stdout);

    execlp("ls", "ls", "-l", (char *) NULL);

    perror("execlp");
    return 1;
}
