#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

int main(void)
{
	printf("PRECHROOT\n");
	if (chroot ("/home/stas/Documents/OS/SysCall/chrootTest") == 0)
		printf("Chroot seccesful\n");
	else
		{
			printf("Error chroot: %s\n", strerror(errno));	
		}
	while (1)
	{
	}
	return 0;
}

