#pragma once
#include <iostream>
#include <iomanip>
#include <windows.h>
#include "display.h"


ULONG TotalDevicesConnected;
BOOL gDoConfigDesc = TRUE;
BOOL gDoAnnotation = TRUE;
BOOL gLogDebug = FALSE;
int  TotalHubs = 0;


void OOPS(int number) {
	return;
	if (GetLastError() == 0) return;
	std::cout << "OOPS! myCode->" << number << " GetLastError->" << GetLastError() << std::endl;
	SetLastError(0);
}

void OOPS(const char* str, int number, bool ena = true) {
	return;
	if (GetLastError() == 0) return;
	std::cout << "OOPS! myCode->" << number << ", GetLastError->" << GetLastError() << ", \"" << str << "\"" << std::endl;
	if (ena) SetLastError(0);
}

void clearError(){ SetLastError(0); }

/*****************************************************************************

FreeDeviceProperties()

Free the device properties structure

*****************************************************************************/
VOID FreeDeviceProperties(_In_ PUSB_DEVICE_PNP_STRINGS *ppDevProps)
{
	if (ppDevProps == NULL)	{ return; }

	if (*ppDevProps == NULL) { return; }

	if ((*ppDevProps)->DeviceId != NULL) { FREE((*ppDevProps)->DeviceId); }

	if ((*ppDevProps)->DeviceDesc != NULL) { FREE((*ppDevProps)->DeviceDesc); }

	//
	// The following are not necessary, but left in case
	// in the future there is a later failure where these
	// pointer fields would be allocated.
	//

	if ((*ppDevProps)->HwId != NULL) { FREE((*ppDevProps)->HwId); }

	if ((*ppDevProps)->Service != NULL) { FREE((*ppDevProps)->Service); }

	if ((*ppDevProps)->DeviceClass != NULL) { FREE((*ppDevProps)->DeviceClass); }

	if ((*ppDevProps)->PowerState != NULL) { FREE((*ppDevProps)->PowerState); }

	FREE(*ppDevProps);
	*ppDevProps = NULL;
}


