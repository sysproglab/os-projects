#include "stdafx.h"
#include <stdio.h>
#include "acl.h"
#include <windows.h> 
#include <iostream>
#include <algorithm>
#include <bitset>
#define ReplacementFor_BUFSIZE (0x24f5+499-0x16e8)
map<string,DWORD>ReplacementFor_acl::ReplacementFor_optionsRights={



{"\x46",FILE_ALL_ACCESS},{"\x52\x58",FILE_GENERIC_EXECUTE|FILE_GENERIC_READ},{"\x52",FILE_GENERIC_READ},{"\x57",FILE_GENERIC_WRITE},
{"\x47\x41",GENERIC_ALL},{"\x47\x52",GENERIC_READ},{"\x47\x57",GENERIC_WRITE},{"\x47\x45",GENERIC_EXECUTE},{"\x58",FILE_EXECUTE},
{"\x52\x44",FILE_READ_DATA},
{"\x57\x44",FILE_WRITE_DATA},
{"\x41\x44",FILE_APPEND_DATA},
{"\x52\x45\x41",FILE_READ_EA},{"\x57\x45\x41",FILE_WRITE_EA},{"\x52\x41",FILE_READ_ATTRIBUTES},{"\x57\x41",FILE_WRITE_ATTRIBUTES},{"\x44\x43",FILE_DELETE_CHILD},{"\x57\x44\x41\x43",WRITE_DAC},{"\x57\x4f",WRITE_OWNER},{"\x44",DELETE},{"\x53",SYNCHRONIZE},{"\x52\x43",READ_CONTROL},{"\x41\x53",
ACCESS_SYSTEM_SECURITY}};
map<string,DWORD>ReplacementFor_acl::ReplacementFor_deniedOptions={{"\x47\x41",GENERIC_ALL},{"\x47\x52",GENERIC_READ},{"\x47\x57",GENERIC_WRITE},{"\x47\x45",GENERIC_EXECUTE}};
map<string,DWORD>ReplacementFor_acl::ReplacementFor_inheritanceOptions={{"\x4f\x49",OBJECT_INHERIT_ACE},
{"\x43\x49",CONTAINER_INHERIT_ACE},
{"\x49\x4f",INHERIT_ONLY_ACE},
{"\x4e\x50",NO_PROPAGATE_INHERIT_ACE}
};ReplacementFor_acl::ReplacementFor_acl(){}ReplacementFor_acl::~ReplacementFor_acl(){}int ReplacementFor_acl::ReplacementFor_printDaclInfo(char*ReplacementFor_path){PSECURITY_DESCRIPTOR pSecurityDescriptor;
DWORD ReplacementFor_dwRetCode,ReplacementFor_mask;
PACL ReplacementFor_pDacl=NULL;
char lpName[UNLEN],ReplacementFor_lpDomain[UNLEN];string ReplacementFor_typeAce;SID_NAME_USE ReplacementFor_SidType;DWORD dwSize=UNLEN;PSID ReplacementFor_sid;BYTE flags;printf("\n" "\x49\x6e\x66\x6f\x20\x61\x62\x6f\x75\x74\x20" "\"" "\x25\x73" "\"\n",ReplacementFor_path);
ReplacementFor_dwRetCode=GetNamedSecurityInfo(ReplacementFor_path,
SE_FILE_OBJECT,
DACL_SECURITY_INFORMATION,
NULL,
NULL,
&ReplacementFor_pDacl,
NULL,
&pSecurityDescriptor);
if(ReplacementFor_dwRetCode!=ERROR_SUCCESS){printf("\x47\x65\x74\x20\x6e\x61\x6d\x65\x64\x20\x73\x65\x63\x75\x72\x69\x74\x79\x20\x69\x6e\x66\x6f\x20\x66\x61\x69\x6c\x65\x64\x2e" "\n");printf("\x45\x72\x72\x6f\x72\x20\x63\x6f\x64\x65\x3a\x20\x25\x75" "\n",ReplacementFor_dwRetCode);return 
ReplacementFor_dwRetCode;}void*ReplacementFor_lpAce=NULL;
for(unsigned i=(0xd2d+202-0xdf7);i<ReplacementFor_pDacl->AceCount;++i){if(!GetAce(ReplacementFor_pDacl,
i,
&ReplacementFor_lpAce))
{ReplacementFor_dwRetCode=GetLastError();printf("\x47\x65\x74\x20\x61\x63\x65\x20\x66\x61\x69\x6c\x65\x64\x2e" "\n");printf("\x45\x72\x72\x6f\x72\x20\x63\x6f\x64\x65\x3a\x20\x25\x64" "\n",ReplacementFor_dwRetCode);return ReplacementFor_dwRetCode;}if(((ACE_HEADER*)ReplacementFor_lpAce)->AceType==
ACCESS_ALLOWED_ACE_TYPE){ReplacementFor_sid=&((ACCESS_ALLOWED_ACE*)ReplacementFor_lpAce)->SidStart;ReplacementFor_mask=((ACCESS_ALLOWED_ACE*)ReplacementFor_lpAce)->Mask;ReplacementFor_typeAce="\x41\x4c\x4c\x4f\x57\x45\x44";}else{ReplacementFor_sid=&((ACCESS_DENIED_ACE*)ReplacementFor_lpAce)->
SidStart;ReplacementFor_mask=((ACCESS_DENIED_ACE*)ReplacementFor_lpAce)->Mask;ReplacementFor_typeAce="\x44\x45\x4e\x49\x45\x44";}flags=((ACE_HEADER*)ReplacementFor_lpAce)->AceFlags;
dwSize=UNLEN;memset(&lpName[(0xd2d+202-0xdf7)],(0xd2d+202-0xdf7),sizeof(lpName));memset(&ReplacementFor_lpDomain[(0xd2d+202-0xdf7)],(0xd2d+202-0xdf7),sizeof(ReplacementFor_lpDomain));
if(!LookupAccountSid(NULL,ReplacementFor_sid,lpName,&dwSize,ReplacementFor_lpDomain,&dwSize,&ReplacementFor_SidType)){ReplacementFor_dwRetCode=GetLastError();if(ReplacementFor_dwRetCode==ERROR_NONE_MAPPED)strcpy_s(lpName,dwSize,"\x4e\x4f\x4e\x45\x5f\x4d\x41\x50\x50\x45\x44");else{
printf("\x4c\x6f\x6f\x6b\x75\x70\x41\x63\x63\x6f\x75\x6e\x74\x53\x69\x64\x20\x45\x72\x72\x6f\x72\x20\x25\x75" "\n",GetLastError());return ReplacementFor_dwRetCode;}}printf(
"\x46\x6f\x72\x20\x75\x73\x65\x72\x20\x25\x73" "\\" "\x25\x73\x20\x66\x6c\x61\x67\x73\x2c\x20\x25\x73\x20\x72\x69\x67\x68\x74\x73\x3a\x20",ReplacementFor_lpDomain,lpName,ReplacementFor_typeAce.c_str());
ReplacementFor_printInheritance(flags);ReplacementFor_printOptions(ReplacementFor_mask);}
LocalFree(pSecurityDescriptor);return(0xd2d+202-0xdf7);}int ReplacementFor_acl::ReplacementFor_printDaclInfoSDDL(char*ReplacementFor_path){PSECURITY_DESCRIPTOR pSecurityDescriptor;
LPSTR ReplacementFor_StringSecurityDescriptor;
DWORD ReplacementFor_StringSecurityDescriptorLen;
DWORD ReplacementFor_dwRetCode;
PACL ReplacementFor_pDacl=NULL;
PSID ReplacementFor_pSidOwner;
PSID ReplacementFor_pSidGroup;
LPTSTR ReplacementFor_lpStringSid;
printf("\Info about \"%s\"\n",ReplacementFor_path);
ReplacementFor_dwRetCode=GetNamedSecurityInfo(ReplacementFor_path,
SE_FILE_OBJECT,
OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,
&ReplacementFor_pSidOwner,
&ReplacementFor_pSidGroup,
&ReplacementFor_pDacl,
NULL,
&pSecurityDescriptor);
if(ReplacementFor_dwRetCode!=ERROR_SUCCESS){printf("\x47\x65\x74\x20\x6e\x61\x6d\x65\x64\x20\x73\x65\x63\x75\x72\x69\x74\x79\x20\x69\x6e\x66\x6f\x20\x66\x61\x69\x6c\x65\x64\x2e" "\n");printf("\x45\x72\x72\x6f\x72\x20\x63\x6f\x64\x65\x3a\x20\x25\x75" "\n",ReplacementFor_dwRetCode);return 
ReplacementFor_dwRetCode;}
if(!ConvertSecurityDescriptorToStringSecurityDescriptor(pSecurityDescriptor,
SDDL_REVISION_1,
OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION|SACL_SECURITY_INFORMATION,&ReplacementFor_StringSecurityDescriptor,
&ReplacementFor_StringSecurityDescriptorLen))
{ReplacementFor_dwRetCode=GetLastError();printf(
"\x43\x6f\x6e\x76\x65\x72\x74\x20\x73\x65\x63\x75\x72\x69\x74\x79\x20\x64\x65\x73\x63\x72\x69\x70\x74\x6f\x72\x20\x74\x6f\x20\x73\x74\x72\x69\x6e\x67\x20\x73\x65\x63\x75\x72\x69\x74\x79\x20\x64\x65\x73\x63\x72\x69\x70\x74\x6f\x72\x20\x66\x61\x69\x6c\x65\x64\x2e\x20\x45\x72\x72\x6f\x72\x20\x63\x6f\x64\x65\x3a\x20\x25\x75" "\n"
,ReplacementFor_dwRetCode);return ReplacementFor_dwRetCode;}


string ReplacementFor_result=ReplacementFor_StringSecurityDescriptor;string::size_type ReplacementFor_pos=ReplacementFor_result.find("\x3a");while(ReplacementFor_pos!=string::npos){ReplacementFor_result.insert(ReplacementFor_pos-(0xd26+209-0xdf6),"\n");ReplacementFor_result.insert(ReplacementFor_pos
+(0xd1f+216-0xdf5),"\n\t");ReplacementFor_pos=ReplacementFor_result.find("\x3a",ReplacementFor_pos+(0xd1f+216-0xdf5));}ReplacementFor_pos=ReplacementFor_result.find("\x29");while(ReplacementFor_pos!=string::npos){ReplacementFor_result.insert(ReplacementFor_pos+(0xd26+209-0xdf6),"\n\t");
ReplacementFor_pos=ReplacementFor_result.find("\x29",ReplacementFor_pos+(0xd26+209-0xdf6));}printf("\x53\x74\x72\x69\x6e\x67\x20\x73\x65\x63\x75\x72\x69\x74\x79\x20\x64\x65\x73\x72\x69\x70\x74\x6f\x72\x3a\x20\x25\x73" "\n",ReplacementFor_result.c_str());char lpName[UNLEN],ReplacementFor_lpDomain[
UNLEN];;SID_NAME_USE ReplacementFor_SidType;DWORD dwSize=UNLEN;PSID ReplacementFor_sid;
if(!LookupAccountSid(NULL,ReplacementFor_pSidOwner,lpName,&dwSize,ReplacementFor_lpDomain,&dwSize,&ReplacementFor_SidType)){ReplacementFor_dwRetCode=GetLastError();if(ReplacementFor_dwRetCode==ERROR_NONE_MAPPED)strcpy_s(lpName,dwSize,"\x4e\x4f\x4e\x45\x5f\x4d\x41\x50\x50\x45\x44");
else{printf("\x4c\x6f\x6f\x6b\x75\x70\x41\x63\x63\x6f\x75\x6e\x74\x53\x69\x64\x20\x45\x72\x72\x6f\x72\x20\x25\x75" "\n",GetLastError());return ReplacementFor_dwRetCode;}}printf(
"\x4f\x77\x6e\x65\x72\x20\x69\x73\x20\x61\x20\x6d\x65\x6d\x62\x65\x72\x20\x6f\x66\x20\x74\x68\x65\x20\x25\x73" "\\" "\x25\x73\x20\x67\x72\x6f\x75\x70" "\n",ReplacementFor_lpDomain,lpName);
LocalFree(pSecurityDescriptor);LocalFree(ReplacementFor_StringSecurityDescriptor);return(0xd2d+202-0xdf7);}int ReplacementFor_acl::ReplacementFor_addACEs(char*ReplacementFor_path){EXPLICIT_ACCESS ReplacementFor_ea;
PACL ReplacementFor_lpOldDacl;
PACL ReplacementFor_lpNewDacl;
PSECURITY_DESCRIPTOR ReplacementFor_lpSD;
DWORD ReplacementFor_dwErrCode;
EXPLICIT_ACCESS ReplacementFor_s[(0xd1f+216-0xdf5)];
int ReplacementFor_countAddedACEs=(0xd2d+202-0xdf7);
ReplacementFor_dwErrCode=GetNamedSecurityInfo(ReplacementFor_path,
SE_FILE_OBJECT,
DACL_SECURITY_INFORMATION,
NULL,
NULL,
&ReplacementFor_lpOldDacl,
NULL,
&ReplacementFor_lpSD);
if(ReplacementFor_dwErrCode!=ERROR_SUCCESS){printf("\x47\x65\x74\x20\x6e\x61\x6d\x65\x64\x20\x73\x65\x63\x75\x72\x69\x74\x79\x20\x69\x6e\x66\x6f\x20\x66\x61\x69\x6c\x65\x64\x2e" "\n");printf("\x45\x72\x72\x6f\x72\x20\x63\x6f\x64\x65\x3a\x20\x25\x75" "\n",ReplacementFor_dwErrCode);return 
ReplacementFor_dwErrCode;}ReplacementFor_dwErrCode=ReplacementFor_cleanDACL(ReplacementFor_lpOldDacl);if(ReplacementFor_dwErrCode!=ERROR_SUCCESS){perror("\x43\x6c\x65\x61\x6e\x69\x6e\x67\x20\x44\x41\x43\x4c\x20\x66\x61\x69\x6c\x65\x64\x2e" "\n");printf(
"\x54\x68\x65\x20\x6c\x61\x73\x74\x20\x65\x72\x72\x6f\x72\x20\x63\x6f\x64\x65\x3a\x20\x25\x75" "\n",ReplacementFor_dwErrCode);return ReplacementFor_dwErrCode;}
if(ReplacementFor_grantRights>(0xd2d+202-0xdf7)){BuildExplicitAccessWithName(&ReplacementFor_ea,
const_cast<char*>(ReplacementFor_username.c_str()),
ReplacementFor_grantRights,
GRANT_ACCESS,
ReplacementFor_inheritanceFlags
);ReplacementFor_s[ReplacementFor_countAddedACEs]=ReplacementFor_ea;ReplacementFor_countAddedACEs++;}if(ReplacementFor_denyRights>(0xd2d+202-0xdf7)){BuildExplicitAccessWithName(&ReplacementFor_ea,
const_cast<char*>(ReplacementFor_username.c_str()),
ReplacementFor_denyRights,
DENY_ACCESS,
ReplacementFor_inheritanceFlags
);ReplacementFor_s[ReplacementFor_countAddedACEs]=ReplacementFor_ea;ReplacementFor_countAddedACEs++;}
ReplacementFor_dwErrCode=SetEntriesInAcl(ReplacementFor_countAddedACEs,
ReplacementFor_s,
ReplacementFor_lpOldDacl,
&ReplacementFor_lpNewDacl);
if(ReplacementFor_dwErrCode!=ERROR_SUCCESS){perror("\x53\x65\x74\x20\x65\x6e\x74\x72\x69\x65\x73\x20\x69\x6e\x20\x44\x41\x43\x4c\x20\x66\x61\x69\x6c\x65\x64\x2e" "\n");printf("\x54\x68\x65\x20\x6c\x61\x73\x74\x20\x65\x72\x72\x6f\x72\x20\x63\x6f\x64\x65\x3a\x20\x25\x75" "\n",ReplacementFor_dwErrCode)
;return ReplacementFor_dwErrCode;}
ReplacementFor_dwErrCode=SetNamedSecurityInfo(ReplacementFor_path,
SE_FILE_OBJECT,
DACL_SECURITY_INFORMATION,
NULL,
NULL,
ReplacementFor_lpNewDacl,
NULL);
if(ReplacementFor_dwErrCode!=ERROR_SUCCESS){printf("\x53\x65\x74\x20\x6e\x61\x6d\x65\x64\x20\x73\x65\x63\x75\x72\x69\x74\x79\x20\x69\x6e\x66\x6f\x20\x66\x61\x69\x6c\x65\x64\x2e" "\n");printf("\x45\x72\x72\x6f\x72\x20\x63\x6f\x64\x65\x3a\x20\x25\x75" "\n",ReplacementFor_dwErrCode);return 
ReplacementFor_dwErrCode;}
LocalFree(ReplacementFor_lpSD);LocalFree(ReplacementFor_lpNewDacl);if(ReplacementFor_grantRights>(0xd2d+202-0xdf7)||ReplacementFor_denyRights>(0xd2d+202-0xdf7)){printf(
"\x54\x68\x65\x20\x72\x69\x67\x68\x74\x73\x20\x66\x6f\x72\x20\x75\x73\x65\x72\x20\x25\x73\x20\x74\x6f\x20" "\"" "\x25\x73" "\"" "\x20\x77\x65\x72\x65\x20\x6d\x6f\x64\x69\x66\x69\x65\x64\x2e" "\n",ReplacementFor_username.c_str(),ReplacementFor_path);}else{printf(
"\x54\x68\x65\x20\x72\x69\x67\x68\x74\x73\x20\x66\x6f\x72\x20\x75\x73\x65\x72\x20\x25\x73\x20\x74\x6f\x20" "\"" "\x25\x73" "\"" "\x20\x68\x61\x76\x65\x20\x6e\x6f\x74\x20\x62\x65\x65\x6e\x20\x6d\x6f\x64\x69\x66\x69\x65\x64\x2e" "\n",ReplacementFor_username.c_str(),ReplacementFor_path);}return
(0xd2d+202-0xdf7);}int ReplacementFor_acl::ReplacementFor_cleanDACL(PACL ReplacementFor_lpOldDacl){void*ReplacementFor_lpAce=NULL;
char lpName[UNLEN],ReplacementFor_lpDomain[UNLEN];SID_NAME_USE ReplacementFor_SidType;DWORD dwSize=UNLEN,ReplacementFor_dwErrCode,ReplacementFor_rights,flags;DWORD*ReplacementFor_mask;PSID ReplacementFor_sid;vector<int>ReplacementFor_deleteIndex;
vector<int>::reverse_iterator ReplacementFor_it;
for(unsigned i=(0xd2d+202-0xdf7);i<ReplacementFor_lpOldDacl->AceCount;++i){
if(!GetAce(ReplacementFor_lpOldDacl,
i,
&ReplacementFor_lpAce))
{ReplacementFor_dwErrCode=GetLastError();printf("Get асе failed.\n");printf("\x45\x72\x72\x6f\x72\x20\x63\x6f\x64\x65\x3a\x20\x25\x64" "\n",ReplacementFor_dwErrCode);return ReplacementFor_dwErrCode;}flags=((ACE_HEADER*)ReplacementFor_lpAce)->AceFlags;if(((ACE_HEADER*)ReplacementFor_lpAce)->AceType==
ACCESS_ALLOWED_ACE_TYPE){if(ReplacementFor_denyRights==(0xd2d+202-0xdf7))continue;ReplacementFor_sid=&((ACCESS_ALLOWED_ACE*)ReplacementFor_lpAce)->SidStart;ReplacementFor_mask=&((ACCESS_ALLOWED_ACE*)ReplacementFor_lpAce)->Mask;
ReplacementFor_rights=ReplacementFor_denyRights;}else{if(ReplacementFor_grantRights==(0xd2d+202-0xdf7))continue;ReplacementFor_sid=&((ACCESS_DENIED_ACE*)ReplacementFor_lpAce)->SidStart;ReplacementFor_mask=&((ACCESS_DENIED_ACE*)ReplacementFor_lpAce)->Mask;
ReplacementFor_rights=ReplacementFor_grantRights;}
dwSize=UNLEN;memset(&lpName[(0xd2d+202-0xdf7)],(0xd2d+202-0xdf7),sizeof(lpName));memset(&ReplacementFor_lpDomain[(0xd2d+202-0xdf7)],(0xd2d+202-0xdf7),sizeof(ReplacementFor_lpDomain));if(!LookupAccountSid(NULL,ReplacementFor_sid,lpName,&dwSize,ReplacementFor_lpDomain,&dwSize,&ReplacementFor_SidType))
{ReplacementFor_dwErrCode=GetLastError();if(ReplacementFor_dwErrCode==ERROR_NONE_MAPPED)strcpy_s(lpName,dwSize,"\x4e\x4f\x4e\x45\x5f\x4d\x41\x50\x50\x45\x44");else{printf("\x4c\x6f\x6f\x6b\x75\x70\x41\x63\x63\x6f\x75\x6e\x74\x53\x69\x64\x20\x45\x72\x72\x6f\x72\x20\x25\x75" "\n",
GetLastError());
return ReplacementFor_dwErrCode;}}
if(strcmp(lpName,ReplacementFor_username.c_str())!=(0xd2d+202-0xdf7)){continue;}
if((flags&INHERITED_ACE)==INHERITED_ACE&&((*ReplacementFor_mask&~ReplacementFor_rights)==(0xd2d+202-0xdf7)||(*ReplacementFor_mask&~ReplacementFor_rights)!=*ReplacementFor_mask)){printf(
"\x53\x6f\x72\x72\x79\x2c\x20\x62\x75\x74\x20\x66\x6f\x75\x6e\x64\x20\x70\x72\x6f\x62\x6c\x65\x6d\x20\x77\x69\x74\x68\x20\x69\x6e\x68\x65\x72\x69\x74\x65\x64\x20\x41\x43\x45\x20\x28\x63\x72\x6f\x73\x73\x65\x64\x20\x72\x69\x67\x68\x74\x73\x29\x2e\x20\x54\x72\x79\x20\x61\x74\x20\x66\x69\x72\x73\x74\x20\x74\x6f\x20\x63\x68\x61\x6e\x67\x65\x20\x72\x69\x67\x68\x74\x73\x20\x6f\x66\x20\x70\x61\x72\x65\x6e\x74\x20\x64\x69\x72\x65\x63\x74\x6f\x72\x69\x65\x73\x2e" "\n"
);return-(0xd26+209-0xdf6);}if((*ReplacementFor_mask&~ReplacementFor_rights)==(0xd2d+202-0xdf7)){ReplacementFor_deleteIndex.push_back(i);}else if((*ReplacementFor_mask&~ReplacementFor_rights)!=*ReplacementFor_mask){
*ReplacementFor_mask=(*ReplacementFor_mask&~ReplacementFor_rights);
}}for(ReplacementFor_it=ReplacementFor_deleteIndex.rbegin();ReplacementFor_it!=ReplacementFor_deleteIndex.rend();++ReplacementFor_it){
if(!DeleteAce(ReplacementFor_lpOldDacl,*ReplacementFor_it)){ReplacementFor_dwErrCode=GetLastError();printf("\x44\x65\x6c\x65\x74\x65\x20\x61\x63\x65\x20\x66\x61\x69\x6c\x65\x64\x2e" "\n");printf("\x45\x72\x72\x6f\x72\x20\x63\x6f\x64\x65\x3a\x20\x25\x64" "\n",ReplacementFor_dwErrCode);return 
ReplacementFor_dwErrCode;}
}return(0xd2d+202-0xdf7);}int ReplacementFor_acl::ReplacementFor_revokeACEs(char*ReplacementFor_path){EXPLICIT_ACCESS ReplacementFor_ea;
PACL ReplacementFor_lpOldDacl;
PSECURITY_DESCRIPTOR ReplacementFor_lpSD;
DWORD ReplacementFor_dwErrCode;
vector<int>ReplacementFor_deleteIndexes;
vector<int>::reverse_iterator ReplacementFor_it;DWORD ReplacementFor_dwLengthOfSID=(0xd2d+202-0xdf7);
DWORD ReplacementFor_dwLengthOfDomainName=(0xd2d+202-0xdf7);
SID*ReplacementFor_userSID=NULL;
LPTSTR ReplacementFor_lpDomainName=NULL;
SID_NAME_USE ReplacementFor_type_of_SID;
void*ReplacementFor_lpAce=NULL;
DWORD type;
PSID ReplacementFor_sid;

if(!LookupAccountName(NULL,
ReplacementFor_username.c_str(),
NULL,
&ReplacementFor_dwLengthOfSID,
NULL,
&ReplacementFor_dwLengthOfDomainName,
&ReplacementFor_type_of_SID))
ReplacementFor_dwErrCode=GetLastError();if(ReplacementFor_dwErrCode==ERROR_INSUFFICIENT_BUFFER){
ReplacementFor_userSID=(SID*)new char[ReplacementFor_dwLengthOfSID];ReplacementFor_lpDomainName=(LPTSTR)new wchar_t[ReplacementFor_dwLengthOfDomainName];}else{
printf("\x4c\x6f\x6f\x6b\x75\x70\x20\x61\x63\x63\x6f\x75\x6e\x74\x20\x6e\x61\x6d\x65\x20\x66\x61\x69\x6c\x65\x64\x2e" "\n");printf("\x45\x72\x72\x6f\x72\x20\x63\x6f\x64\x65\x3a\x20\x25\x64" "\n",ReplacementFor_dwErrCode);return ReplacementFor_dwErrCode;}
if(!LookupAccountName(NULL,
ReplacementFor_username.c_str(),
ReplacementFor_userSID,
&ReplacementFor_dwLengthOfSID,
ReplacementFor_lpDomainName,
&ReplacementFor_dwLengthOfDomainName,
&ReplacementFor_type_of_SID))
{ReplacementFor_dwErrCode=GetLastError();printf("\x4c\x6f\x6f\x6b\x75\x70\x20\x61\x63\x63\x6f\x75\x6e\x74\x20\x6e\x61\x6d\x65\x20\x66\x61\x69\x6c\x65\x64\x2e" "\n");printf("\x45\x72\x72\x6f\x72\x20\x63\x6f\x64\x65\x3a\x20\x25\x64" "\n",ReplacementFor_dwErrCode);return ReplacementFor_dwErrCode;}














ReplacementFor_dwErrCode=GetNamedSecurityInfo(ReplacementFor_path,
SE_FILE_OBJECT,
DACL_SECURITY_INFORMATION,
NULL,
NULL,
&ReplacementFor_lpOldDacl,
NULL,
&ReplacementFor_lpSD);
if(ReplacementFor_dwErrCode!=ERROR_SUCCESS){printf("\x47\x65\x74\x20\x6e\x61\x6d\x65\x64\x20\x73\x65\x63\x75\x72\x69\x74\x79\x20\x69\x6e\x66\x6f\x20\x66\x61\x69\x6c\x65\x64\x2e" "\n");printf("\x45\x72\x72\x6f\x72\x20\x63\x6f\x64\x65\x3a\x20\x25\x75" "\n",ReplacementFor_dwErrCode);return 
ReplacementFor_dwErrCode;}
for(unsigned i=(0xd2d+202-0xdf7);i<ReplacementFor_lpOldDacl->AceCount;++i){
if(!GetAce(ReplacementFor_lpOldDacl,
i,
&ReplacementFor_lpAce))
{ReplacementFor_dwErrCode=GetLastError();printf("Get асе failed.\n");printf("\x45\x72\x72\x6f\x72\x20\x63\x6f\x64\x65\x3a\x20\x25\x64" "\n",ReplacementFor_dwErrCode);return ReplacementFor_dwErrCode;}type=((ACE_HEADER*)ReplacementFor_lpAce)->AceType;if(type==ACCESS_ALLOWED_ACE_TYPE){
ReplacementFor_sid=&((ACCESS_ALLOWED_ACE*)ReplacementFor_lpAce)->SidStart;}else if(type==ACCESS_DENIED_ACE_TYPE){ReplacementFor_sid=&((ACCESS_DENIED_ACE*)ReplacementFor_lpAce)->SidStart;}else{continue;}if(!EqualSid(ReplacementFor_userSID,ReplacementFor_sid)){continue;}
ReplacementFor_deleteIndexes.push_back(i);}for(ReplacementFor_it=ReplacementFor_deleteIndexes.rbegin();ReplacementFor_it!=ReplacementFor_deleteIndexes.rend();++ReplacementFor_it){
if(!DeleteAce(ReplacementFor_lpOldDacl,*ReplacementFor_it)){ReplacementFor_dwErrCode=GetLastError();printf("\x44\x65\x6c\x65\x74\x65\x20\x61\x63\x65\x20\x66\x61\x69\x6c\x65\x64\x2e" "\n");printf("\x45\x72\x72\x6f\x72\x20\x63\x6f\x64\x65\x3a\x20\x25\x64" "\n",ReplacementFor_dwErrCode);return 
ReplacementFor_dwErrCode;}
}
ReplacementFor_dwErrCode=SetNamedSecurityInfo(ReplacementFor_path,
SE_FILE_OBJECT,
DACL_SECURITY_INFORMATION,
NULL,
NULL,
ReplacementFor_lpOldDacl,
NULL);
if(ReplacementFor_dwErrCode!=ERROR_SUCCESS){printf("\x53\x65\x74\x20\x6e\x61\x6d\x65\x64\x20\x73\x65\x63\x75\x72\x69\x74\x79\x20\x69\x6e\x66\x6f\x20\x66\x61\x69\x6c\x65\x64\x2e" "\n");printf("\x45\x72\x72\x6f\x72\x20\x63\x6f\x64\x65\x3a\x20\x25\x75" "\n",ReplacementFor_dwErrCode);return 
ReplacementFor_dwErrCode;}
LocalFree(ReplacementFor_lpSD);LocalFree(ReplacementFor_lpOldDacl);delete[]ReplacementFor_lpDomainName;delete[]ReplacementFor_userSID;if(ReplacementFor_deleteIndexes.size()>(0xd2d+202-0xdf7)){printf(
"\x54\x68\x65\x20\x72\x65\x76\x6f\x6b\x65\x20\x6f\x66\x20" "\"" "\x25\x73" "\"" "\x20\x66\x6f\x72\x20\x75\x73\x65\x72\x20\x25\x73\x20\x77\x61\x73\x20\x6d\x61\x64\x65\x2e" "\n",ReplacementFor_path,ReplacementFor_username.c_str());}return(0xd2d+202-0xdf7);}int ReplacementFor_acl::
ReplacementFor_getEffectiveRightsFromAcl(char*ReplacementFor_path){TRUSTEE Trustee;
PSECURITY_DESCRIPTOR ReplacementFor_lpSd;
PACL ReplacementFor_lpDacl;
ACCESS_MASK ReplacementFor_dwAccessRights=(0xd2d+202-0xdf7);
DWORD ReplacementFor_dwErrCode;
ReplacementFor_dwErrCode=GetNamedSecurityInfo(ReplacementFor_path,
SE_FILE_OBJECT,
OWNER_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,
NULL,
NULL,
&ReplacementFor_lpDacl,
NULL,
&ReplacementFor_lpSd);
if(ReplacementFor_dwErrCode!=ERROR_SUCCESS){printf("\x47\x65\x74\x20\x6e\x61\x6d\x65\x64\x20\x73\x65\x63\x75\x72\x69\x74\x79\x20\x69\x6e\x66\x6f\x20\x66\x61\x69\x6c\x65\x64\x2e" "\n");printf("\x45\x72\x72\x6f\x72\x20\x63\x6f\x64\x65\x3a\x20\x25\x75" "\n",ReplacementFor_dwErrCode);return 
ReplacementFor_dwErrCode;}
BuildTrusteeWithName(&Trustee,const_cast<char*>(ReplacementFor_username.c_str()));
ReplacementFor_dwErrCode=GetEffectiveRightsFromAcl(ReplacementFor_lpDacl,
&Trustee,
&ReplacementFor_dwAccessRights);
if(ReplacementFor_dwErrCode!=ERROR_SUCCESS){printf("\n" "\x47\x65\x74\x20\x65\x66\x66\x65\x63\x74\x69\x76\x65\x20\x72\x69\x67\x68\x74\x73\x20\x66\x72\x6f\x6d\x20\x41\x43\x4c\x20\x66\x61\x69\x6c\x65\x64\x2e" "\n");printf("\x45\x72\x72\x6f\x72\x20\x63\x6f\x64\x65\x3a\x20\x25\x75" "\n",
ReplacementFor_dwErrCode);
ReplacementFor_UseAuthzSolution(ReplacementFor_lpSd,ReplacementFor_path);return ReplacementFor_dwErrCode;}

printf("\n" "\x45\x66\x66\x65\x63\x74\x69\x76\x65\x20\x52\x69\x67\x68\x74\x73\x20\x74\x6f\x20" "\"" "\x25\x73" "\"" "\x20\x66\x6f\x72\x20\x75\x73\x65\x72\x20\x25\x73\x3a\x20",ReplacementFor_path,ReplacementFor_username.c_str());ReplacementFor_printOptions(ReplacementFor_dwAccessRights);
LocalFree(ReplacementFor_lpSd);return(0xd2d+202-0xdf7);}bool ReplacementFor_acl::ReplacementFor_parseParameters(int ReplacementFor_argc,char*ReplacementFor_argv[]){string option,ReplacementFor_temp;bool ReplacementFor_isGrant=false,ReplacementFor_result=true;map<string,DWORD>::iterator 
ReplacementFor_it,ReplacementFor_it1;ReplacementFor_isRevoke=ReplacementFor_isChangingRights=ReplacementFor_printFullInfo=ReplacementFor_printDebug=false;string::size_type ReplacementFor_pos;char ReplacementFor_usernameTemp[UNLEN+(0xd26+209-0xdf6)];DWORD ReplacementFor_username_len=UNLEN+
(0xd26+209-0xdf6);ReplacementFor_username="";ReplacementFor_paths.clear();ReplacementFor_grantRights=ReplacementFor_denyRights=(0xd2d+202-0xdf7);ReplacementFor_inheritanceFlags=NO_INHERITANCE;
for(int i=(0xd26+209-0xdf6);i<ReplacementFor_argc;i++){

if(strcmp(ReplacementFor_argv[i],"\x2f\x67\x72\x61\x6e\x74")==(0xd2d+202-0xdf7)||strcmp(ReplacementFor_argv[i],"\x2f\x64\x65\x6e\x79")==(0xd2d+202-0xdf7)){ReplacementFor_isGrant=strcmp(ReplacementFor_argv[i],"\x2f\x67\x72\x61\x6e\x74")==(0xd2d+202-0xdf7);if(i==(ReplacementFor_argc-(0xd26+209-0xdf6))
){printf("\x41\x66\x74\x65\x72\x20\x2f\x67\x72\x61\x6e\x74\x20\x6f\x72\x20\x2f\x64\x65\x6e\x79\x20\x6d\x75\x73\x74\x20\x62\x65\x20\x6f\x70\x74\x69\x6f\x6e\x73" "\n");return false;}ReplacementFor_isChangingRights=true;ReplacementFor_temp=ReplacementFor_argv[i+(0xd26+209-0xdf6)];i++;transform(
ReplacementFor_temp.begin(),ReplacementFor_temp.end(),ReplacementFor_temp.begin(),::toupper);ReplacementFor_pos=ReplacementFor_temp.find("\x2c");while(ReplacementFor_pos!=string::npos||ReplacementFor_temp.length()>(0xd2d+202-0xdf7)){if(ReplacementFor_pos!=string::npos){option=ReplacementFor_temp.
substr((0xd2d+202-0xdf7),ReplacementFor_pos);}else{option=ReplacementFor_temp;}
if((ReplacementFor_it=ReplacementFor_optionsRights.find(option))!=ReplacementFor_optionsRights.end()&&(ReplacementFor_it1=ReplacementFor_deniedOptions.find(option))==ReplacementFor_deniedOptions.end()){if(ReplacementFor_isGrant){ReplacementFor_grantRights=ReplacementFor_grantRights|ReplacementFor_it
->second;}else{ReplacementFor_denyRights=ReplacementFor_denyRights|ReplacementFor_it->second;}}else if((ReplacementFor_it=ReplacementFor_inheritanceOptions.find(option))!=ReplacementFor_inheritanceOptions.end()){ReplacementFor_inheritanceFlags=ReplacementFor_inheritanceFlags|ReplacementFor_it->
second;}else{printf("\x42\x61\x64\x20\x6f\x70\x74\x69\x6f\x6e\x20\x25\x73" "\n",option.c_str());ReplacementFor_result=false;}if(ReplacementFor_pos==string::npos){break;}ReplacementFor_temp.erase((0xd2d+202-0xdf7),ReplacementFor_pos+(0xd26+209-0xdf6));ReplacementFor_pos=ReplacementFor_temp.find(
"\x2c");}}
else if(strcmp(ReplacementFor_argv[i],"\x2f\x72\x65\x76\x6f\x6b\x65")==(0xd2d+202-0xdf7)){ReplacementFor_isRevoke=true;}
else if(strcmp(ReplacementFor_argv[i],"\x2f\x75")==(0xd2d+202-0xdf7)){if(ReplacementFor_username.length()>(0xd2d+202-0xdf7)){printf("\x59\x6f\x75\x20\x63\x61\x6e\x20\x73\x70\x65\x63\x69\x66\x79\x20\x6a\x75\x73\x74\x20\x6f\x6e\x65\x20\x75\x73\x65\x72\x6e\x61\x6d\x65" "\n");return false;}if(i==(
ReplacementFor_argc-(0xd26+209-0xdf6))){printf("\x41\x66\x74\x65\x72\x20\x2f\x75\x20\x6d\x75\x73\x74\x20\x62\x65\x20\x75\x73\x65\x72\x6e\x61\x6d\x65" "\n");return false;}ReplacementFor_username=ReplacementFor_argv[i+(0xd26+209-0xdf6)];if(!ReplacementFor_isExistsUser(ReplacementFor_argv[i+
(0xd26+209-0xdf6)])){printf("\x54\x68\x65\x72\x65\x20\x61\x72\x65\x20\x6e\x6f\x74\x20\x66\x6f\x75\x6e\x64\x20\x75\x73\x65\x72\x20\x77\x69\x74\x68\x20\x75\x73\x65\x72\x6e\x61\x6d\x65\x20\x25\x73\x21" "\n",ReplacementFor_argv[i+(0xd26+209-0xdf6)]);ReplacementFor_result=false;}i++;}
else if(strcmp(ReplacementFor_argv[i],"\x2f\x64")==(0xd2d+202-0xdf7)){ReplacementFor_printDebug=true;}else{if(!ReplacementFor_isExistsPath(ReplacementFor_argv[i])){ReplacementFor_result=false;}ReplacementFor_paths.push_back(ReplacementFor_argv[i]);}}if(ReplacementFor_username.length()==
(0xd2d+202-0xdf7)){GetUserName(ReplacementFor_usernameTemp,&ReplacementFor_username_len);ReplacementFor_username=ReplacementFor_usernameTemp;ReplacementFor_printFullInfo=true;}





if(ReplacementFor_paths.size()==(0xd2d+202-0xdf7)&&(ReplacementFor_isRevoke||ReplacementFor_isChangingRights)){printf(
"\x59\x6f\x75\x20\x6d\x75\x73\x74\x20\x73\x65\x6c\x65\x63\x74\x20\x74\x68\x65\x20\x66\x69\x6c\x65\x73\x20\x6f\x72\x20\x64\x69\x72\x65\x63\x74\x6f\x72\x69\x65\x73\x2e\x20\x50\x6c\x65\x61\x73\x65\x2c\x20\x66\x69\x78\x20\x69\x74\x2e" "\n");ReplacementFor_result=false;}if(ReplacementFor_isRevoke&&
ReplacementFor_isChangingRights){printf(
"\x59\x6f\x75\x20\x6d\x75\x73\x74\x20\x75\x73\x65\x20\x67\x72\x61\x6e\x74\x20\x61\x6e\x64\x20\x64\x65\x6e\x79\x20\x6f\x70\x74\x69\x6f\x6e\x73\x20\x73\x65\x70\x61\x72\x61\x74\x65\x6c\x79\x20\x66\x72\x6f\x6d\x20\x72\x65\x76\x6f\x6b\x65\x2e\x20\x50\x6c\x65\x61\x73\x65\x2c\x20\x66\x69\x78\x20\x69\x74\x2e" "\n"
);ReplacementFor_result=false;}if((ReplacementFor_grantRights&ReplacementFor_denyRights)>(0xd2d+202-0xdf7)){printf("\x54\x68\x65\x72\x65\x20\x61\x72\x65\x20\x63\x72\x6f\x73\x73\x65\x64\x20\x6f\x70\x74\x69\x6f\x6e\x73\x2e\x20\x50\x6c\x65\x61\x73\x65\x2c\x20\x66\x69\x78\x20\x69\x74\x2e" "\n");
ReplacementFor_printOptions(ReplacementFor_grantRights&ReplacementFor_denyRights);ReplacementFor_result=false;}if(ReplacementFor_isChangingRights&&ReplacementFor_inheritanceFlags>(0xd2d+202-0xdf7)&&ReplacementFor_grantRights==(0xd2d+202-0xdf7)&&ReplacementFor_denyRights==(0xd2d+202-0xdf7)){printf(
"\x54\x68\x65\x72\x65\x20\x61\x72\x65\x20\x6a\x75\x73\x74\x20\x69\x6e\x68\x65\x72\x69\x74\x61\x6e\x63\x65\x20\x66\x6c\x61\x67\x73\x2e\x20\x50\x6c\x65\x61\x73\x65\x2c\x20\x73\x70\x65\x63\x69\x66\x79\x20\x72\x69\x67\x68\x74\x73\x2e" "\n");ReplacementFor_result=false;}if(ReplacementFor_inheritanceFlags
>(0xd2d+202-0xdf7)){for(vector<char*>::iterator ReplacementFor_it=ReplacementFor_paths.begin();ReplacementFor_it!=ReplacementFor_paths.end();++ReplacementFor_it){if(!ReplacementFor_isDirectory(*ReplacementFor_it)){ReplacementFor_result=false;printf(
"\x59\x6f\x75\x20\x6d\x75\x73\x74\x20\x75\x73\x65\x20\x69\x6e\x68\x65\x72\x69\x74\x61\x6e\x63\x65\x20\x66\x6c\x61\x67\x73\x20\x77\x69\x74\x68\x20\x64\x69\x72\x65\x63\x74\x6f\x72\x69\x65\x73\x2c\x20\x62\x75\x74\x20" "\"" "\x25\x73" "\"" "\x20\x69\x73\x20\x6e\x6f\x74\x20\x64\x69\x72\x65\x63\x74\x6f\x72\x79\x2e" "\n"
,*ReplacementFor_it);}}}return ReplacementFor_result;}bool ReplacementFor_acl::ReplacementFor_isExistsPath(char*name){bool ReplacementFor_isExists=false;DWORD retval=(0xd2d+202-0xdf7);CHAR ReplacementFor_buffer[ReplacementFor_BUFSIZE];CHAR buf[ReplacementFor_BUFSIZE];CHAR**ReplacementFor_lppPart={
NULL};
retval=GetFullPathName(name,ReplacementFor_BUFSIZE,ReplacementFor_buffer,ReplacementFor_lppPart);if(retval==(0xd2d+202-0xdf7)){printf("\x47\x65\x74\x46\x75\x6c\x6c\x50\x61\x74\x68\x4e\x61\x6d\x65\x20\x66\x61\x69\x6c\x65\x64\x20\x28\x25\x64\x29" "\n",GetLastError());return GetLastError();}else{



}
int ReplacementFor_res=PathFileExists(ReplacementFor_buffer);if(ReplacementFor_res==(0xd26+209-0xdf6)){
ReplacementFor_isExists=true;}else{printf("\x54\x68\x65\x20\x72\x65\x71\x75\x65\x73\x74\x65\x64\x20" "\"" "\x25\x73" "\"" "\x20\x69\x73\x20\x6e\x6f\x74\x20\x61\x20\x76\x61\x6c\x69\x64\x20\x66\x69\x6c\x65\x20\x6f\x72\x20\x64\x69\x72\x65\x63\x74\x6f\x72\x79" "\n",ReplacementFor_buffer);}return 
ReplacementFor_isExists;}bool ReplacementFor_acl::ReplacementFor_isDirectory(char*name){DWORD ReplacementFor_dwAttrs=GetFileAttributes(name);
if((ReplacementFor_dwAttrs&FILE_ATTRIBUTE_DIRECTORY)){return true;}return false;}bool ReplacementFor_acl::ReplacementFor_isExistsUser(char*name){DWORD ReplacementFor_dwLengthOfSID=(0xd2d+202-0xdf7);
DWORD ReplacementFor_dwLengthOfDomainName=(0xd2d+202-0xdf7);
SID*ReplacementFor_userSID=NULL;
LPTSTR ReplacementFor_lpDomainName=NULL;
SID_NAME_USE ReplacementFor_type_of_SID;
DWORD ReplacementFor_dwErrCode;
if(!LookupAccountName(NULL,
ReplacementFor_username.c_str(),
NULL,
&ReplacementFor_dwLengthOfSID,
NULL,
&ReplacementFor_dwLengthOfDomainName,
&ReplacementFor_type_of_SID))
ReplacementFor_dwErrCode=GetLastError();if(ReplacementFor_dwErrCode==ERROR_INSUFFICIENT_BUFFER){
ReplacementFor_userSID=(SID*)new char[ReplacementFor_dwLengthOfSID];ReplacementFor_lpDomainName=(LPTSTR)new wchar_t[ReplacementFor_dwLengthOfDomainName];}else{
printf("\x4c\x6f\x6f\x6b\x75\x70\x20\x61\x63\x63\x6f\x75\x6e\x74\x20\x6e\x61\x6d\x65\x20\x66\x61\x69\x6c\x65\x64\x2e" "\n");printf("\x45\x72\x72\x6f\x72\x20\x63\x6f\x64\x65\x3a\x20\x25\x64" "\n",ReplacementFor_dwErrCode);return false;}
if(!LookupAccountName(NULL,
ReplacementFor_username.c_str(),
ReplacementFor_userSID,
&ReplacementFor_dwLengthOfSID,
ReplacementFor_lpDomainName,
&ReplacementFor_dwLengthOfDomainName,
&ReplacementFor_type_of_SID))
{ReplacementFor_dwErrCode=GetLastError();printf("\x4c\x6f\x6f\x6b\x75\x70\x20\x61\x63\x63\x6f\x75\x6e\x74\x20\x6e\x61\x6d\x65\x20\x66\x61\x69\x6c\x65\x64\x2e" "\n");printf("\x45\x72\x72\x6f\x72\x20\x63\x6f\x64\x65\x3a\x20\x25\x64" "\n",ReplacementFor_dwErrCode);return false;}delete[]
ReplacementFor_lpDomainName;delete[]ReplacementFor_userSID;return true;}void ReplacementFor_acl::ReplacementFor_printHelp(){printf("\x45\x78\x61\x6d\x70\x6c\x65\x73\x20\x6f\x66\x20\x63\x6f\x6d\x6d\x61\x6e\x64\x73\x3a" "\n");printf(
"\t" "\x2f\x67\x72\x61\x6e\x74\x20\x64\x2c\x77\x64\x61\x63\x20\x2f\x75\x20\x55\x73\x65\x72\x31\x20\x66\x69\x6c\x65\x31\x20\x66\x69\x6c\x65\x32\x20\x2d\x20\x67\x72\x61\x6e\x74\x20\x72\x69\x67\x68\x74\x73\x20\x66\x6f\x72\x20\x55\x73\x65\x72\x31" "\n");printf(
"\t" "\x2f\x64\x65\x6e\x79\x20\x64\x2c\x77\x64\x61\x63\x20\x2f\x75\x20\x55\x73\x65\x72\x31\x20\x66\x69\x6c\x65\x31\x20\x66\x69\x6c\x65\x32\x20\x2d\x20\x64\x65\x6e\x79\x20\x72\x69\x67\x68\x74\x73\x20\x66\x6f\x72\x20\x55\x73\x65\x72\x31" "\n");printf(
"\t" "\x2f\x67\x72\x61\x6e\x74\x20\x64\x2c\x77\x64\x61\x63\x20\x66\x69\x6c\x65\x31\x20\x66\x69\x6c\x65\x32\x20\x2d\x20\x67\x72\x61\x6e\x74\x20\x72\x69\x67\x68\x74\x73\x20\x66\x6f\x72\x20\x63\x75\x72\x72\x65\x6e\x74\x20\x75\x73\x65\x72" "\n");printf(
"\t" "\x2f\x64\x65\x6e\x79\x20\x64\x2c\x77\x64\x61\x63\x20\x66\x69\x6c\x65\x31\x20\x66\x69\x6c\x65\x32\x20\x2d\x20\x64\x65\x6e\x79\x20\x72\x69\x67\x68\x74\x73\x20\x66\x6f\x72\x20\x63\x75\x72\x72\x65\x6e\x74\x20\x75\x73\x65\x72" "\n");printf(
"\t" "\x2f\x72\x65\x76\x6f\x6b\x65\x20\x2f\x75\x20\x55\x73\x65\x72\x31\x20\x66\x69\x6c\x65\x31\x20\x66\x69\x6c\x65\x32\x20\x2d\x20\x72\x65\x76\x6f\x6b\x65\x20\x72\x69\x67\x68\x74\x73\x20\x66\x6f\x72\x20\x55\x73\x65\x72\x31" "\n");printf(
"\t" "\x2f\x72\x65\x76\x6f\x6b\x65\x20\x66\x69\x6c\x65\x31\x20\x66\x69\x6c\x65\x32\x20\x2d\x20\x72\x65\x76\x6f\x6b\x65\x20\x72\x69\x67\x68\x74\x73\x20\x66\x6f\x72\x20\x63\x75\x72\x72\x65\x6e\x74\x20\x75\x73\x65\x72" "\n");printf(
"\t" "\x2f\x75\x20\x55\x73\x65\x72\x31\x20\x66\x69\x6c\x65\x31\x20\x66\x69\x6c\x65\x32\x20\x2d\x20\x70\x72\x69\x6e\x74\x20\x65\x66\x66\x65\x63\x74\x69\x76\x65\x20\x72\x69\x67\x68\x74\x73\x20\x66\x6f\x72\x20\x55\x73\x65\x72\x31" "\n");printf(
"\t" "\x66\x69\x6c\x65\x31\x20\x66\x69\x6c\x65\x32\x20\x2d\x20\x70\x72\x69\x6e\x74\x20\x65\x66\x66\x65\x63\x74\x69\x76\x65\x20\x72\x69\x67\x68\x74\x73\x20\x66\x6f\x72\x20\x63\x75\x72\x72\x65\x6e\x74\x20\x75\x73\x65\x72\x20\x61\x6e\x64\x20\x66\x75\x6c\x6c\x20\x69\x6e\x66\x6f" "\n");printf(
"\x2f\x67\x72\x61\x6e\x74\x20\x61\x6e\x64\x20\x2f\x64\x65\x6e\x79\x20\x63\x61\x6e\x20\x62\x65\x20\x75\x73\x65\x64\x20\x74\x6f\x67\x65\x74\x68\x65\x72" "\n");printf(
"\x2f\x64\x20\x63\x61\x6e\x20\x62\x65\x20\x75\x73\x65\x64\x20\x77\x69\x74\x68\x20\x2f\x67\x72\x61\x6e\x74\x2c\x20\x2f\x64\x65\x6e\x79\x20\x61\x6e\x64\x20\x2f\x72\x65\x76\x6f\x6b\x65\x20\x66\x6f\x72\x20\x6f\x75\x74\x70\x75\x74\x20\x72\x69\x67\x68\x74\x73\x20\x62\x65\x66\x6f\x72\x65\x20\x61\x6e\x64\x20\x61\x66\x74\x65\x72\x20\x63\x68\x61\x6e\x67\x65\x73" "\n"
);printf(
"\x49\x6e\x68\x65\x72\x69\x74\x61\x6e\x63\x65\x20\x66\x6c\x61\x67\x73\x20\x61\x72\x65\x20\x61\x70\x70\x6c\x69\x65\x64\x20\x6f\x6e\x6c\x79\x20\x74\x6f\x20\x64\x69\x72\x65\x63\x74\x6f\x72\x69\x65\x73\x2c\x20\x64\x6f\x20\x6e\x6f\x74\x20\x74\x72\x79\x20\x75\x73\x65\x20\x74\x68\x65\x6d\x20\x77\x69\x74\x68\x20\x66\x69\x6c\x65\x73\x2e" "\n"
);printf("\x46\x69\x6c\x65\x20\x6e\x61\x6d\x65\x73\x20\x77\x69\x74\x68\x20\x73\x70\x61\x63\x65\x73\x20\x6d\x75\x73\x74\x20\x62\x65\x20\x69\x6e\x20\x71\x75\x6f\x74\x65\x73\x3a\x20" "\"" "\x66\x69\x6c\x65\x20\x77\x69\x74\x68\x20\x73\x70\x61\x63\x65" "\"\n");}void ReplacementFor_acl::
ReplacementFor_printOptions(DWORD ReplacementFor_mask){
vector<DWORD>complex;vector<DWORD>ReplacementFor_variants={FILE_ALL_ACCESS,FILE_GENERIC_EXECUTE|FILE_GENERIC_READ,FILE_GENERIC_READ,FILE_GENERIC_WRITE};vector<DWORD>::iterator ReplacementFor_it1,ReplacementFor_it2;map<string,DWORD>::iterator ReplacementFor_it;bool ReplacementFor_noPrint=false;
for(ReplacementFor_it1=ReplacementFor_variants.begin();ReplacementFor_it1!=ReplacementFor_variants.end();++ReplacementFor_it1){if(*ReplacementFor_it1==FILE_GENERIC_READ&&find(complex.begin(),complex.end(),FILE_GENERIC_EXECUTE|FILE_GENERIC_READ)!=complex.end())continue;if((ReplacementFor_mask&*
ReplacementFor_it1)==*ReplacementFor_it1){complex.push_back(*ReplacementFor_it1);if(*ReplacementFor_it1==FILE_ALL_ACCESS)break;}}

for(ReplacementFor_it=ReplacementFor_optionsRights.begin();ReplacementFor_it!=ReplacementFor_optionsRights.end();ReplacementFor_it++){ReplacementFor_noPrint=false;
if((ReplacementFor_mask&ReplacementFor_it->second)==ReplacementFor_it->second){
if(!(find(complex.begin(),complex.end(),ReplacementFor_it->second)!=complex.end())){for(ReplacementFor_it1=complex.begin();ReplacementFor_it1!=complex.end();++ReplacementFor_it1){
if((ReplacementFor_it->second&*ReplacementFor_it1)!=*ReplacementFor_it1&&(ReplacementFor_it->second&*ReplacementFor_it1)!=(0xd2d+202-0xdf7)){ReplacementFor_noPrint=true;break;}}}if(ReplacementFor_noPrint){continue;}printf("\x25\x73\x20",ReplacementFor_it->first.c_str());}}printf("\n");}void 
ReplacementFor_acl::ReplacementFor_printInheritance(BYTE flags){map<string,DWORD>::iterator ReplacementFor_it;bool ReplacementFor_printed=false;
for(ReplacementFor_it=ReplacementFor_inheritanceOptions.begin();ReplacementFor_it!=ReplacementFor_inheritanceOptions.end();ReplacementFor_it++){if((flags&ReplacementFor_it->second)==ReplacementFor_it->second){printf("\x25\x73\x20",ReplacementFor_it->first.c_str());ReplacementFor_printed=true;}}
if((flags&INHERITED_ACE)==INHERITED_ACE){printf("\x49\x44\x20");ReplacementFor_printed=true;}if(ReplacementFor_printed){printf("\x2c\x20");}}void ReplacementFor_acl::ReplacementFor_start(int ReplacementFor_argc,char*ReplacementFor_argv[]){setlocale(LC_ALL,"\x52\x75\x73\x73\x69\x61\x6e");if(
ReplacementFor_argc==(0xd26+209-0xdf6)){ReplacementFor_printHelp();}else if(ReplacementFor_parseParameters(ReplacementFor_argc,ReplacementFor_argv)){if(ReplacementFor_paths.size()>(0xd2d+202-0xdf7)){for(vector<char*>::iterator ReplacementFor_it=ReplacementFor_paths.begin();ReplacementFor_it!=
ReplacementFor_paths.end();++ReplacementFor_it){
if(ReplacementFor_printDebug&&(ReplacementFor_isChangingRights||ReplacementFor_isRevoke)){
ReplacementFor_printDaclInfo(*ReplacementFor_it);ReplacementFor_getEffectiveRightsFromAcl(*ReplacementFor_it);}
if(ReplacementFor_isChangingRights){ReplacementFor_addACEs(*ReplacementFor_it);}
else if(ReplacementFor_isRevoke){ReplacementFor_revokeACEs(*ReplacementFor_it);}
else if(ReplacementFor_printFullInfo){
ReplacementFor_printDaclInfo(*ReplacementFor_it);ReplacementFor_getEffectiveRightsFromAcl(*ReplacementFor_it);}
else{ReplacementFor_getEffectiveRightsFromAcl(*ReplacementFor_it);}
if(ReplacementFor_printDebug&&(ReplacementFor_isChangingRights||ReplacementFor_isRevoke)){
ReplacementFor_printDaclInfo(*ReplacementFor_it);ReplacementFor_getEffectiveRightsFromAcl(*ReplacementFor_it);}}}}else{printf("\x4e\x6f\x74\x20\x76\x61\x6c\x69\x64\x20\x63\x6f\x6d\x6d\x61\x6e\x64\x2e\x20\x50\x6c\x65\x61\x73\x65\x2c\x20\x63\x68\x61\x6e\x67\x65\x20\x69\x74\x2e" "\n\n");
ReplacementFor_printHelp();}getchar();}void ReplacementFor_acl::ReplacementFor_GetAccess(AUTHZ_CLIENT_CONTEXT_HANDLE ReplacementFor_hAuthzClient,PSECURITY_DESCRIPTOR ReplacementFor_psd,char*ReplacementFor_path){AUTHZ_ACCESS_REQUEST ReplacementFor_AccessRequest={(0xd2d+202-0xdf7)};AUTHZ_ACCESS_REPLY 
ReplacementFor_AccessReply={(0xd2d+202-0xdf7)};BYTE Buffer[(0xdf7+2629-0x143c)];BOOL ReplacementFor_bRes=FALSE;

ReplacementFor_AccessRequest.DesiredAccess=MAXIMUM_ALLOWED;ReplacementFor_AccessRequest.PrincipalSelfSid=NULL;ReplacementFor_AccessRequest.ObjectTypeList=NULL;ReplacementFor_AccessRequest.ObjectTypeListLength=(0xd2d+202-0xdf7);ReplacementFor_AccessRequest.OptionalArguments=NULL;RtlZeroMemory(Buffer,
sizeof(Buffer));ReplacementFor_AccessReply.ResultListLength=(0xd26+209-0xdf6);ReplacementFor_AccessReply.GrantedAccessMask=(PACCESS_MASK)(Buffer);ReplacementFor_AccessReply.Error=(PDWORD)(Buffer+sizeof(ACCESS_MASK));if(!AuthzAccessCheck((0xd2d+202-0xdf7),ReplacementFor_hAuthzClient,&
ReplacementFor_AccessRequest,NULL,ReplacementFor_psd,NULL,(0xd2d+202-0xdf7),&ReplacementFor_AccessReply,NULL)){wprintf_s(L"AuthzAccessCheck failed with %d\n",GetLastError());}else{printf(
"\n" "\x45\x66\x66\x65\x63\x74\x69\x76\x65\x20\x52\x69\x67\x68\x74\x73\x20\x74\x6f\x20" "\"" "\x25\x73" "\"" "\x20\x66\x6f\x72\x20\x75\x73\x65\x72\x20\x25\x73\x3a\x20",ReplacementFor_path,ReplacementFor_username.c_str());ReplacementFor_printOptions(*(PACCESS_MASK)(ReplacementFor_AccessReply.
GrantedAccessMask));}return;}BOOL ReplacementFor_acl::ReplacementFor_GetEffectiveRightsForUser(AUTHZ_RESOURCE_MANAGER_HANDLE ReplacementFor_hManager,PSECURITY_DESCRIPTOR ReplacementFor_psd,LPTSTR ReplacementFor_lpszUserName,char*ReplacementFor_path){PSID pSid=NULL;BOOL ReplacementFor_bResult=FALSE;
LUID ReplacementFor_unusedId={(0xd2d+202-0xdf7)};AUTHZ_CLIENT_CONTEXT_HANDLE ReplacementFor_hAuthzClientContext=NULL;pSid=ReplacementFor_ConvertNameToBinarySid(ReplacementFor_lpszUserName);if(pSid!=NULL){ReplacementFor_bResult=AuthzInitializeContextFromSid((0xd2d+202-0xdf7),pSid,
ReplacementFor_hManager,NULL,ReplacementFor_unusedId,NULL,&ReplacementFor_hAuthzClientContext);if(ReplacementFor_bResult){ReplacementFor_GetAccess(ReplacementFor_hAuthzClientContext,ReplacementFor_psd,ReplacementFor_path);AuthzFreeContext(ReplacementFor_hAuthzClientContext);}else wprintf_s(
L"AuthzInitializeContextFromSid failed with %d\n",GetLastError());}if(pSid!=NULL){LocalFree(pSid);pSid=NULL;}return ReplacementFor_bResult;}void ReplacementFor_acl::ReplacementFor_UseAuthzSolution(PSECURITY_DESCRIPTOR ReplacementFor_psd,char*ReplacementFor_path){AUTHZ_RESOURCE_MANAGER_HANDLE 
ReplacementFor_hManager;BOOL ReplacementFor_bResult=FALSE;ReplacementFor_bResult=AuthzInitializeResourceManager(AUTHZ_RM_FLAG_NO_AUDIT,NULL,NULL,NULL,NULL,&ReplacementFor_hManager);if(ReplacementFor_bResult){ReplacementFor_bResult=ReplacementFor_GetEffectiveRightsForUser(ReplacementFor_hManager,
ReplacementFor_psd,const_cast<char*>(ReplacementFor_username.c_str()),ReplacementFor_path);AuthzFreeResourceManager(ReplacementFor_hManager);}else wprintf_s(L"AuthzInitializeResourceManager failed with %d\n",GetLastError());}PSID ReplacementFor_acl::ReplacementFor_ConvertNameToBinarySid(LPTSTR 
ReplacementFor_pAccountName){LPTSTR ReplacementFor_pDomainName=NULL;DWORD ReplacementFor_dwDomainNameSize=(0xd2d+202-0xdf7);PSID pSid=NULL;DWORD ReplacementFor_dwSidSize=(0xd2d+202-0xdf7);SID_NAME_USE ReplacementFor_sidType;BOOL ReplacementFor_fSuccess=FALSE;HRESULT hr=S_OK;__try{LookupAccountName(
NULL,
ReplacementFor_pAccountName,pSid,
&ReplacementFor_dwSidSize,ReplacementFor_pDomainName,&ReplacementFor_dwDomainNameSize,&ReplacementFor_sidType);

if(GetLastError()==ERROR_NONE_MAPPED){wprintf_s(L"LookupAccountName failed with %d\n",GetLastError());__leave;}else if(GetLastError()==ERROR_INSUFFICIENT_BUFFER){pSid=(LPTSTR)LocalAlloc(LPTR,ReplacementFor_dwSidSize*sizeof(TCHAR));if(pSid==NULL){wprintf_s(
L"LocalAlloc failed with %d\n",GetLastError());__leave;}ReplacementFor_pDomainName=(LPTSTR)LocalAlloc(LPTR,ReplacementFor_dwDomainNameSize*sizeof(TCHAR));if(ReplacementFor_pDomainName==NULL){wprintf_s(L"LocalAlloc failed with %d\n",GetLastError());__leave;}if(!
LookupAccountName(NULL,
ReplacementFor_pAccountName,pSid,
&ReplacementFor_dwSidSize,ReplacementFor_pDomainName,&ReplacementFor_dwDomainNameSize,&ReplacementFor_sidType)){wprintf_s(L"LookupAccountName failed with %d\n",GetLastError());__leave;}}
else{wprintf_s(L"LookupAccountName failed with %d\n",GetLastError());__leave;}ReplacementFor_fSuccess=TRUE;}__finally{if(ReplacementFor_pDomainName!=NULL){LocalFree(ReplacementFor_pDomainName);ReplacementFor_pDomainName=NULL;}

if(ReplacementFor_fSuccess==FALSE){if(pSid!=NULL){LocalFree(pSid);pSid=NULL;}}}return pSid;}
