#pragma once

#include <string>
#include "sqlite3.h"

class DataAccessObjecct
{
public:
	DataAccessObjecct();
	~DataAccessObjecct();
	std::string getHardwareByLicenseKey(std::string);
	int setHardwareToLicenseKey(std::string, std::string);
	std::string addLicenseKey(std::string licenseKey);

private:
	static int getHardwareByLicenseKeyCallback(void *data, int argc, char **argv, char **azColName);
	static int setHardwareToLicenseKeyCallback(void *data, int argc, char **argv, char **azColName);
	
};