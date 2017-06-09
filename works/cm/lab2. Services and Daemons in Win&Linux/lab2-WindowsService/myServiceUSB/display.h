#pragma once
#include <fstream>
int diplayInfo = 0;
using namespace ::std;


ofstream fout;


// function prints general information about USB Host Controllers
int printInfoHC(PUSBHOSTCONTROLLERINFO hcInfo)
{
	if (diplayInfo != 0 && diplayInfo != 1) return -1;

	fout << "*********************************************" << endl;
	if (hcInfo == NULL || hcInfo->UsbDeviceProperties == NULL) return -2;

	fout << hcInfo->UsbDeviceProperties->DeviceDesc << endl
		<< endl;
	fout << "DeviceClass:       " << hcInfo->UsbDeviceProperties->DeviceClass << endl;
	fout << "Bus:Device:        " << hcInfo->BusNumber << ":0x" << hcInfo->BusDevice << endl;
	fout << "DeviceId:          " << hcInfo->UsbDeviceProperties->DeviceId << endl;
	fout << "Service:           " << hcInfo->UsbDeviceProperties->Service << endl;
	fout << "DriverKey:         " << hcInfo->DriverKey << endl;
	fout << "VendorId:          " << hex << "0x" << hcInfo->VendorID << endl;
	fout << "DeviceID:          " << hex << "0x" << hcInfo->DeviceID << endl;
	fout << "SubSysID:          " << hex << "0x" << hcInfo->SubSysID << endl;
	fout << "Revision:          " << hex << "0x" << hcInfo->Revision << endl
		<< endl << endl;
	//power map
	//hcInfo->USBPowerInfo[nIndex]
}

// function prints general information about USB Hubs (Root/External)
void printInfoHub(PVOID info)
{
	if (diplayInfo != 0 && diplayInfo != 2) return;

	fout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;
	if (info == NULL) return;
	fout << ((PUSBROOTHUBINFO)info)->HubName << endl
		<< endl;
	fout << "Hub is bus powered:  " << 
		(((PUSBROOTHUBINFO)info)->HubInfo->u.HubInformation.HubIsBusPowered ? "Yes" : "No") << endl;
	fout << "Number of Ports:     " << (unsigned int)((PUSBROOTHUBINFO)info)->HubInfo->u.HubInformation.HubDescriptor.bNumberOfPorts << endl;

	if (((PUSBROOTHUBINFO)info)->HubCapabilityEx != NULL)
	{
		fout << "High speed capable:  " <<
			(((PUSBROOTHUBINFO)info)->HubCapabilityEx->CapabilityFlags.HubIsHighSpeedCapable
			? "Yes" : "No") << endl;
		fout << "High speed:          " <<
			(((PUSBROOTHUBINFO)info)->HubCapabilityEx->CapabilityFlags.HubIsHighSpeed
			? "Yes" : "No") << endl;
		fout << "Multiple transaction translations capable:                 " <<
			(((PUSBROOTHUBINFO)info)->HubCapabilityEx->CapabilityFlags.HubIsMultiTtCapable
			? "Yes" : "No") << endl;
		fout << "Performs multiple transaction translations simultaneously: " <<
			(((PUSBROOTHUBINFO)info)->HubCapabilityEx->CapabilityFlags.HubIsMultiTt
			? "Yes" : "No") << endl;
		fout << "Hub wakes when device is connected:                        " <<
			(((PUSBROOTHUBINFO)info)->HubCapabilityEx->CapabilityFlags.HubIsArmedWakeOnConnect
			? "Yes" : "No") << endl;
		fout << "Hub is bus powered:  " <<
			(((PUSBROOTHUBINFO)info)->HubCapabilityEx->CapabilityFlags.HubIsBusPowered
			? "Yes" : "No") << endl;
		fout << "Hub is root:         " <<
			(((PUSBROOTHUBINFO)info)->HubCapabilityEx->CapabilityFlags.HubIsRoot
			? "Yes" : "No") << endl;
	}

	if (((PUSBROOTHUBINFO)info)->UsbDeviceProperties != NULL) {
		fout << "DeviceClass:       " << ((PUSBROOTHUBINFO)info)->UsbDeviceProperties->DeviceClass << endl;
		fout << "DeviceId:          " << ((PUSBROOTHUBINFO)info)->UsbDeviceProperties->DeviceId << endl;
		fout << "Service:           " << ((PUSBROOTHUBINFO)info)->UsbDeviceProperties->Service << endl;
	}
	fout << endl;
}


// function prints general information about all USB devices
void printInfoDevice(PUSBDEVICEINFO info)
{
	if (diplayInfo != 0 && diplayInfo != 3) return;

	fout << "---------------------------------------" << endl;
	if (info == NULL) return;
	if (info->ConnectionInfo != NULL &&
		info->ConnectionInfo->DeviceDescriptor.idVendor == 0)
		return;

	if (info->DeviceInfoType != NULL) {
		fout << "Device Type:         ";
		switch (info->DeviceInfoType)
		{
		case HostControllerInfo:
			fout << "HostControllerInfo type of device" << endl;
			break;
		case RootHubInfo:
			fout << "RootHubInfo type of device" << endl;
			break;
		case ExternalHubInfo:
			fout << "ExternalHubInfo type of device" << endl;
			break;
		case DeviceInfo:
			fout << "DeviceInfo type of device" << endl;
			break;
		default:
			fout << "Unkown type of device" << endl;
			break;
		}
	}
	if (info->UsbDeviceProperties != NULL) {
		if (info->UsbDeviceProperties->DeviceClass != NULL)
			fout << "DeviceClass:         " << info->UsbDeviceProperties->DeviceClass << endl;
		if (info->UsbDeviceProperties->DeviceDesc != NULL)
			fout << "DeviceDesc:          " << info->UsbDeviceProperties->DeviceDesc << endl;
		if (info->UsbDeviceProperties->DeviceId != NULL)
			fout << "DeviceId:            " << info->UsbDeviceProperties->DeviceId << endl;
		if (info->UsbDeviceProperties->HwId != NULL)
			fout << "HwId:                " << info->UsbDeviceProperties->HwId << endl;
		if (info->UsbDeviceProperties->Service != NULL)
			fout << "Service:             " << info->UsbDeviceProperties->Service << endl;
		if (info->UsbDeviceProperties->PowerState != NULL)
			fout << "PowerState:          " << info->UsbDeviceProperties->PowerState << endl;
	}
	if (info->ConnectionInfo != NULL) {
		fout << "idVendor:            " << hex << "0x" << info->ConnectionInfo->DeviceDescriptor.idVendor << endl;
		fout << "iProduct:            " << hex << "0x" << (unsigned int)info->ConnectionInfo->DeviceDescriptor.iProduct << endl;
		fout << "bcdDevice:           " << hex << "0x" << info->ConnectionInfo->DeviceDescriptor.bcdDevice << endl;
		fout << "iManufacturer:       " << hex << "0x" << (unsigned int)info->ConnectionInfo->DeviceDescriptor.iManufacturer << endl;
	}
	if (info->DeviceInfoNode != NULL) {
		fout << "DeviceDriverName:    " << info->DeviceInfoNode->DeviceDriverName << endl;
	}
	fout << endl;
}