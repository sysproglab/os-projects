#include <stdlib.h>
#include <tchar.h>
#include <iostream>
#include <iomanip>
#include <cstring>
#include "header.h"

using namespace ::std;

#pragma comment (lib, "Setupapi.lib") // for fix LINK2019


PCHAR ConnectionStatuses[] =
{
	"",                   // 0  - NoDeviceConnected
	"",                   // 1  - DeviceConnected
	"FailedEnumeration",  // 2  - DeviceFailedEnumeration
	"GeneralFailure",     // 3  - DeviceGeneralFailure
	"Overcurrent",        // 4  - DeviceCausedOvercurrent
	"NotEnoughPower",     // 5  - DeviceNotEnoughPower
	"NotEnoughBandwidth", // 6  - DeviceNotEnoughBandwidth
	"HubNestedTooDeeply", // 7  - DeviceHubNestedTooDeeply
	"InLegacyHub",        // 8  - DeviceInLegacyHub
	"Enumerating",        // 9  - DeviceEnumerating
	"Reset"               // 10 - DeviceReset
};

VOID
EnumerateHubPorts(
	HTREEITEM   hTreeParent,
	HANDLE      hHubDevice,
	ULONG       NumPorts
);

PCHAR
GetDriverKeyName(
	HANDLE  Hub,
	ULONG   ConnectionIndex
);

PDEVICE_INFO_NODE
FindMatchingDeviceNodeForDriverName(
	PSTR   DriverKeyName,
	BOOLEAN IsHub
);

PUSB_DESCRIPTOR_REQUEST
GetConfigDescriptor(
	HANDLE  hHubDevice,
	ULONG   ConnectionIndex,
	UCHAR   DescriptorIndex
);

PUSB_DESCRIPTOR_REQUEST
GetBOSDescriptor(
	HANDLE  hHubDevice,
	ULONG   ConnectionIndex
);

BOOL
AreThereStringDescriptors(
	PUSB_DEVICE_DESCRIPTOR          DeviceDesc,
	PUSB_CONFIGURATION_DESCRIPTOR   ConfigDesc
);


PSTRING_DESCRIPTOR_NODE
GetAllStringDescriptors(
	HANDLE                          hHubDevice,
	ULONG                           ConnectionIndex,
	PUSB_DEVICE_DESCRIPTOR          DeviceDesc,
	PUSB_CONFIGURATION_DESCRIPTOR   ConfigDesc
);

PSTRING_DESCRIPTOR_NODE
GetStringDescriptor(
	HANDLE  hHubDevice,
	ULONG   ConnectionIndex,
	UCHAR   DescriptorIndex,
	USHORT  LanguageID
);

HRESULT
GetStringDescriptors(
	HANDLE                         hHubDevice,
	ULONG                          ConnectionIndex,
	UCHAR                          DescriptorIndex,
	ULONG                          NumLanguageIDs,
	USHORT  *LanguageIDs,
	PSTRING_DESCRIPTOR_NODE        StringDescNodeHead
);

PCHAR GetExternalHubName(
	HANDLE  Hub,
	ULONG   ConnectionIndex
);


