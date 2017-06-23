#pragma once
#include "string"
using namespace std;

class MemberRepository
{
public:
	virtual bool getFirstRuning() = 0;
	virtual bool setFirstRuning(bool) = 0;

	virtual bool setLicenseKey(string) = 0;
	virtual string getLicenseKey() = 0;

	virtual bool setActivateKey(string) = 0;
	virtual string getActivateKey() = 0;
};