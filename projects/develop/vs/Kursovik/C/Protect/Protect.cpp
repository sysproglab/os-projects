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