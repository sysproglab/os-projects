#include <sys/socket.h>
#include <sys/un.h>

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
#include <pthread.h>

pthread_mutex_t lockI2C; // mutex for critical section

// i2c.general
#define I2C_BUS		 "/dev/i2c-1"	// path to i2c bus
#define	I2C_DIGISPARK_ADR	0x26    // The I2C slaveAdress
// i2c.commands
#define I2C_CMD_BATTERY 	0xBB    // "Check battery status" (frequency == 60 sec.)
#define I2C_CMD_CHECK   	0xCC    // "Check general status" (frequency == 1 sec.)
#define I2C_CMD_BRIGTH  	0xDD    // "Set display brightness" (async.)
#define I2C_CMD_SHUT    	0x88    // "RPi shut down" (async.)


// function for getting "Battery charge level" (int percent 0-100)
char getBatteryChargeLvl(void);
// function for setting of display`s brightness (in percent)
int setDispBrightness(char brightValue);
// function for warning the MCU about power off
int warnMCU(void);
// function for Shutdown the System
void powerOff(void);
// function for execution of request to User
void askTheUser();

void *thrdCheckFun(void *arg);

//char *socket_path = "./socket";
char *socket_path = "\0hidden";
char status = 0;
int batteryCheckCnt = 0;
char batteryChargeLvl = 100;

int main(int argc, char *argv[]) {
	struct sockaddr_un addr;
	char buf[100];
	int fd,cl,rc;
	pthread_t thrdChecker;	// thread

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
		unlink(socket_path);
	}

	if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		perror("bind error");
		exit(-1);
	}

	if (listen(fd, 5) == -1) {
		perror("listen error");
		exit(-2);
	}

	batteryChargeLvl = getBatteryChargeLvl(); 	// from 0 to 100
	// thread. Creation and start
	if (pthread_create(&thrdChecker, NULL, thrdCheckFun, NULL) != 0) {
		printf("Error#3: errno=%d; Could not create check-thread;", errno);
		exit(-3);  
	}

	while (true) {
		if ( (cl = accept(fd, NULL, NULL)) == -1) {
			perror("accept error");
			continue;
		}

		while ((rc=read(cl,buf,sizeof(buf))) > 0) {
			switch (buf[0]) {
				// I2C_CMD_BATTERY-command "Check battery status" (frequency == 60 sec.)
				case I2C_CMD_BATTERY:
					printf("I2C_CMD_BATTERY, batteryChargeLvl=%d\n", batteryChargeLvl);	// from 0 to 100
					if (write(cl, &batteryChargeLvl, 1) != 1)			// send info about Battery charge to client
						printf("write error;\n");
					break;


				// I2C_CMD_CHECK-command "Check general status" (frequency == 1 sec.)
				// "status" - result of I2C_CMD_CHECK-command executed in another thread
				case I2C_CMD_CHECK:
					printf("I2C_CMD_CHECK, status=%d\n", status);
					if (write(cl, &status, 1) != 1)		// send Status to client
						printf("write error;\n");
					break;


				// I2C_CMD_BRIGTH-command "Set display brightness" (async.)
				case I2C_CMD_BRIGTH:
					printf("I2C_CMD_BRIGTH, ");

					pthread_mutex_lock(&lockI2C);		// lock I2C-bus
					buf[0] = setDispBrightness(buf[1]);	// from 0 to 100
					pthread_mutex_unlock(&lockI2C);		// unlock I2C-bus
					printf("setDispBrightness()=%d\n", buf[0]);

					if (write(cl, buf, 1) != 1)			// send result of operation to client
						printf("write error;\n");
					break;


				// I2C_CMD_SHUT-command "RPi shut down" (async.)
				case I2C_CMD_SHUT:
					printf("I2C_CMD_SHUT, ");

					pthread_mutex_lock(&lockI2C);		// lock I2C-bus
					buf[0] = warnMCU();					// warn the MCU about power off
					pthread_mutex_unlock(&lockI2C);		// unlock I2C-bus
					printf("warnMCU()=%d\n", buf[0]);

					if (write(cl, buf, 1) != 1)			// send result of operation to client
						printf("write error;\n");

					if (buf[0] == 0)					// only if MCU is warned - turn off the RPi
						powerOff();
					break;

				// INVALID command
				default:
					printf("default\n");
					write(cl, buf, 1);					// resend bad command to client
					break;
			}
			printf("read %u bytes: %.*s\n", rc, rc, buf);
		}


		if (rc == -1) {
			//perror("read");
			exit(-1);
		}
		else if (rc == 0) {
			//printf("EOF\n");
			close(cl);
		}
	}

	// wainting finish of checker-thread
	pthread_join(thrdChecker, NULL);
	return 0;
}

