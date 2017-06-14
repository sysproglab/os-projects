#include "DataAccessObject.h"

DataAccessObjecct::DataAccessObjecct()
{

}

DataAccessObjecct::~DataAccessObjecct()
{

}

//key - значение ключа
//Возвращает значение аппаратного обеспечения, если оно уже зарегистрированно
//Возвращает NULL если не зарегистрированно
//Возвращает пустую строку, если лицензионный ключ не существует.
std::string DataAccessObjecct::getHardwareByLicenseKey(std::string key)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	std::string sql;
	std::string* hardware = new std::string("");
	
	/* Open database */
	rc = sqlite3_open("licenses.db", &db);
	if (rc){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return(0);
	}
	else{
		fprintf(stderr, "Opened database successfully\n");
	}

	/* Create SQL statement */
	sql = "SELECT hardware FROM licenses WHERE licenseKey = \'" + key + "\';";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql.c_str(), getHardwareByLicenseKeyCallback,
		(void*)hardware, &zErrMsg);
	if (rc != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else{
		fprintf(stdout, "Operation done successfully\n");
	}
	sqlite3_close(db);

	std::string hw = std::string(*hardware);
	delete hardware;
	return hw;
}

int DataAccessObjecct::getHardwareByLicenseKeyCallback(void *ptrData, int argc, char **argv, char **azColName){
	std::string* hardware = (std::string*)ptrData;

	for (int i = 0; i<argc; i++){
		if (strcmp(argv[i], "") == 0)
			*hardware = "NULL";
		else
			*hardware = argv[i];
	}
	return 0;
}

int DataAccessObjecct::setHardwareToLicenseKey(std::string licenseKey, std::string hardware)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	std::string sql;
	const char* str = "";

	/* Open database */
	rc = sqlite3_open("licenses.db", &db);
	if (rc){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return(0);
	}
	else{
		fprintf(stderr, "Opened database successfully\n");
	}

	/* Create SQL statement */
	sql = "UPDATE licenses SET hardware = \'" + hardware + 
		"\' WHERE licenseKey = \'" + licenseKey + "\';";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql.c_str(), setHardwareToLicenseKeyCallback,
		(void*)str, &zErrMsg);
	if (rc != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else{
		fprintf(stdout, "Operation done successfully\n");
	}
	sqlite3_close(db);

	return 0;
}

int DataAccessObjecct::setHardwareToLicenseKeyCallback(void *data, int argc, char **argv, char **azColName)
{
	int i = 0;
	return 0;
}