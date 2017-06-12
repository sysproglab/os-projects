#include <sys/resource.h>
#include <stdio.h>
#include <errno.h>

int which = PRIO_PROCESS;
id_t pid;
int priority = -20;
int ret;
int main(int argc, char *argv[])
{
	pid = getpid();
	errno = 0;
	ret = getpriority(which, pid);

	if (errno == ESRCH) {
		printf("ESRCH: Not found process for which and who\n");
		return -1;
	}
	else if (errno == EINVAL) {
		printf("EINVAL: Which not equals RIO_PROCESS, PRIO_PGRP or PRIO_USER\n");
		return -1;
	}

	printf("Priority = %d for process with pid = %d\n", ret, pid);
	// The following example sets the priority for the current process ID to -20.
	ret = setpriority(which, pid, priority);
	printf("Setpriority returns %d for process with pid = %d\n", ret, pid);
	if (ret == -1) {
		if (errno == ESRCH) {
			printf("ESRCH: Not found process for which and who\n");
		}
		else if (errno == EINVAL) {
			printf("EINVAL: Which not equals RIO_PROCESS, PRIO_PGRP or PRIO_USER\n");
		}
		else if (errno == EPERM) {
			printf("EPERM: A process was located, but its effective user ID did not match either the effective or the real user ID of the caller, and was not privileged (on Linux: did not have the CAP_SYS_NICE capability).\n");
		}
		else if (errno == EACCES)
		{
			printf("EACCES: The caller attempted to set a lower nice value (i.e., a higher process priority), but did not have the required privilege (on Linux: did not have the CAP_SYS_NICE capability).\n");
		}
		return -1;
	}

	errno = 0;
	ret = getpriority(which, pid);
	if (errno == ESRCH) {
		printf("ESRCH: Not found process for which and who\n");
		return -1;
	}
	else if (errno == EINVAL) {
		printf("EINVAL: Which not equals RIO_PROCESS, PRIO_PGRP or PRIO_USER\n");
		return -1;
	}
	printf("Priority = %d for process with pid = %d\n", ret, pid);
	return 0;
}