VOID
EnumerateHub(
	HTREEITEM	hTreeParent,
	PCHAR		HubName,
	size_t		cbHubName,
	PUSB_NODE_CONNECTION_INFORMATION_EX    ConnectionInfo,
	PUSB_NODE_CONNECTION_INFORMATION_EX_V2 ConnectionInfoV2,
	PUSB_PORT_CONNECTOR_PROPERTIES         PortConnectorProps,
	PUSB_DESCRIPTOR_REQUEST                ConfigDesc,
	PUSB_DESCRIPTOR_REQUEST                BosDesc,
	PSTRING_DESCRIPTOR_NODE                StringDescs,
	PUSB_DEVICE_PNP_STRINGS                DevProps
)
{
	OOPS("EnumerateHub", 0);
	// Initialize locals to not allocated state so the error cleanup routine
	// only tries to cleanup things that were successfully allocated.
	//
	PUSB_NODE_INFORMATION    hubInfo = NULL;
	PUSB_HUB_INFORMATION_EX  hubInfoEx = NULL;
	PUSB_HUB_CAPABILITIES_EX hubCapabilityEx = NULL;
	HANDLE                  hHubDevice = INVALID_HANDLE_VALUE;
	HTREEITEM               hItem = NULL;
	PVOID                   info = NULL;
	PCHAR                   deviceName = NULL;
	ULONG                   nBytes = 0;
	BOOL                    success = 0;
	DWORD                   dwSizeOfLeafName = 0;
	HRESULT                 hr = S_OK;
	size_t                  cchHeader = 0;
	size_t                  cchFullHubName = 0;

	// Allocate some space for a USBDEVICEINFO structure to hold the
	// hub info, hub name, and connection info pointers.  GPTR zero
	// initializes the structure for us.
	//
	info = ALLOC(sizeof(USBEXTERNALHUBINFO));
	if (info == NULL)
	{
		OOPS("EnumerateHub", 5);
		goto EnumerateHubError;
	}

	// Allocate some space for a USB_NODE_INFORMATION structure for this Hub
	//
	hubInfo = (PUSB_NODE_INFORMATION)ALLOC(sizeof(USB_NODE_INFORMATION));
	if (hubInfo == NULL)
	{
		OOPS("EnumerateHub", 10);
		goto EnumerateHubError;
	}

	hubInfoEx = (PUSB_HUB_INFORMATION_EX)ALLOC(sizeof(USB_HUB_INFORMATION_EX));
	if (hubInfoEx == NULL)
	{
		OOPS("EnumerateHub", 15);
		goto EnumerateHubError;
	}

	hubCapabilityEx = (PUSB_HUB_CAPABILITIES_EX)ALLOC(sizeof(USB_HUB_CAPABILITIES_EX));
	if (hubCapabilityEx == NULL)
	{
		OOPS("EnumerateHub", 20);
		goto EnumerateHubError;
	}

	// Keep copies of the Hub Name, Connection Info, and Configuration
	// Descriptor pointers
	//
	((PUSBROOTHUBINFO)info)->HubInfo = hubInfo;
	((PUSBROOTHUBINFO)info)->HubName = HubName;

	if (ConnectionInfo != NULL)
	{
		// external hub
		OOPS("EnumerateHub", 25);
		((PUSBEXTERNALHUBINFO)info)->DeviceInfoType = ExternalHubInfo;
		((PUSBEXTERNALHUBINFO)info)->ConnectionInfo = ConnectionInfo;
		((PUSBEXTERNALHUBINFO)info)->ConfigDesc = ConfigDesc;
		((PUSBEXTERNALHUBINFO)info)->StringDescs = StringDescs;
		((PUSBEXTERNALHUBINFO)info)->PortConnectorProps = PortConnectorProps;
		((PUSBEXTERNALHUBINFO)info)->HubInfoEx = hubInfoEx;
		((PUSBEXTERNALHUBINFO)info)->HubCapabilityEx = hubCapabilityEx;
		((PUSBEXTERNALHUBINFO)info)->BosDesc = BosDesc;
		((PUSBEXTERNALHUBINFO)info)->ConnectionInfoV2 = ConnectionInfoV2;
		((PUSBEXTERNALHUBINFO)info)->UsbDeviceProperties = DevProps;
	}
	else
	{
		// rootHub
		OOPS("EnumerateHub", 30);
		((PUSBROOTHUBINFO)info)->DeviceInfoType = RootHubInfo;
		((PUSBROOTHUBINFO)info)->HubInfoEx = hubInfoEx;
		((PUSBROOTHUBINFO)info)->HubCapabilityEx = hubCapabilityEx;
		((PUSBROOTHUBINFO)info)->PortConnectorProps = PortConnectorProps;
		((PUSBROOTHUBINFO)info)->UsbDeviceProperties = DevProps;
	}

	// Allocate a temp buffer for the full hub device name.
	//
	hr = StringCbLength((LPCTSTR)"\\\\.\\", MAX_DEVICE_PROP, &cchHeader);
	if (FAILED(hr))
	{
		OOPS("EnumerateHub", 35);
		goto EnumerateHubError;
	}
	cchFullHubName = cchHeader + cbHubName + 1;
	deviceName = (PCHAR)ALLOC((DWORD)cchFullHubName);
	if (deviceName == NULL)
	{
		OOPS("EnumerateHub", 40);
		goto EnumerateHubError;
	}

	// Create the full hub device name
	//
	hr = StringCchCopyN((LPWSTR)deviceName, cchFullHubName, (LPWSTR)"\\\\?\\", cchHeader);
	if (FAILED(hr))
	{
		OOPS("EnumerateHub", 45);
		goto EnumerateHubError;
	}
	hr = StringCchCatN((LPWSTR)deviceName, cchFullHubName, (LPWSTR)HubName, cbHubName);
	if (FAILED(hr))
	{
		OOPS("EnumerateHub", 50);
		goto EnumerateHubError;
	}

	// Try to hub the open device
	//
	hHubDevice = CreateFileA(deviceName,	// CreateFileA -> ANSI, CreateFileW -> Unicode
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	// Done with temp buffer for full hub device name
	//FREE(deviceName);
	OOPS("EnumerateHub", 60);
	if (hHubDevice == INVALID_HANDLE_VALUE)
	{
		OOPS("EnumerateHub", 65);
		goto EnumerateHubError;
	}

	//
	// Now query USBHUB for the USB_NODE_INFORMATION structure for this hub.
	// This will tell us the number of downstream ports to enumerate, among
	// other things.
	//
	success = DeviceIoControl(hHubDevice,
								IOCTL_USB_GET_NODE_INFORMATION,
								hubInfo,
								sizeof(USB_NODE_INFORMATION),
								hubInfo,
								sizeof(USB_NODE_INFORMATION),
								&nBytes,
								NULL);
	OOPS("EnumerateHub", 70);
	if (!success)
	{
		OOPS("EnumerateHub", 75);
		goto EnumerateHubError;
	}
	
	// fail only for RootHub
	success = DeviceIoControl(hHubDevice,
								IOCTL_USB_GET_HUB_INFORMATION_EX,
								hubInfoEx,
								sizeof(USB_HUB_INFORMATION_EX),
								hubInfoEx,
								sizeof(USB_HUB_INFORMATION_EX),
								&nBytes,
								NULL);

	OOPS("EnumerateHub", 80, false);
	//
	// Fail gracefully for downlevel OS's from Win8
	// (must be fail for ROOTHUB)
	//
	if (!success || nBytes < sizeof(USB_HUB_INFORMATION_EX))
	{
		FREE(hubInfoEx);
		hubInfoEx = NULL;
		if (ConnectionInfo != NULL)
		{
			OOPS("EnumerateHub", 81);
			((PUSBEXTERNALHUBINFO)info)->HubInfoEx = NULL;
		}
		else
		{
			OOPS("EnumerateHub", 82);
			((PUSBROOTHUBINFO)info)->HubInfoEx = NULL;		
		}
	}

	//
	// Obtain Hub Capabilities
	//
	success = DeviceIoControl(hHubDevice,
		IOCTL_USB_GET_HUB_CAPABILITIES_EX,
		hubCapabilityEx,
		sizeof(USB_HUB_CAPABILITIES_EX),
		hubCapabilityEx,
		sizeof(USB_HUB_CAPABILITIES_EX),
		&nBytes,
		NULL);
	OOPS("EnumerateHub", 95);



	//
	// Fail gracefully
	// (must be fail for ROOTHUB)
	//
	if (!success || nBytes < sizeof(USB_HUB_CAPABILITIES_EX))
	{
		FREE(hubCapabilityEx);
		hubCapabilityEx = NULL;
		if (ConnectionInfo != NULL)
		{
			OOPS("EnumerateHub", 100, true);
			((PUSBEXTERNALHUBINFO)info)->HubCapabilityEx = NULL;
		}
		else
		{
			OOPS("EnumerateHub", 105, true);
			((PUSBROOTHUBINFO)info)->HubCapabilityEx = NULL;
		}
	}

	// Now recursively enumerate the ports of this hub.
	//
	printInfoHub(info);
	OOPS("EnumerateHub", 130);
	EnumerateHubPorts(
		NULL,//hItem,
		hHubDevice,
		hubInfo->u.HubInformation.HubDescriptor.bNumberOfPorts
		);
	CloseHandle(hHubDevice);
	return;

EnumerateHubError:
	OOPS("EnumerateHubERROR", 135);
	//
	// Clean up any stuff that got allocated
	//

	if (hHubDevice != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hHubDevice);
		hHubDevice = INVALID_HANDLE_VALUE;
	}

	if (hubInfo)		FREE(hubInfo);
	if (hubInfoEx)		FREE(hubInfoEx);
	if (info)			FREE(info);
	if (HubName)		FREE(HubName);
	if (ConnectionInfo) FREE(ConnectionInfo);
	if (ConfigDesc)		FREE(ConfigDesc);
	if (BosDesc)		FREE(BosDesc);

	if (StringDescs != NULL)
	{
		PSTRING_DESCRIPTOR_NODE Next;

		do {

			Next = StringDescs->Next;
			FREE(StringDescs);
			StringDescs = Next;

		} while (StringDescs != NULL);
	}
}