void *thrdCheckFun(void *arg) {
	char buf = 0;
    int i2cBusDesc = -1;
	int ans = -1;
	while (true){
		buf = I2C_CMD_CHECK;


	// *** lockI2C ***
		pthread_mutex_lock(&lockI2C);
		
		// open bus
		if ((i2cBusDesc = open(I2C_BUS, O_RDWR)) < 0) {
			printf("Failed to open the bus.\n");
			goto waiting;
		}

		// get access to slave with address I2C_DIGISPARK_ADR
		if (ioctl(i2cBusDesc, I2C_SLAVE, I2C_DIGISPARK_ADR) < 0) {
			printf("Failed to get access to the slave (%x).\n", I2C_DIGISPARK_ADR);
			goto waiting;
		}

		// write cmd
		if (write(i2cBusDesc, &buf, 1) != 1) {
			printf("Failed to write to the i2c bus.\n");
			goto waiting;
		}
		usleep(100000);

		// read answer
		if (read(i2cBusDesc, &buf, 1) != 1) {
			printf("Failed to read to the i2c bus.\n");
			goto waiting;
		}
		pthread_mutex_unlock(&lockI2C);
	// *** unlocked ***

		// update the status of MCU
		status = buf;
		// try to power off the RPi
		if ((buf & 0x02) != 0x02)
			askTheUser();

		// check battery status each 60 sec
		if (batteryCheckCnt >= 60) {
			pthread_mutex_lock(&lockI2C);				// lock I2C-bus
			batteryChargeLvl = getBatteryChargeLvl(); 	// from 0 to 100
			pthread_mutex_unlock(&lockI2C);				// unlock I2C-bus
			batteryCheckCnt = 0;
		}

		// power off the RPi if Battery is finished
		if (batteryChargeLvl <= 10)
			powerOff();

waiting:
		usleep(1000000);	// 1sec
		batteryCheckCnt++;
	}
}


// function for getting "Battery charge level" (int percent 0-100)
char getBatteryChargeLvl(void) {
	int i2cBusDesc = -1;	// descriptor of MCU Digispark on the I2C-bus
	int batteryLvl = -1;	// battery charge level
	char buf[2] = {0};		// buffer for transmission through I2C

	// open bus
	if ((i2cBusDesc = open(I2C_BUS, O_RDWR)) < 0)
		return -1;	// Failed to open the bus

	// get access to slave with address I2C_DIGISPARK_ADR
	if (ioctl(i2cBusDesc, I2C_SLAVE, I2C_DIGISPARK_ADR) < 0)
		return -2;	// Failed to get access to the slave (I2C_DIGISPARK_ADR)

	// write cmd
	buf[0] = (int)I2C_CMD_BATTERY;
	if (write(i2cBusDesc, buf, 1) != 1)
		return -3;	// Failed to write to the i2c bus.

	// time-delay (waiting ADC)
	usleep(400000);

	// read answer
	if (read(i2cBusDesc, buf, 2) != 2)
		return -4;	// Failed to read to the i2c bus

	// convert ADC data and return "Battery charge level"
	batteryLvl = ((int)buf[1] & 0x00FF) | ((int)buf[0] << 8 & 0xFF00);		// ADC data (0-1024)
	return (char)(((double) batteryLvl) / 10.24); 	// "Battery charge level"
}


// function for setting of display`s brightness (in percent)
int setDispBrightness(char brightValue) {
	// check asserts
	if (brightValue > 100) 	brightValue = 100;
	if (brightValue < 0)	brightValue = 0;


	int i2cBusDesc = -1;	// descriptor of MCU Digispark on the I2C-bus
	char buf[2] = {0};		// buffer for transmission through I2C
	int converter = 255*brightValue/100;

	// open bus
	if ((i2cBusDesc = open(I2C_BUS, O_RDWR)) < 0)
		return -1;	// Failed to open the bus

	// get access to slave with address I2C_DIGISPARK_ADR
	if (ioctl(i2cBusDesc, I2C_SLAVE, I2C_DIGISPARK_ADR) < 0)
		return -2;	// Failed to get access to the slave (I2C_DIGISPARK_ADR)

	// clear i2c bus
	for (int i = 0; i<10; i++)
		read(i2cBusDesc, buf, 1);
	
	// write cmd
	buf[0] = (int)I2C_CMD_BRIGTH;
	buf[1] = (char) converter;//( ((int)brightValue) * 255) / 100;
	if (write(i2cBusDesc, buf, 2) != 2)
		return -3;	// Failed to write to the i2c bus.

	// time-delay (waintig of execution)
	usleep(300000);

	// read answer
	if (read(i2cBusDesc, buf, 1) != 1)
		return -4;	// Failed to read to the i2c bus

	// check result
	if (buf[0] != (char) converter) {
		printf("Brigness: try to set  - %d\n", (char)converter);
		printf("Brigness: as a result - %d\n", (char)buf[0]);
		return -5;	// Failed to set brightness
	}
	// time-delay (measures for protection of I2C-bus)
	usleep(200000);

	return 0;
}


// function for warning the MCU about power off
int warnMCU(void) {
	int i2cBusDesc = -1;	// descriptor of MCU Digispark on the I2C-bus
	char buf[2] = {0};		// buffer for transmission through I2C
	int ans = -1;			// answer by MCU. It has to be an equivalent of "brightValue"

	// open bus
	if ((i2cBusDesc = open(I2C_BUS, O_RDWR)) < 0)
		return -1;	// Failed to open the bus

	// get access to slave with address I2C_DIGISPARK_ADR
	if (ioctl(i2cBusDesc, I2C_SLAVE, I2C_DIGISPARK_ADR) < 0)
		return -2;	// Failed to get access to the slave (I2C_DIGISPARK_ADR)

	// write cmd
	buf[0] = (int)I2C_CMD_SHUT;
	if (write(i2cBusDesc, buf, 1) != 1)
		return -3;	// Failed to write to the i2c bus.

	// time-delay (waintig of execution)
	usleep(300000);

	// read answer
	if (read(i2cBusDesc, buf, 1) != 1)
		return -4;	// Failed to read to the i2c bus

	// check result
	if ((int)buf[0] != I2C_CMD_SHUT)
		return -5;	// Failed to power off
	
	// time-delay (measures for protection of I2C-bus)
	usleep(200000);

	return 0;
}


// function for Shutdown the System
void powerOff(void) {
	printf("shutdown -h now");
	system("shutdown -h now");	// stump
}


// function for execution of request to User
void askTheUser() {
	printf("Are you shure? (*stump)");
	return;
}