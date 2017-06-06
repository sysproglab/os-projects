/*****************************************************************************
I N C L U D E S
*****************************************************************************/
#include <windows.h>
#include <windowsx.h>
#include <initguid.h>
#include <devioctl.h>
#include <dbt.h>
#include <stdio.h>
#include <commctrl.h>
#include <usbioctl.h>
#include <usbiodef.h>
#include <strsafe.h>
#include <specstrings.h>
#include <usb.h>
#include <usbuser.h>
#include <basetyps.h>
#include <wtypes.h>
#include <objbase.h>
#include <io.h>
#include <conio.h>
#include <shellapi.h>
#include <cfgmgr32.h>
#include <setupapi.h>
#include <winioctl.h>
#include <devpkey.h>
#include <math.h>


/*****************************************************************************
P R A G M A S
*****************************************************************************/

#pragma once

/*****************************************************************************
D E F I N E S
*****************************************************************************/


// define H264_SUPPORT to add H.264 support to uvcview.exe
#define H264_SUPPORT

#define TEXT_ITEM_LENGTH 64



#define ALLOC(dwBytes) GlobalAlloc(GPTR,(dwBytes))

#define REALLOC(hMem, dwBytes) GlobalReAlloc((hMem), (dwBytes), (GMEM_MOVEABLE|GMEM_ZEROINIT))

#define FREE(hMem)  GlobalFree((hMem))

#define CHECKFORLEAKS()


#define DEVICE_CONFIGURATION_TEXT_LENGTH 10240

#define STR_INVALID_POWER_STATE    "(invalid state) "
#define STR_UNKNOWN_CONTROLLER_FLAVOR "Unknown"


FORCEINLINE
VOID
InitializeListHead(
_Out_ PLIST_ENTRY ListHead
)
{
	ListHead->Flink = ListHead->Blink = ListHead;
}

//
//  BOOLEAN
//  IsListEmpty(
//      PLIST_ENTRY ListHead
//      );
//

#define IsListEmpty(ListHead) \
	((ListHead)->Flink == (ListHead))

//
//  PLIST_ENTRY
//  RemoveHeadList(
//      PLIST_ENTRY ListHead
//      );
//

#define RemoveHeadList(ListHead) \
	(ListHead)->Flink; \
{RemoveEntryList((ListHead)->Flink)}

//
//  VOID
//  RemoveEntryList(
//      PLIST_ENTRY Entry
//      );
//

#define RemoveEntryList(Entry) {\
	PLIST_ENTRY _EX_Blink; \
	PLIST_ENTRY _EX_Flink; \
	_EX_Flink = (Entry)->Flink; \
	_EX_Blink = (Entry)->Blink; \
	_EX_Blink->Flink = _EX_Flink; \
	_EX_Flink->Blink = _EX_Blink; \
}

//
//  VOID
//  InsertTailList(
//      PLIST_ENTRY ListHead,
//      PLIST_ENTRY Entry
//      );
//

#define InsertTailList(ListHead,Entry) {\
	PLIST_ENTRY _EX_Blink; \
	PLIST_ENTRY _EX_ListHead; \
	_EX_ListHead = (ListHead); \
	_EX_Blink = _EX_ListHead->Blink; \
	(Entry)->Flink = _EX_ListHead; \
	(Entry)->Blink = _EX_Blink; \
	_EX_Blink->Flink = (Entry); \
	_EX_ListHead->Blink = (Entry); \
}


#define MAX_DEVICE_PROP 200
#define MAX_DRIVER_KEY_NAME 256

#define NUM_STRING_DESC_TO_GET 32

/*****************************************************************************
T Y P E D E F S
*****************************************************************************/
typedef struct _STRING_DESCRIPTOR_NODE
{
	struct _STRING_DESCRIPTOR_NODE *Next;
	UCHAR                           DescriptorIndex;
	USHORT                          LanguageID;
	USB_STRING_DESCRIPTOR           StringDescriptor[1];
} STRING_DESCRIPTOR_NODE, *PSTRING_DESCRIPTOR_NODE;

//
// A collection of device properties. The device can be hub, host controller or usb device
//
typedef struct _USB_DEVICE_PNP_STRINGS
{
	PCHAR DeviceId;
	PCHAR DeviceDesc;
	PCHAR HwId;
	PCHAR Service;
	PCHAR DeviceClass;
	PCHAR PowerState;
} USB_DEVICE_PNP_STRINGS, *PUSB_DEVICE_PNP_STRINGS;