VOID
EnumerateHubPorts(
	HTREEITEM   hTreeParent,
	HANDLE      hHubDevice,
	ULONG       NumPorts
)
{
	ULONG       index = 0;
	BOOL        success = 0;
	HRESULT     hr = S_OK;
	PCHAR       driverKeyName = NULL;
	PUSB_DEVICE_PNP_STRINGS DevProps;
	DWORD       dwSizeOfLeafName = 0;
	int         icon = 0;

	PUSB_NODE_CONNECTION_INFORMATION_EX    connectionInfoEx;
	PUSB_PORT_CONNECTOR_PROPERTIES         pPortConnectorProps;
	USB_PORT_CONNECTOR_PROPERTIES          portConnectorProps;
	PUSB_DESCRIPTOR_REQUEST                configDesc;
	PUSB_DESCRIPTOR_REQUEST                bosDesc;
	PSTRING_DESCRIPTOR_NODE                stringDescs;
	PUSBDEVICEINFO                         info;
	PUSB_NODE_CONNECTION_INFORMATION_EX_V2 connectionInfoExV2;
	PDEVICE_INFO_NODE                      pNode;

	// Loop over all ports of the hub.
	//
	// Port indices are 1 based, not 0 based.
	//
	for (index = 1; index <= NumPorts; index++)
	{
		ULONG nBytesEx;
		ULONG nBytes = 0;

		connectionInfoEx = NULL;
		pPortConnectorProps = NULL;
		ZeroMemory(&portConnectorProps, sizeof(portConnectorProps));
		configDesc = NULL;
		bosDesc = NULL;
		stringDescs = NULL;
		info = NULL;
		connectionInfoExV2 = NULL;
		pNode = NULL;
		DevProps = NULL;

		//
		// Allocate space to hold the connection info for this port.
		// For now, allocate it big enough to hold info for 30 pipes.
		//
		// Endpoint numbers are 0-15.  Endpoint number 0 is the standard
		// control endpoint which is not explicitly listed in the Configuration
		// Descriptor.  There can be an IN endpoint and an OUT endpoint at
		// endpoint numbers 1-15 so there can be a maximum of 30 endpoints
		// per device configuration.
		//
		// Should probably size this dynamically at some point.
		//

		nBytesEx = sizeof(USB_NODE_CONNECTION_INFORMATION_EX)+
			(sizeof(USB_PIPE_INFO)* 30);

		connectionInfoEx = (PUSB_NODE_CONNECTION_INFORMATION_EX)ALLOC(nBytesEx);

		if (connectionInfoEx == NULL)
		{
			OOPS("ports", 1);
			break;
		}

		connectionInfoExV2 = (PUSB_NODE_CONNECTION_INFORMATION_EX_V2)
			ALLOC(sizeof(USB_NODE_CONNECTION_INFORMATION_EX_V2));

		if (connectionInfoExV2 == NULL)
		{
			OOPS("ports", 2);
			FREE(connectionInfoEx);
			break;
		}

		//
		// Now query USBHUB for the structures
		// for this port.  This will tell us if a device is attached to this
		// port, among other things.
		// The fault tolerate code is executed first.
		//

		portConnectorProps.ConnectionIndex = index;
		OOPS("ports", 5);
		success = DeviceIoControl(hHubDevice,
								IOCTL_USB_GET_PORT_CONNECTOR_PROPERTIES,	// get information about a specific (concrete)
								&portConnectorProps,						// port (failed)
								sizeof(USB_PORT_CONNECTOR_PROPERTIES),
								&portConnectorProps,
								sizeof(USB_PORT_CONNECTOR_PROPERTIES),
								&nBytes,
								NULL);
		OOPS("ports", 10, false);

		if (success && nBytes == sizeof(USB_PORT_CONNECTOR_PROPERTIES)) // not success
		{
			pPortConnectorProps = (PUSB_PORT_CONNECTOR_PROPERTIES)
				ALLOC(portConnectorProps.ActualLength);

			if (pPortConnectorProps != NULL)
			{
				pPortConnectorProps->ConnectionIndex = index;
				success = DeviceIoControl(hHubDevice,
					IOCTL_USB_GET_PORT_CONNECTOR_PROPERTIES,
					pPortConnectorProps,
					portConnectorProps.ActualLength,
					pPortConnectorProps,
					portConnectorProps.ActualLength,
					&nBytes,
					NULL);
				OOPS("ports", 13, false);
				if (!success || nBytes < portConnectorProps.ActualLength)
				{
					OOPS("ports", 14);
					FREE(pPortConnectorProps);
					pPortConnectorProps = NULL;
				}
			}
			else OOPS("ports", 12);
		}
		else OOPS("ports", 11);

		connectionInfoExV2->ConnectionIndex = index;
		connectionInfoExV2->Length = sizeof(USB_NODE_CONNECTION_INFORMATION_EX_V2);
		connectionInfoExV2->SupportedUsbProtocols.Usb300 = 1;

		success = DeviceIoControl(hHubDevice,
			IOCTL_USB_GET_NODE_CONNECTION_INFORMATION_EX_V2, // to retrieve information about the protocols,
			connectionInfoExV2,								 // that are supported by a particular USB port on a hub
			sizeof(USB_NODE_CONNECTION_INFORMATION_EX_V2),	 // (don`t use for RooTHub)
			connectionInfoExV2,
			sizeof(USB_NODE_CONNECTION_INFORMATION_EX_V2),
			&nBytes,
			NULL);
		if (!success) OOPS("ports", 25, false);
		if (!success || nBytes < sizeof(USB_NODE_CONNECTION_INFORMATION_EX_V2))
		{
			OOPS("ports", 26);
			FREE(connectionInfoExV2);
			connectionInfoExV2 = NULL;
		}
		connectionInfoEx->ConnectionIndex = index;

		/*
		send the hub an IOCTL_USB_GET_NODE_CONNECTION_INFORMATION_EX
		request for each downstream port of the hub to get info about the
		device (if any) attached to each port.
		*/
		success = DeviceIoControl(hHubDevice,
			IOCTL_USB_GET_NODE_CONNECTION_INFORMATION_EX,
			connectionInfoEx,
			nBytesEx,
			connectionInfoEx,
			nBytesEx,
			&nBytesEx,
			NULL);
		OOPS("ports", 30, false);

		if (success) {
			//
			// Since the USB_NODE_CONNECTION_INFORMATION_EX is used to display
			// the device speed, but the hub driver doesn't support indication
			// of superspeed, we overwrite the value if the super speed
			// data structures are available and indicate the device is operating
			// at SuperSpeed.
			// 

			if (connectionInfoEx->Speed == UsbHighSpeed
				&& connectionInfoExV2 != NULL
				&& (connectionInfoExV2->Flags.DeviceIsOperatingAtSuperSpeedOrHigher))
				//|| connectionInfoExV2->Flags.DeviceIsOperatingAtSuperSpeedPlusOrHigher)) // no member a the enum{}
			{
				connectionInfoEx->Speed = UsbSuperSpeed;
			}
		}
		else
		{
			OOPS("ports", 31, false);
			cout << "Unsupported part of code." << endl;
			system("pause"); //***
			PUSB_NODE_CONNECTION_INFORMATION    connectionInfo = NULL;

			// Try using IOCTL_USB_GET_NODE_CONNECTION_INFORMATION
			// instead of IOCTL_USB_GET_NODE_CONNECTION_INFORMATION_EX
			//

			nBytes = sizeof(USB_NODE_CONNECTION_INFORMATION)+
				sizeof(USB_PIPE_INFO)* 30;

			connectionInfo = (PUSB_NODE_CONNECTION_INFORMATION)ALLOC(nBytes);

			if (connectionInfo == NULL)
			{
				OOPS(203);

				FREE(connectionInfoEx);
				if (pPortConnectorProps != NULL)
				{
					FREE(pPortConnectorProps);
				}
				if (connectionInfoExV2 != NULL)
				{
					FREE(connectionInfoExV2);
				}
				continue;
			}

			connectionInfo->ConnectionIndex = index;

			success = DeviceIoControl(hHubDevice,
				IOCTL_USB_GET_NODE_CONNECTION_INFORMATION,
				connectionInfo,
				nBytes,
				connectionInfo,
				nBytes,
				&nBytes,
				NULL);

			if (!success)
			{
				OOPS(204);

				FREE(connectionInfo);
				FREE(connectionInfoEx);
				if (pPortConnectorProps != NULL)
				{
					FREE(pPortConnectorProps);
				}
				if (connectionInfoExV2 != NULL)
				{
					FREE(connectionInfoExV2);
				}
				continue;
			}

			// Copy IOCTL_USB_GET_NODE_CONNECTION_INFORMATION into
			// IOCTL_USB_GET_NODE_CONNECTION_INFORMATION_EX structure.
			//
			connectionInfoEx->ConnectionIndex = connectionInfo->ConnectionIndex;
			connectionInfoEx->DeviceDescriptor = connectionInfo->DeviceDescriptor;
			connectionInfoEx->CurrentConfigurationValue = connectionInfo->CurrentConfigurationValue;
			connectionInfoEx->Speed = connectionInfo->LowSpeed ? UsbLowSpeed : UsbFullSpeed;
			connectionInfoEx->DeviceIsHub = connectionInfo->DeviceIsHub;
			connectionInfoEx->DeviceAddress = connectionInfo->DeviceAddress;
			connectionInfoEx->NumberOfOpenPipes = connectionInfo->NumberOfOpenPipes;
			connectionInfoEx->ConnectionStatus = connectionInfo->ConnectionStatus;

			memcpy(&connectionInfoEx->PipeList[0],
				&connectionInfo->PipeList[0],
				sizeof(USB_PIPE_INFO)* 30);

			FREE(connectionInfo);
		}

		// Update the count of connected devices
		//
		if (connectionInfoEx->ConnectionStatus == DeviceConnected)
		{
			TotalDevicesConnected++;
		}

		if (connectionInfoEx->DeviceIsHub)
		{
			TotalHubs++;
		}

		
		// If there is a device connected, get the Device Description
		//
		if (connectionInfoEx->ConnectionStatus != NoDeviceConnected)
		{
			driverKeyName = GetDriverKeyName(hHubDevice, index);
			if (driverKeyName)
			{
				size_t cbDriverName = 0;

				hr = StringCbLength((LPCTSTR)driverKeyName, MAX_DRIVER_KEY_NAME, &cbDriverName);
				if (SUCCEEDED(hr))
				{
					DevProps = DriverNameToDeviceProperties(driverKeyName, cbDriverName);
					//if (DevProps != NULL) cout << DevProps->DeviceDesc << endl;

					pNode = FindMatchingDeviceNodeForDriverName(driverKeyName, connectionInfoEx->DeviceIsHub);
					//if (pNode != NULL) cout << pNode->DeviceDescName << endl;
				}
				FREE(driverKeyName);
			}

		}

		// If there is a device connected to the port, try to retrieve the
		// Configuration Descriptor from the device.
		//
		if (gDoConfigDesc &&
			connectionInfoEx->ConnectionStatus == DeviceConnected)
		{
			configDesc = GetConfigDescriptor(hHubDevice,
				index,
				0);
		}
		else
		{
			configDesc = NULL;
		}

		if (configDesc != NULL &&	// configDesc != NULL is TRUE
			connectionInfoEx->DeviceDescriptor.bcdUSB > 0x0200) // is FALSE
		{
			bosDesc = GetBOSDescriptor(hHubDevice,
				index);
		}
		else
		{
			bosDesc = NULL;
		}

		if (configDesc != NULL &&
			AreThereStringDescriptors(&connectionInfoEx->DeviceDescriptor,
			(PUSB_CONFIGURATION_DESCRIPTOR)(configDesc + 1)))
		{
			stringDescs = GetAllStringDescriptors(
				hHubDevice,
				index,
				&connectionInfoEx->DeviceDescriptor,
				(PUSB_CONFIGURATION_DESCRIPTOR)(configDesc + 1));
		}
		else
		{
			stringDescs = NULL;
		}

		// If the device connected to the port is an external hub, get the
		// name of the external hub and recursively enumerate it.
		//
		if (connectionInfoEx->DeviceIsHub)
		{
			PCHAR extHubName;
			size_t cbHubName = 0;

			extHubName = GetExternalHubName(hHubDevice, index);
			if (extHubName != NULL)
			{
				hr = StringCbLength((LPCTSTR)extHubName, MAX_DRIVER_KEY_NAME, &cbHubName);
				if (SUCCEEDED(hr))
				{
					EnumerateHub(hTreeParent, //hPortItem,
						extHubName,
						cbHubName,
						connectionInfoEx,
						connectionInfoExV2,
						pPortConnectorProps,
						configDesc,
						bosDesc,
						stringDescs,
						DevProps);
				}
			}
		}
		else
		{
			// Allocate some space for a USBDEVICEINFO structure to hold the
			// hub info, hub name, and connection info pointers.  GPTR zero
			// initializes the structure for us.
			//
			info = (PUSBDEVICEINFO)ALLOC(sizeof(USBDEVICEINFO));

			if (info == NULL)
			{
				OOPS(205);
				if (configDesc != NULL)
				{
					FREE(configDesc);
				}
				if (bosDesc != NULL)
				{
					FREE(bosDesc);
				}
				FREE(connectionInfoEx);

				if (pPortConnectorProps != NULL)
				{
					FREE(pPortConnectorProps);
				}
				if (connectionInfoExV2 != NULL)
				{
					FREE(connectionInfoExV2);
				}
				break;
			}
			info->DeviceInfoType = DeviceInfo;		// type of device (HostControllerInfo, RootHubInfo, ExternalHubInfo, DeviceInfo)
			info->ConnectionInfo = connectionInfoEx; //
			info->PortConnectorProps = pPortConnectorProps;
			info->ConfigDesc = configDesc;
			info->StringDescs = stringDescs;
			info->BosDesc = bosDesc;
			info->ConnectionInfoV2 = connectionInfoExV2;
			info->UsbDeviceProperties = DevProps;
			info->DeviceInfoNode = pNode;
			printInfoDevice(info);
		}
	} // for
}