PUSB_DEVICE_PNP_STRINGS
DriverNameToDeviceProperties(
PCHAR  DriverName,
size_t cbDriverName
)
{
	OOPS("DriverNameToDeviceProperties", 0);
	HDEVINFO        deviceInfo = INVALID_HANDLE_VALUE;
	SP_DEVINFO_DATA deviceInfoData = { 0 };
	ULONG           len;
	ULONG           lenX2; //*
	LPTSTR bufOfWStr = NULL; //*
	size_t			memLen = 0; //*
	BOOL            status;
	PUSB_DEVICE_PNP_STRINGS DevProps = NULL;
	DWORD           lastError;

	// Allocate device propeties structure
	DevProps = (PUSB_DEVICE_PNP_STRINGS)ALLOC(sizeof(USB_DEVICE_PNP_STRINGS));
	OOPS("DriverNameToDeviceProperties", 5);
	if (NULL == DevProps)
	{
		status = FALSE;
		goto Done;
	}

	// Get device instance
	status = DriverNameToDeviceInst(DriverName, cbDriverName, &deviceInfo, &deviceInfoData);

	OOPS("DriverNameToDeviceProperties", 9, false);
	if (status == FALSE)
	{
		OOPS("DriverNameToDeviceProperties", 10);
		goto Done;
	}

	OOPS("DriverNameToDeviceProperties", 15);
	len = 0;
	status = SetupDiGetDeviceInstanceId(deviceInfo,
		&deviceInfoData,
		NULL,
		0,
		&len);
	lastError = GetLastError();
	//OOPS("DriverNameToDeviceProperties", 20, false); // 122 == ERROR_INSUFFICIENT_BUFFER

	if (status != FALSE && lastError != ERROR_INSUFFICIENT_BUFFER)
	{
		OOPS("DriverNameToDeviceProperties", 25);
		status = FALSE;
		goto Done;
	}

	//
	// An extra byte is required for the terminating character
	//

	// buf for multi string
	lenX2 = 2 * len + 1; // because multibyte code
	bufOfWStr = (LPTSTR)ALLOC(lenX2);
	if (bufOfWStr == NULL)
	{
		OOPS("DriverNameToDeviceProperties", 31);
		status = FALSE;
		goto Done;
	}

	len++;
	// ***DeviceId***
	DevProps->DeviceId = (PCHAR)ALLOC(len);
	if (DevProps->DeviceId == NULL)
	{
		OOPS("DriverNameToDeviceProperties", 30);
		status = FALSE;
		goto Done;
	}
	status = SetupDiGetDeviceInstanceId(deviceInfo,
		&deviceInfoData,
		bufOfWStr,
		lenX2,
		&lenX2);
	OOPS("DriverNameToDeviceProperties", 35);
	if (status == FALSE)
		goto Done;

	{
		// converting from MultiByteString to ByteString
		_bstr_t converW2C(bufOfWStr);
		memcpy(DevProps->DeviceId, (PCHAR)converW2C, len);
		//DevProps->DeviceId = (PCHAR)converW2C;
		//cout << "DeviceId: " << DevProps->DeviceId << endl;
		OOPS("DriverNameToDeviceProperties-converting", 36);
	}
	FREE(bufOfWStr);
	bufOfWStr = NULL;

	// ***DeviceDesc***
	memLen = GetDeviceProperty(deviceInfo,
		&deviceInfoData,
		SPDRP_DEVICEDESC,
		&bufOfWStr);
	OOPS("DriverNameToDeviceProperties", 45);
	if (status == FALSE)
	{
		goto Done;
	}
	DevProps->DeviceDesc = (PCHAR)ALLOC(memLen);	// len is exactly
	{
		// converting from MultiByteString to ByteString
		_bstr_t converW2C(bufOfWStr);
		memcpy(DevProps->DeviceDesc, (PCHAR)converW2C, memLen);
		//cout << "DeviceDesc: " << DevProps->DeviceDesc << endl;
	}
	FREE(bufOfWStr);
	bufOfWStr = NULL;

	//    
	// We don't fail if the following registry query fails as these fields are additional information only
	//
	// ***HwId***
	memLen = GetDeviceProperty(deviceInfo,
		&deviceInfoData,
		SPDRP_HARDWAREID,
		&bufOfWStr);
	OOPS("DriverNameToDeviceProperties", 55);
	DevProps->HwId = (PCHAR)ALLOC(memLen);			// len is exactly
	{
		_bstr_t converW2C(bufOfWStr);
		memcpy(DevProps->HwId, (PCHAR)converW2C, memLen);
		//cout << "HwId: " << DevProps->HwId << endl;
	}
	FREE(bufOfWStr);
	bufOfWStr = NULL;


	// ***Service***
	memLen = GetDeviceProperty(deviceInfo,
		&deviceInfoData,
		SPDRP_SERVICE,
		&bufOfWStr);
	OOPS("DriverNameToDeviceProperties", 60);
	DevProps->Service = (PCHAR)ALLOC(memLen);			// len - exactly
	{
		_bstr_t converW2C(bufOfWStr);
		memcpy(DevProps->Service, (PCHAR)converW2C, memLen);
		//cout << "Service: " << DevProps->Service << endl;
	}
	FREE(bufOfWStr);
	bufOfWStr = NULL;

	// DeviceClass
	memLen = GetDeviceProperty(deviceInfo,
		&deviceInfoData,
		SPDRP_CLASS,
		&bufOfWStr);
	OOPS("DriverNameToDeviceProperties", 65);
	DevProps->DeviceClass = (PCHAR)ALLOC(memLen);			// len - exactly
	{
		_bstr_t converW2C(bufOfWStr);
		memcpy(DevProps->DeviceClass, (PCHAR)converW2C, memLen);
		//cout << "DeviceClass: " << DevProps->DeviceClass << endl;
	}
	FREE(bufOfWStr);
	bufOfWStr = NULL;

Done:
	FREE(bufOfWStr);
	bufOfWStr = NULL;

	if (deviceInfo != INVALID_HANDLE_VALUE)
	{
		SetupDiDestroyDeviceInfoList(deviceInfo); // lake*** very bad
		OOPS("DriverNameToDeviceProperties", 70);
	}

	if (status == FALSE)
	{
		if (DevProps != NULL)
		{
			FreeDeviceProperties(&DevProps);
		}
	}
	OOPS("DriverNameToDeviceProperties", 75);
	return DevProps;
}



