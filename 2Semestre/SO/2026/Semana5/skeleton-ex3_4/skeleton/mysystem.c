#include "mysystem.h"


// recebe um comando por argumento
// returna -1 se o fork falhar
// caso contrario retorna o valor do comando executado
int mysystem (const char* command) {
	if (command == NULL) {
		return 1;
	}

	char *cmd = strdup(command);
	if (cmd == NULL) {
		return -1;
	}

	char *argv[256];
	int argc = 0;
	char *tok = strtok(cmd, " \t\n");
	while (tok != NULL && argc < 255) {
		argv[argc++] = tok;
		tok = strtok(NULL, " \t\n");
	}
	argv[argc] = NULL;

	if (argc == 0) {
		free(cmd);
		return 0;
	}

	pid_t pid = fork();
	if (pid < 0) {
		free(cmd);
		return -1;
	}

	if (pid == 0) {
		execvp(argv[0], argv);
		_exit(127);
	}

	int status = -1;
	while (waitpid(pid, &status, 0) < 0) {
		if (errno != EINTR) {
			status = -1;
			break;
		}
	}

	free(cmd);
	return status;
}