PCHAR GetDriverKeyName(
	HANDLE  Hub,
	ULONG   ConnectionIndex
	)
{
	BOOL                                success = 0;
	ULONG                               nBytes = 0;
	USB_NODE_CONNECTION_DRIVERKEY_NAME  driverKeyName;
	PUSB_NODE_CONNECTION_DRIVERKEY_NAME driverKeyNameW = NULL;
	PCHAR                               driverKeyNameA = NULL;

	// Get the length of the name of the driver key of the device attached to
	// the specified port.
	//
	driverKeyName.ConnectionIndex = ConnectionIndex;

	success = DeviceIoControl(Hub,
		IOCTL_USB_GET_NODE_CONNECTION_DRIVERKEY_NAME,
		&driverKeyName,
		sizeof(driverKeyName),
		&driverKeyName,
		sizeof(driverKeyName),
		&nBytes,
		NULL);

	if (!success)
	{
		OOPS(207);
		goto GetDriverKeyNameError;
	}

	// Allocate space to hold the driver key name
	//
	nBytes = driverKeyName.ActualLength;

	if (nBytes <= sizeof(driverKeyName))
	{
		OOPS(208);
		goto GetDriverKeyNameError;
	}

	driverKeyNameW = (PUSB_NODE_CONNECTION_DRIVERKEY_NAME)ALLOC(nBytes);
	if (driverKeyNameW == NULL)
	{
		OOPS(209);
		goto GetDriverKeyNameError;
	}

	// Get the name of the driver key of the device attached to
	// the specified port.
	//
	driverKeyNameW->ConnectionIndex = ConnectionIndex;

	success = DeviceIoControl(Hub,
		IOCTL_USB_GET_NODE_CONNECTION_DRIVERKEY_NAME,
		driverKeyNameW,
		nBytes,
		driverKeyNameW,
		nBytes,
		&nBytes,
		NULL);

	if (!success)
	{
		OOPS(210);
		goto GetDriverKeyNameError;
	}

	// Convert the driver key name
	//
	driverKeyNameA = WideStrToMultiStr(driverKeyNameW->DriverKeyName, 
		nBytes - sizeof(USB_NODE_CONNECTION_DRIVERKEY_NAME)+sizeof(WCHAR));

	// All done, free the uncoverted driver key name and return the
	// converted driver key name
	//
	FREE(driverKeyNameW);

	return driverKeyNameA;


GetDriverKeyNameError:
	// There was an error, free anything that was allocated
	//
	if (driverKeyNameW != NULL)
	{
		FREE(driverKeyNameW);
		driverKeyNameW = NULL;
	}

	return NULL;
}

