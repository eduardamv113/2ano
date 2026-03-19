

pid_t pid;
int fds[2];


int x;
read(fds[0], &x, sizeof(x));
printf("x: %d\n", x);

close(fds[0]);