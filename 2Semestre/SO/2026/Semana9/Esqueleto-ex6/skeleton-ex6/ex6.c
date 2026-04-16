#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_COMMANDS 10

// parse the argument and execvp
// returns the result of exec
int exec_command(char *arg)
{

	// We are assuming a maximum number of arguments of 10.
	// This could be improved with realloc, for example.
	char *exec_args[10];
	int args_count = 0;
	int exec_ret = 0;

	char *token, *string, *tofree;

	tofree = string = strdup(arg);
	assert(string != NULL);

	while ((token = strsep(&string, " ")) != NULL)
	{
		exec_args[args_count] = token;
		args_count++;
	}
	exec_args[args_count] = NULL;

	exec_ret = execvp(exec_args[0], exec_args);

	perror("Exec error");

	free(tofree);

	return exec_ret;
}

int main(int argc, char **argv)
{

	int pipefd1[2], pipefd2[2], pipefd3[2];
	pid_t pid1, pid2, pid3, pid4;

	// Create first pipe
	if (pipe(pipefd1) == -1) {
		perror("pipe1");
		exit(EXIT_FAILURE);
	}

	pid1 = fork();
	if (pid1 == -1) {
		perror("fork1");
		exit(EXIT_FAILURE);
	}

	if (pid1 == 0) {
		// First child: execute `grep -v ^# /etc/passwd`
		close(pipefd1[0]); // Close unused read end
		dup2(pipefd1[1], STDOUT_FILENO); // Redirect stdout to pipe
		close(pipefd1[1]);

		execlp("grep", "grep", "-v", "^#", "/etc/passwd", NULL);
		perror("execlp grep");
		exit(EXIT_FAILURE);
	}

	// Create second pipe
	if (pipe(pipefd2) == -1) {
		perror("pipe2");
		exit(EXIT_FAILURE);
	}

	pid2 = fork();
	if (pid2 == -1) {
		perror("fork2");
		exit(EXIT_FAILURE);
	}

	if (pid2 == 0) {
		// Second child: execute `cut -f7 -d:`
		close(pipefd1[1]); // Close unused write end of first pipe
		dup2(pipefd1[0], STDIN_FILENO); // Redirect stdin to first pipe
		close(pipefd1[0]);

		close(pipefd2[0]); // Close unused read end of second pipe
		dup2(pipefd2[1], STDOUT_FILENO); // Redirect stdout to second pipe
		close(pipefd2[1]);

		execlp("cut", "cut", "-f7", "-d:", NULL);
		perror("execlp cut");
		exit(EXIT_FAILURE);
	}

	// Create third pipe
	if (pipe(pipefd3) == -1) {
		perror("pipe3");
		exit(EXIT_FAILURE);
	}

	pid3 = fork();
	if (pid3 == -1) {
		perror("fork3");
		exit(EXIT_FAILURE);
	}

	if (pid3 == 0) {
		// Third child: execute `uniq`
		close(pipefd2[1]); // Close unused write end of second pipe
		dup2(pipefd2[0], STDIN_FILENO); // Redirect stdin to second pipe
		close(pipefd2[0]);

		close(pipefd3[0]); // Close unused read end of third pipe
		dup2(pipefd3[1], STDOUT_FILENO); // Redirect stdout to third pipe
		close(pipefd3[1]);

		execlp("uniq", "uniq", NULL);
		perror("execlp uniq");
		exit(EXIT_FAILURE);
	}

	pid4 = fork();
	if (pid4 == -1) {
		perror("fork4");
		exit(EXIT_FAILURE);
	}

	if (pid4 == 0) {
		// Fourth child: execute `wc -l`
		close(pipefd3[1]); // Close unused write end of third pipe
		dup2(pipefd3[0], STDIN_FILENO); // Redirect stdin to third pipe
		close(pipefd3[0]);

		execlp("wc", "wc", "-l", NULL);
		perror("execlp wc");
		exit(EXIT_FAILURE);
	}

	// Close all pipes in parent process
	close(pipefd1[0]);
	close(pipefd1[1]);
	close(pipefd2[0]);
	close(pipefd2[1]);
	close(pipefd3[0]);
	close(pipefd3[1]);

	// Wait for all child processes to finish
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	waitpid(pid3, NULL, 0);
	waitpid(pid4, NULL, 0);

	return 0;
}
