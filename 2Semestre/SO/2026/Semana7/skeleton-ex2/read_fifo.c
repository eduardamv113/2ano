// Lê do FIFO e escreve no stdout
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