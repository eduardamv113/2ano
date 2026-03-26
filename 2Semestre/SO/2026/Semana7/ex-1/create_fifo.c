#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
//make
//./create_fifo
//./write_fifo
//abrir outro terminal
//./read_fifo

int main(){
    int r = mkfifo("fifo", 0600);
    printf("FIFO criado com sucesso!\n");
    if (r == -1) {
        perror("Erro ao criar FIFO");
        return 1;
    }
    return 0;
}