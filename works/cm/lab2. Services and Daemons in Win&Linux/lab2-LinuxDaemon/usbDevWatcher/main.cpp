#include <iostream>
#include <fstream>
#include <libusb-1.0/libusb.h>
#include <unistd.h>

using namespace std;

ofstream outFile;
int getCountOfAllDevices();
void printInfoAllDevices();
void printInfoDevice(libusb_device *dev);

int main() {
	int counterOfDev = -1;
	int counterOfDevOld = !counterOfDev;
	while (true) {
		counterOfDev = getCountOfAllDevices();
		if (counterOfDev == counterOfDevOld) {
			sleep(1);
			continue;
		}
		counterOfDevOld = counterOfDev;
		outFile.open("/home/michael/usbDevWatcher/Note.txt");
		outFile << "Count of all USB devices: " << counterOfDev << endl; //print total number of usb devices
		printInfoAllDevices();
		outFile.close();
	}
	return 0;
}

int getCountOfAllDevices() {
	libusb_device **devs; 		//pointer to pointer of device, used to retrieve a list of devices
	libusb_context *ctx = NULL; 	//a libusb session
	int r;			//for return values
	ssize_t counterOfDev; 		//holding number of devices in list
	r = libusb_init(&ctx); 	//initialize a library session
	if(r < 0) {
		outFile.open("/home/michael/usbDevWatcher/Note.txt");
		outFile << "Init Error " << r << endl; //there was an error
		outFile.close();
		return -1;
	}

	counterOfDev = libusb_get_device_list(ctx, &devs); //get the list of devices
	if(counterOfDev < 0) {
		outFile.open("/home/michael/usbDevWatcher/Note.txt");
		outFile<<"Get Device Error"<<endl; //there was an error
		outFile.close();
		return -2;
	}
	libusb_free_device_list(devs, 1); //free the list, unref the devices in it
	libusb_exit(ctx); //close the session
	return counterOfDev;
}

void printInfoAllDevices() {
	outFile << "Information about all devices" << endl;
	libusb_device **devs; 		//pointer to pointer of device, used to retrieve a list of devices
	libusb_context *ctx = NULL; 	//a libusb session
	int r;			//for return values
	ssize_t cnt; 		//holding number of devices in list
	r = libusb_init(&ctx); 	//initialize a library session
	if(r < 0) {
		outFile<<"Init Error "<<r<<endl; //there was an error
		return;
	}

	cnt = libusb_get_device_list(ctx, &devs); //get the list of devices
	if(cnt < 0) {
		outFile<<"Get Device Error"<<endl; //there was an error
	}
	
	for(ssize_t i = 0; i < cnt; i++) {
		printInfoDevice(devs[i]); //print specs of this device
	}
	libusb_free_device_list(devs, 1); //free the list, unref the devices in it
	libusb_exit(ctx); //close the session
}

// function prints general information about all USB devices
void printInfoDevice(libusb_device *dev)
{
	outFile << "---------------------------------------" << endl;
	//if (*dev == NULL) return;
	libusb_device_descriptor desc;
	int r = libusb_get_device_descriptor(dev, &desc);
	if (r < 0) {
		outFile<<"failed to get device descriptor"<<endl;
		return;
	}

	outFile << "bDeviceClass:    " << (int)desc.bDeviceClass << endl;
	outFile << "idVendor:        " << "0x" << hex << desc.idVendor << endl;
	outFile << "idProduct:       " << "0x" << hex << desc.idProduct << endl;
	outFile << "iProduct:        " << (int)desc.iProduct << endl;
	outFile << "iManufacturer:   " << (int)desc.iManufacturer << endl;
	outFile << "bNumConfigs:     " << (int)desc.iProduct << endl;
	outFile << "bLength:         " << (int)desc.bLength << endl;
	outFile << "bDescriptorType: " << (int)desc.bDescriptorType << endl;
	outFile << "bcdUSB:          " << (int)desc.bcdUSB << endl;
	outFile << "bDeviceSubClass: " << (int)desc.bDeviceSubClass << endl;
	outFile << "bDeviceProtocol: " << (int)desc.bDeviceProtocol << endl;
	outFile << "bMaxPacketSize0: " << (int)desc.bMaxPacketSize0 << endl;
	outFile << endl;
}
