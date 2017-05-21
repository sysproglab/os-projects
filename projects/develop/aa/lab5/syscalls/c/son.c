#include <stdio.h>
#include <unistd.h>

int main () {
	int pid, ppid;
	pid = getpid();
	ppid = getppid();
	printf("\n Son param : pid = %d, ppid = %d\n", pid, ppid);
	sleep (3);
	return 0;
}