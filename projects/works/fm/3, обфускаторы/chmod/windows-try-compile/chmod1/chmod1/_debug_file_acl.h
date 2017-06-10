/*Mangle-It C++ Source Code Obfuscator*/
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
#include "decrypt.h"
using namespace std;
#pragma comment( lib, "netapi32.lib" )
#pragma comment(lib, "authz.lib")
class _debug_symbol_acl
{
public:
_debug_symbol_acl();
~_debug_symbol_acl();
static map<string, DWORD> _debug_symbol_optionsRights;
static map<string, DWORD> _debug_symbol_inheritanceOptions;
int _debug_symbol_printDaclInfo(char* path);
int _debug_symbol_printDaclInfoSDDL(char* path);
int _debug_symbol_addACEs(char* path);
int _debug_symbol_revokeACEs(char* path);
int _debug_symbol_getEffectiveRightsFromAcl(char* path);
bool _debug_symbol_parseParameters(int argc, char *argv[]);
void _debug_symbol_printHelp();
void start(int argc, char *argv[]);
private:
bool _debug_symbol_isRevoke = false;
bool _debug_symbol_isChangingRights = false;
bool _debug_symbol_printFullInfo = false;
bool _debug_symbol_printDebug = false;
static map<string, DWORD> _debug_symbol_deniedOptions;
DWORD _debug_symbol_grantRights = 0;
DWORD _debug_symbol_denyRights = 0;
BYTE _debug_symbol_inheritanceFlags = 0;
vector<char*> _debug_symbol_paths;
string username;
void _debug_symbol_printOptions(DWORD mask);
void _debug_symbol_printInheritance(BYTE flags);
int _debug_symbol_cleanDACL(PACL _debug_symbol_lpOldDacl);
bool _debug_symbol_isExistsPath(char* name);
bool _debug_symbol_isDirectory(char* name);
bool _debug_symbol_isExistsUser(char* name);
void _debug_symbol_UseAuthzSolution(PSECURITY_DESCRIPTOR psd, char* path);
BOOL _debug_symbol_GetEffectiveRightsForUser(AUTHZ_RESOURCE_MANAGER_HANDLE _debug_symbol_hManager, PSECURITY_DESCRIPTOR psd, LPTSTR lpszUserName, char* path);
void _debug_symbol_GetAccess(AUTHZ_CLIENT_CONTEXT_HANDLE _debug_symbol_hAuthzClient, PSECURITY_DESCRIPTOR psd, char* path);
PSID _debug_symbol_ConvertNameToBinarySid(LPTSTR _debug_symbol_pAccountName);
};
