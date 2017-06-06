#include <stdlib.h>
#include <cstring>
#include <comdef.h>
#include "guid.h"
#include "hub.h"

VOID
EnumerateHostControllers(
	HTREEITEM  hTreeParent,
	ULONG     *DevicesConnected
);

VOID
EnumerateHostController(
	HTREEITEM        hTreeParent,
	HANDLE           hHCDev,
	PCHAR            leafName,
	HANDLE           deviceInfo,
	PSP_DEVINFO_DATA deviceInfoData
);

PCHAR
GetHCDDriverKeyName(
	HANDLE HCD
);

DWORD
GetHostControllerPowerMap(
	HANDLE hHCDev,
	PUSBHOSTCONTROLLERINFO hcInfo
);

DWORD
GetHostControllerInfo(
	HANDLE hHCDev,
	PUSBHOSTCONTROLLERINFO hcInfo
);

PCHAR GetRootHubName(
	HANDLE HostController
);

//------------------------------------------------------------------
VOID
EnumerateHostControllers(
HTREEITEM  hTreeParent,
ULONG     *DevicesConnected
)
{
	HANDLE                           hHCDev = NULL;
	HDEVINFO                         deviceInfo = NULL;
	SP_DEVINFO_DATA                  deviceInfoData;
	SP_DEVICE_INTERFACE_DATA         deviceInterfaceData;
	PSP_DEVICE_INTERFACE_DETAIL_DATA deviceDetailData = NULL;
	ULONG                            index = 0;
	ULONG                            requiredLength = 0;
	BOOL                             success;

	TotalDevicesConnected = 0;
	TotalHubs = 0;

	EnumerateAllDevices(NULL);	// create list of Hubs and list of Devices

	// Iterate over host controllers using the new GUID based interface
	//
	deviceInfo = SetupDiGetClassDevs(
		(LPGUID)&GUID_CLASS_USB_HOST_CONTROLLER,
		NULL,
		NULL,
		(DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));

	OOPS("HostControllers", 1);
	deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

	for (index = 0;
		SetupDiEnumDeviceInfo(deviceInfo, index, &deviceInfoData);
		index++)
	{
		deviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

		success = SetupDiEnumDeviceInterfaces(deviceInfo,
			0,
			(LPGUID)&GUID_CLASS_USB_HOST_CONTROLLER,
			index,
			&deviceInterfaceData);
		OOPS("HostControllers", 5, false);

		if (!success)
		{
			OOPS("HostControllers", 6);
			break;
		}

		success = SetupDiGetDeviceInterfaceDetail(deviceInfo,
			&deviceInterfaceData,
			NULL,
			0,
			&requiredLength,
			NULL);

		if (!success && GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		{
			OOPS("HostControllers", 11);
			break;
		}
		clearError(); // we reset ERROR_INSUFFICIENT_BUFFER

		deviceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)ALLOC(requiredLength);
		OOPS("HostControllers", 15);
		if (deviceDetailData == NULL)
		{
			OOPS("HostControllers", 16);
			break;
		}

		deviceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
		success = SetupDiGetDeviceInterfaceDetail(deviceInfo,
			&deviceInterfaceData,
			deviceDetailData,
			requiredLength,
			&requiredLength,
			NULL);
		OOPS("HostControllers", 20);
		if (!success)
		{
			OOPS("HostControllers", 21);
			break;
		}

		hHCDev = CreateFile(deviceDetailData->DevicePath,
			GENERIC_WRITE,
			FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);
		OOPS("HostControllers", 25);
		// If the handle is valid, then we've successfully opened a Host
		// Controller.  Display some info about the Host Controller itself,
		// then enumerate the Root Hub attached to the Host Controller.
		//
		if (hHCDev != INVALID_HANDLE_VALUE)
		{
			
			OOPS("EnumerateHostControllers", 12);
			EnumerateHostController(hTreeParent,
				hHCDev,
				(PCHAR)deviceDetailData->DevicePath,
				deviceInfo,
				&deviceInfoData);

			OOPS("HostControllers", 26);
			CloseHandle(hHCDev);

		}

		FREE(deviceDetailData);
		OOPS("HostControllers", 30);
	}
	SetupDiDestroyDeviceInfoList(deviceInfo);
	OOPS("HostControllers", 35);
	*DevicesConnected = TotalDevicesConnected; // count of all device connected to PC (RootHub aren`t)

	return;
}


