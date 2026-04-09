#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main (){
    int fd_in = open("/etc/passwd", O_RDONLY);
    int fd_out = open("saida.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    int fd_err = open("erros.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    int original_stdout = dup(STDOUT_FILENO);


    dup2(fd_in, STDIN_FILENO);
    dup2(fd_out, STDOUT_FILENO);
    dup2(fd_err, STDERR_FILENO);
    close(fd_in);
    close(fd_out);
    close(fd_err);

    if (fork()==0){
        ssize_t r;
        char buffer[1024];
        while ((r = read(STDIN_FILENO, buffer, 1024)) > 0) {
            buffer[r] = '\0';
            printf("%s", buffer);
            write(STDOUT_FILENO, buffer, r);
        }
        exit(0);
    }

    wait(NULL);

    dup2(original_stdout, STDOUT_FILENO);
    close(original_stdout); 
}