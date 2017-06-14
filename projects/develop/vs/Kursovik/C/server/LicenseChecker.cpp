#include "LicenseChecker.h"

LicenseChecker::LicenseChecker(std::string ahardwareParam, std::string alicenseKey)
{
	hardwareParam = ahardwareParam;
	licenseKey = alicenseKey;
}

LicenseChecker::~LicenseChecker()
{

}

std::string LicenseChecker::checkLicense()
{
	//Pair pair = checkKeyValidation();
	DataAccessObjecct dao = DataAccessObjecct();
	std::string hw = dao.getHardwareByLicenseKey(licenseKey);
	if (hw == "")
	{
		return "_1 Invalid Key";
	}	
	else if (hw == "NULL")
	{
		dao.setHardwareToLicenseKey(licenseKey, hardwareParam);
	}
	else if (hw != hardwareParam)
	{
		return "_2 Invalid hardware parameters";
	}
	return getHash();
}

//ToDO sqlite3
Pair LicenseChecker::checkKeyValidation()
{
	Pair pair = Pair();
	DataAccessObjecct dao = DataAccessObjecct();
	std::string hw = dao.getHardwareByLicenseKey(licenseKey);
	if (hw == "")
		pair.key = "";
	else if (hw == "NULL")
	{

	}
	if (licenseKey != "LIC")
	{
		pair.key = "";
		pair.hardwareParam = "";
	}
	else
	{
		pair.key = "LIC";
		pair.hardwareParam = "VA50_HC_CR";
	}

	return pair;
}


std::string LicenseChecker::getHash()
{
	std::string output = sha256(hardwareParam);
	return output;
}