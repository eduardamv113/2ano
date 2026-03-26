#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "defs.h"

int main (int argc, char * argv[]){

    if (argc < 2) {
        printf("Missing argument.\n");
        _exit(1);
    }

    char fifo_name[30];
    sprintf(fifo_name, "fifo_%d", getpid());
    mkfifo(fifo_name, 0600);

    int fd_server = open("fifo", O_WRONLY);
    Msg m;
    m.needle = atoi(argv[1]);
    m.pid = getpid();
    m.occurrences = 0;
    write(fd_server, &m, sizeof(Msg));



    int fd_client = open(fifo_name, O_RDONLY);
    read(fd_client, &m, sizeof(Msg));
    printf("%d appears %d times\n", m.needle, m.occurrences);

    close(fd_client);
    close(fd_server);
    return 0;

}