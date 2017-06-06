#include <stdlib.h>
#include <tchar.h>
#include <iostream>
#include <iomanip>
#include <cstring>
#include "Header.h"

using namespace ::std;

#pragma comment (lib, "Setupapi.lib") // for fix LINK2019
//#pragma comment (lib, "Winusb.lib") // for fix LINK1120


// vars
LIST_ENTRY EnumeratedHCListHead =
{
	&EnumeratedHCListHead,
	&EnumeratedHCListHead
};

DEVICE_GUID_LIST gHubList;
DEVICE_GUID_LIST gDeviceList;

///------------------------------------------------------------------
VOID FREEDeviceInfoNode(PDEVICE_INFO_NODE *ppNode);
size_t GetDeviceProperty(HDEVINFO			DeviceInfoSet,
						PSP_DEVINFO_DATA	DeviceInfoData,
						DWORD				Property,
						LPTSTR				*ppBuffer);
int EnumerateAllDevicesWithGuid(PDEVICE_GUID_LIST DeviceList,
	LPGUID Guid);
void ClearDeviceList(PDEVICE_GUID_LIST DeviceList);


///------------------------------------------------------------------
void EnumerateAllDevices(PDEVICE_HUB_LIST ask){
	int countOfHubs = EnumerateAllDevicesWithGuid(&gHubList,
						(LPGUID)&GUID_DEVINTERFACE_USB_HUB);

	int countOfDevices = EnumerateAllDevicesWithGuid(&gDeviceList,
						(LPGUID)&GUID_DEVINTERFACE_USB_DEVICE);
	if (ask != NULL){
		ask->countOfHubs = countOfHubs;
		ask->countOfDevices = countOfDevices;
	}
}

int EnumerateAllDevicesWithGuid(
	PDEVICE_GUID_LIST DeviceList,
	LPGUID Guid)
{
	int countOfDevice = 0;
	if (DeviceList->DeviceInfo != INVALID_HANDLE_VALUE)
	{
		OOPS("guid", 1); // it`s all rigth, no important - trifle(мелочь)
		ClearDeviceList(DeviceList);
	}

	DeviceList->DeviceInfo = SetupDiGetClassDevs(Guid,	// not NULL
		NULL,
		NULL,
		(DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));

	OOPS("guid", 2);
	if (DeviceList->DeviceInfo != INVALID_HANDLE_VALUE)
	{
		ULONG index;
		DWORD error;

		error = 0;
		index = 0;

		while (error != ERROR_NO_MORE_ITEMS)
		{
			BOOL success;
			PDEVICE_INFO_NODE pNode;

			pNode = (PDEVICE_INFO_NODE)ALLOC(sizeof(DEVICE_INFO_NODE));
			if (pNode == NULL)
			{
				OOPS("guid", 5);
				break;
			}
			pNode->DeviceInfo = DeviceList->DeviceInfo;
			pNode->DeviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
			pNode->DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

			success = SetupDiEnumDeviceInfo(DeviceList->DeviceInfo,
											index,
											&pNode->DeviceInfoData);
			index++;

			if (success == FALSE)
			{
				error = GetLastError();

				if (error != ERROR_NO_MORE_ITEMS)
				{
					OOPS("guid", 10);
				}
				FREEDeviceInfoNode(&pNode);
			}
			else
			{
				BOOL   bResult;
				ULONG  requiredLength;

				bResult = GetDeviceProperty(DeviceList->DeviceInfo,
					&pNode->DeviceInfoData,
					SPDRP_DEVICEDESC,
					&pNode->DeviceDescName);
				OOPS("guid", 15, false);

				if (bResult == FALSE)
				{
					FREEDeviceInfoNode(&pNode);
					OOPS("guid", 16);
					break;
				}


				bResult = GetDeviceProperty(DeviceList->DeviceInfo,
											&pNode->DeviceInfoData,
											SPDRP_DRIVER,
											&pNode->DeviceDriverName);
				OOPS("guid", 20, false);
				if (bResult == FALSE)
				{
					OOPS("guid", 21);
					FREEDeviceInfoNode(&pNode);
					OOPS("guid", 22);
					break;
				}
				pNode->DeviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

				success = SetupDiEnumDeviceInterfaces(DeviceList->DeviceInfo,
														0,
														Guid,
														index - 1,
														&pNode->DeviceInterfaceData);
				OOPS("guid", 25);
				if (!success)
				{
					OOPS("guid", 26);
					FREEDeviceInfoNode(&pNode);
					OOPS("guid", 27);
					break;
				}

				success = SetupDiGetDeviceInterfaceDetail(DeviceList->DeviceInfo,
					&pNode->DeviceInterfaceData,
					NULL,
					0,
					&requiredLength,
					NULL);

				error = GetLastError();		// error must be = ERROR_INSUFFICIENT_BUFFER
				//OOPS("guid", 30, false);	// it`s all rigth

				if (!success && error != ERROR_INSUFFICIENT_BUFFER)
				{
					OOPS("guid", 31);
					FREEDeviceInfoNode(&pNode);
					OOPS("guid", 32);
					break;
				}

				pNode->DeviceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)ALLOC(requiredLength);

				if (pNode->DeviceDetailData == NULL)
				{
					FREEDeviceInfoNode(&pNode);
					OOPS("guid", 35);
					break;
				}

				pNode->DeviceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

				success = SetupDiGetDeviceInterfaceDetail(DeviceList->DeviceInfo,
					&pNode->DeviceInterfaceData,
					pNode->DeviceDetailData,
					requiredLength,
					&requiredLength,
					NULL);

				OOPS("guid", 40);
				if (!success)
				{
					FREEDeviceInfoNode(&pNode);
					OOPS("guid", 41);
					break;
				}
				/*
				_tprintf(_T("%d)\n%s\n\n"), index, pNode->DeviceDetailData->DevicePath);
				*/
				InsertTailList(&DeviceList->ListHead, &pNode->ListEntry);
				countOfDevice++;
			}
			//if (error == ERROR_NO_MORE_ITEMS) OOPS(701);
		}
	}
	//_tprintf(_T("Count of found devices: %d\n"), countOfDevice);
	return countOfDevice;
}