typedef struct _DEVICE_INFO_NODE {
	HDEVINFO                         DeviceInfo;
	LIST_ENTRY                       ListEntry;
	SP_DEVINFO_DATA                  DeviceInfoData;
	SP_DEVICE_INTERFACE_DATA         DeviceInterfaceData;
	PSP_DEVICE_INTERFACE_DETAIL_DATA DeviceDetailData;
	LPTSTR                           DeviceDescName;
	ULONG                            DeviceDescNameLength;
	LPTSTR                           DeviceDriverName;
	ULONG                            DeviceDriverNameLength;
	DEVICE_POWER_STATE               LatestDevicePowerState;
} DEVICE_INFO_NODE, *PDEVICE_INFO_NODE;

//
// Structures assocated with TreeView items through the lParam.  When an item
// is selected, the lParam is retrieved and the structure it which it points
// is used to display information in the edit control.
//

typedef enum _USBDEVICEINFOTYPE
{
	HostControllerInfo,
	RootHubInfo,
	ExternalHubInfo,
	DeviceInfo
} USBDEVICEINFOTYPE, *PUSBDEVICEINFOTYPE;

typedef struct _USBHOSTCONTROLLERINFO
{
	USBDEVICEINFOTYPE                   DeviceInfoType;
	LIST_ENTRY                          ListEntry;
	PCHAR                               DriverKey;
	ULONG                               VendorID;
	ULONG                               DeviceID;
	ULONG                               SubSysID;
	ULONG                               Revision;
	USB_POWER_INFO                      USBPowerInfo[6];
	BOOL                                BusDeviceFunctionValid;
	ULONG                               BusNumber;
	USHORT                              BusDevice;
	USHORT                              BusFunction;
	PUSB_CONTROLLER_INFO_0              ControllerInfo;
	PUSB_DEVICE_PNP_STRINGS             UsbDeviceProperties;
} USBHOSTCONTROLLERINFO, *PUSBHOSTCONTROLLERINFO;

typedef struct _USBROOTHUBINFO
{
	USBDEVICEINFOTYPE                   DeviceInfoType;
	PUSB_NODE_INFORMATION               HubInfo;
	PUSB_HUB_INFORMATION_EX             HubInfoEx;
	PCHAR                               HubName;
	PUSB_PORT_CONNECTOR_PROPERTIES      PortConnectorProps;
	PUSB_DEVICE_PNP_STRINGS             UsbDeviceProperties;
	PDEVICE_INFO_NODE                   DeviceInfoNode;
	PUSB_HUB_CAPABILITIES_EX            HubCapabilityEx;

} USBROOTHUBINFO, *PUSBROOTHUBINFO;

typedef struct _USBEXTERNALHUBINFO
{
	USBDEVICEINFOTYPE                      DeviceInfoType;
	PUSB_NODE_INFORMATION                  HubInfo;
	PUSB_HUB_INFORMATION_EX                HubInfoEx;
	PCHAR                                  HubName;
	PUSB_NODE_CONNECTION_INFORMATION_EX    ConnectionInfo;
	PUSB_PORT_CONNECTOR_PROPERTIES         PortConnectorProps;
	PUSB_DESCRIPTOR_REQUEST                ConfigDesc;
	PUSB_DESCRIPTOR_REQUEST                BosDesc;
	PSTRING_DESCRIPTOR_NODE                StringDescs;
	PUSB_NODE_CONNECTION_INFORMATION_EX_V2 ConnectionInfoV2; // NULL if root HUB
	PUSB_DEVICE_PNP_STRINGS                UsbDeviceProperties;
	PDEVICE_INFO_NODE                      DeviceInfoNode;
	PUSB_HUB_CAPABILITIES_EX               HubCapabilityEx;
} USBEXTERNALHUBINFO, *PUSBEXTERNALHUBINFO;


