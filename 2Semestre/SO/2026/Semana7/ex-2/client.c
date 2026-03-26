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

    int number = atoi(argv[1]);
    int occurrences;

    const char *fifo_request = "fifo_request";
    const char *fifo_response = "fifo_response";

    // Abre o FIFO de requisição para escrita
    int request_fd = open(fifo_request, O_WRONLY);
    if (request_fd == -1) {
        perror("Erro ao abrir FIFO_REQUEST");
        exit(EXIT_FAILURE);
    }

    // Envia o número ao servidor
    write(request_fd, &number, sizeof(int));
    close(request_fd);

    // Abre o FIFO de resposta para leitura
    int response_fd = open(fifo_response, O_RDONLY);
    if (response_fd == -1) {
        perror("Erro ao abrir FIFO_RESPONSE");
        exit(EXIT_FAILURE);
    }

    // Lê o número de ocorrências enviado pelo servidor
    read(response_fd, &occurrences, sizeof(int));
    close(response_fd);

    printf("O número %d ocorre %d vez(es) no vetor.\n", number, occurrences);

    return 0;
}