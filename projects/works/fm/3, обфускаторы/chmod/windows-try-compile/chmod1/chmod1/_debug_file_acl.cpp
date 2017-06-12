/*Mangle-It C++ Source Code Obfuscator*/
#include "stdafx.h"
#include <stdio.h>
#include "_debug_file_acl.h"
#include <windows.h>
#include <iostream>
#include <algorithm>
#include <bitset>
#define _debug_symbol_BUFSIZE 4096
map<string, DWORD> _debug_symbol_acl::_debug_symbol_optionsRights = {
{  decrypt::_debug_symbol_dec_debug(_T( "_debug_F")), FILE_ALL_ACCESS },
{  decrypt::_debug_symbol_dec_debug(_T( "_debug_RX")), FILE_GENERIC_EXECUTE | FILE_GENERIC_READ },
{  decrypt::_debug_symbol_dec_debug(_T( "_debug_R")), FILE_GENERIC_READ },
{  decrypt::_debug_symbol_dec_debug(_T( "_debug_W")), FILE_GENERIC_WRITE },
{  decrypt::_debug_symbol_dec_debug(_T( "_debug_GA")), GENERIC_ALL },
{  decrypt::_debug_symbol_dec_debug(_T( "_debug_GR")), GENERIC_READ },
{  decrypt::_debug_symbol_dec_debug(_T( "_debug_GW")), GENERIC_WRITE },
{  decrypt::_debug_symbol_dec_debug(_T( "_debug_GE")), GENERIC_EXECUTE },
{  decrypt::_debug_symbol_dec_debug(_T( "_debug_X")), FILE_EXECUTE },
{  decrypt::_debug_symbol_dec_debug(_T( "_debug_RD")), FILE_READ_DATA },
{  decrypt::_debug_symbol_dec_debug(_T( "_debug_WD")), FILE_WRITE_DATA },
{  decrypt::_debug_symbol_dec_debug(_T( "_debug_AD")), FILE_APPEND_DATA },
{  decrypt::_debug_symbol_dec_debug(_T( "_debug_REA")), FILE_READ_EA },
{  decrypt::_debug_symbol_dec_debug(_T( "_debug_WEA")), FILE_WRITE_EA },
{  decrypt::_debug_symbol_dec_debug(_T( "_debug_RA")), FILE_READ_ATTRIBUTES },
{  decrypt::_debug_symbol_dec_debug(_T( "_debug_WA")), FILE_WRITE_ATTRIBUTES },
{  decrypt::_debug_symbol_dec_debug(_T( "_debug_DC")), FILE_DELETE_CHILD },
{  decrypt::_debug_symbol_dec_debug(_T( "_debug_WDAC")), WRITE_DAC },
{  decrypt::_debug_symbol_dec_debug(_T( "_debug_WO")), WRITE_OWNER },
{  decrypt::_debug_symbol_dec_debug(_T( "_debug_D")), DELETE },
{  decrypt::_debug_symbol_dec_debug(_T( "_debug_S")), SYNCHRONIZE },
{  decrypt::_debug_symbol_dec_debug(_T( "_debug_RC")), READ_CONTROL },
{  decrypt::_debug_symbol_dec_debug(_T( "_debug_AS")), ACCESS_SYSTEM_SECURITY }
};
map<string, DWORD> _debug_symbol_acl::_debug_symbol_deniedOptions = {
{  decrypt::_debug_symbol_dec_debug(_T( "_debug_GA")), GENERIC_ALL },
{  decrypt::_debug_symbol_dec_debug(_T( "_debug_GR")), GENERIC_READ },
{  decrypt::_debug_symbol_dec_debug(_T( "_debug_GW")), GENERIC_WRITE },
{  decrypt::_debug_symbol_dec_debug(_T( "_debug_GE")), GENERIC_EXECUTE }
};
map<string, DWORD> _debug_symbol_acl::_debug_symbol_inheritanceOptions = {
{  decrypt::_debug_symbol_dec_debug(_T( "_debug_OI")), OBJECT_INHERIT_ACE },
{  decrypt::_debug_symbol_dec_debug(_T( "_debug_CI")), CONTAINER_INHERIT_ACE },
{  decrypt::_debug_symbol_dec_debug(_T( "_debug_IO")), INHERIT_ONLY_ACE },
{  decrypt::_debug_symbol_dec_debug(_T( "_debug_NP")), NO_PROPAGATE_INHERIT_ACE }
};
_debug_symbol_acl::_debug_symbol_acl()
{
}
_debug_symbol_acl::~_debug_symbol_acl()
{
}
int _debug_symbol_acl::_debug_symbol_printDaclInfo(char* path)
{
PSECURITY_DESCRIPTOR pSecurityDescriptor;
DWORD _debug_symbol_dwRetCode, mask;
PACL pDacl = NULL;
char lpName[UNLEN], lpDomain[UNLEN];
string _debug_symbol_typeAce;
SID_NAME_USE SidType;
DWORD dwSize = UNLEN;
PSID sid;
BYTE flags;
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nInfo about \"%s\"\n")), path);
_debug_symbol_dwRetCode = GetNamedSecurityInfo(
path,
SE_FILE_OBJECT,
DACL_SECURITY_INFORMATION,
NULL,
NULL,
&pDacl,
NULL,
&pSecurityDescriptor);
if (_debug_symbol_dwRetCode != ERROR_SUCCESS) {
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Get named security info failed.\n")));
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Error code: %u\n")), _debug_symbol_dwRetCode);
return _debug_symbol_dwRetCode;
}
void *_debug_symbol_lpAce = NULL;
for (unsigned i = 0; i < pDacl->AceCount; ++i)
{
if (!GetAce(
pDacl,
i,
&_debug_symbol_lpAce))
{
_debug_symbol_dwRetCode = GetLastError();
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Get ace failed.\n")));
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Error code: %d\n")), _debug_symbol_dwRetCode);
return _debug_symbol_dwRetCode;
}
if (((ACE_HEADER*)_debug_symbol_lpAce)->AceType == ACCESS_ALLOWED_ACE_TYPE) {
sid = &((ACCESS_ALLOWED_ACE*)_debug_symbol_lpAce)->SidStart;
mask = ((ACCESS_ALLOWED_ACE*)_debug_symbol_lpAce)->Mask;
_debug_symbol_typeAce =  decrypt::_debug_symbol_dec_debug(_T( "_debug_ALLOWED"));
}
else {
sid = &((ACCESS_DENIED_ACE*)_debug_symbol_lpAce)->SidStart;
mask = ((ACCESS_DENIED_ACE*)_debug_symbol_lpAce)->Mask;
_debug_symbol_typeAce =  decrypt::_debug_symbol_dec_debug(_T( "_debug_DENIED"));
}
flags = ((ACE_HEADER*)_debug_symbol_lpAce)->AceFlags;
dwSize = UNLEN;
memset(&lpName[0], 0, sizeof(lpName));
memset(&lpDomain[0], 0, sizeof(lpDomain));
if (!LookupAccountSid(NULL, sid, lpName, &dwSize, lpDomain, &dwSize, &SidType))
{
_debug_symbol_dwRetCode = GetLastError();
if (_debug_symbol_dwRetCode == ERROR_NONE_MAPPED)
strcpy_s(lpName, dwSize,  decrypt::_debug_symbol_dec_debug(_T( "_debug_NONE_MAPPED")));
else
{
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_LookupAccountSid Error %u\n")), GetLastError());
return _debug_symbol_dwRetCode;
}
}
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_For user %s\\%s flags, %s rights: ")), lpDomain, lpName, _debug_symbol_typeAce.c_str());
_debug_symbol_printInheritance(flags);
_debug_symbol_printOptions(mask);
}
LocalFree(pSecurityDescriptor);
return 0;
}
int _debug_symbol_acl::_debug_symbol_printDaclInfoSDDL(char* path)
{
PSECURITY_DESCRIPTOR pSecurityDescriptor;
LPSTR StringSecurityDescriptor;
DWORD StringSecurityDescriptorLen;
DWORD _debug_symbol_dwRetCode;
PACL pDacl = NULL;
PSID _debug_symbol_pSidOwner;
PSID _debug_symbol_pSidGroup;
LPTSTR _debug_symbol_lpStringSid;
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\Info about \"%s\"\n")), path);
_debug_symbol_dwRetCode = GetNamedSecurityInfo(
path,
SE_FILE_OBJECT,
OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION |
DACL_SECURITY_INFORMATION,
&_debug_symbol_pSidOwner,
&_debug_symbol_pSidGroup,
&pDacl,
NULL,
&pSecurityDescriptor);
if (_debug_symbol_dwRetCode != ERROR_SUCCESS) {
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Get named security info failed.\n")));
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Error code: %u\n")), _debug_symbol_dwRetCode);
return _debug_symbol_dwRetCode;
}
if (!ConvertSecurityDescriptorToStringSecurityDescriptor(
pSecurityDescriptor,
SDDL_REVISION_1,
OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION |
DACL_SECURITY_INFORMATION | SACL_SECURITY_INFORMATION,
&StringSecurityDescriptor,
&StringSecurityDescriptorLen))
{
_debug_symbol_dwRetCode = GetLastError();
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Convert security descriptor to string security descriptor failed. Error code: %u\n")), _debug_symbol_dwRetCode);
return _debug_symbol_dwRetCode;
}
string result = StringSecurityDescriptor;
string::size_type pos = result.find( decrypt::_debug_symbol_dec_debug(_T( "_debug_:")));
while (pos != string::npos) {
result.insert(pos - 1,  decrypt::_debug_symbol_dec_debug(_T( "_debug_\n")));
result.insert(pos + 2,  decrypt::_debug_symbol_dec_debug(_T( "_debug_\n\t")));
pos = result.find( decrypt::_debug_symbol_dec_debug(_T( "_debug_:")), pos + 2);
}
pos = result.find( decrypt::_debug_symbol_dec_debug(_T( "_debug_)")));
while (pos != string::npos) {
result.insert(pos + 1,  decrypt::_debug_symbol_dec_debug(_T( "_debug_\n\t")));
pos = result.find( decrypt::_debug_symbol_dec_debug(_T( "_debug_)")), pos + 1);
}
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_String security desriptor: %s\n")), result.c_str());
char lpName[UNLEN], lpDomain[UNLEN];;
SID_NAME_USE SidType;
DWORD dwSize = UNLEN;
PSID sid;
if (!LookupAccountSid(NULL, _debug_symbol_pSidOwner, lpName, &dwSize, lpDomain, &dwSize, &SidType))
{
_debug_symbol_dwRetCode = GetLastError();
if (_debug_symbol_dwRetCode == ERROR_NONE_MAPPED)
strcpy_s(lpName, dwSize,  decrypt::_debug_symbol_dec_debug(_T( "_debug_NONE_MAPPED")));
else
{
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_LookupAccountSid Error %u\n")), GetLastError());
return _debug_symbol_dwRetCode;
}
}
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Owner is a member of the %s\\%s group\n")), lpDomain, lpName);
LocalFree(pSecurityDescriptor);
LocalFree(StringSecurityDescriptor);
return 0;
}
int _debug_symbol_acl::_debug_symbol_addACEs(char* path)
{
EXPLICIT_ACCESS _debug_symbol_ea;
PACL _debug_symbol_lpOldDacl;
PACL _debug_symbol_lpNewDacl;
PSECURITY_DESCRIPTOR _debug_symbol_lpSD;
DWORD dwErrCode;
EXPLICIT_ACCESS s [2];
int _debug_symbol_countAddedACEs = 0;
dwErrCode = GetNamedSecurityInfo(
path,
SE_FILE_OBJECT,
DACL_SECURITY_INFORMATION,
NULL,
NULL,
&_debug_symbol_lpOldDacl,
NULL,
&_debug_symbol_lpSD);
if (dwErrCode != ERROR_SUCCESS)
{
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Get named security info failed.\n")));
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Error code: %u\n")), dwErrCode);
return dwErrCode;
}
dwErrCode = _debug_symbol_cleanDACL(_debug_symbol_lpOldDacl);
if (dwErrCode != ERROR_SUCCESS)
{
perror( decrypt::_debug_symbol_dec_debug(_T( "_debug_Cleaning DACL failed.\n")));
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_The last error code: %u\n")), dwErrCode);
return dwErrCode;
}
if (_debug_symbol_grantRights > 0)
{
BuildExplicitAccessWithName(
&_debug_symbol_ea,
const_cast<char *>(username.c_str()),
_debug_symbol_grantRights,
GRANT_ACCESS,
_debug_symbol_inheritanceFlags
);
s[_debug_symbol_countAddedACEs] = _debug_symbol_ea;
_debug_symbol_countAddedACEs++;
}
if (_debug_symbol_denyRights > 0)
{
BuildExplicitAccessWithName(
&_debug_symbol_ea,
const_cast<char *>(username.c_str()),
_debug_symbol_denyRights,
DENY_ACCESS,
_debug_symbol_inheritanceFlags
);
s[_debug_symbol_countAddedACEs] = _debug_symbol_ea;
_debug_symbol_countAddedACEs++;
}
dwErrCode = SetEntriesInAcl(_debug_symbol_countAddedACEs,
s,
_debug_symbol_lpOldDacl,
&_debug_symbol_lpNewDacl);
if (dwErrCode != ERROR_SUCCESS)
{
perror( decrypt::_debug_symbol_dec_debug(_T( "_debug_Set entries in DACL failed.\n")));
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_The last error code: %u\n")), dwErrCode);
return dwErrCode;
}
dwErrCode = SetNamedSecurityInfo(
path,
SE_FILE_OBJECT,
DACL_SECURITY_INFORMATION,
NULL,
NULL,
_debug_symbol_lpNewDacl,
NULL);
if (dwErrCode != ERROR_SUCCESS)
{
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Set named security info failed.\n")));
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Error code: %u\n")), dwErrCode);
return dwErrCode;
}
LocalFree(_debug_symbol_lpSD);
LocalFree(_debug_symbol_lpNewDacl);
if (_debug_symbol_grantRights > 0 || _debug_symbol_denyRights > 0) {
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_The rights for user %s to \"%s\" were modified.\n")), username.c_str(), path);
}
else {
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_The rights for user %s to \"%s\" have not been modified.\n")), username.c_str(), path);
}
return 0;
}
int _debug_symbol_acl::_debug_symbol_cleanDACL(PACL _debug_symbol_lpOldDacl)
{
void *_debug_symbol_lpAce = NULL;
char lpName[UNLEN], lpDomain[UNLEN];
SID_NAME_USE SidType;
DWORD dwSize = UNLEN, dwErrCode, _debug_symbol_rights, flags;
DWORD *mask;
PSID sid;
vector<int> _debug_symbol_deleteIndex;
vector<int>::reverse_iterator it;
for (unsigned i = 0; i < _debug_symbol_lpOldDacl->AceCount; ++i)
{
if (!GetAce(
_debug_symbol_lpOldDacl,
i,
&_debug_symbol_lpAce))
{
dwErrCode = GetLastError();
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Get асе failed.\n")));
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Error code: %d\n")), dwErrCode);
return dwErrCode;
}
flags = ((ACE_HEADER*)_debug_symbol_lpAce)->AceFlags;
if (((ACE_HEADER*)_debug_symbol_lpAce)->AceType == ACCESS_ALLOWED_ACE_TYPE) {
if (_debug_symbol_denyRights == 0) continue;
sid = &((ACCESS_ALLOWED_ACE*)_debug_symbol_lpAce)->SidStart;
mask = &((ACCESS_ALLOWED_ACE*)_debug_symbol_lpAce)->Mask;
_debug_symbol_rights = _debug_symbol_denyRights;
}
else {
if (_debug_symbol_grantRights == 0) continue;
sid = &((ACCESS_DENIED_ACE*)_debug_symbol_lpAce)->SidStart;
mask = &((ACCESS_DENIED_ACE*)_debug_symbol_lpAce)->Mask;
_debug_symbol_rights = _debug_symbol_grantRights;
}
dwSize = UNLEN;
memset(&lpName[0], 0, sizeof(lpName));
memset(&lpDomain[0], 0, sizeof(lpDomain));
if (!LookupAccountSid(NULL, sid,
lpName, &dwSize, lpDomain,
&dwSize, &SidType))
{
dwErrCode = GetLastError();
if (dwErrCode == ERROR_NONE_MAPPED)
strcpy_s(lpName, dwSize,  decrypt::_debug_symbol_dec_debug(_T( "_debug_NONE_MAPPED")));
else
{
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_LookupAccountSid Error %u\n")), GetLastError());
return dwErrCode;
}
}
if (strcmp(lpName, username.c_str()) != 0) {
continue;
}
if ((flags & INHERITED_ACE) == INHERITED_ACE && ((*mask & ~_debug_symbol_rights) == 0 || (*mask & ~_debug_symbol_rights) != *mask)) {
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Sorry, but found problem with inherited ACE (crossed rights). Try at first to change rights of parent directories.\n")));
return - 1;
}
if ((*mask & ~_debug_symbol_rights) == 0) {
_debug_symbol_deleteIndex.push_back(i);
}
else if ((*mask & ~_debug_symbol_rights) != *mask) {
*mask = (*mask & ~_debug_symbol_rights);
}
}
for (it = _debug_symbol_deleteIndex.rbegin(); it != _debug_symbol_deleteIndex.rend(); ++it)
{
if (!DeleteAce(_debug_symbol_lpOldDacl, *it))
{
dwErrCode = GetLastError();
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Delete ace failed.\n")));
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Error code: %d\n")), dwErrCode);
return dwErrCode;
}
}
return 0;
}
int _debug_symbol_acl::_debug_symbol_revokeACEs(char* path)
{
EXPLICIT_ACCESS _debug_symbol_ea;
PACL _debug_symbol_lpOldDacl;
PSECURITY_DESCRIPTOR _debug_symbol_lpSD;
DWORD dwErrCode;
vector<int> _debug_symbol_deleteIndexes;
vector<int>::reverse_iterator it;
DWORD _debug_symbol_dwLengthOfSID = 0;
DWORD _debug_symbol_dwLengthOfDomainName = 0;
SID *_debug_symbol_userSID = NULL;
LPTSTR _debug_symbol_lpDomainName = NULL;
SID_NAME_USE _debug_symbol_type_of_SID;
void *_debug_symbol_lpAce = NULL;
DWORD type;
PSID sid;
if (!LookupAccountName(
NULL,
username.c_str(),
NULL,
&_debug_symbol_dwLengthOfSID,
NULL,
&_debug_symbol_dwLengthOfDomainName,
&_debug_symbol_type_of_SID))
dwErrCode = GetLastError();
if (dwErrCode == ERROR_INSUFFICIENT_BUFFER)
{
_debug_symbol_userSID = (SID*) new char[_debug_symbol_dwLengthOfSID];
_debug_symbol_lpDomainName = (LPTSTR) new wchar_t[_debug_symbol_dwLengthOfDomainName];
}
else
{
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Lookup account name failed.\n")));
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Error code: %d\n")), dwErrCode);
return dwErrCode;
}
if (!LookupAccountName(
NULL,
username.c_str(),
_debug_symbol_userSID,
&_debug_symbol_dwLengthOfSID,
_debug_symbol_lpDomainName,
&_debug_symbol_dwLengthOfDomainName,
&_debug_symbol_type_of_SID))
{
dwErrCode = GetLastError();
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Lookup account name failed.\n")));
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Error code: %d\n")), dwErrCode);
return dwErrCode;
}
dwErrCode = GetNamedSecurityInfo(
path,
SE_FILE_OBJECT,
DACL_SECURITY_INFORMATION,
NULL,
NULL,
&_debug_symbol_lpOldDacl,
NULL,
&_debug_symbol_lpSD);
if (dwErrCode != ERROR_SUCCESS)
{
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Get named security info failed.\n")));
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Error code: %u\n")), dwErrCode);
return dwErrCode;
}
for (unsigned i = 0; i < _debug_symbol_lpOldDacl->AceCount; ++i)
{
if (!GetAce(
_debug_symbol_lpOldDacl,
i,
&_debug_symbol_lpAce))
{
dwErrCode = GetLastError();
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Get асе failed.\n")));
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Error code: %d\n")), dwErrCode);
return dwErrCode;
}
type = ((ACE_HEADER*)_debug_symbol_lpAce)->AceType;
if (type == ACCESS_ALLOWED_ACE_TYPE) {
sid = &((ACCESS_ALLOWED_ACE*)_debug_symbol_lpAce)->SidStart;
}
else if (type == ACCESS_DENIED_ACE_TYPE) {
sid = &((ACCESS_DENIED_ACE*)_debug_symbol_lpAce)->SidStart;
}
else {
continue;
}
if (!EqualSid(_debug_symbol_userSID, sid)) {
continue;
}
_debug_symbol_deleteIndexes.push_back(i);
}
for (it = _debug_symbol_deleteIndexes.rbegin(); it != _debug_symbol_deleteIndexes.rend(); ++it)
{
if (!DeleteAce(_debug_symbol_lpOldDacl, *it))
{
dwErrCode = GetLastError();
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Delete ace failed.\n")));
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Error code: %d\n")), dwErrCode);
return dwErrCode;
}
}
dwErrCode = SetNamedSecurityInfo(
path,
SE_FILE_OBJECT,
DACL_SECURITY_INFORMATION,
NULL,
NULL,
_debug_symbol_lpOldDacl,
NULL);
if (dwErrCode != ERROR_SUCCESS)
{
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Set named security info failed.\n")));
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Error code: %u\n")), dwErrCode);
return dwErrCode;
}
LocalFree(_debug_symbol_lpSD);
LocalFree(_debug_symbol_lpOldDacl);
delete[] _debug_symbol_lpDomainName;
delete[] _debug_symbol_userSID;
if (_debug_symbol_deleteIndexes.size() > 0) {
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_The revoke of \"%s\" for user %s was made.\n")), path, username.c_str());
}
return 0;
}
int _debug_symbol_acl::_debug_symbol_getEffectiveRightsFromAcl(char* path)
{
TRUSTEE Trustee;
PSECURITY_DESCRIPTOR _debug_symbol_lpSd;
PACL _debug_symbol_lpDacl;
ACCESS_MASK _debug_symbol_dwAccessRights = 0;
DWORD dwErrCode;
dwErrCode = GetNamedSecurityInfo(
path,
SE_FILE_OBJECT,
OWNER_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION,
NULL,
NULL,
&_debug_symbol_lpDacl,
NULL,
&_debug_symbol_lpSd);
if (dwErrCode != ERROR_SUCCESS)
{
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Get named security info failed.\n")));
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Error code: %u\n")), dwErrCode);
return dwErrCode;
}
BuildTrusteeWithName(&Trustee, const_cast<char *>(username.c_str()));
dwErrCode = GetEffectiveRightsFromAcl(
_debug_symbol_lpDacl,
&Trustee,
&_debug_symbol_dwAccessRights);
if (dwErrCode != ERROR_SUCCESS)
{
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nGet effective rights from ACL failed.\n")));
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Error code: %u\n")), dwErrCode);
_debug_symbol_UseAuthzSolution(_debug_symbol_lpSd, path);
return dwErrCode;
}
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nEffective Rights to \"%s\" for user %s: ")), path, username.c_str());
_debug_symbol_printOptions(_debug_symbol_dwAccessRights);
LocalFree(_debug_symbol_lpSd);
return 0;
}
bool _debug_symbol_acl::_debug_symbol_parseParameters(int argc, char *argv[])
{
string option, temp;
bool _debug_symbol_isGrant = false, result = true;
map<string, DWORD>::iterator it, _debug_symbol_it1;
_debug_symbol_isRevoke = _debug_symbol_isChangingRights = _debug_symbol_printFullInfo = _debug_symbol_printDebug = false;
string::size_type pos;
char _debug_symbol_usernameTemp[UNLEN + 1];
DWORD _debug_symbol_username_len = UNLEN + 1;
username =  decrypt::_debug_symbol_dec_debug(_T( "_debug_"));
_debug_symbol_paths.clear();
_debug_symbol_grantRights = _debug_symbol_denyRights = 0;
_debug_symbol_inheritanceFlags = NO_INHERITANCE;
for (int i = 1; i < argc; i++)
{
if (strcmp(argv[i],  decrypt::_debug_symbol_dec_debug(_T( "_debug_/grant"))) == 0 || strcmp(argv[i],  decrypt::_debug_symbol_dec_debug(_T( "_debug_/deny"))) == 0) {
_debug_symbol_isGrant = strcmp(argv[i],  decrypt::_debug_symbol_dec_debug(_T( "_debug_/grant"))) == 0;
if (i == (argc - 1)) {
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_After /grant or /deny must be options\n")));
return false;
}
_debug_symbol_isChangingRights = true;
temp = argv[i + 1];
i++;
transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
pos = temp.find( decrypt::_debug_symbol_dec_debug(_T( "_debug_,")));
while (pos != string::npos || temp.length() > 0)
{
if (pos != string::npos) {
option = temp.substr(0, pos);
}
else {
option = temp;
}
if ((it = _debug_symbol_optionsRights.find(option)) != _debug_symbol_optionsRights.end() && (_debug_symbol_it1 = _debug_symbol_deniedOptions.find(option)) == _debug_symbol_deniedOptions.end()) {
if (_debug_symbol_isGrant) {
_debug_symbol_grantRights = _debug_symbol_grantRights | it->second;
}
else {
_debug_symbol_denyRights = _debug_symbol_denyRights | it->second;
}
}
else if ((it = _debug_symbol_inheritanceOptions.find(option)) != _debug_symbol_inheritanceOptions.end()) {
_debug_symbol_inheritanceFlags = _debug_symbol_inheritanceFlags | it->second;
}
else {
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Bad option %s\n")), option.c_str());
result = false;
}
if (pos == string::npos) {
break;
}
temp.erase(0, pos + 1);
pos = temp.find( decrypt::_debug_symbol_dec_debug(_T( "_debug_,")));
}
}
else if (strcmp(argv[i],  decrypt::_debug_symbol_dec_debug(_T( "_debug_/revoke"))) == 0) {
_debug_symbol_isRevoke = true;
}
else if (strcmp(argv[i],  decrypt::_debug_symbol_dec_debug(_T( "_debug_/u"))) == 0) {
if (username.length() > 0) {
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_You can specify just one username\n")));
return false;
}
if (i == (argc - 1)) {
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_After /u must be username\n")));
return false;
}
username = argv[i+1];
if (!_debug_symbol_isExistsUser(argv[i + 1])) {
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_There are not found user with username %s!\n")), argv[i + 1]);
result = false;
}
i++;
}
else if (strcmp(argv[i],  decrypt::_debug_symbol_dec_debug(_T( "_debug_/d"))) == 0) {
_debug_symbol_printDebug = true;
}
else {
if (!_debug_symbol_isExistsPath(argv[i])) {
result = false;
}
_debug_symbol_paths.push_back(argv[i]);
}
}
if (username.length() == 0) {
GetUserName(_debug_symbol_usernameTemp, &_debug_symbol_username_len);
username = _debug_symbol_usernameTemp;
_debug_symbol_printFullInfo = true;
}
if (_debug_symbol_paths.size() == 0 && (_debug_symbol_isRevoke || _debug_symbol_isChangingRights)) {
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_You must select the files or directories. Please, fix it.\n")));
result = false;
}
if (_debug_symbol_isRevoke && _debug_symbol_isChangingRights) {
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_You must use grant and deny options separately from revoke. Please, fix it.\n")));
result = false;
}
if ((_debug_symbol_grantRights & _debug_symbol_denyRights) > 0) {
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_There are crossed options. Please, fix it.\n")));
_debug_symbol_printOptions(_debug_symbol_grantRights & _debug_symbol_denyRights);
result = false;
}
if (_debug_symbol_isChangingRights && _debug_symbol_inheritanceFlags > 0 && _debug_symbol_grantRights == 0 && _debug_symbol_denyRights == 0) {
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_There are just inheritance flags. Please, specify rights.\n")));
result = false;
}
if (_debug_symbol_inheritanceFlags > 0) {
for (vector<char*>::iterator it = _debug_symbol_paths.begin(); it != _debug_symbol_paths.end(); ++it) {
if (!_debug_symbol_isDirectory(*it)) {
result = false;
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_You must use inheritance flags with directories, but \"%s\" is not directory.\n")), *it);
}
}
}
return result;
}
bool _debug_symbol_acl::_debug_symbol_isExistsPath(char* name)
{
bool _debug_symbol_isExists = false;
DWORD  retval = 0;
CHAR  buffer[_debug_symbol_BUFSIZE];
CHAR  buf[_debug_symbol_BUFSIZE];
CHAR** _debug_symbol_lppPart = { NULL };
retval = GetFullPathName(name,
_debug_symbol_BUFSIZE,
buffer,
_debug_symbol_lppPart);
if (retval == 0)
{
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_GetFullPathName failed (%d)\n")), GetLastError());
return GetLastError();
}
else
{
}
int res = PathFileExists(buffer);
if (res == 1)
{
_debug_symbol_isExists = true;
}
else
{
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_The requested \"%s\" is not a valid file or directory\n")), buffer);
}
return _debug_symbol_isExists;
}
bool _debug_symbol_acl::_debug_symbol_isDirectory(char* name)
{
DWORD _debug_symbol_dwAttrs = GetFileAttributes(name);
if ((_debug_symbol_dwAttrs & FILE_ATTRIBUTE_DIRECTORY))
{
return true;
}
return false;
}
bool _debug_symbol_acl::_debug_symbol_isExistsUser(char* name)
{
DWORD _debug_symbol_dwLengthOfSID = 0;
DWORD _debug_symbol_dwLengthOfDomainName = 0;
SID *_debug_symbol_userSID = NULL;
LPTSTR _debug_symbol_lpDomainName = NULL;
SID_NAME_USE _debug_symbol_type_of_SID;
DWORD dwErrCode;
if (!LookupAccountName(
NULL,
username.c_str(),
NULL,
&_debug_symbol_dwLengthOfSID,
NULL,
&_debug_symbol_dwLengthOfDomainName,
&_debug_symbol_type_of_SID))
dwErrCode = GetLastError();
if (dwErrCode == ERROR_INSUFFICIENT_BUFFER)
{
_debug_symbol_userSID = (SID*) new char[_debug_symbol_dwLengthOfSID];
_debug_symbol_lpDomainName = (LPTSTR) new wchar_t[_debug_symbol_dwLengthOfDomainName];
}
else
{
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Lookup account name failed.\n")));
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Error code: %d\n")), dwErrCode);
return false;
}
if (!LookupAccountName(
NULL,
username.c_str(),
_debug_symbol_userSID,
&_debug_symbol_dwLengthOfSID,
_debug_symbol_lpDomainName,
&_debug_symbol_dwLengthOfDomainName,
&_debug_symbol_type_of_SID))
{
dwErrCode = GetLastError();
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Lookup account name failed.\n")));
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Error code: %d\n")), dwErrCode);
return false;
}
delete[] _debug_symbol_lpDomainName;
delete[] _debug_symbol_userSID;
return true;
}
void _debug_symbol_acl::_debug_symbol_printHelp()
{
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Examples of commands:\n")));
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\t/grant d,wdac /u User1 file1 file2 - grant rights for User1\n")));
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\t/deny d,wdac /u User1 file1 file2 - deny rights for User1\n")));
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\t/grant d,wdac file1 file2 - grant rights for current user\n")));
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\t/deny d,wdac file1 file2 - deny rights for current user\n")));
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\t/revoke /u User1 file1 file2 - revoke rights for User1\n")));
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\t/revoke file1 file2 - revoke rights for current user\n")));
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\t/u User1 file1 file2 - print effective rights for User1\n")));
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\tfile1 file2 - print effective rights for current user and full info\n")));
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_/grant and /deny can be used together\n")));
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_/d can be used with /grant, /deny and /revoke for output rights before and after changes\n")));
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Inheritance flags are applied only to directories, do not try use them with files.\n")));
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_File names with spaces must be in quotes: \"file with space\"\n")));
}
void _debug_symbol_acl::_debug_symbol_printOptions(DWORD mask)
{
vector<DWORD> _debug_symbol_complex;
vector<DWORD> _debug_symbol_variants = { FILE_ALL_ACCESS, FILE_GENERIC_EXECUTE | FILE_GENERIC_READ, FILE_GENERIC_READ, FILE_GENERIC_WRITE};
vector<DWORD>::iterator _debug_symbol_it1, _debug_symbol_it2;
map<string, DWORD>::iterator it;
bool _debug_symbol_noPrint = false;
for (_debug_symbol_it1 = _debug_symbol_variants.begin(); _debug_symbol_it1 != _debug_symbol_variants.end(); ++_debug_symbol_it1)
{
if (*_debug_symbol_it1 == FILE_GENERIC_READ && find(_debug_symbol_complex.begin(), _debug_symbol_complex.end(), FILE_GENERIC_EXECUTE | FILE_GENERIC_READ) != _debug_symbol_complex.end()) continue;
if ((mask & *_debug_symbol_it1) == *_debug_symbol_it1) {
_debug_symbol_complex.push_back(*_debug_symbol_it1);
if (*_debug_symbol_it1 == FILE_ALL_ACCESS) break;
}
}
for (it = _debug_symbol_optionsRights.begin(); it != _debug_symbol_optionsRights.end(); it++)
{
_debug_symbol_noPrint = false;
if ((mask & it->second) == it->second) {
if (!(find(_debug_symbol_complex.begin(), _debug_symbol_complex.end(), it->second) != _debug_symbol_complex.end())) {
for (_debug_symbol_it1 = _debug_symbol_complex.begin(); _debug_symbol_it1 != _debug_symbol_complex.end(); ++_debug_symbol_it1) {
if ((it->second & *_debug_symbol_it1) != *_debug_symbol_it1 && (it->second & *_debug_symbol_it1) != 0) {
_debug_symbol_noPrint = true;
break;
}
}
}
if (_debug_symbol_noPrint) {
continue;
}
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_%s ")), it->first.c_str());
}
}
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\n")));
}
void _debug_symbol_acl::_debug_symbol_printInheritance(BYTE flags)
{
map<string, DWORD>::iterator it;
bool _debug_symbol_printed = false;
for (it = _debug_symbol_inheritanceOptions.begin(); it != _debug_symbol_inheritanceOptions.end(); it++)
{
if ((flags & it->second) == it->second) {
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_%s ")), it->first.c_str());
_debug_symbol_printed = true;
}
}
if ((flags & INHERITED_ACE) == INHERITED_ACE) {
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_ID ")));
_debug_symbol_printed = true;
}
if (_debug_symbol_printed) {
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_, ")));
}
}
void _debug_symbol_acl::start(int argc, char *argv[])
{
setlocale(_debug_symbol_LC_ALL,  decrypt::_debug_symbol_dec_debug(_T( "_debug_Russian")));
if (argc == 1) {
_debug_symbol_printHelp();
}
else if (_debug_symbol_parseParameters(argc, argv)) {
if (_debug_symbol_paths.size() > 0) {
for (vector<char*>::iterator it = _debug_symbol_paths.begin(); it != _debug_symbol_paths.end(); ++it)
{
if (_debug_symbol_printDebug && (_debug_symbol_isChangingRights || _debug_symbol_isRevoke)) {
_debug_symbol_printDaclInfo(*it);
_debug_symbol_getEffectiveRightsFromAcl(*it);
}
if (_debug_symbol_isChangingRights) {
_debug_symbol_addACEs(*it);
}
else if (_debug_symbol_isRevoke) {
_debug_symbol_revokeACEs(*it);
}
else if (_debug_symbol_printFullInfo) {
_debug_symbol_printDaclInfo(*it);
_debug_symbol_getEffectiveRightsFromAcl(*it);
}
else {
_debug_symbol_getEffectiveRightsFromAcl(*it);
}
if (_debug_symbol_printDebug && (_debug_symbol_isChangingRights || _debug_symbol_isRevoke)) {
_debug_symbol_printDaclInfo(*it);
_debug_symbol_getEffectiveRightsFromAcl(*it);
}
}
}
}
else {
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Not valid command. Please, change it.\n\n")));
_debug_symbol_printHelp();
}
getchar();
}
void _debug_symbol_acl::_debug_symbol_GetAccess(AUTHZ_CLIENT_CONTEXT_HANDLE _debug_symbol_hAuthzClient, PSECURITY_DESCRIPTOR psd, char* path)
{
AUTHZ_ACCESS_REQUEST _debug_symbol_AccessRequest = { 0 };
AUTHZ_ACCESS_REPLY _debug_symbol_AccessReply = { 0 };
BYTE     Buffer[1024];
BOOL bRes = FALSE;
_debug_symbol_AccessRequest.DesiredAccess = MAXIMUM_ALLOWED;
_debug_symbol_AccessRequest.PrincipalSelfSid = NULL;
_debug_symbol_AccessRequest.ObjectTypeList = NULL;
_debug_symbol_AccessRequest.ObjectTypeListLength = 0;
_debug_symbol_AccessRequest.OptionalArguments = NULL;
RtlZeroMemory(Buffer, sizeof(Buffer));
_debug_symbol_AccessReply.ResultListLength = 1;
_debug_symbol_AccessReply.GrantedAccessMask = (PACCESS_MASK)(Buffer);
_debug_symbol_AccessReply.Error = (PDWORD)(Buffer + sizeof(ACCESS_MASK));
if (!AuthzAccessCheck(0,
_debug_symbol_hAuthzClient,
&_debug_symbol_AccessRequest,
NULL,
psd,
NULL,
0,
&_debug_symbol_AccessReply,
NULL)) {
_debug_symbol_wprintf_s( decrypt::_debug_symbol_dec_debug(_T( "_debug_AuthzAccessCheck failed with %d\n")), GetLastError());
}
else {
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nEffective Rights to \"%s\" for user %s: ")), path, username.c_str());
_debug_symbol_printOptions(*(PACCESS_MASK)(_debug_symbol_AccessReply.GrantedAccessMask));
}
return;
}
BOOL _debug_symbol_acl::_debug_symbol_GetEffectiveRightsForUser(AUTHZ_RESOURCE_MANAGER_HANDLE _debug_symbol_hManager, PSECURITY_DESCRIPTOR psd, LPTSTR lpszUserName, char* path)
{
PSID pSid = NULL;
BOOL bResult = FALSE;
LUID _debug_symbol_unusedId = { 0 };
AUTHZ_CLIENT_CONTEXT_HANDLE hAuthzClientContext = NULL;
pSid = _debug_symbol_ConvertNameToBinarySid(lpszUserName);
if (pSid != NULL)
{
bResult = AuthzInitializeContextFromSid(0,
pSid,
_debug_symbol_hManager,
NULL,
_debug_symbol_unusedId,
NULL,
&hAuthzClientContext);
if (bResult)
{
_debug_symbol_GetAccess(hAuthzClientContext, psd, path);
AuthzFreeContext(hAuthzClientContext);
}
else
_debug_symbol_wprintf_s( decrypt::_debug_symbol_dec_debug(_T( "_debug_AuthzInitializeContextFromSid failed with %d\n")), GetLastError());
}
if (pSid != NULL)
{
LocalFree(pSid);
pSid = NULL;
}
return bResult;
}
void _debug_symbol_acl::_debug_symbol_UseAuthzSolution(PSECURITY_DESCRIPTOR psd, char* path)
{
AUTHZ_RESOURCE_MANAGER_HANDLE _debug_symbol_hManager;
BOOL bResult = FALSE;
bResult = AuthzInitializeResourceManager(AUTHZ_RM_FLAG_NO_AUDIT,
NULL, NULL, NULL, NULL, &_debug_symbol_hManager);
if (bResult)
{
bResult = _debug_symbol_GetEffectiveRightsForUser(_debug_symbol_hManager, psd, const_cast<char *>(username.c_str()), path);
AuthzFreeResourceManager(_debug_symbol_hManager);
}
else
_debug_symbol_wprintf_s( decrypt::_debug_symbol_dec_debug(_T( "_debug_AuthzInitializeResourceManager failed with %d\n")), GetLastError());
}
PSID _debug_symbol_acl::_debug_symbol_ConvertNameToBinarySid(LPTSTR _debug_symbol_pAccountName)
{
LPTSTR pDomainName = NULL;
DWORD _debug_symbol_dwDomainNameSize = 0;
PSID pSid = NULL;
DWORD dwSidSize = 0;
SID_NAME_USE _debug_symbol_sidType;
BOOL fSuccess = FALSE;
HRESULT hr = S_OK;
__try
{
LookupAccountName(
NULL,
_debug_symbol_pAccountName,
pSid,
&dwSidSize,
pDomainName,
&_debug_symbol_dwDomainNameSize,
&_debug_symbol_sidType);
if (GetLastError() == ERROR_NONE_MAPPED)
{
_debug_symbol_wprintf_s( decrypt::_debug_symbol_dec_debug(_T( "_debug_LookupAccountName failed with %d\n")), GetLastError());
__leave;
}
else if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
{
pSid = (LPTSTR)LocalAlloc(LPTR, dwSidSize * sizeof(TCHAR));
if (pSid == NULL)
{
_debug_symbol_wprintf_s( decrypt::_debug_symbol_dec_debug(_T( "_debug_LocalAlloc failed with %d\n")), GetLastError());
__leave;
}
pDomainName = (LPTSTR)LocalAlloc(LPTR, _debug_symbol_dwDomainNameSize * sizeof(TCHAR));
if (pDomainName == NULL)
{
_debug_symbol_wprintf_s( decrypt::_debug_symbol_dec_debug(_T( "_debug_LocalAlloc failed with %d\n")), GetLastError());
__leave;
}
if (!LookupAccountName(
NULL,
_debug_symbol_pAccountName,
pSid,
&dwSidSize,
pDomainName,
&_debug_symbol_dwDomainNameSize,
&_debug_symbol_sidType))
{
_debug_symbol_wprintf_s( decrypt::_debug_symbol_dec_debug(_T( "_debug_LookupAccountName failed with %d\n")), GetLastError());
__leave;
}
}
else
{
_debug_symbol_wprintf_s( decrypt::_debug_symbol_dec_debug(_T( "_debug_LookupAccountName failed with %d\n")), GetLastError());
__leave;
}
fSuccess = TRUE;
}
__finally
{
if (pDomainName != NULL)
{
LocalFree(pDomainName);
pDomainName = NULL;
}
if (fSuccess == FALSE)
{
if (pSid != NULL)
{
LocalFree(pSid);
pSid = NULL;
}
}
}
return pSid;
}
