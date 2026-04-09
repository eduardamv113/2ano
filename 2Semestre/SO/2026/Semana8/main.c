#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main () {
    int fd = open ("asd.txt", O_CREAT | O_WRONLY  | O_TRUNC, 0600);
    if(fd == -1) {
        perror("open");
        return 1;
    }
    printf("asdasddas\n");
    dup2(fd, STDOUT_FILENO);
    close(fd);
    printf("ajsdjdasdafa\n");
    return 0;
}