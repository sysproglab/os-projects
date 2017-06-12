#pragma once
#include <windows.h>
#include <map>
#include <string>
#include <vector>
#include <lm.h>
#include <aclapi.h>
#include <sddl.h> 
#include <atlstr.h>
#include <authz.h>
#include <iostream>

using namespace std;

#pragma comment( lib, "netapi32.lib" ) // подключаем сетевую библиотеку 
#pragma comment(lib, "authz.lib") // используем, как второй вариант получения эффективных прав пользователя

class acl
{
public:
	acl();
	~acl();
	static map<string, DWORD> optionsRights; // пользователь может указать несколько прав из карты
	static map<string, DWORD> inheritanceOptions; // флаги наследования, применяются только к директориям
	int printDaclInfo(char* path); // Печатает информацию о файле
	int printDaclInfoSDDL(char* path); // Печатает информацию о файле на SDDL
	int addACEs(char* path); // Добавляет элементы с разрешаемыми и запрещенными правами
	int revokeACEs(char* path); // Удалить записи из списка DACL, относящиеся к пользователю 
	int getEffectiveRightsFromAcl(char* path); // Получение эффективных прав
	bool parseParameters(int argc, char *argv[]); // Разбор параметров
	void printHelp(); // Печать использования
	void start(int argc, char *argv[], FILE * f); // Запуск приложения
private:
	bool isRevoke = false; // Флаг, показывающий, что права для пользователя удаляются
	bool isChangingRights = false; // Флаг, показывающий, что права для пользователя изменяются
	bool printFullInfo = false; // Выводить о правах всю информацию
	bool printDebug = false; // Выводить права до и после изменений
	static map<string, DWORD> deniedOptions; // Запрещенные опции прав, потому что обходят проверки противоречий
	DWORD grantRights = 0; // маска с разрешаемыми правами
	DWORD denyRights = 0; // маска с запрещенными правами
	BYTE inheritanceFlags = 0; // Флаги наследования (для директорий)
	vector<char*> paths; // пути к файлам и директориям
	string username; // username пользователя, для которого изменяем или удаляем права
	void printOptions(DWORD mask); // Выводит опции, установленные в маске
	void printInheritance(BYTE flags); // Выводит установленные флаги наследования
	int cleanDACL(PACL lpOldDacl); // Используется для изменения или очистки записей в списке DACL, противоречащие новым правам. Может также поменять разрешаемые и запрещаемые права, если они противоречат наследуемым записям в списке DACL
	bool isExistsPath(char* name); // Проверяет, существует ли файл или директория
	bool isDirectory(char* name); // Проверяет, является ли путь директорией
	bool isExistsUser(char* name); // Проверяет, существует ли пользователь с заданным именем

	// используем, как второй вариант получения эффективных прав пользователя
	void UseAuthzSolution(PSECURITY_DESCRIPTOR psd, char* path);
	BOOL GetEffectiveRightsForUser(AUTHZ_RESOURCE_MANAGER_HANDLE hManager, PSECURITY_DESCRIPTOR psd, LPTSTR lpszUserName, char* path);
	void GetAccess(AUTHZ_CLIENT_CONTEXT_HANDLE hAuthzClient, PSECURITY_DESCRIPTOR psd, char* path);
	PSID ConvertNameToBinarySid(LPTSTR pAccountName);
	FILE * file;
};

