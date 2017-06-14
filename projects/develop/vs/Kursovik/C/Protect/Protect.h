#pragma once
#pragma comment (lib, "Setupapi.lib")

#include "cstring"
#include "LocalRepository.h"
#include "NetClient.h"
#include "sha256.h"

#include <SetupAPI.h>
#include <cfgmgr32.h>
#include <tchar.h>

#include <vector>

#include <stdio.h>
#pragma comment(lib, "user32.lib")

using namespace std;

//Наверное не нужен
struct USBPair
{
	string vid;
	string pid;

public:
	USBPair(string v, string p)
	{
		vid = v;
		pid = p;
	}

	bool equals(USBPair pair)
	{
		if (pair.vid == vid && pair.pid == pid)
			return true;
		return false;
	}

	friend bool operator==(const USBPair& left, const USBPair& right)
	{
		if (left.vid == right.vid && left.pid == right.pid)
			return true;
		return false;
	}
};

class Protect
{
public:
	Protect();
	~Protect();

	int checkProtection();
	bool setLicenseKey(string key);
	int checkUSBKeyProtection();
private:

	string getHardwareParameters();
	int registerProduct();
	int checkLicense();
	string getHash();
	vector<USBPair> getUSBPairs();
	

	LocalRepository rep;

	string licenseKey;

};