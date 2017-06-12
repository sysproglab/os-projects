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

using namespace std;

#pragma comment( lib, "netapi32.lib" ) // ���������� ������� ���������� 
#pragma comment(lib, "authz.lib") // ����������, ��� ������ ������� ��������� ����������� ���� ������������

class acl
{
public:
	acl();
	~acl();
	static map<string, DWORD> optionsRights; // ������������ ����� ������� ��������� ���� �� �����
	static map<string, DWORD> inheritanceOptions; // ����� ������������, ����������� ������ � �����������
	int printDaclInfo(char* path); // �������� ���������� � �����
	int printDaclInfoSDDL(char* path); // �������� ���������� � ����� �� SDDL
	int addACEs(char* path); // ��������� �������� � ������������ � ������������ �������
	int revokeACEs(char* path); // ������� ������ �� ������ DACL, ����������� � ������������ 
	int getEffectiveRightsFromAcl(char* path); // ��������� ����������� ����
	bool parseParameters(int argc, char *argv[]); // ������ ����������
	void printHelp(); // ������ �������������
	void start(int argc, char *argv[]); // ������ ����������
private:
	bool isRevoke = false; // ����, ������������, ��� ����� ��� ������������ ���������
	bool isChangingRights = false; // ����, ������������, ��� ����� ��� ������������ ����������
	bool printFullInfo = false; // �������� � ������ ��� ����������
	bool printDebug = false; // �������� ����� �� � ����� ���������
	static map<string, DWORD> deniedOptions; // ����������� ����� ����, ������ ��� ������� �������� ������������
	DWORD grantRights = 0; // ����� � ������������ �������
	DWORD denyRights = 0; // ����� � ������������ �������
	BYTE inheritanceFlags = 0; // ����� ������������ (��� ����������)
	vector<char*> paths; // ���� � ������ � �����������
	string username; // username ������������, ��� �������� �������� ��� ������� �����
	void printOptions(DWORD mask); // ������� �����, ������������� � �����
	void printInheritance(BYTE flags); // ������� ������������� ����� ������������
	int cleanDACL(PACL lpOldDacl); // ������������ ��� ��������� ��� ������� ������� � ������ DACL, �������������� ����� ������. ����� ����� �������� ����������� � ����������� �����, ���� ��� ������������ ����������� ������� � ������ DACL
	bool isExistsPath(char* name); // ���������, ���������� �� ���� ��� ����������
	bool isDirectory(char* name); // ���������, �������� �� ���� �����������
	bool isExistsUser(char* name); // ���������, ���������� �� ������������ � �������� ������

	// ����������, ��� ������ ������� ��������� ����������� ���� ������������
	void UseAuthzSolution(PSECURITY_DESCRIPTOR psd, char* path);
	BOOL GetEffectiveRightsForUser(AUTHZ_RESOURCE_MANAGER_HANDLE hManager, PSECURITY_DESCRIPTOR psd, LPTSTR lpszUserName, char* path);
	void GetAccess(AUTHZ_CLIENT_CONTEXT_HANDLE hAuthzClient, PSECURITY_DESCRIPTOR psd, char* path);
	PSID ConvertNameToBinarySid(LPTSTR pAccountName);
};

