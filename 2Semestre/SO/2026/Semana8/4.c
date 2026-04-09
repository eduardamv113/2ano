#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pipe_fd[2];

    // Create the pipe
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) { // Child process
        // Redirect standard input to the read end of the pipe
        close(pipe_fd[1]); // Close the write end of the pipe
        dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[0]); // Close the original read end of the pipe

        // Execute the wc command
        execlp("wc", "wc", NULL);
        perror("execlp"); // If execlp fails
        exit(1);
    } else { // Parent process
        // Write user input to the write end of the pipe
        close(pipe_fd[0]); // Close the read end of the pipe

        char buffer[256];
        printf("Enter lines of text (Ctrl+D to finish):\n");
        while (fgets(buffer, sizeof(buffer), stdin)) {
            write(pipe_fd[1], buffer, sizeof(buffer));
        }

        close(pipe_fd[1]); // Close the write end of the pipe to signal EOF

        // Wait for the child process to finish
        wait(NULL);
    }

    return 0;
}