PDEVICE_INFO_NODE
FindMatchingDeviceNodeForDriverName(
_In_ PSTR   DriverKeyName,
_In_ BOOLEAN IsHub
)
{
	PDEVICE_INFO_NODE pNode = NULL;
	PDEVICE_GUID_LIST pList = NULL;
	PLIST_ENTRY       pEntry = NULL;

	pList = IsHub ? &gHubList : &gDeviceList;

	pEntry = pList->ListHead.Flink;

	while (pEntry != &pList->ListHead)
	{
		pNode = CONTAINING_RECORD(pEntry,
			DEVICE_INFO_NODE,
			ListEntry);

		PCHAR tmpDriverName = NULL;
		_bstr_t tmp_bstr_t(pNode->DeviceDriverName);
		tmpDriverName = tmp_bstr_t;
		if (_stricmp(DriverKeyName, tmpDriverName) == 0)
		{
			return pNode;
		}

		pEntry = pEntry->Flink;
	}
	return NULL;
}


/*
PDEVICE_INFO_NODE
FindMatchingDeviceNodeForDriverName(
PSTR   DriverKeyName,
BOOLEAN IsHub
)
{
PDEVICE_INFO_NODE pNode = NULL;
PDEVICE_GUID_LIST pList = NULL;
PLIST_ENTRY       pEntry = NULL;

const int bufSize = 200;
char DeviceDriverName[bufSize];

pList = IsHub ? &gHubList : &gDeviceList;

pEntry = pList->ListHead.Flink;

while (pEntry != &pList->ListHead)
{
pNode = CONTAINING_RECORD(pEntry,
DEVICE_INFO_NODE,
ListEntry);

const wchar_t * inputW = pNode->DeviceDriverName;
cout << &inputW << endl;
system("pause");
char * outputString = new char[bufSize];
memset(DeviceDriverName, 0, bufSize);
size_t charsConverted = 0;
inputW = pNode->DeviceDriverName;
wcstombs_s(&charsConverted, outputString, wcslen(inputW) + 1, inputW, wcslen(inputW));
// conversion
/*
wcstombs_s(DeviceDriverName, pNode->DeviceDriverName, wcslen(pNode->DeviceDriverName));
wcstombs_s(wcslen(pNode->DeviceDriverName), )
DeviceDriverName[bufSize - 1] = 0;	// finish string

size_t outputSize = wcslen(pNode->DeviceDriverName) + 1; // +1 for null terminator
memset(DeviceDriverName, 0, bufSize);

//const wchar_t * inputW = input.c_str();
wcstombs_s(&charsConverted, DeviceDriverName, outputSize, pNode->DeviceDriverName, wcslen(pNode->DeviceDriverName));
int a = 0;
/
if (_stricmp(DriverKeyName, DeviceDriverName) == 0)
{
cout << "it`s ound" << endl;
return pNode;
}

pEntry = pEntry->Flink;
}

return NULL;
}
*/

PUSB_DESCRIPTOR_REQUEST
GetConfigDescriptor(
	HANDLE  hHubDevice,
	ULONG   ConnectionIndex,
	UCHAR   DescriptorIndex
)
{
	BOOL    success = 0;
	ULONG   nBytes = 0;
	ULONG   nBytesReturned = 0;

	UCHAR   configDescReqBuf[sizeof(USB_DESCRIPTOR_REQUEST)+
		sizeof(USB_CONFIGURATION_DESCRIPTOR)];

	PUSB_DESCRIPTOR_REQUEST         configDescReq = NULL;
	PUSB_CONFIGURATION_DESCRIPTOR   configDesc = NULL;


	// Request the Configuration Descriptor the first time using our
	// local buffer, which is just big enough for the Cofiguration
	// Descriptor itself.
	//
	nBytes = sizeof(configDescReqBuf);

	configDescReq = (PUSB_DESCRIPTOR_REQUEST)configDescReqBuf;
	configDesc = (PUSB_CONFIGURATION_DESCRIPTOR)(configDescReq + 1);

	// Zero fill the entire request structure
	//
	memset(configDescReq, 0, nBytes);

	// Indicate the port from which the descriptor will be requested
	//
	configDescReq->ConnectionIndex = ConnectionIndex;

	//
	// USBHUB uses URB_FUNCTION_GET_DESCRIPTOR_FROM_DEVICE to process this
	// IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION request.
	//
	// USBD will automatically initialize these fields:
	//     bmRequest = 0x80
	//     bRequest  = 0x06
	//
	// We must inititialize these fields:
	//     wValue    = Descriptor Type (high) and Descriptor Index (low byte)
	//     wIndex    = Zero (or Language ID for String Descriptors)
	//     wLength   = Length of descriptor buffer
	//
	configDescReq->SetupPacket.wValue = (USB_CONFIGURATION_DESCRIPTOR_TYPE << 8)
		| DescriptorIndex;

	configDescReq->SetupPacket.wLength = (USHORT)(nBytes - sizeof(USB_DESCRIPTOR_REQUEST));

	// Now issue the get descriptor request.
	//
	success = DeviceIoControl(hHubDevice,
		IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION,
		configDescReq,
		nBytes,
		configDescReq,
		nBytes,
		&nBytesReturned,
		NULL);

	if (!success)
	{
		OOPS(211);
		return NULL;
	}

	if (nBytes != nBytesReturned)
	{
		OOPS(212);
		return NULL;
	}

	if (configDesc->wTotalLength < sizeof(USB_CONFIGURATION_DESCRIPTOR))
	{
		OOPS(213);
		return NULL;
	}

	// Now request the entire Configuration Descriptor using a dynamically
	// allocated buffer which is sized big enough to hold the entire descriptor
	//
	nBytes = sizeof(USB_DESCRIPTOR_REQUEST)+configDesc->wTotalLength;

	configDescReq = (PUSB_DESCRIPTOR_REQUEST)ALLOC(nBytes);

	if (configDescReq == NULL)
	{
		OOPS(214);
		return NULL;
	}

	configDesc = (PUSB_CONFIGURATION_DESCRIPTOR)(configDescReq + 1);

	// Indicate the port from which the descriptor will be requested
	//
	configDescReq->ConnectionIndex = ConnectionIndex;

	//
	// USBHUB uses URB_FUNCTION_GET_DESCRIPTOR_FROM_DEVICE to process this
	// IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION request.
	//
	// USBD will automatically initialize these fields:
	//     bmRequest = 0x80
	//     bRequest  = 0x06
	//
	// We must inititialize these fields:
	//     wValue    = Descriptor Type (high) and Descriptor Index (low byte)
	//     wIndex    = Zero (or Language ID for String Descriptors)
	//     wLength   = Length of descriptor buffer
	//
	configDescReq->SetupPacket.wValue = (USB_CONFIGURATION_DESCRIPTOR_TYPE << 8)
		| DescriptorIndex;

	configDescReq->SetupPacket.wLength = (USHORT)(nBytes - sizeof(USB_DESCRIPTOR_REQUEST));

	// Now issue the get descriptor request.
	//

	success = DeviceIoControl(hHubDevice,
		IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION,
		configDescReq,
		nBytes,
		configDescReq,
		nBytes,
		&nBytesReturned,
		NULL);

	if (!success)
	{
		OOPS(215);
		FREE(configDescReq);
		return NULL;
	}

	if (nBytes != nBytesReturned)
	{
		OOPS(216);
		FREE(configDescReq);
		return NULL;
	}

	if (configDesc->wTotalLength != (nBytes - sizeof(USB_DESCRIPTOR_REQUEST)))
	{
		OOPS(217);
		FREE(configDescReq);
		return NULL;
	}

	return configDescReq;
}