VOID
EnumerateHostController(
HTREEITEM        hTreeParent,
HANDLE           hHCDev,
PCHAR            leafName,
HANDLE           deviceInfo,
PSP_DEVINFO_DATA deviceInfoData
)
{
	PCHAR                   driverKeyName = NULL;
	HTREEITEM               hHCItem = NULL;
	PCHAR                   rootHubName = NULL;
	PLIST_ENTRY             listEntry = NULL;
	PUSBHOSTCONTROLLERINFO  hcInfo = NULL;
	PUSBHOSTCONTROLLERINFO  hcInfoInList = NULL;
	DWORD                   dwSuccess;
	BOOL                    success = FALSE;
	ULONG                   deviceAndFunction = 0;
	PUSB_DEVICE_PNP_STRINGS DevProps = NULL;


	// Allocate a structure to hold information about this host controller.
	//
	hcInfo = (PUSBHOSTCONTROLLERINFO)ALLOC(sizeof(USBHOSTCONTROLLERINFO));
	OOPS("EnumerateHostController", 1);
	// just return if could not alloc memory
	if (NULL == hcInfo)
		return;

	hcInfo->DeviceInfoType = HostControllerInfo;

	// Obtain the driver key name for this host controller.
	//
	driverKeyName = GetHCDDriverKeyName(hHCDev);	// checked. it works correctly
	OOPS("EnumerateHostController", 5, false);

	if (NULL == driverKeyName)
	{
		// Failure obtaining driver key name.
		OOPS("EnumerateHostController", 6);
		FREE(hcInfo);
		return;
	}
	// Don't enumerate this host controller again if it already
	// on the list of enumerated host controllers.
	//
	listEntry = EnumeratedHCListHead.Flink;

	while (listEntry != &EnumeratedHCListHead)
	{
		hcInfoInList = CONTAINING_RECORD(listEntry,
										USBHOSTCONTROLLERINFO,
										ListEntry);
		OOPS("EnumerateHostController", 10);
		if (strcmp(driverKeyName, hcInfoInList->DriverKey) == 0)
		{
			// Already on the list, exit
			//
			cout << "warning: Already on the list, exit" << endl;
			FREE(driverKeyName);
			FREE(hcInfo);
			return;
		}

		listEntry = listEntry->Flink;
	}

	// Obtain host controller device properties
	{
		size_t cbDriverName = 0;
		HRESULT hr = S_OK;
		hr = StringCbLength((LPCTSTR)driverKeyName, MAX_DRIVER_KEY_NAME, &cbDriverName);
		OOPS("EnumerateHostController", 15);
		if (SUCCEEDED(hr))
		{
			DevProps = DriverNameToDeviceProperties(driverKeyName, cbDriverName);
			OOPS("EnumerateHostController", 20);
		}
	}
	
	hcInfo->DriverKey = driverKeyName;
	if (DevProps)
	{
		ULONG   ven, dev, subsys, rev;
		ven = dev = subsys = rev = 0;
		if (sscanf_s(DevProps->DeviceId,
			"PCI\\VEN_%x&DEV_%x&SUBSYS_%x&REV_%x",
			&ven, &dev, &subsys, &rev) != 4)
		{
			OOPS("EnumerateHostController", 25);
		}

		hcInfo->VendorID = ven;
		hcInfo->DeviceID = dev;
		hcInfo->SubSysID = subsys;
		hcInfo->Revision = rev;
		hcInfo->UsbDeviceProperties = DevProps;
	}
	else
	{
		OOPS(15);
	}

	if (DevProps != NULL && DevProps->DeviceDesc != NULL)
	{
		leafName = DevProps->DeviceDesc;
	}
	else
	{
		OOPS(16);
	}

	// Get the USB Host Controller power map
	dwSuccess = GetHostControllerPowerMap(hHCDev, hcInfo);

	if (ERROR_SUCCESS != dwSuccess)
	{
		OOPS(17);
	}


	// Get bus, device, and function
	//
	hcInfo->BusDeviceFunctionValid = FALSE;

	success = SetupDiGetDeviceRegistryProperty(deviceInfo,
		deviceInfoData,
		SPDRP_BUSNUMBER,
		NULL,
		(PBYTE)&hcInfo->BusNumber,
		sizeof(hcInfo->BusNumber),
		NULL);

	if (success)
	{
		success = SetupDiGetDeviceRegistryProperty(deviceInfo,
			deviceInfoData,
			SPDRP_ADDRESS,
			NULL,
			(PBYTE)&deviceAndFunction,
			sizeof(deviceAndFunction),
			NULL);
	}

	if (success)
	{
		hcInfo->BusDevice = deviceAndFunction >> 16;
		hcInfo->BusFunction = deviceAndFunction & 0xffff;
		hcInfo->BusDeviceFunctionValid = TRUE;
	}

	// Get the USB Host Controller info
	dwSuccess = GetHostControllerInfo(hHCDev, hcInfo);

	if (ERROR_SUCCESS != dwSuccess)
	{
		OOPS(18);
	}
	else {
		printInfoHC(hcInfo); // print info about HC
	}
	
	// Add this host controller to the list of enumerated
	// host controllers.
	//
	InsertTailList(&EnumeratedHCListHead,
		&hcInfo->ListEntry);
	// Get the name of the root hub for this host
	// controller and then enumerate the root hub.
	//
	rootHubName = GetRootHubName(hHCDev);

	if (rootHubName != NULL)
	{
		size_t cbHubName = 0;
		HRESULT hr = S_OK;

		hr = StringCbLength((LPCTSTR)rootHubName, MAX_DRIVER_KEY_NAME, &cbHubName);
		if (SUCCEEDED(hr))
		{
			EnumerateHub(hHCItem,
			rootHubName,
			cbHubName,
			NULL,       // ConnectionInfo
			NULL,       // ConnectionInfoV2
			NULL,       // PortConnectorProps
			NULL,       // ConfigDesc
			NULL,       // BosDesc
			NULL,       // StringDescs
			NULL);      // We do not pass DevProps for RootHub
		}
	}
	else
	{
		// Failure obtaining root hub name.
		OOPS(19);
	}
	return;
}



