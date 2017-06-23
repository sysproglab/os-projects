#include "Protect.h"

Protect::Protect()
{
	rep = LocalRepository();
	licenseKey = "";
}

Protect::~Protect()
{

}

int Protect::checkProtection()
{	
	if (rep.getFirstRuning())
	{
		//отсутствует лисензионный ключ
		if (licenseKey == "")
			return -1;

		//
		int res = registerProduct();
		if (res != 0)
			return res;

		rep.setLicenseKey(licenseKey);
		rep.setFirstRuning(false);
	}
	else
	{
		licenseKey = rep.getLicenseKey();
	}

	if (checkLicense() != 0)
	{
		rep.setLicenseKey("");
		rep.setFirstRuning(true);
		return -3;
	}
	
	return 0;
}

bool Protect::setLicenseKey(string key)
{
	//ToDo размер ключа
	licenseKey = key;
	return true;
}

//ToDo нужно бќльше параметров
string Protect::getHardwareParameters()
{
	LPDWORD lType[100];
	//char pData[255]; //PVOID   pData[100];
	TCHAR pData[255] = { 0 };
	DWORD  lSize = 100;

	LPCSTR BaseKey = "SOFTWARE\\Microsoft\\.NETFramework";
	int i = RegGetValue(HKEY_LOCAL_MACHINE,
		TEXT("HARDWARE\\DESCRIPTION\\System\\BIOS"),
		TEXT("BaseBoardProduct"),
		RRF_RT_REG_SZ,
		NULL, &pData, &lSize);

	return string(pData);
}

int Protect::registerProduct()
{
	if (licenseKey == "")
		return -11;

	string hardwareParam;
	string activateKey;

	hardwareParam = getHardwareParameters();

	NetClient client(hardwareParam, licenseKey);
	activateKey = client.getActivateKey();

	if (activateKey == "")
		return -12;
	else
	{
		if (activateKey[0] == '_')
		{
			if (activateKey[1] == '1')
			{
				return -13;
			}
			else
			{
				return -14;
			}
		}
	}

	rep.setActivateKey(activateKey);

	return 0;
}

int Protect::checkLicense()
{
	if (licenseKey == "")
		return -2;

	string hash;	
	hash = getHash();

	if (hash != rep.getActivateKey())
		return -1;

	return 0;
}

string Protect::getHash()
{

	string hardwareParam = getHardwareParameters();

	string output = sha256(hardwareParam);

	return output;
}

int Protect::checkUSBKeyProtection()
{
	vector <USBPair> vec = getUSBPairs();

	USBPair usbKey(rep.getUsbKeyVid(), rep.getUsbKeyPid());

	bool haveUSBKey = false;
	for (USBPair pair : vec)
	{
		pair.vid = sha256(pair.vid);
		pair.pid = sha256(pair.pid);
		if (pair == usbKey)
		{
			haveUSBKey = true;
			break;
		}
	}

	if (haveUSBKey != true)
		return -1;
	return 0;
}

vector<USBPair> Protect::getUSBPairs() {
	DWORD deviceIndex = 0;
	SP_DEVINFO_DATA deviceInfoData;
	SP_DEVICE_INTERFACE_DATA deviceInterfaceData;
	deviceInfoData.cbSize = sizeof(deviceInfoData);
	vector <USBPair> vec;

	//buried somewhere deep in the ddk
	static GUID GUID_DEVINTERFACE_USB_HUB = { 0xf18a0e88, 0xc30c, 0x11d0, { 0x88, 0x15, 0x00, 0xa0, 0xc9, 0x06, 0xbe, 0xd8 } };
	static GUID GUID_DEVINTERFACE_USB_DEVICE = { 0xA5DCBF10L, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } };
	static GUID GUID_DEVINTERFACE_USB_HOST_CONTROLLER = { 0x3abf6f2d, 0x71c4, 0x462a, { 0x8a, 0x92, 0x1e, 0x68, 0x61, 0xe6, 0xaf, 0x27 } };

	HDEVINFO deviceInterfaceSet = SetupDiGetClassDevs(&GUID_DEVINTERFACE_USB_DEVICE, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
	//HDEVINFO deviceInterfaceSet = SetupDiGetClassDevs(NULL, NULL, NULL, DIGCF_PRESENT | DIGCF_ALLCLASSES | DIGCF_DEVICEINTERFACE);
	//HDEVINFO deviceInterfaceSet = SetupDiGetClassDevs(NULL, NULL, NULL, DIGCF_ALLCLASSES | DIGCF_PRESENT);
	if (deviceInterfaceSet == INVALID_HANDLE_VALUE)
	{
		// Insert error handling here.
		return vec;
	}

	while (SetupDiEnumDeviceInfo(deviceInterfaceSet, deviceIndex, &deviceInfoData))
	{
		deviceInfoData.cbSize = sizeof(deviceInfoData);

		ULONG IDSize;
		CM_Get_Device_ID_Size(&IDSize, deviceInfoData.DevInst, 0);

		TCHAR* deviceID = new TCHAR[IDSize];

		CM_Get_Device_ID(deviceInfoData.DevInst, deviceID, MAX_PATH, 0);

		char vid[5];
		char pid[5];
		strncpy_s(vid, deviceID + 8, 4);
		strncpy_s(pid, deviceID + 17, 4);

		vec.push_back(USBPair(vid, pid));

		deviceIndex++;
	}

	SetupDiDestroyDeviceInfoList(deviceInterfaceSet);
	return vec;
}