BOOL
DriverNameToDeviceInst(
PCHAR DriverName,
size_t cbDriverName,
HDEVINFO *pDevInfo,
PSP_DEVINFO_DATA pDevInfoData
)
{
	HDEVINFO         deviceInfo = INVALID_HANDLE_VALUE;
	BOOL             status = TRUE;
	ULONG            deviceIndex;
	SP_DEVINFO_DATA  deviceInfoData;
	BOOL             bResult = FALSE;
	PCHAR            pDriverName = NULL;
	LPTSTR           buf = NULL;
	BOOL             done = FALSE;

	if (pDevInfo == NULL)
	{
		cout << "Error #1: DriverNameToDeviceInst" << endl;
		return FALSE;
	}

	if (pDevInfoData == NULL)
	{
		cout << "Error #2: DriverNameToDeviceInst" << endl;
		return FALSE;
	}

	memset(pDevInfoData, 0, sizeof(SP_DEVINFO_DATA));

	*pDevInfo = INVALID_HANDLE_VALUE;
	// Use local string to guarantee zero termination
	pDriverName = (PCHAR)ALLOC((DWORD)cbDriverName + 1);
	OOPS("DriverNameToDeviceInst", 5);
	if (NULL == pDriverName)
	{
		status = FALSE;
		goto Done;
	}

	StringCbCopyN((LPTSTR)pDriverName, cbDriverName + 1, (LPCTSTR)DriverName, cbDriverName);
	OOPS("DriverNameToDeviceInst", 10);
	//
	// We cannot walk the device tree with CM_Get_Sibling etc. unless we assume
	// the device tree will stabilize. Any devnode removal (even outside of USB)
	// would force us to retry. Instead we use Setup API to snapshot all
	// devices.
	//

	// Examine all present devices to see if any match the given DriverName
	//

	deviceInfo = SetupDiGetClassDevs(NULL,
		NULL,
		NULL,
		DIGCF_ALLCLASSES | DIGCF_PRESENT);

	OOPS("DriverNameToDeviceInst", 15, false);

	if (deviceInfo == INVALID_HANDLE_VALUE)
	{
		OOPS("DriverNameToDeviceInst", 16);
		status = FALSE;
		goto Done;
	}

	deviceIndex = 0;
	deviceInfoData.cbSize = sizeof(deviceInfoData);

	clearError();
	while (done == FALSE)
	{
		// alloc and clear memory for DRIVER_KEY_NAME
		//buf = (LPTSTR)ALLOC(MAX_DRIVER_KEY_NAME);
		//ZeroMemory(&buf, sizeof(MAX_DRIVER_KEY_NAME));

		//
		// Get devinst of the next device
		//
		status = SetupDiEnumDeviceInfo(deviceInfo,
			deviceIndex,
			&deviceInfoData);

		OOPS("DriverNameToDeviceInst", 20, false);
		deviceIndex++;

		if (!status)
		{
			//
			// This could be an error, or indication that all devices have been
			// processed. Either way the desired device was not found.
			//
			OOPS("DriverNameToDeviceInst", 21);
			done = TRUE;
			break;
		}

		//
		// Get the DriverName value
		//
		size_t memSize = GetDeviceProperty(deviceInfo,
			&deviceInfoData,
			SPDRP_DRIVER,
			&buf);
		if (memSize > 0) bResult = TRUE;	// fix for GetDeviceProperty (not stump)
		OOPS("DriverNameToDeviceInst", 25);

		PCHAR tmpDriverName = NULL;
		_bstr_t tmp_bstr_t(buf);
		tmpDriverName = tmp_bstr_t;
		//if (buf != NULL) wcout << tmpDriverName << "-" << bResult << "-" << _stricmp(pDriverName, tmpDriverName) << " = ";

		// If the DriverName value matches, return the DeviceInstance
		//
		if (bResult == TRUE && buf != NULL && _stricmp(pDriverName, tmpDriverName) == 0)
		{
			done = TRUE;
			*pDevInfo = deviceInfo;
			CopyMemory(pDevInfoData, &deviceInfoData, sizeof(deviceInfoData));
			FREE(buf);
			break;
		}

		if (buf != NULL)
		{
			FREE(buf);
			buf = NULL;
		}
	}
	OOPS("DriverNameToDeviceInst", 30);
Done:

	if (bResult == FALSE)
	{
		if (deviceInfo != INVALID_HANDLE_VALUE)
		{
			SetupDiDestroyDeviceInfoList(deviceInfo);
			OOPS("DriverNameToDeviceInst", 35);
		}
	}

	OOPS("DriverNameToDeviceInst", 40);
	if (pDriverName != NULL)
	{
		FREE(pDriverName);
	}
	OOPS("DriverNameToDeviceInst", 41);
	return status;
}


PCHAR WideStrToMultiStr(
	PWCHAR WideStr,
	size_t cbWideStr
	)
{
	OOPS("WideStrToMultiStr", 1);
	ULONG  nBytes = 0;
	PCHAR  MultiStr = NULL;
	PWCHAR pWideStr = NULL;

	// Use local string to guarantee zero termination
	pWideStr = (PWCHAR)ALLOC((DWORD)cbWideStr + sizeof(WCHAR));
	OOPS("WideStrToMultiStr", 5);
	if (NULL == pWideStr)
	{
		return NULL;
	}

	memset(pWideStr, 0, cbWideStr + sizeof(WCHAR));
	memcpy(pWideStr, WideStr, cbWideStr);

	// Get the length of the converted string
	//
	nBytes = WideCharToMultiByte(CP_ACP,
		WC_NO_BEST_FIT_CHARS,
		pWideStr,
		-1,
		NULL,
		0,
		NULL,
		NULL);

	OOPS("WideStrToMultiStr", 10);
	if (nBytes == 0)
	{
		FREE(pWideStr);
		return NULL;
	}
	// Allocate space to hold the converted string
	//
	MultiStr = (PCHAR)ALLOC(nBytes);
	OOPS("WideStrToMultiStr", 15);
	if (MultiStr == NULL)
	{
		FREE(pWideStr);
		return NULL;
	}
	// Convert the string
	//
	nBytes = WideCharToMultiByte(CP_ACP,
		WC_NO_BEST_FIT_CHARS,
		pWideStr,
		-1,
		MultiStr,
		nBytes,
		NULL,
		NULL);
	OOPS("WideStrToMultiStr", 20);
	if (nBytes == 0)
	{
		FREE(MultiStr);
		FREE(pWideStr);
		return NULL;
	}
	OOPS("WideStrToMultiStr", 21);
	FREE(pWideStr);
	return MultiStr;
}