#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>

// i2c.general
#define I2C_BUS		 "/dev/i2c-1"	// path to i2c bus
#define	I2C_DIGISPARK_ADR	0x26    // The I2C slaveAdress
// i2c.commands
#define I2C_CMD_BATTERY 	0xBB    // "Check battery status" (frequency == 60 sec.)
#define I2C_CMD_CHECK   	0xCC    // "Check general status" (frequency == 1 sec.)
#define I2C_CMD_BRIGTH  	0xDD    // "Set display brightness" (async.)
#define I2C_CMD_SHUT    	0x88    // "RPi shut down" (async.)
									// OTHER commands are invalid
								
int main(void) {
    char buf[10] = {0};
    int i2cBusDesc = -1;
	int ans = -1;

	for (short int i=-255; i<255; i+=25){
		// open bus
		if ((i2cBusDesc = open(I2C_BUS, O_RDWR)) < 0) {
			printf("Failed to open the bus.\n");
			exit(-1);
		}

		// get access to slave with address I2C_DIGISPARK_ADR
		if (ioctl(i2cBusDesc, I2C_SLAVE, I2C_DIGISPARK_ADR) < 0) {
			printf("Failed to get access to the slave (%x).\n", I2C_DIGISPARK_ADR);
			exit(-2);
		}

		// write cmd
		buf[0] = I2C_CMD_BRIGTH;
		buf[1] = abs(i);
		if (write(i2cBusDesc, buf, 2) != 2) {
			printf("Failed to write to the i2c bus.\n");
			exit(-3);
		}
		usleep(300000);

		// read answer
		buf[0] = 0;
		if ((ans = read(i2cBusDesc, buf, 1)) != 1) {
			printf("Failed to read to the i2c bus. Ans=%d\n", ans);
			exit(-4);
		}
		printf("Battery charge level: %d\n", buf[0]);
		usleep(50000);
	}
	return 0;
}