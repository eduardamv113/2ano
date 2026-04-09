/*
Para o exercício 1, fez-se as seguintes alterações:
- Redirecionou-se o descritor associado ao standard input para o ficheiro `/etc/passwd` utilizando `open` e `dup2`.
- Redirecionou-se o standard output para o ficheiro `saida.txt` e o standard error para o ficheiro `erros.txt`.
- Antes de terminar, imprimiu-se "terminei" no terminal utilizando `fprintf`.

Para o exercício 2, fez-se as seguintes alterações:
- Criou-se um novo processo utilizando `fork()`.
- No processo filho, realizaram-se operações de leitura e escrita, lendo do stdin e escrevendo no stdout.
- No processo pai, utilizou-se `wait()` para aguardar a conclusão do processo filho.
- Após a conclusão do processo filho, o processo pai imprimiu "terminei" no terminal.
*/

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main () {
    // Open files for redirection
    int fd_in = open("/etc/passwd", O_RDONLY);
    if (fd_in == -1) {
        perror("Error opening /etc/passwd");
        return 1;
    }

    int fd_out = open("saida.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd_out == -1) {
        perror("Error opening saida.txt");
        close(fd_in);
        return 1;
    }

    int fd_err = open("erros.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd_err == -1) {
        perror("Error opening erros.txt");
        close(fd_in);
        close(fd_out);
        return 1;
    }

    // Redirect standard input, output, and error
    dup2(fd_in, STDIN_FILENO);
    dup2(fd_out, STDOUT_FILENO);
    dup2(fd_err, STDERR_FILENO);

    // Close original file descriptors
    close(fd_in);
    close(fd_out);
    close(fd_err);

    // Create a new process
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) { // Child process
        // Example operation: read from stdin and write to stdout
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), stdin)) {
            printf("%s", buffer);
        }
        return 0;
    } else { // Parent process
        // Wait for the child process to finish
        wait(NULL);
        // Restore standard output and print termination message
        fprintf(stdout, "terminei\n");
    }

    return 0;
}