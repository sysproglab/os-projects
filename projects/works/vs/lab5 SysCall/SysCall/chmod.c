#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

int main(void)
{
	int mod=0;
	mod = S_IXOTH;
	printf("PRECHANGE\n");
	if (chmod ("/home/stas/Documents/OS/SysCall/testFile",mod) == 0)
		printf("chmod seccesful\n");
	else
		{
			printf("Error chmod: %s\n", strerror(errno));	
		}
	return 0;
}

