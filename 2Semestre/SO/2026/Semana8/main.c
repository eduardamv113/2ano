#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

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

    // Example operation: read from stdin and write to stdout
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), stdin)) {
        printf("%s", buffer);
    }

    // Restore standard output and print termination message
    fflush(stdout);
    fprintf(stderr, "\nErrors logged\n");
    fprintf(stdout, "terminei\n");

    return 0;
}