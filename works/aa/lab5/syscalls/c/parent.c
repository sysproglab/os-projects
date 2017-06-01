#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main () {
	int pid, ppid, status;
	pid = getpid();
	ppid = getppid();
	printf("\nFATHER PARAM : pid = %d, ppid = %d\n\n", pid, ppid);

	int cpid = fork();
	if (cpid < 0) {
		printf("Eror in fork\n");
		exit(-1);
	} else if (cpid == 0) {
		execl("son", "son", NULL );
	} else {
		wait(&status);
	}
	exit(0);
}