//*****************************************************************************
//
// GetBOSDescriptor()
//
// hHubDevice - Handle of the hub device containing the port from which the
// Configuration Descriptor will be requested.
//
// ConnectionIndex - Identifies the port on the hub to which a device is
// attached from which the BOS Descriptor will be requested.
//
//*****************************************************************************

PUSB_DESCRIPTOR_REQUEST
GetBOSDescriptor(
	HANDLE  hHubDevice,
	ULONG   ConnectionIndex
)
{
	BOOL    success = 0;
	ULONG   nBytes = 0;
	ULONG   nBytesReturned = 0;

	UCHAR   bosDescReqBuf[sizeof(USB_DESCRIPTOR_REQUEST)+
		sizeof(USB_BOS_DESCRIPTOR)];

	PUSB_DESCRIPTOR_REQUEST bosDescReq = NULL;
	PUSB_BOS_DESCRIPTOR     bosDesc = NULL;


	// Request the BOS Descriptor the first time using our
	// local buffer, which is just big enough for the BOS
	// Descriptor itself.
	//
	nBytes = sizeof(bosDescReqBuf);

	bosDescReq = (PUSB_DESCRIPTOR_REQUEST)bosDescReqBuf;
	bosDesc = (PUSB_BOS_DESCRIPTOR)(bosDescReq + 1);

	// Zero fill the entire request structure
	//
	memset(bosDescReq, 0, nBytes);

	// Indicate the port from which the descriptor will be requested
	//
	bosDescReq->ConnectionIndex = ConnectionIndex;

	//
	// USBHUB uses URB_FUNCTION_GET_DESCRIPTOR_FROM_DEVICE to process this
	// IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION request.
	//
	// USBD will automatically initialize these fields:
	//     bmRequest = 0x80
	//     bRequest  = 0x06
	//
	// We must inititialize these fields:
	//     wValue    = Descriptor Type (high) and Descriptor Index (low byte)
	//     wIndex    = Zero (or Language ID for String Descriptors)
	//     wLength   = Length of descriptor buffer
	//
	bosDescReq->SetupPacket.wValue = (USB_BOS_DESCRIPTOR_TYPE << 8);

	bosDescReq->SetupPacket.wLength = (USHORT)(nBytes - sizeof(USB_DESCRIPTOR_REQUEST));

	// Now issue the get descriptor request.
	//
	success = DeviceIoControl(hHubDevice,
		IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION,
		bosDescReq,
		nBytes,
		bosDescReq,
		nBytes,
		&nBytesReturned,
		NULL);

	if (!success)
	{
		OOPS(218);
		return NULL;
	}

	if (nBytes != nBytesReturned)
	{
		OOPS(219);
		return NULL;
	}

	if (bosDesc->wTotalLength < sizeof(USB_BOS_DESCRIPTOR))
	{
		OOPS(220);
		return NULL;
	}

	// Now request the entire BOS Descriptor using a dynamically
	// allocated buffer which is sized big enough to hold the entire descriptor
	//
	nBytes = sizeof(USB_DESCRIPTOR_REQUEST)+bosDesc->wTotalLength;

	bosDescReq = (PUSB_DESCRIPTOR_REQUEST)ALLOC(nBytes);

	if (bosDescReq == NULL)
	{
		OOPS(221);
		return NULL;
	}

	bosDesc = (PUSB_BOS_DESCRIPTOR)(bosDescReq + 1);

	// Indicate the port from which the descriptor will be requested
	//
	bosDescReq->ConnectionIndex = ConnectionIndex;

	//
	// USBHUB uses URB_FUNCTION_GET_DESCRIPTOR_FROM_DEVICE to process this
	// IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION request.
	//
	// USBD will automatically initialize these fields:
	//     bmRequest = 0x80
	//     bRequest  = 0x06
	//
	// We must inititialize these fields:
	//     wValue    = Descriptor Type (high) and Descriptor Index (low byte)
	//     wIndex    = Zero (or Language ID for String Descriptors)
	//     wLength   = Length of descriptor buffer
	//
	bosDescReq->SetupPacket.wValue = (USB_BOS_DESCRIPTOR_TYPE << 8);

	bosDescReq->SetupPacket.wLength = (USHORT)(nBytes - sizeof(USB_DESCRIPTOR_REQUEST));

	// Now issue the get descriptor request.
	//

	success = DeviceIoControl(hHubDevice,
		IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION,
		bosDescReq,
		nBytes,
		bosDescReq,
		nBytes,
		&nBytesReturned,
		NULL);

	if (!success)
	{
		OOPS(222);
		FREE(bosDescReq);
		return NULL;
	}

	if (nBytes != nBytesReturned)
	{
		OOPS(223);
		FREE(bosDescReq);
		return NULL;
	}

	if (bosDesc->wTotalLength != (nBytes - sizeof(USB_DESCRIPTOR_REQUEST)))
	{
		OOPS(224);
		FREE(bosDescReq);
		return NULL;
	}

	return bosDescReq;
}



//*****************************************************************************
//
// AreThereStringDescriptors()
//
// DeviceDesc - Device Descriptor for which String Descriptors should be
// checked.
//
// ConfigDesc - Configuration Descriptor (also containing Interface Descriptor)
// for which String Descriptors should be checked.
//
//*****************************************************************************

BOOL
AreThereStringDescriptors(
	PUSB_DEVICE_DESCRIPTOR          DeviceDesc,
	PUSB_CONFIGURATION_DESCRIPTOR   ConfigDesc
)
{
	PUCHAR                  descEnd = NULL;
	PUSB_COMMON_DESCRIPTOR  commonDesc = NULL;

	//
	// Check Device Descriptor strings
	//
	if (DeviceDesc->iManufacturer ||
		DeviceDesc->iProduct ||
		DeviceDesc->iSerialNumber
		)
	{
		return TRUE;
	}


	//
	// Check the Configuration and Interface Descriptor strings
	//

	descEnd = (PUCHAR)ConfigDesc + ConfigDesc->wTotalLength;

	commonDesc = (PUSB_COMMON_DESCRIPTOR)ConfigDesc;

	while ((PUCHAR)commonDesc + sizeof(USB_COMMON_DESCRIPTOR) < descEnd &&
		(PUCHAR)commonDesc + commonDesc->bLength <= descEnd)
	{
		switch (commonDesc->bDescriptorType)
		{
		case USB_CONFIGURATION_DESCRIPTOR_TYPE:
		case USB_OTHER_SPEED_CONFIGURATION_DESCRIPTOR_TYPE:
			if (commonDesc->bLength != sizeof(USB_CONFIGURATION_DESCRIPTOR))
			{
				OOPS(225);
				break;
			}
			if (((PUSB_CONFIGURATION_DESCRIPTOR)commonDesc)->iConfiguration)
			{
				return TRUE;
			}
			commonDesc = (PUSB_COMMON_DESCRIPTOR)((PUCHAR)commonDesc + commonDesc->bLength);
			continue;

		case USB_INTERFACE_DESCRIPTOR_TYPE:
			if (commonDesc->bLength != sizeof(USB_INTERFACE_DESCRIPTOR) &&
				commonDesc->bLength != sizeof(USB_INTERFACE_DESCRIPTOR2))
			{
				OOPS(226);
				break;
			}
			if (((PUSB_INTERFACE_DESCRIPTOR)commonDesc)->iInterface)
			{
				return TRUE;
			}
			commonDesc = (PUSB_COMMON_DESCRIPTOR)((PUCHAR)commonDesc + commonDesc->bLength);
			continue;

		default:
			commonDesc = (PUSB_COMMON_DESCRIPTOR)((PUCHAR)commonDesc + commonDesc->bLength);
			continue;
		}
		break;
	}

	return FALSE;
}


