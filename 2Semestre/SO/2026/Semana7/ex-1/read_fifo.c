#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    const char *fifo_path = "fifo";
    char buffer[256];

    // Abre o FIFO para leitura
    int fifo_fd = open(fifo_path, O_RDONLY);
    if (fifo_fd == -1) {
        perror("Erro ao abrir o FIFO");
        exit(EXIT_FAILURE);
    }

    printf("Lendo do FIFO e a escrever no stdout:\n");

    // Lê do FIFO e escreve no stdout
    ssize_t bytes_read;
    while ((bytes_read = read(fifo_fd, buffer, sizeof(buffer))) > 0) {
        if (write(STDOUT_FILENO, buffer, bytes_read) == -1) {
            perror("Erro ao escrever no stdout");
            close(fifo_fd);
            exit(EXIT_FAILURE);
        }
    }

    if (bytes_read == -1) {
        perror("Erro ao ler do FIFO");
    }

    close(fifo_fd);
    return 0;
}