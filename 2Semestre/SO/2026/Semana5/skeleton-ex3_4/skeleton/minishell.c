#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

static int parse_line(char *line, char **argv, int max_args, int *background) {
    int argc = 0;
    char *token;

    *background = 0;

    token = strtok(line, " \t\n");
    while (token != NULL && argc < max_args - 1) {
        argv[argc++] = token;
        token = strtok(NULL, " \t\n");
    }

    argv[argc] = NULL;

    if (argc > 0 && strcmp(argv[argc - 1], "&") == 0) {
        *background = 1;
        argv[argc - 1] = NULL;
        argc--;
    }

    return argc;
}

int main(void) {
    char *line = NULL;
    size_t cap = 0;

    while (1) {
        char *argv[128];
        int background = 0;
        int argc;
        ssize_t nread;

        printf("mini$ ");
        fflush(stdout);

        nread = getline(&line, &cap, stdin);
        if (nread < 0) {
            printf("\n");
            break;
        }

        argc = parse_line(line, argv, 128, &background);
        if (argc == 0) {
            continue;
        }

        if (strcmp(argv[0], "exit") == 0) {
            break;
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            continue;
        }

        if (pid == 0) {
            execvp(argv[0], argv);
            perror("execvp");
            _exit(127);
        }

        if (background) {
            printf("[bg] pid=%d\n", pid);
        } else {
            int status;
            while (waitpid(pid, &status, 0) < 0) {
                if (errno != EINTR) {
                    perror("waitpid");
                    break;
                }
            }
        }

        while (waitpid(-1, NULL, WNOHANG) > 0) {
        }
    }

    free(line);
    return 0;
}
