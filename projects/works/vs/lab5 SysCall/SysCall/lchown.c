#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

int main(void)
{
	printf("PRELCHOWN\n");
	if (lchown ("/home/stas/Documents/OS/SysCall/testFile",0,0) == 0)
		printf("Lchown seccesful\n");
	else
		{
			printf("Error Lchown: %s\n", strerror(errno));	
		}
	return 0;
}

