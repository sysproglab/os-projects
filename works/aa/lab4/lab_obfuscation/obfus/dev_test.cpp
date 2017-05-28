#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define DEFAULT_FILE "./myout"

int main(int argc, char** argv) {
	char *portname = DEFAULT_FILE;
    if (argc > 1)
        portname = argv[1];

	FILE* fd = fopen (portname, "w");
	if (fd < 0)	{
        /*error_message ("error %d opening %s: %s", errno, portname, strerror (errno));*/
        printf("error opening the device\n");
        return 1;
	}

    char buf [100];
    int n = scanf ("%s", buf); // read up to 100 characters if ready to read
    printf("Input: %s\n", buf);

	fprintf(fd, "%s\n", buf); // send 8 character greeting
	return 0;
}