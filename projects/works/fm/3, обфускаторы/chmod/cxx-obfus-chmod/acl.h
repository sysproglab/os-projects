
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
#pragma comment( lib, "netapi32.lib" ) // подключаем сетевую библиотеку 
#pragma comment(lib, "authz.lib") // используем, как второй вариант получения эффективных прав пользователя
class ReplacementFor_acl{public:ReplacementFor_acl();~ReplacementFor_acl();static map<string,DWORD>ReplacementFor_optionsRights;
static map<string,DWORD>ReplacementFor_inheritanceOptions;
int ReplacementFor_printDaclInfo(char*ReplacementFor_path);
int ReplacementFor_printDaclInfoSDDL(char*ReplacementFor_path);
int ReplacementFor_addACEs(char*ReplacementFor_path);
int ReplacementFor_revokeACEs(char*ReplacementFor_path);
int ReplacementFor_getEffectiveRightsFromAcl(char*ReplacementFor_path);
bool ReplacementFor_parseParameters(int ReplacementFor_argc,char*ReplacementFor_argv[]);
void ReplacementFor_printHelp();
void ReplacementFor_start(int ReplacementFor_argc,char*ReplacementFor_argv[]);
private:bool ReplacementFor_isRevoke=false;
bool ReplacementFor_isChangingRights=false;
bool ReplacementFor_printFullInfo=false;
bool ReplacementFor_printDebug=false;
static map<string,DWORD>ReplacementFor_deniedOptions;
DWORD ReplacementFor_grantRights=(0xd2d+202-0xdf7);
DWORD ReplacementFor_denyRights=(0xd2d+202-0xdf7);
BYTE ReplacementFor_inheritanceFlags=(0xd2d+202-0xdf7);
vector<char*>ReplacementFor_paths;
string ReplacementFor_username;
void ReplacementFor_printOptions(DWORD ReplacementFor_mask);
void ReplacementFor_printInheritance(BYTE flags);
int ReplacementFor_cleanDACL(PACL ReplacementFor_lpOldDacl);
bool ReplacementFor_isExistsPath(char*name);
bool ReplacementFor_isDirectory(char*name);
bool ReplacementFor_isExistsUser(char*name);

void ReplacementFor_UseAuthzSolution(PSECURITY_DESCRIPTOR ReplacementFor_psd,char*ReplacementFor_path);BOOL ReplacementFor_GetEffectiveRightsForUser(AUTHZ_RESOURCE_MANAGER_HANDLE ReplacementFor_hManager,PSECURITY_DESCRIPTOR ReplacementFor_psd,LPTSTR ReplacementFor_lpszUserName,char*
ReplacementFor_path);void ReplacementFor_GetAccess(AUTHZ_CLIENT_CONTEXT_HANDLE ReplacementFor_hAuthzClient,PSECURITY_DESCRIPTOR ReplacementFor_psd,char*ReplacementFor_path);PSID ReplacementFor_ConvertNameToBinarySid(LPTSTR ReplacementFor_pAccountName);};