PCHAR GetHCDDriverKeyName(
	HANDLE  HCD
	)
{
	BOOL                    success = 0;
	ULONG                   nBytes = 0;
	USB_HCD_DRIVERKEY_NAME  driverKeyName = { 0 };
	PUSB_HCD_DRIVERKEY_NAME driverKeyNameW = NULL;
	PCHAR                   driverKeyNameA = NULL;

	ZeroMemory(&driverKeyName, sizeof(driverKeyName));
	OOPS("EnumerateHostController -> GetHCDDriverKeyName", 1);
	// Get the length of the name of the driver key of the HCD
	//
	success = DeviceIoControl(HCD,
		IOCTL_GET_HCD_DRIVERKEY_NAME,
		&driverKeyName,
		sizeof(driverKeyName),
		&driverKeyName,
		sizeof(driverKeyName),
		&nBytes,
		NULL);

	OOPS("EnumerateHostController -> GetHCDDriverKeyName", 5);
	if (!success)
	{
		goto GetHCDDriverKeyNameError;
	}

	// Allocate space to hold the driver key name
	//
	nBytes = driverKeyName.ActualLength;
	OOPS("EnumerateHostController -> GetHCDDriverKeyName", 10);
	if (nBytes <= sizeof(driverKeyName))
	{
		goto GetHCDDriverKeyNameError;
	}

	driverKeyNameW = (PUSB_HCD_DRIVERKEY_NAME)ALLOC(nBytes);
	OOPS("EnumerateHostController -> GetHCDDriverKeyName", 15);
	if (driverKeyNameW == NULL)
	{
		goto GetHCDDriverKeyNameError;
	}

	// Get the name of the driver key of the device attached to
	// the specified port.
	//

	success = DeviceIoControl(HCD,
								IOCTL_GET_HCD_DRIVERKEY_NAME,
								driverKeyNameW,
								nBytes,
								driverKeyNameW,
								nBytes,
								&nBytes,
								NULL);
	OOPS("EnumerateHostController -> GetHCDDriverKeyName", 20);
	if (!success)
	{
		OOPS("EnumerateHostController -> GetHCDDriverKeyName", 21);
		goto GetHCDDriverKeyNameError;
	}

	//
	// Convert the driver key name
	// Pass the length of the DriverKeyName string
	// 
	driverKeyNameA = WideStrToMultiStr(driverKeyNameW->DriverKeyName, nBytes - sizeof(USB_HCD_DRIVERKEY_NAME)+sizeof(WCHAR));
	OOPS("EnumerateHostController -> GetHCDDriverKeyName", 25);
	// All done, free the uncoverted driver key name and return the
	// converted driver key name
	//
	FREE(driverKeyNameW);
	OOPS("EnumerateHostController -> GetHCDDriverKeyName", 30);
	return driverKeyNameA;

GetHCDDriverKeyNameError:
	// There was an error, free anything that was allocated
	//
	cout << "GetHCDDriverKeyNameError. look at EnumerateHostController -> GetHCDDriverKeyName" << endl;
	if (driverKeyNameW != NULL)
	{
		FREE(driverKeyNameW);
		driverKeyNameW = NULL;
	}

	return NULL;
}



