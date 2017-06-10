#include <sys/utsname.h>
#include <errno.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	struct utsname unameData;
	int ret = 0;
	// Might check return value here (non-0 = failure)
	ret = uname(&unameData);
	if (ret != 0) {
		if (errno == EFAULT) {
			printf("EFAULT - Not valid value for buf.\n");
		}
		else {
			printf("Error with uname.\n");
		}
		return -1;
	}
	//After this, the struct will contain the info you want:
	printf("sysname: %s\n", unameData.sysname);
	printf("nodename: %s\n", unameData.nodename);
	printf("release: %s\n", unameData.release);
	printf("version: %s\n", unameData.version);
	printf("machine: %s\n", unameData.machine);
	#ifdef _GNU_SOURCE
	printf("domainname: %s\n", unameData.domainname);
	#endif
	return 0;
}
