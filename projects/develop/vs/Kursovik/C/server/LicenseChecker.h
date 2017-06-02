#pragma once

#include "string"
#include "sha256.h"
#include "DataAccessObject.h"

//using namespace std;

struct Pair
{
	std::string key;
	std::string hardwareParam;
};

class LicenseChecker
{
public:
	LicenseChecker(std::string hardwareParam, std::string licenseKey);
	~LicenseChecker();
	std::string checkLicense();

private:
	std::string hardwareParam, licenseKey;

	std::string getHash();
	Pair checkKeyValidation();

};