DWORD
GetHostControllerPowerMap(
HANDLE hHCDev,
PUSBHOSTCONTROLLERINFO hcInfo)
{
	USBUSER_POWER_INFO_REQUEST UsbPowerInfoRequest;
	PUSB_POWER_INFO            pUPI = &UsbPowerInfoRequest.PowerInformation;
	DWORD                      dwError = 0;
	DWORD                      dwBytes = 0;
	BOOL                       bSuccess = FALSE;
	int                        nIndex = 0;
	int                        nPowerState = WdmUsbPowerSystemWorking;

	for (; nPowerState <= WdmUsbPowerSystemShutdown; nIndex++, nPowerState++)
	{
		// zero initialize our request
		memset(&UsbPowerInfoRequest, 0, sizeof(UsbPowerInfoRequest));

		// set the header and request sizes
		UsbPowerInfoRequest.Header.UsbUserRequest = USBUSER_GET_POWER_STATE_MAP;
		UsbPowerInfoRequest.Header.RequestBufferLength = sizeof(UsbPowerInfoRequest);
		UsbPowerInfoRequest.PowerInformation.SystemState = (WDMUSB_POWER_STATE)nPowerState;

		//
		// Now query USBHUB for the USB_POWER_INFO structure for this hub.
		// For Selective Suspend support
		//
		bSuccess = DeviceIoControl(hHCDev,
			IOCTL_USB_USER_REQUEST,
			&UsbPowerInfoRequest,
			sizeof(UsbPowerInfoRequest),
			&UsbPowerInfoRequest,
			sizeof(UsbPowerInfoRequest),
			&dwBytes,
			NULL);

		if (!bSuccess)
		{
			dwError = GetLastError();
			OOPS(24);
		}
		else
		{
			// copy the data into our USB Host Controller's info structure
			memcpy(&(hcInfo->USBPowerInfo[nIndex]), pUPI, sizeof(USB_POWER_INFO));
		}
	}

	return dwError;
}


