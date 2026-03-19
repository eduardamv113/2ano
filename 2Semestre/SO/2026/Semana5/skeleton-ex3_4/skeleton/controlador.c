#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "mysystem.h"

static const char *program_name(const char *path) {
    const char *slash = strrchr(path, '/');
    return (slash == NULL) ? path : slash + 1;
}

static int worker_run_until_success(const char *command) {
    int count = 0;

    while (1) {
        int status;
        count++;
        status = mysystem(command);

        if (status == -1) {
            return -1;
        }

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            return count;
        }
    }
}

void controller(int n, char **commands) {
    pid_t pids[n];
    int pipes[n][2];
    int counts[n];

    for (int i = 0; i < n; i++) {
        counts[i] = -1;
        if (pipe(pipes[i]) < 0) {
            perror("pipe");
            exit(1);
        }
    }

    for (int i = 0; i < n; i++) {
        pids[i] = fork();
        if (pids[i] < 0) {
            perror("fork");
            exit(1);
        }

        if (pids[i] == 0) {
            int res;

            for (int j = 0; j < n; j++) {
                close(pipes[j][0]);
                if (j != i) {
                    close(pipes[j][1]);
                }
            }

            res = worker_run_until_success(commands[i]);
            if (write(pipes[i][1], &res, sizeof(res)) != sizeof(res)) {
                _exit(1);
            }

            close(pipes[i][1]);
            _exit(0);
        }
    }

    for (int i = 0; i < n; i++) {
        close(pipes[i][1]);
    }

    for (int i = 0; i < n; i++) {
        int status;

        if (read(pipes[i][0], &counts[i], sizeof(counts[i])) != sizeof(counts[i])) {
            counts[i] = -1;
        }

        close(pipes[i][0]);

        while (waitpid(pids[i], &status, 0) < 0) {
            if (errno != EINTR) {
                break;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        printf("%s %d\n", program_name(commands[i]), counts[i]);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s programa1 [programa2 ...]\n", argv[0]);
        return 1;
    }

    controller(argc - 1, &argv[1]);
    return 0;
}
