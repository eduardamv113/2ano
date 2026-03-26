#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "defs.h"
#include "vector.h"

int mkfifo(const char *pathname, mode_t mode);

int main (int argc, char * argv[]){
    init_vector();
    print_vector();

    mkfifo("fifo",0600);
    int fd = open("fifo", O_RDONLY);

    Msg m;
    while(read(fd, &m, sizeof(Msg)) > 0){
        m.occurrences = count_needle(m.needle);

        char fifo_name[30];
        sprintf(fifo_name, "fifo_%d", m.pid);
        int fd_client = open(fifo_name, O_WRONLY);
        write(fd_client, &m, sizeof(Msg));
        close(fd_client);    
    }

close(fd);
unlink("fifo");
return 0;
}