//*****************************************************************************
//
// GetAllStringDescriptors()
//
// hHubDevice - Handle of the hub device containing the port from which the
// String Descriptors will be requested.
//
// ConnectionIndex - Identifies the port on the hub to which a device is
// attached from which the String Descriptors will be requested.
//
// DeviceDesc - Device Descriptor for which String Descriptors should be
// requested.
//
// ConfigDesc - Configuration Descriptor (also containing Interface Descriptor)
// for which String Descriptors should be requested.
//
//*****************************************************************************

PSTRING_DESCRIPTOR_NODE
GetAllStringDescriptors(
	HANDLE                          hHubDevice,
	ULONG                           ConnectionIndex,
	PUSB_DEVICE_DESCRIPTOR          DeviceDesc,
	PUSB_CONFIGURATION_DESCRIPTOR   ConfigDesc
)
{
	PSTRING_DESCRIPTOR_NODE supportedLanguagesString = NULL;
	ULONG                   numLanguageIDs = 0;
	USHORT                  *languageIDs = NULL;

	PUCHAR                  descEnd = NULL;
	PUSB_COMMON_DESCRIPTOR  commonDesc = NULL;
	UCHAR                   uIndex = 1;
	UCHAR                   bInterfaceClass = 0;
	BOOL                    getMoreStrings = FALSE;
	HRESULT                 hr = S_OK;

	//
	// Get the array of supported Language IDs, which is returned
	// in String Descriptor 0
	//
	supportedLanguagesString = GetStringDescriptor(hHubDevice,
		ConnectionIndex,
		0,
		0);

	if (supportedLanguagesString == NULL)
	{
		return NULL;
	}

	numLanguageIDs = (supportedLanguagesString->StringDescriptor->bLength - 2) / 2;

	languageIDs = (USHORT *)&supportedLanguagesString->StringDescriptor->bString[0]; //***

	//
	// Get the Device Descriptor strings
	//

	if (DeviceDesc->iManufacturer)
	{
		GetStringDescriptors(hHubDevice,
			ConnectionIndex,
			DeviceDesc->iManufacturer,
			numLanguageIDs,
			languageIDs,
			supportedLanguagesString);
	}

	if (DeviceDesc->iProduct)
	{
		GetStringDescriptors(hHubDevice,
			ConnectionIndex,
			DeviceDesc->iProduct,
			numLanguageIDs,
			languageIDs,
			supportedLanguagesString);
	}

	if (DeviceDesc->iSerialNumber)
	{
		GetStringDescriptors(hHubDevice,
			ConnectionIndex,
			DeviceDesc->iSerialNumber,
			numLanguageIDs,
			languageIDs,
			supportedLanguagesString);
	}

	//
	// Get the Configuration and Interface Descriptor strings
	//

	descEnd = (PUCHAR)ConfigDesc + ConfigDesc->wTotalLength;

	commonDesc = (PUSB_COMMON_DESCRIPTOR)ConfigDesc;

	while ((PUCHAR)commonDesc + sizeof(USB_COMMON_DESCRIPTOR) < descEnd &&
		(PUCHAR)commonDesc + commonDesc->bLength <= descEnd)
	{
		switch (commonDesc->bDescriptorType)
		{
		case USB_CONFIGURATION_DESCRIPTOR_TYPE:
			if (commonDesc->bLength != sizeof(USB_CONFIGURATION_DESCRIPTOR))
			{
				OOPS(227);
				break;
			}
			if (((PUSB_CONFIGURATION_DESCRIPTOR)commonDesc)->iConfiguration)
			{
				GetStringDescriptors(hHubDevice,
					ConnectionIndex,
					((PUSB_CONFIGURATION_DESCRIPTOR)commonDesc)->iConfiguration,
					numLanguageIDs,
					languageIDs,
					supportedLanguagesString);
			}
			commonDesc = (PUSB_COMMON_DESCRIPTOR)((PUCHAR)commonDesc + commonDesc->bLength);
			continue;

		case USB_IAD_DESCRIPTOR_TYPE:
			if (commonDesc->bLength < sizeof(USB_IAD_DESCRIPTOR))
			{
				OOPS(228);
				break;
			}
			if (((PUSB_IAD_DESCRIPTOR)commonDesc)->iFunction)
			{
				GetStringDescriptors(hHubDevice,
					ConnectionIndex,
					((PUSB_IAD_DESCRIPTOR)commonDesc)->iFunction,
					numLanguageIDs,
					languageIDs,
					supportedLanguagesString);
			}
			commonDesc = (PUSB_COMMON_DESCRIPTOR)((PUCHAR)commonDesc + commonDesc->bLength);
			continue;

		case USB_INTERFACE_DESCRIPTOR_TYPE:
			if (commonDesc->bLength != sizeof(USB_INTERFACE_DESCRIPTOR) &&
				commonDesc->bLength != sizeof(USB_INTERFACE_DESCRIPTOR2))
			{
				OOPS(229);
				break;
			}
			if (((PUSB_INTERFACE_DESCRIPTOR)commonDesc)->iInterface)
			{
				GetStringDescriptors(hHubDevice,
					ConnectionIndex,
					((PUSB_INTERFACE_DESCRIPTOR)commonDesc)->iInterface,
					numLanguageIDs,
					languageIDs,
					supportedLanguagesString);
			}

			//
			// We need to display more string descriptors for the following
			// interface classes
			//
			bInterfaceClass = ((PUSB_INTERFACE_DESCRIPTOR)commonDesc)->bInterfaceClass;
			if (bInterfaceClass == USB_DEVICE_CLASS_VIDEO)
			{
				getMoreStrings = TRUE;
			}
			commonDesc = (PUSB_COMMON_DESCRIPTOR)((PUCHAR)commonDesc + commonDesc->bLength);
			continue;

		default:
			commonDesc = (PUSB_COMMON_DESCRIPTOR)((PUCHAR)commonDesc + commonDesc->bLength);
			continue;
		}
		break;
	}

	if (getMoreStrings)
	{
		//
		// We might need to display strings later that are referenced only in
		// class-specific descriptors. Get String Descriptors 1 through 32 (an
		// arbitrary upper limit for Strings needed due to "bad devices"
		// returning an infinite repeat of Strings 0 through 4) until one is not
		// found.
		//
		// There are also "bad devices" that have issues even querying 1-32, but
		// historically USBView made this query, so the query should be safe for
		// video devices.
		//
		for (uIndex = 1; SUCCEEDED(hr) && (uIndex < NUM_STRING_DESC_TO_GET); uIndex++)
		{
			hr = GetStringDescriptors(hHubDevice,
				ConnectionIndex,
				uIndex,
				numLanguageIDs,
				languageIDs,
				supportedLanguagesString);
		}
	}

	return supportedLanguagesString;
}


//*****************************************************************************
//
// GetStringDescriptor()
//
// hHubDevice - Handle of the hub device containing the port from which the
// String Descriptor will be requested.
//
// ConnectionIndex - Identifies the port on the hub to which a device is
// attached from which the String Descriptor will be requested.
//
// DescriptorIndex - String Descriptor index.
//
// LanguageID - Language in which the string should be requested.
//
//*****************************************************************************