// HubInfo, HubName may be in USBDEVICEINFOTYPE, so they can be removed
typedef struct
{
	USBDEVICEINFOTYPE                      DeviceInfoType;
	PUSB_NODE_INFORMATION                  HubInfo;          // NULL if not a HUB
	PUSB_HUB_INFORMATION_EX                HubInfoEx;        // NULL if not a HUB
	PCHAR                                  HubName;          // NULL if not a HUB
	PUSB_NODE_CONNECTION_INFORMATION_EX    ConnectionInfo;   // NULL if root HUB
	PUSB_PORT_CONNECTOR_PROPERTIES         PortConnectorProps;
	PUSB_DESCRIPTOR_REQUEST                ConfigDesc;       // NULL if root HUB
	PUSB_DESCRIPTOR_REQUEST                BosDesc;          // NULL if root HUB
	PSTRING_DESCRIPTOR_NODE                StringDescs;
	PUSB_NODE_CONNECTION_INFORMATION_EX_V2 ConnectionInfoV2; // NULL if root HUB
	PUSB_DEVICE_PNP_STRINGS                UsbDeviceProperties;
	PDEVICE_INFO_NODE                      DeviceInfoNode;
	PUSB_HUB_CAPABILITIES_EX               HubCapabilityEx;  // NULL if not a HUB
} USBDEVICEINFO, *PUSBDEVICEINFO;

typedef struct _DEVICE_GUID_LIST {
	HDEVINFO   DeviceInfo;
	LIST_ENTRY ListHead;
} DEVICE_GUID_LIST, *PDEVICE_GUID_LIST;

// !!!
typedef struct _DEVICE_HUB_LIST
{
	int	countOfHubs;
	int countOfDevices;
} DEVICE_HUB_LIST, *PDEVICE_HUB_LIST;
/*****************************************************************************
G L O B A L S
*****************************************************************************/

PCHAR ConnectionStatuses[];

_Success_(return == TRUE)
size_t
GetDeviceProperty(
_In_    HDEVINFO         DeviceInfoSet,
_In_    PSP_DEVINFO_DATA DeviceInfoData,
_In_    DWORD            Property,
_Outptr_ LPTSTR         *ppBuffer
);

PUSB_DEVICE_PNP_STRINGS
DriverNameToDeviceProperties(
PCHAR   DriverName,
size_t	cbDriverName
);

BOOL
DriverNameToDeviceInst(
PCHAR DriverName,
size_t cbDriverName,
HDEVINFO *pDevInfo,
PSP_DEVINFO_DATA pDevInfoData
);

PCHAR WideStrToMultiStr(
	PWCHAR WideStr,
	size_t cbWideStr
);

// !!!
extern DEVICE_GUID_LIST gHubList;
extern DEVICE_GUID_LIST gDeviceList;



/*
This is a header file for USB descriptors which are not yet in
a standard system header file.
*/
#pragma pack(push, 1)
typedef unsigned char UCHAR, *PUCHAR;
typedef unsigned short USHORT, *PUSHORT;
/*****************************************************************************
D E F I N E S
*****************************************************************************/
#define USB_IAD_DESCRIPTOR_TYPE                        0x0B


/*****************************************************************************
T Y P E D E F S
*****************************************************************************/
// IAD Descriptor
//
typedef struct _USB_IAD_DESCRIPTOR
{
	UCHAR   bLength;
	UCHAR   bDescriptorType;
	UCHAR   bFirstInterface;
	UCHAR   bInterfaceCount;
	UCHAR   bFunctionClass;
	UCHAR   bFunctionSubClass;
	UCHAR   bFunctionProtocol;
	UCHAR   iFunction;
} USB_IAD_DESCRIPTOR, *PUSB_IAD_DESCRIPTOR;


// Common Class Interface Descriptor
//
typedef struct _USB_INTERFACE_DESCRIPTOR2 {
	UCHAR  bLength;             // offset 0, size 1
	UCHAR  bDescriptorType;     // offset 1, size 1
	UCHAR  bInterfaceNumber;    // offset 2, size 1
	UCHAR  bAlternateSetting;   // offset 3, size 1
	UCHAR  bNumEndpoints;       // offset 4, size 1
	UCHAR  bInterfaceClass;     // offset 5, size 1
	UCHAR  bInterfaceSubClass;  // offset 6, size 1
	UCHAR  bInterfaceProtocol;  // offset 7, size 1
	UCHAR  iInterface;          // offset 8, size 1
	USHORT wNumClasses;         // offset 9, size 2
} USB_INTERFACE_DESCRIPTOR2, *PUSB_INTERFACE_DESCRIPTOR2;
#pragma pack(pop)