VOID FREEDeviceInfoNode(PDEVICE_INFO_NODE *ppNode)
{
	if (ppNode == NULL) { return; }

	if (*ppNode == NULL) { return; }

	if ((*ppNode)->DeviceDetailData != NULL) { FREE((*ppNode)->DeviceDetailData); }

	if ((*ppNode)->DeviceDescName != NULL) { FREE((*ppNode)->DeviceDescName); }

	if ((*ppNode)->DeviceDriverName != NULL) { FREE((*ppNode)->DeviceDriverName); }

	FREE(*ppNode);
	*ppNode = NULL;
}

size_t
GetDeviceProperty(
HDEVINFO         DeviceInfoSet,
PSP_DEVINFO_DATA DeviceInfoData,
DWORD            Property,
LPTSTR			*ppBuffer
)
{
	BOOL bResult;
	DWORD requiredLength = 0;
	DWORD memLength = 0;
	DWORD lastError;

	if (ppBuffer == NULL)
	{
		cout << "Error #1. GetDeviceProperty" << endl;
		return 0;
	}

	OOPS("GetDeviceProperty", 1);
	*ppBuffer = NULL;

	bResult = SetupDiGetDeviceRegistryProperty(DeviceInfoSet,
		DeviceInfoData,
		Property,
		NULL,
		NULL,
		0,
		&requiredLength);
	lastError = GetLastError();

	if ((requiredLength == 0) || (bResult != FALSE && lastError != ERROR_INSUFFICIENT_BUFFER))
	{
		//OOPS("GetDeviceProperty", 5); // no save, but repeat in the searching (spam)
		clearError();
		return 0;
	}
	memLength = requiredLength;
	*ppBuffer = (LPTSTR)ALLOC(memLength);

	if (*ppBuffer == NULL)
	{
		OOPS("GetDeviceProperty", 10);
		return 0;
	}

	bResult = SetupDiGetDeviceRegistryProperty(DeviceInfoSet,
		DeviceInfoData,
		Property,
		NULL,
		(PBYTE)*ppBuffer,
		requiredLength,
		&requiredLength);

	OOPS("GetDeviceProperty", 15);
	if (bResult == FALSE)
	{
		FREE(*ppBuffer);
		*ppBuffer = NULL;
		return 0;
	}
	return memLength;
}


void
ClearDeviceList(
PDEVICE_GUID_LIST DeviceList
)
{
	if (DeviceList->DeviceInfo != INVALID_HANDLE_VALUE)
	{
		SetupDiDestroyDeviceInfoList(DeviceList->DeviceInfo);
		DeviceList->DeviceInfo = INVALID_HANDLE_VALUE;
	}

	while (!IsListEmpty(&DeviceList->ListHead))
	{
	PDEVICE_INFO_NODE pNode = NULL;
	PLIST_ENTRY pEntry;

	pEntry = RemoveHeadList(&DeviceList->ListHead);

	pNode = CONTAINING_RECORD(pEntry,
	DEVICE_INFO_NODE,
	ListEntry);

	FREEDeviceInfoNode(&pNode);
	}
}

void
ClearHCList()
{
	PLIST_ENTRY listEntry = EnumeratedHCListHead.Flink;
	while (listEntry != &EnumeratedHCListHead)
	{
		RemoveEntryList(listEntry);
		listEntry = listEntry->Flink;
	}
}