PSTRING_DESCRIPTOR_NODE
GetStringDescriptor(
HANDLE  hHubDevice,
ULONG   ConnectionIndex,
UCHAR   DescriptorIndex,
USHORT  LanguageID
)
{
	BOOL    success = 0;
	ULONG   nBytes = 0;
	ULONG   nBytesReturned = 0;

	UCHAR   stringDescReqBuf[sizeof(USB_DESCRIPTOR_REQUEST)+
		MAXIMUM_USB_STRING_LENGTH];

	PUSB_DESCRIPTOR_REQUEST stringDescReq = NULL;
	PUSB_STRING_DESCRIPTOR  stringDesc = NULL;
	PSTRING_DESCRIPTOR_NODE stringDescNode = NULL;

	nBytes = sizeof(stringDescReqBuf);

	stringDescReq = (PUSB_DESCRIPTOR_REQUEST)stringDescReqBuf;
	stringDesc = (PUSB_STRING_DESCRIPTOR)(stringDescReq + 1);

	// Zero fill the entire request structure
	//
	memset(stringDescReq, 0, nBytes);

	// Indicate the port from which the descriptor will be requested
	//
	stringDescReq->ConnectionIndex = ConnectionIndex;

	//
	// USBHUB uses URB_FUNCTION_GET_DESCRIPTOR_FROM_DEVICE to process this
	// IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION request.
	//
	// USBD will automatically initialize these fields:
	//     bmRequest = 0x80
	//     bRequest  = 0x06
	//
	// We must inititialize these fields:
	//     wValue    = Descriptor Type (high) and Descriptor Index (low byte)
	//     wIndex    = Zero (or Language ID for String Descriptors)
	//     wLength   = Length of descriptor buffer
	//
	stringDescReq->SetupPacket.wValue = (USB_STRING_DESCRIPTOR_TYPE << 8)
		| DescriptorIndex;

	stringDescReq->SetupPacket.wIndex = LanguageID;

	stringDescReq->SetupPacket.wLength = (USHORT)(nBytes - sizeof(USB_DESCRIPTOR_REQUEST));

	// Now issue the get descriptor request.
	//
	success = DeviceIoControl(hHubDevice,
		IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION,
		stringDescReq,
		nBytes,
		stringDescReq,
		nBytes,
		&nBytesReturned,
		NULL);

	//
	// Do some sanity checks on the return from the get descriptor request.
	//

	if (!success)
	{
		OOPS(230);
		return NULL;
	}

	if (nBytesReturned < 2)
	{
		OOPS(231);
		return NULL;
	}

	if (stringDesc->bDescriptorType != USB_STRING_DESCRIPTOR_TYPE)
	{
		OOPS(232);
		return NULL;
	}

	if (stringDesc->bLength != nBytesReturned - sizeof(USB_DESCRIPTOR_REQUEST))
	{
		OOPS(233);
		return NULL;
	}

	if (stringDesc->bLength % 2 != 0)
	{
		OOPS(234);
		return NULL;
	}

	//
	// Looks good, allocate some (zero filled) space for the string descriptor
	// node and copy the string descriptor to it.
	//

	stringDescNode = (PSTRING_DESCRIPTOR_NODE)ALLOC(sizeof(STRING_DESCRIPTOR_NODE)+
		stringDesc->bLength);

	if (stringDescNode == NULL)
	{
		OOPS(235);
		return NULL;
	}

	stringDescNode->DescriptorIndex = DescriptorIndex;
	stringDescNode->LanguageID = LanguageID;

	memcpy(stringDescNode->StringDescriptor,
		stringDesc,
		stringDesc->bLength);

	return stringDescNode;
}


//*****************************************************************************
//
// GetStringDescriptors()
//
// hHubDevice - Handle of the hub device containing the port from which the
// String Descriptor will be requested.
//
// ConnectionIndex - Identifies the port on the hub to which a device is
// attached from which the String Descriptor will be requested.
//
// DescriptorIndex - String Descriptor index.
//
// NumLanguageIDs -  Number of languages in which the string should be
// requested.
//
// LanguageIDs - Languages in which the string should be requested.
//
// StringDescNodeHead - First node in linked list of device's string descriptors
//
// Return Value: HRESULT indicating whether the string is on the list
//
//*****************************************************************************

HRESULT
GetStringDescriptors(
HANDLE                         hHubDevice,
ULONG                          ConnectionIndex,
UCHAR                          DescriptorIndex,
ULONG                          NumLanguageIDs,
USHORT  *LanguageIDs,
PSTRING_DESCRIPTOR_NODE        StringDescNodeHead
)
{
	PSTRING_DESCRIPTOR_NODE tail = NULL;
	PSTRING_DESCRIPTOR_NODE trailing = NULL;
	ULONG i = 0;

	//
	// Go to the end of the linked list, searching for the requested index to
	// see if we've already retrieved it
	//
	for (tail = StringDescNodeHead; tail != NULL; tail = tail->Next)
	{
		if (tail->DescriptorIndex == DescriptorIndex)
		{
			return S_OK;
		}

		trailing = tail;
	}

	tail = trailing;

	//
	// Get the next String Descriptor. If this is NULL, then we're done (return)
	// Otherwise, loop through all Language IDs
	//
	for (i = 0; (tail != NULL) && (i < NumLanguageIDs); i++)
	{
		tail->Next = GetStringDescriptor(hHubDevice,
			ConnectionIndex,
			DescriptorIndex,
			LanguageIDs[i]);

		tail = tail->Next;
	}

	if (tail == NULL)
	{
		return E_FAIL;
	}
	else {
		return S_OK;
	}
}

PCHAR GetExternalHubName(
	HANDLE  Hub,
	ULONG   ConnectionIndex
	)
{
	BOOL                        success = 0;
	ULONG                       nBytes = 0;
	USB_NODE_CONNECTION_NAME    extHubName;
	PUSB_NODE_CONNECTION_NAME   extHubNameW = NULL;
	PCHAR                       extHubNameA = NULL;

	// Get the length of the name of the external hub attached to the
	// specified port.
	//
	extHubName.ConnectionIndex = ConnectionIndex;

	success = DeviceIoControl(Hub,
		IOCTL_USB_GET_NODE_CONNECTION_NAME,
		&extHubName,
		sizeof(extHubName),
		&extHubName,
		sizeof(extHubName),
		&nBytes,
		NULL);

	if (!success)
	{
		OOPS(236);
		goto GetExternalHubNameError;
	}

	// Allocate space to hold the external hub name
	//
	nBytes = extHubName.ActualLength;

	if (nBytes <= sizeof(extHubName))
	{
		OOPS(237);
		goto GetExternalHubNameError;
	}

	extHubNameW = (PUSB_NODE_CONNECTION_NAME) ALLOC(nBytes);

	if (extHubNameW == NULL)
	{
		OOPS(238);
		goto GetExternalHubNameError;
	}

	// Get the name of the external hub attached to the specified port
	//
	extHubNameW->ConnectionIndex = ConnectionIndex;

	success = DeviceIoControl(Hub,
		IOCTL_USB_GET_NODE_CONNECTION_NAME,
		extHubNameW,
		nBytes,
		extHubNameW,
		nBytes,
		&nBytes,
		NULL);

	if (!success)
	{
		OOPS(239);
		goto GetExternalHubNameError;
	}

	// Convert the External Hub name
	//
	extHubNameA = WideStrToMultiStr(extHubNameW->NodeName, nBytes - sizeof(USB_NODE_CONNECTION_NAME)+sizeof(WCHAR));

	// All done, free the uncoverted external hub name and return the
	// converted external hub name
	//
	FREE(extHubNameW);

	return extHubNameA;


GetExternalHubNameError:
	// There was an error, free anything that was allocated
	//
	if (extHubNameW != NULL)
	{
		FREE(extHubNameW);
		extHubNameW = NULL;
	}

	return NULL;
}