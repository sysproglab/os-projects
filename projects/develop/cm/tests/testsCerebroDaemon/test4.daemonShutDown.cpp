#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define I2C_CMD_BATTERY     0xBB    // "Check battery status" (frequency == 60 sec.)
#define I2C_CMD_CHECK       0xCC    // "Check general status" (frequency == 1 sec.)
#define I2C_CMD_BRIGTH      0xDD    // "Set display brightness" (async.)
#define I2C_CMD_SHUT        0x88    // "RPi shut down" (async.)
                                    // OTHER commands are invalid
                                    
//char *socket_path = "./socket4242";
char *socket_path = "\0hidden";

int main(int argc, char *argv[]) {
    struct sockaddr_un addr;
    char buf[100];
    int fd,rc;

    if (argc > 1) socket_path=argv[1];

    if ( (fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(-1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    if (*socket_path == '\0') {
        *addr.sun_path = '\0';
        strncpy(addr.sun_path+1, socket_path+1, sizeof(addr.sun_path)-2);
    } else {
        strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);
    }

    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("connect error");
        exit(-1);
    }

    buf[0] = I2C_CMD_SHUT;
    if (write(fd, buf, 1) != 1) {
        fprintf(stderr,"write error");
        exit(-1);
    }

    if (read(fd,buf,1) != 1) {
        fprintf(stderr,"read error");
        exit(-2);
    }
	
    printf("Shutdown: %s(%d)\n", (buf[0] == 0) ? "OK" : "FAIL", buf[0]);
    return 0;
}