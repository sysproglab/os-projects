#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

int main(void)
{
	printf("PRECHOWN\n");
	if (chown ("/home/stas/Documents/OS/SysCall/testFile",0,0) == 0)
		printf("chown seccesful\n");
	else
		{
			printf("Error chown: %s\n", strerror(errno));	
		}
	return 0;
}

