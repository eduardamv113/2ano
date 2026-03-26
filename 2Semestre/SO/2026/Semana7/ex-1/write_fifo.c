#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    char buffer[1024];
    ssize_t r;
    printf("Criar o FIFO...\n");
    int fd = open("fifo", O_WRONLY);
    if (fd == -1) {
        perror("fifo");
        return 1;
    }

    printf("FIFO aberto, escreva algo para testar:\n");
    while((r= read(STDIN_FILENO, buffer, 1024))>0) {
        write(fd, buffer, r);
        }   
        
        close(fd);
        return 0;

}