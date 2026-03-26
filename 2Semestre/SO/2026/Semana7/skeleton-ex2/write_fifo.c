    // Lê do stdin e escreve no FIFO
    while (fgets(buffer, sizeof(buffer), stdin)) {
        size_t len = strlen(buffer); // Calcula o tamanho real dos dados lidos
        if (write(fifo_fd, buffer, len) == -1) {
            perror("Erro ao escrever no FIFO");
            close(fifo_fd);
            exit(EXIT_FAILURE);
        }
    }