DWORD
GetHostControllerInfo(
HANDLE hHCDev,
PUSBHOSTCONTROLLERINFO hcInfo)
{
	USBUSER_CONTROLLER_INFO_0 UsbControllerInfo;
	DWORD                      dwError = 0;
	DWORD                      dwBytes = 0;
	BOOL                       bSuccess = FALSE;

	memset(&UsbControllerInfo, 0, sizeof(UsbControllerInfo));

	// set the header and request sizes
	UsbControllerInfo.Header.UsbUserRequest = USBUSER_GET_CONTROLLER_INFO_0;
	UsbControllerInfo.Header.RequestBufferLength = sizeof(UsbControllerInfo);

	//
	// Query for the USB_CONTROLLER_INFO_0 structure
	//
	bSuccess = DeviceIoControl(hHCDev,
		IOCTL_USB_USER_REQUEST,
		&UsbControllerInfo,
		sizeof(UsbControllerInfo),
		&UsbControllerInfo,
		sizeof(UsbControllerInfo),
		&dwBytes,
		NULL);

	if (!bSuccess)
	{
		dwError = GetLastError();
		OOPS(25);
	}
	else
	{
		hcInfo->ControllerInfo = (PUSB_CONTROLLER_INFO_0)ALLOC(sizeof(USB_CONTROLLER_INFO_0));
		if (NULL == hcInfo->ControllerInfo)
		{
			dwError = GetLastError();
			OOPS(26);
		}
		else
		{
			// copy the data into our USB Host Controller's info structure
			memcpy(hcInfo->ControllerInfo, &UsbControllerInfo.Info0, sizeof(USB_CONTROLLER_INFO_0));
		}
	}
	return dwError;
}


PCHAR GetRootHubName(
	HANDLE HostController
	)
{
	BOOL                success = 0;
	ULONG               nBytes = 0;
	USB_ROOT_HUB_NAME   rootHubName;
	PUSB_ROOT_HUB_NAME  rootHubNameW = NULL;
	PCHAR               rootHubNameA = NULL;

	// Get the length of the name of the Root Hub attached to the
	// Host Controller
	//
	success = DeviceIoControl(HostController,
		IOCTL_USB_GET_ROOT_HUB_NAME,
		0,
		0,
		&rootHubName,
		sizeof(rootHubName),
		&nBytes,
		NULL);

	if (!success)
	{
		OOPS(27);
		goto GetRootHubNameError;
	}

	// Allocate space to hold the Root Hub name
	//
	nBytes = rootHubName.ActualLength;

	rootHubNameW = (PUSB_ROOT_HUB_NAME)ALLOC(nBytes);
	if (rootHubNameW == NULL)
	{
		OOPS(28);
		goto GetRootHubNameError;
	}

	// Get the name of the Root Hub attached to the Host Controller
	//
	success = DeviceIoControl(HostController,
		IOCTL_USB_GET_ROOT_HUB_NAME,
		NULL,
		0,
		rootHubNameW,
		nBytes,
		&nBytes,
		NULL);
	if (!success)
	{
		OOPS(29);
		goto GetRootHubNameError;
	}

	// Convert the Root Hub name
	//
	rootHubNameA = WideStrToMultiStr(rootHubNameW->RootHubName, nBytes - sizeof(USB_ROOT_HUB_NAME)+sizeof(WCHAR));

	// All done, free the uncoverted Root Hub name and return the
	// converted Root Hub name
	//
	FREE(rootHubNameW);

	return rootHubNameA;

GetRootHubNameError:
	// There was an error, free anything that was allocated
	//
	if (rootHubNameW != NULL)
	{
		FREE(rootHubNameW);
		rootHubNameW = NULL;
	}
	return NULL;
}

