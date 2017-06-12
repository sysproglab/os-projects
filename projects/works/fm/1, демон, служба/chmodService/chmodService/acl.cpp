#include "stdafx.h"
#include <stdio.h>
#include "acl.h"
#include <windows.h> 
#include <iostream>
#include <algorithm>
#include <bitset>

#define BUFSIZE 4096

map<string, DWORD> acl::optionsRights = {
	/*
	FILE_GENERIC_EXECUTE //FILE_EXECUTE, FILE_READ_ATTRIBUTES,  STANDARD_RIGHTS_EXECUTE, SYNCHRONIZE
	FILE_GENERIC_READ // FILE_READ_ATTRIBUTES, FILE_READ_DATA, FILE_READ_EA, STANDARD_RIGHTS_READ, SYNCHRONIZE
	FILE_GENERIC_WRITE // FILE_APPEND_DATA, FILE_WRITE_ATTRIBUTES, FILE_WRITE_DATA, FILE_WRITE_EA, STANDARD_RIGHTS_WRITE, SYNCHRONIZE
	*/
	{ "F", FILE_ALL_ACCESS },
	{ "RX", FILE_GENERIC_EXECUTE | FILE_GENERIC_READ },
	{ "R", FILE_GENERIC_READ },
	{ "W", FILE_GENERIC_WRITE },

	//��� ������ c GA ������-�� � ������ ��������� F, � GW - W, GR - R, ������ ������ ��������� (��� �������� 1 ���, � ����� ������� ������ � �� ����� ��� �����������), ������ ��������� � ������� deniedOptions
	{ "GA", GENERIC_ALL },
	{ "GR", GENERIC_READ },
	{ "GW", GENERIC_WRITE },
	{ "GE", GENERIC_EXECUTE },

	{ "X", FILE_EXECUTE }, // FILE_EXECUTE ��� FILE_TRAVERSE
	{ "RD", FILE_READ_DATA }, // FILE_READ_DATA ��� FILE_LIST_DIRECTORY
	{ "WD", FILE_WRITE_DATA }, // FILE_WRITE_DATA ��� FILE_ADD_FILE
	{ "AD", FILE_APPEND_DATA }, // FILE_APPEND_DATA ��� FILE_ADD_SUBDIRECTORY
	{ "REA", FILE_READ_EA },
	{ "WEA", FILE_WRITE_EA },
	{ "RA", FILE_READ_ATTRIBUTES },
	{ "WA", FILE_WRITE_ATTRIBUTES },
	{ "DC", FILE_DELETE_CHILD },

	{ "WDAC", WRITE_DAC },
	{ "WO", WRITE_OWNER },
	{ "D", DELETE },
	{ "S", SYNCHRONIZE },
	{ "RC", READ_CONTROL },

	{ "AS", ACCESS_SYSTEM_SECURITY }
};

//��� ������ c GA ������-�� � ������ ��������� F, � GW - W, GR - R, ������ ������ ��������� (��� �������� 1 ���, � ����� ������� ������ � �� ����� ��� �����������)
map<string, DWORD> acl::deniedOptions = {
	{ "GA", GENERIC_ALL },
	{ "GR", GENERIC_READ },
	{ "GW", GENERIC_WRITE },
	{ "GE", GENERIC_EXECUTE }
};

// ������������ � BuildExplicitAccessWithName
map<string, DWORD> acl::inheritanceOptions = {
	{ "OI", OBJECT_INHERIT_ACE }, // object inherit
	{ "CI", CONTAINER_INHERIT_ACE }, // container inherit
	{ "IO", INHERIT_ONLY_ACE }, // inherit only
	{ "NP", NO_PROPAGATE_INHERIT_ACE } // do not propagate inherit
};

acl::acl()
{
}


acl::~acl()
{
}

int acl::printDaclInfo(char* path)
{
	PSECURITY_DESCRIPTOR pSecurityDescriptor; // ��������� �� SD 
	DWORD dwRetCode, mask; // ��� �������� 
	PACL pDacl = NULL; // DACL
	char lpName[UNLEN], lpDomain[UNLEN];
	string typeAce;
	SID_NAME_USE SidType;
	DWORD dwSize = UNLEN;
	PSID sid;
	BYTE flags;

	fprintf(file, "\nInfo about \"%s\"\n", path);

	// �������� ���������� ������������ �����
	dwRetCode = GetNamedSecurityInfo(
		path, // ��� ����� 
		SE_FILE_OBJECT, // ������ ���� 
		DACL_SECURITY_INFORMATION, // ������ ������ DACL, ��������� � ������
		NULL, // ����� ��������� �� SID ��������� 
		NULL, // ����� ��������� �� ��������� ������ 
		&pDacl, // ��������� �� DACL
		NULL, // ��������� �� SACL �� ����� 
		&pSecurityDescriptor); // ����� ��������� �� SD 
	if (dwRetCode != ERROR_SUCCESS) {
		fprintf(file, "Get named security info failed.\n");
		fprintf(file, "Error code: %u\n", dwRetCode);
		return dwRetCode;
	}

	void *lpAce = NULL; // ��������� �� ������� ������
	for (unsigned i = 0; i < pDacl->AceCount; ++i)
	{
		if (!GetAce(
			pDacl, // ����� DACL 
			i, // ������ �������� 
			&lpAce)) // ��������� �� ������� ������ 
		{
			dwRetCode = GetLastError();
			fprintf(file, "Get ace failed.\n");
			fprintf(file, "Error code: %d\n", dwRetCode);
			return dwRetCode;
		}
		if (((ACE_HEADER*)lpAce)->AceType == ACCESS_ALLOWED_ACE_TYPE) {
			sid = &((ACCESS_ALLOWED_ACE*)lpAce)->SidStart;
			mask = ((ACCESS_ALLOWED_ACE*)lpAce)->Mask;
			typeAce = "ALLOWED";
		}
		else {
			sid = &((ACCESS_DENIED_ACE*)lpAce)->SidStart;
			mask = ((ACCESS_DENIED_ACE*)lpAce)->Mask;
			typeAce = "DENIED";
		}
		flags = ((ACE_HEADER*)lpAce)->AceFlags;

		// ������� �� ������ ������������ � �����
		dwSize = UNLEN;
		memset(&lpName[0], 0, sizeof(lpName));
		memset(&lpDomain[0], 0, sizeof(lpDomain));
		// �������� ��� ������������
		if (!LookupAccountSid(NULL, sid, lpName, &dwSize, lpDomain, &dwSize, &SidType))
		{
			dwRetCode = GetLastError();
			if (dwRetCode == ERROR_NONE_MAPPED)
				strcpy_s(lpName, dwSize, "NONE_MAPPED");
			else
			{
				fprintf(file, "LookupAccountSid Error %u\n", GetLastError());
				return dwRetCode;
			}
		}
		fprintf(file, "For user %s\\%s flags, %s rights: ", lpDomain, lpName, typeAce.c_str());
		// �������� ����� � �����
		printInheritance(flags);
		printOptions(mask);
	}

	// ����������� ������ 
	LocalFree(pSecurityDescriptor);
	return 0;
}

int acl::printDaclInfoSDDL(char* path)
{
	PSECURITY_DESCRIPTOR pSecurityDescriptor; // ��������� �� SD 
	LPSTR StringSecurityDescriptor; // ������ � SD 
	DWORD StringSecurityDescriptorLen; // ����� ������ � SD 
	DWORD dwRetCode; // ��� �������� 
	PACL pDacl = NULL; // DACL
	PSID pSidOwner; // ��������� �� SID ��������� �������
	PSID pSidGroup; // ��������� �� SID ��������� ������ ������� 
	LPTSTR lpStringSid; // ��������� �� ������ SID 

	fprintf(file, "\Info about \"%s\"\n", path);

	// �������� ���������� ������������ �����
	dwRetCode = GetNamedSecurityInfo(
		path, // ��� ����� 
		SE_FILE_OBJECT, // ������ ���� 
		OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION |
		DACL_SECURITY_INFORMATION, // ������ ������ DACL, ��������� � ������
		&pSidOwner, // ����� ��������� �� SID ��������� 
		&pSidGroup, // ����� ��������� �� ��������� ������ 
		&pDacl, // ��������� �� DACL
		NULL, // ��������� �� SACL �� ����� 
		&pSecurityDescriptor); // ����� ��������� �� SD 
	if (dwRetCode != ERROR_SUCCESS) {
		fprintf(file, "Get named security info failed.\n");
		fprintf(file, "Error code: %u\n", dwRetCode);
		return dwRetCode;
	}

	// ����������� ���������� ������������ � ������ 
	if (!ConvertSecurityDescriptorToStringSecurityDescriptor(
		pSecurityDescriptor, // ����� ����������� ������������ 
		SDDL_REVISION_1, // ������ ����� �������� 
		OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION |
		DACL_SECURITY_INFORMATION | SACL_SECURITY_INFORMATION,
		&StringSecurityDescriptor, // ����� ��� ������ 
		&StringSecurityDescriptorLen)) // ����� ������ 
	{
		dwRetCode = GetLastError();
		fprintf(file, "Convert security descriptor to string security descriptor failed. Error code: %u\n", dwRetCode);
		return dwRetCode;
	}

	//printf("String security descriptor length: %u\n", StringSecurityDescriptorLen);
	//printf("String security desriptor: %s\n", StringSecurityDescriptor);
	// ����������� ���������� ������������ ��������� � ����������
	string result = StringSecurityDescriptor;
	string::size_type pos = result.find(":");
	while (pos != string::npos) {
		result.insert(pos - 1, "\n");
		result.insert(pos + 2, "\n\t");
		pos = result.find(":", pos + 2);
	}
	pos = result.find(")");
	while (pos != string::npos) {
		result.insert(pos + 1, "\n\t");
		pos = result.find(")", pos + 1);
	}
	fprintf(file, "String security desriptor: %s\n", result.c_str());

	char lpName[UNLEN], lpDomain[UNLEN];;
	SID_NAME_USE SidType;
	DWORD dwSize = UNLEN;
	PSID sid;

	// �������� � ������� ��� ��������� �����
	if (!LookupAccountSid(NULL, pSidOwner, lpName, &dwSize, lpDomain, &dwSize, &SidType))
	{
		dwRetCode = GetLastError();
		if (dwRetCode == ERROR_NONE_MAPPED)
			strcpy_s(lpName, dwSize, "NONE_MAPPED");
		else
		{
			fprintf(file, "LookupAccountSid Error %u\n", GetLastError());
			return dwRetCode;
		}
	}
	fprintf(file, "Owner is a member of the %s\\%s group\n", lpDomain, lpName);

	// ����������� ������ 
	LocalFree(pSecurityDescriptor);
	LocalFree(StringSecurityDescriptor);
	return 0;
}

int acl::addACEs(char* path)
{
	EXPLICIT_ACCESS ea; // ���������� ��� �������� ������ DACL 
	PACL lpOldDacl; // ��������� �� ������ ������ DACL 
	PACL lpNewDacl; // ��������� �� ����� ������ DACL 
	PSECURITY_DESCRIPTOR lpSD; // ��������� �� ���������� ������������ 

	DWORD dwErrCode; // ��� ��������
	EXPLICIT_ACCESS s [2]; // ������ � ����������� �� ��������� ������ DACL 
	int countAddedACEs = 0;

	// �������� SD
	dwErrCode = GetNamedSecurityInfo(
		path, // ��� ����� ��� ��������
		SE_FILE_OBJECT, // ������ ���� 
		DACL_SECURITY_INFORMATION, // �������� DACL 
		NULL, // ����� ��������� �� SID ��������� 
		NULL, // ����� ��������� �� ��������� ������ 
		&lpOldDacl, // ��������� �� DACL 
		NULL, // ��������� �� SACL �� ����� 
		&lpSD); // ����� ��������� �� ���������� ������������ 
	if (dwErrCode != ERROR_SUCCESS)
	{
		fprintf(file, "Get named security info failed.\n");
		fprintf(file, "Error code: %u\n", dwErrCode);
		return dwErrCode;
	}

	dwErrCode = cleanDACL(lpOldDacl);
	if (dwErrCode != ERROR_SUCCESS)
	{
		perror("Cleaning DACL failed.\n");
		fprintf(file, "The last error code: %u\n", dwErrCode);
		return dwErrCode;
	}

	// ������ ��������� EXPLICIT_ACCESS �� �����
	if (grantRights > 0)
	{
		BuildExplicitAccessWithName(
			&ea, // ����� ��������� ExplicitAccess 
			const_cast<char *>(username.c_str()), // ��� ������� ������
			grantRights, // ����������� �����
			GRANT_ACCESS, // ��������� ������ 
			inheritanceFlags // ����� ������������ 
		);
		s[countAddedACEs] = ea;
		countAddedACEs++;
	}
	if (denyRights > 0)
	{
		BuildExplicitAccessWithName(
			&ea, // ����� ��������� ExplicitAccess 
			const_cast<char *>(username.c_str()), // ��� ������� ������
			denyRights, // ����������� �����
			DENY_ACCESS, // ��������� ������
			inheritanceFlags // ����� ������������ 
		);
		s[countAddedACEs] = ea;
		countAddedACEs++;
	}

	// ������� ������ DACL 
	dwErrCode = SetEntriesInAcl(countAddedACEs, // ��������� �� ������ �� ���� ��������� � ������ DACL 
		s, // ������ ��������
		lpOldDacl, // ����� ������� ������ DACL 
		&lpNewDacl); // ����� ��������� �� ����� ������ DACL

	if (dwErrCode != ERROR_SUCCESS)
	{
		perror("Set entries in DACL failed.\n");
		fprintf(file, "The last error code: %u\n", dwErrCode); 
		return dwErrCode;
	}

	// ������������� ����� ������ DACL 
	dwErrCode = SetNamedSecurityInfo(
		path, // ��� ����� 
		SE_FILE_OBJECT, // ������ ���� 
		DACL_SECURITY_INFORMATION, // �������� ������ DACL 
		NULL, // ��������� �� �������� 
		NULL, // ��������� ������ �� �������� 
		lpNewDacl, // ����� DACL 
		NULL); // SACL �� �������� 
	if (dwErrCode != ERROR_SUCCESS)
	{
		fprintf(file, "Set named security info failed.\n");
		fprintf(file, "Error code: %u\n", dwErrCode);
		return dwErrCode;
	}

	// ����������� ������ 
	LocalFree(lpSD);
	LocalFree(lpNewDacl);
	if (grantRights > 0 || denyRights > 0) {
		fprintf(file, "The rights for user %s to \"%s\" were modified.\n", username.c_str(), path);
	}
	else {
		fprintf(file, "The rights for user %s to \"%s\" have not been modified.\n", username.c_str(), path);
	}
	return 0;
}

int acl::cleanDACL(PACL lpOldDacl) 
{
	void *lpAce = NULL; // ��������� �� ������� ������ 
	char lpName[UNLEN], lpDomain[UNLEN];
	SID_NAME_USE SidType;
	DWORD dwSize = UNLEN, dwErrCode, rights, flags;
	DWORD *mask;
	PSID sid;
	vector<int> deleteIndex; // ������� ���������, ������� ����� �������
	vector<int>::reverse_iterator it;

	// ������� ��� �������� ������, �������������� rights ��� �����������
	for (unsigned i = 0; i < lpOldDacl->AceCount; ++i)
	{
		// �������� ������� ������ DACL 
		if (!GetAce(
			lpOldDacl, // ����� DACL 
			i, // ������ ��������
			&lpAce)) // ��������� �� ������� ������
		{ 
			dwErrCode = GetLastError();
			fprintf(file, "Get ��� failed.\n");
			fprintf(file, "Error code: %d\n", dwErrCode);
			return dwErrCode;
		}
		
		flags = ((ACE_HEADER*)lpAce)->AceFlags;
		if (((ACE_HEADER*)lpAce)->AceType == ACCESS_ALLOWED_ACE_TYPE) {
			if (denyRights == 0) continue;
			sid = &((ACCESS_ALLOWED_ACE*)lpAce)->SidStart;
			mask = &((ACCESS_ALLOWED_ACE*)lpAce)->Mask;
			// ���� ����� ��������� �����, �� ������� �� �� ALLOWED ������
			rights = denyRights;
		}
		else {
			if (grantRights == 0) continue;
			sid = &((ACCESS_DENIED_ACE*)lpAce)->SidStart;
			mask = &((ACCESS_DENIED_ACE*)lpAce)->Mask;
			// ���� ����� ��������� �����, �� ������� �� �� DENIED ������
			rights = grantRights;
		}

		// �������� ������, ��������� � ������������� ��� �������� ������ �����
		dwSize = UNLEN;
		memset(&lpName[0], 0, sizeof(lpName));
		memset(&lpDomain[0], 0, sizeof(lpDomain));
		if (!LookupAccountSid(NULL, sid,
			lpName, &dwSize, lpDomain,
			&dwSize, &SidType))
		{
			dwErrCode = GetLastError();
			if (dwErrCode == ERROR_NONE_MAPPED)
				strcpy_s(lpName, dwSize, "NONE_MAPPED");
			else
			{
				fprintf(file, "LookupAccountSid Error %u\n", GetLastError());
				//continue;
				return dwErrCode;
			}
		}
		//fprintf(file, "User of ACE is a member of the %s\\%s group\n", lpDomain, lpName);
		if (strcmp(lpName, username.c_str()) != 0) {
			continue;
		}

		// ���� ���������� �������������� �� ������ ����������� ������, �� �� �� ����� � �������, �������������, �� ������� �������� ��������� �������������� ������
		if ((flags & INHERITED_ACE) == INHERITED_ACE && ((*mask & ~rights) == 0 || (*mask & ~rights) != *mask)) {
			fprintf(file, "Sorry, but found problem with inherited ACE (crossed rights). Try at first to change rights of parent directories.\n");
			return - 1;
		}

		if ((*mask & ~rights) == 0) {
			deleteIndex.push_back(i);
		}
		else if ((*mask & ~rights) != *mask) {
			// �������� �����, ����� ����� � ALLOWED(DENIED) ������ �� ������������� ������, ������� �� ��������� � DENIED(ALLOWED) ������
			*mask = (*mask & ~rights);
			//fprintf(file, "Changed old ACE\n");
		}
	}
	for (it = deleteIndex.rbegin(); it != deleteIndex.rend(); ++it)
	{
		// ������� ������� �� ������ DACL, ������ ��� ��� ����� �� ����� �������� �� ����� ��������� � DENIED(ALLOWED) ������
		if (!DeleteAce(lpOldDacl, *it))
		{
			dwErrCode = GetLastError();
			fprintf(file, "Delete ace failed.\n");
			fprintf(file, "Error code: %d\n", dwErrCode);
			return dwErrCode;
		}
		//fprintf(file, "Deleted old ACE at index %d\n", *it);
	}
	return 0;
}

int acl::revokeACEs(char* path)
{
	EXPLICIT_ACCESS ea; // ���������� ��� �������� ������ DACL 
	PACL lpOldDacl; // ��������� �� ������ ������ DACL 
	PSECURITY_DESCRIPTOR lpSD; // ��������� �� ���������� ������������ 
	DWORD dwErrCode; // ��� ��������
	vector<int> deleteIndexes; // ������� ���������, ������� ����� �������
	vector<int>::reverse_iterator it;

	DWORD dwLengthOfSID = 0; // ����� SID 
	DWORD dwLengthOfDomainName = 0; // ����� ����� ������ 
	SID *userSID = NULL; // ��������� �� SID ������������
	LPTSTR lpDomainName = NULL; // ��������� �� ��� ������ 
	SID_NAME_USE type_of_SID; // ��� ������� ������ 

	void *lpAce = NULL; // ��������� �� ������� ������ 
	DWORD type; // ��� ACE ������
	PSID sid; // ��������� �� SID ������

	// ���������� ����� SID ������������ 
	if (!LookupAccountName(
		NULL, // ���� ��� �� ��������� ���������� 
		username.c_str(), // ��� ������������ 
		NULL, // ���������� ����� SID 
		&dwLengthOfSID, // ����� SID
		NULL, // ���������� ��� ������ 
		&dwLengthOfDomainName, // ����� ����� ������ 
		&type_of_SID)) // ��� ������� ������ 
		dwErrCode = GetLastError();
	if (dwErrCode == ERROR_INSUFFICIENT_BUFFER)
	{
		// ������������ ������ ��� SID � ����� ������ 
		userSID = (SID*) new char[dwLengthOfSID];
		lpDomainName = (LPTSTR) new wchar_t[dwLengthOfDomainName];
	}
	else
	{
		// ������� �� ��������� 
		fprintf(file, "Lookup account name failed.\n");
		fprintf(file, "Error code: %d\n", dwErrCode);
		return dwErrCode;
	}

	// ���������� SID � ��� ������ ������������ 
	if (!LookupAccountName(
		NULL, // ���� ��� �� ��������� ���������� 
		username.c_str(), // ��� ������������ 
		userSID, // ��������� �� SID 
		&dwLengthOfSID, // ����� SID 
		lpDomainName, // ��������� �� ��� ������ 
		&dwLengthOfDomainName, // ����� ����� ������ 
		&type_of_SID)) // ��� ������� ������ 
	{
		dwErrCode = GetLastError();
		fprintf(file, "Lookup account name failed.\n");
		fprintf(file, "Error code: %d\n", dwErrCode);
		return dwErrCode;
	}
	/*
	// ������� SID �� ������� 
	fprintf(file, "SID revision: %u\n", userSID->Revision);
	fprintf(file, "SubAuthorityCount: %u\n", userSID->SubAuthorityCount);
	fprintf(file, "IdentifierAuthority: ");
	for (int i = 0; i < 6; ++i)
		fprintf(file, "%u ", userSID->IdentifierAuthority.Value[i]);
	fprintf(file, "\n");
	fprintf(file, "SubAuthorities: ");
	for (int i = 0; i < userSID->SubAuthorityCount; ++i)
		fprintf(file, "%u ", userSID->SubAuthority[i]);
	fprintf(file, "\n");
	// ������� ��� ������ 
	fprintf(file, "Domain name: %s\n", lpDomainName);
	*/

	// �������� SD
	dwErrCode = GetNamedSecurityInfo(
		path, // ��� ����� ��� ��������
		SE_FILE_OBJECT, // ������ ���� 
		DACL_SECURITY_INFORMATION, // �������� DACL 
		NULL, // ����� ��������� �� SID ��������� 
		NULL, // ����� ��������� �� ��������� ������ 
		&lpOldDacl, // ��������� �� DACL 
		NULL, // ��������� �� SACL �� ����� 
		&lpSD); // ����� ��������� �� ���������� ������������ 
	if (dwErrCode != ERROR_SUCCESS)
	{
		fprintf(file, "Get named security info failed.\n");
		fprintf(file, "Error code: %u\n", dwErrCode);
		return dwErrCode;
	}

	// ������� ������, ��������� � �������������
	for (unsigned i = 0; i < lpOldDacl->AceCount; ++i)
	{
		// �������� ������� ������ DACL 
		if (!GetAce(
			lpOldDacl, // ����� DACL 
			i, // ������ ��������
			&lpAce)) // ��������� �� ������� ������
		{
			dwErrCode = GetLastError();
			fprintf(file, "Get ��� failed.\n");
			fprintf(file, "Error code: %d\n", dwErrCode);
			return dwErrCode;
		}

		type = ((ACE_HEADER*)lpAce)->AceType;
		if (type == ACCESS_ALLOWED_ACE_TYPE) {
			sid = &((ACCESS_ALLOWED_ACE*)lpAce)->SidStart;
		}
		else if (type == ACCESS_DENIED_ACE_TYPE) {
			sid = &((ACCESS_DENIED_ACE*)lpAce)->SidStart;
		}
		else {
			continue;
		}

		if (!EqualSid(userSID, sid)) {
			continue;
		}

		// ���� sid � ������ ����� SID ������������, �� ����������� ������ ������ � ������ ���������
		deleteIndexes.push_back(i);
	}
	for (it = deleteIndexes.rbegin(); it != deleteIndexes.rend(); ++it)
	{
		// ������� ������� �� ������ DACL, ������� ������� � �������������
		if (!DeleteAce(lpOldDacl, *it))
		{
			dwErrCode = GetLastError();
			fprintf(file, "Delete ace failed.\n");
			fprintf(file, "Error code: %d\n", dwErrCode);
			return dwErrCode;
		}
		//fprintf(file, "Deleted old ACE at index %d\n", *it);
	}

	// ������������� ����� ������ DACL 
	dwErrCode = SetNamedSecurityInfo(
		path, // ��� ����� 
		SE_FILE_OBJECT, // ������ ���� 
		DACL_SECURITY_INFORMATION, // �������� ������ DACL 
		NULL, // ��������� �� �������� 
		NULL, // ��������� ������ �� �������� 
		lpOldDacl, // ����� DACL 
		NULL); // SACL �� �������� 
	if (dwErrCode != ERROR_SUCCESS)
	{
		fprintf(file, "Set named security info failed.\n");
		fprintf(file, "Error code: %u\n", dwErrCode);
		return dwErrCode;
	}

	// ����������� ������ 
	LocalFree(lpSD);
	LocalFree(lpOldDacl);
	delete[] lpDomainName;
	delete[] userSID;
	if (deleteIndexes.size() > 0) {
		fprintf(file, "The revoke of \"%s\" for user %s was made.\n", path, username.c_str());
	}
	return 0;
}

int acl::getEffectiveRightsFromAcl(char* path)
{
	TRUSTEE Trustee; // ���������� �� ������� ������ 
	PSECURITY_DESCRIPTOR lpSd; // ��������� �� ���������� ������������ 
	PACL lpDacl; // ��������� �� ������ DACL 
	ACCESS_MASK dwAccessRights = 0; // ����� ������� 
	DWORD dwErrCode; // ��� �������� 

	dwErrCode = GetNamedSecurityInfo(
		path, // ��� ����� 
		SE_FILE_OBJECT, // ������ ���� 
		OWNER_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION, // �������� DACL 
		NULL, // ����� ��������� �� SID ��������� 
		NULL, // ����� ��������� �� ��������� ������ 
		&lpDacl, // ��������� �� DACL 
		NULL, // ��������� �� SACL �� ����� 
		&lpSd); // ����� ��������� �� ���������� ������������ 
	if (dwErrCode != ERROR_SUCCESS)
	{
		fprintf(file, "Get named security info failed.\n");
		fprintf(file, "Error code: %u\n", dwErrCode);
		return dwErrCode;
	}

	// ������ ��������� TRUSTEE �� �����
	BuildTrusteeWithName(&Trustee, const_cast<char *>(username.c_str()));

	// �������� ����� ������� ��� ��������� ����� 
	dwErrCode = GetEffectiveRightsFromAcl(
		lpDacl, // ��������� �� ������ DACL 
		&Trustee, // ����� ��������� TRUSTEE 
		&dwAccessRights); // ����� ����� � ������� 
	if (dwErrCode != ERROR_SUCCESS)
	{
		fprintf(file, "\nGet effective rights from ACL failed.\n");
		fprintf(file, "Error code: %u\n", dwErrCode);
		// ������ ������� ������� ����������� �����
		UseAuthzSolution(lpSd, path);
		return dwErrCode;
	}

	// ������� �� ������� ����� ������� 
	/*fprintf(file, "Effective ACCESS MASK: %x\nBinary Effective ACCESS MASK: ", dwAccessRights);
	cout << bitset<32>(dwAccessRights) << endl;*/
	fprintf(file, "\nEffective Rights to \"%s\" for user %s: ", path, username.c_str());
	printOptions(dwAccessRights);

	// ����������� ������ 
	LocalFree(lpSd);
	return 0;
}

bool acl::parseParameters(int argc, char *argv[])
{
	string option, temp;
	bool isGrant = false, result = true;
	map<string, DWORD>::iterator it, it1;
	isRevoke = isChangingRights = printFullInfo = printDebug = false;
	string::size_type pos;
	char usernameTemp[UNLEN + 1];
	DWORD username_len = UNLEN + 1;
	username = "";
	paths.clear();
	grantRights = denyRights = 0;
	inheritanceFlags = NO_INHERITANCE; // ��� ������������

	for (int i = 1; i < argc; i++)
	{
		//fprintf(file, "Arg: %d\nValue: %s\n", i, argv[i]);
		// ��������� ����� ���������� ����� /grant � /deny
		if (strcmp(argv[i], "/grant") == 0 || strcmp(argv[i], "/deny") == 0) {
			isGrant = strcmp(argv[i], "/grant") == 0;
			if (i == (argc - 1)) {
				fprintf(file, "After /grant or /deny must be options\n");
				return false;
			}
			isChangingRights = true;
			temp = argv[i + 1];
			i++;

			transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
			pos = temp.find(",");
			while (pos != string::npos || temp.length() > 0)
			{
				if (pos != string::npos) {
					option = temp.substr(0, pos);
				}
				else {
					option = temp;
				}
				//fprintf(file, "Option %s\n", option.c_str());

				if ((it = optionsRights.find(option)) != optionsRights.end() && (it1 = deniedOptions.find(option)) == deniedOptions.end()) {
					if (isGrant) {
						grantRights = grantRights | it->second;
					}
					else {
						denyRights = denyRights | it->second;
					}
				}
				else if ((it = inheritanceOptions.find(option)) != inheritanceOptions.end()) {
					inheritanceFlags = inheritanceFlags | it->second;
				}
				else {
					fprintf(file, "Bad option %s\n", option.c_str());
					result = false;
				}

				if (pos == string::npos) {
					break;
				}
				temp.erase(0, pos + 1);
				pos = temp.find(",");
			}
		}
		// ������������� ����, ������������, ��� ����� ��������� ������
		else if (strcmp(argv[i], "/revoke") == 0) {
			isRevoke = true;
		}
		// �������� ���������� ��� ������������
		else if (strcmp(argv[i], "/u") == 0) {
			if (username.length() > 0) {
				fprintf(file, "You can specify just one username\n");
				return false;
			}
			if (i == (argc - 1)) {
				fprintf(file, "After /u must be username\n");
				return false;
			}
			username = argv[i+1];
			if (!isExistsUser(argv[i + 1])) {
				fprintf(file, "There are not found user with username %s!\n", argv[i + 1]);
				result = false;
			}
			i++;
		}
		// ������������� ����, ������������, ��� ����� �������� ���������� �� � ����� ���������
		else if (strcmp(argv[i], "/d") == 0) {
			printDebug = true;
		}
		else {
			if (!isExistsPath(argv[i])) {
				result = false;
			}
			paths.push_back(argv[i]);
		}
	}

	if (username.length() == 0) {
		GetUserName(usernameTemp, &username_len);
		username = usernameTemp;
		printFullInfo = true;
	}
	//fprintf(file, "Username - %s\n", username.c_str());

	/*if (paths.size() > 0) {
		fprintf(file, "The contents of paths are:\n");
		for (vector<char*>::iterator it = paths.begin(); it != paths.end(); ++it)
			fprintf(file, "%s\n", *it);
	}*/

	//�������� �� ������������ � ������ � ����������
	if (paths.size() == 0 && (isRevoke || isChangingRights)) {
		fprintf(file, "You must select the files or directories. Please, fix it.\n");
		result = false;
	}

	if (isRevoke && isChangingRights) {
		fprintf(file, "You must use grant and deny options separately from revoke. Please, fix it.\n");
		result = false;
	}

	if ((grantRights & denyRights) > 0) {
		fprintf(file, "There are crossed options. Please, fix it.\n");
		printOptions(grantRights & denyRights);
		result = false;
	}

	if (isChangingRights && inheritanceFlags > 0 && grantRights == 0 && denyRights == 0) {
		fprintf(file, "There are just inheritance flags. Please, specify rights.\n");
		result = false;
	}

	if (inheritanceFlags > 0) {
		for (vector<char*>::iterator it = paths.begin(); it != paths.end(); ++it) {
			if (!isDirectory(*it)) {
				result = false;
				fprintf(file, "You must use inheritance flags with directories, but \"%s\" is not directory.\n", *it);
			}
		}
	}

	return result;
}

bool acl::isExistsPath(char* name)
{
	bool isExists = false;
	DWORD  retval = 0;
	CHAR  buffer[BUFSIZE];
	CHAR  buf[BUFSIZE];
	CHAR** lppPart = { NULL };

	// �������� ������ ���� � �����
	retval = GetFullPathName(name,
		BUFSIZE,
		buffer,
		lppPart);

	if (retval == 0)
	{
		fprintf(file, "GetFullPathName failed (%d)\n", GetLastError());
		return GetLastError();
	}
	else
	{
		/*_tprintf(TEXT("The full path name is:  %s\n"), buffer);
		if (lppPart != NULL && *lppPart != 0)
		{
		_tprintf(TEXT("The final component in the path name is:  %s\n"), *lppPart);
		}*/
	}

	// ���������, ��� ���� ��� ���������� ����������
	int res = PathFileExists(buffer);
	if (res == 1)
	{
		//fprintf(file, "The requested \"%s\" is a valid file or directory\n", buffer);
		isExists = true;
	}
	else
	{
		fprintf(file, "The requested \"%s\" is not a valid file or directory\n", buffer);
	}
	return isExists;
}

bool acl::isDirectory(char* name)
{
	DWORD dwAttrs = GetFileAttributes(name);

	// ��������, ��� ��� ����������
	if ((dwAttrs & FILE_ATTRIBUTE_DIRECTORY))
	{
		return true;
	}
	return false;
}

bool acl::isExistsUser(char* name)
{
	DWORD dwLengthOfSID = 0; // ����� SID 
	DWORD dwLengthOfDomainName = 0; // ����� ����� ������ 
	SID *userSID = NULL; // ��������� �� SID ������������
	LPTSTR lpDomainName = NULL; // ��������� �� ��� ������ 
	SID_NAME_USE type_of_SID; // ��� ������� ������ 
	DWORD dwErrCode;

	// ���������� ����� SID ������������ 
	if (!LookupAccountName(
		NULL, // ���� ��� �� ��������� ���������� 
		username.c_str(), // ��� ������������ 
		NULL, // ���������� ����� SID 
		&dwLengthOfSID, // ����� SID
		NULL, // ���������� ��� ������ 
		&dwLengthOfDomainName, // ����� ����� ������ 
		&type_of_SID)) // ��� ������� ������ 
		dwErrCode = GetLastError();
	if (dwErrCode == ERROR_INSUFFICIENT_BUFFER)
	{
		// ������������ ������ ��� SID � ����� ������ 
		userSID = (SID*) new char[dwLengthOfSID];
		lpDomainName = (LPTSTR) new wchar_t[dwLengthOfDomainName];
	}
	else
	{
		// ������� �� ��������� 
		fprintf(file, "Lookup account name failed.\n");
		fprintf(file, "Error code: %d\n", dwErrCode);
		return false;
	}

	// ���������� SID � ��� ������ ������������ 
	if (!LookupAccountName(
		NULL, // ���� ��� �� ��������� ���������� 
		username.c_str(), // ��� ������������ 
		userSID, // ��������� �� SID 
		&dwLengthOfSID, // ����� SID 
		lpDomainName, // ��������� �� ��� ������ 
		&dwLengthOfDomainName, // ����� ����� ������ 
		&type_of_SID)) // ��� ������� ������ 
	{
		dwErrCode = GetLastError();
		fprintf(file, "Lookup account name failed.\n");
		fprintf(file, "Error code: %d\n", dwErrCode);
		return false;
	}

	delete[] lpDomainName;
	delete[] userSID;
	return true;
}

void acl::printHelp()
{
	fprintf(file, "Examples of commands:\n");
	fprintf(file, "\t/grant d,wdac /u User1 file1 file2 - grant rights for User1\n");
	fprintf(file, "\t/deny d,wdac /u User1 file1 file2 - deny rights for User1\n");
	fprintf(file, "\t/grant d,wdac file1 file2 - grant rights for current user\n");
	fprintf(file, "\t/deny d,wdac file1 file2 - deny rights for current user\n");
	fprintf(file, "\t/revoke /u User1 file1 file2 - revoke rights for User1\n");
	fprintf(file, "\t/revoke file1 file2 - revoke rights for current user\n");
	fprintf(file, "\t/u User1 file1 file2 - print effective rights for User1\n");
	fprintf(file, "\tfile1 file2 - print effective rights for current user and full info\n");
	fprintf(file, "/grant and /deny can be used together\n");
	fprintf(file, "/d can be used with /grant, /deny and /revoke for output rights before and after changes\n");
	fprintf(file, "Inheritance flags are applied only to directories, do not try use them with files.\n");
	fprintf(file, "File names with spaces must be in quotes: \"file with space\"\n");
}

void acl::printOptions(DWORD mask)
{
	// ��� ������� ������������, ��� ���� ����� ��������� �� ������ �����, ������� �������� � ����� ������� ���������� (� �������, FILE_GENERIC_READ)
	vector<DWORD> complex;
	vector<DWORD> variants = { FILE_ALL_ACCESS, FILE_GENERIC_EXECUTE | FILE_GENERIC_READ, FILE_GENERIC_READ, FILE_GENERIC_WRITE};
	vector<DWORD>::iterator it1, it2;
	map<string, DWORD>::iterator it;
	bool noPrint = false;

	// ����������� ������� ��������� ����, �������������� � ���������� �����
	for (it1 = variants.begin(); it1 != variants.end(); ++it1)
	{
		if (*it1 == FILE_GENERIC_READ && find(complex.begin(), complex.end(), FILE_GENERIC_EXECUTE | FILE_GENERIC_READ) != complex.end()) continue;
		if ((mask & *it1) == *it1) {
			complex.push_back(*it1);
			if (*it1 == FILE_ALL_ACCESS) break;
		}
	}

	//cout << bitset<32>(mask) << endl;

	// ���������� ��� ��������� ����� ����
	for (it = optionsRights.begin(); it != optionsRights.end(); it++)
	{
		noPrint = false;
		// �������� �� ��, ��� � ����� ���������� �����
		if ((mask & it->second) == it->second) {
			// ���� ����� ������������ � complex, �� ����� ��������
			if (!(find(complex.begin(), complex.end(), it->second) != complex.end())) {
				for (it1 = complex.begin(); it1 != complex.end(); ++it1) {
					// ���� ����� ������������ � ����� �� ��������� ����, �� ��� �� ���� ��������
					if ((it->second & *it1) != *it1 && (it->second & *it1) != 0) {
						noPrint = true;
						break;
					}
				}
			}
			if (noPrint) {
				continue;
			}
			fprintf(file, "%s ", it->first.c_str());
		}
	}
	fprintf(file, "\n");
}

void acl::printInheritance(BYTE flags)
{
	map<string, DWORD>::iterator it;
	bool printed = false;

	// ���������� ��� ����� ������������
	for (it = inheritanceOptions.begin(); it != inheritanceOptions.end(); it++)
	{
		if ((flags & it->second) == it->second) {
			fprintf(file, "%s ", it->first.c_str());
			printed = true;
		}
	}

	// �������, ���� ����������� ������
	if ((flags & INHERITED_ACE) == INHERITED_ACE) {
		fprintf(file, "ID ");
		printed = true;
	}

	if (printed) {
		fprintf(file, ", ");
	}
}

void acl::start(int argc, char *argv[], FILE * f)
{
	setlocale(LC_ALL, "Russian");
	file = f;

	if (argc == 1) {
		printHelp();
	}
	else if (parseParameters(argc, argv)) {
		if (paths.size() > 0) {
			for (vector<char*>::iterator it = paths.begin(); it != paths.end(); ++it)
			{
				// �������� ����� �� ���������
				if (printDebug && (isChangingRights || isRevoke)) {
					//printDaclInfoSDDL(*it);
					printDaclInfo(*it);
					getEffectiveRightsFromAcl(*it);
				}

				// �������� �����
				if (isChangingRights) {
					addACEs(*it);
				}
				// ������� ������ � ������
				else if (isRevoke) {
					revokeACEs(*it);
				}
				// �������� ������ ����������
				else if (printFullInfo) {
					//printDaclInfoSDDL(*it);
					printDaclInfo(*it);
					getEffectiveRightsFromAcl(*it);
				}
				// �������� ����������� ����� ������������
				else {
					getEffectiveRightsFromAcl(*it);
				}

				// �������� ����� ����� ���������
				if (printDebug && (isChangingRights || isRevoke)) {
					//printDaclInfoSDDL(*it);
					printDaclInfo(*it);
					getEffectiveRightsFromAcl(*it);
				}
			}
		}
	}
	else {
		fprintf(file, "Not valid command. Please, change it.\n\n");
		printHelp();
	}
	getchar();
}
void acl::GetAccess(AUTHZ_CLIENT_CONTEXT_HANDLE hAuthzClient, PSECURITY_DESCRIPTOR psd, char* path)
{
	AUTHZ_ACCESS_REQUEST AccessRequest = { 0 };
	AUTHZ_ACCESS_REPLY AccessReply = { 0 };
	BYTE     Buffer[1024];
	BOOL bRes = FALSE;  // assume error

						//  Do AccessCheck.
	AccessRequest.DesiredAccess = MAXIMUM_ALLOWED;
	AccessRequest.PrincipalSelfSid = NULL;
	AccessRequest.ObjectTypeList = NULL;
	AccessRequest.ObjectTypeListLength = 0;
	AccessRequest.OptionalArguments = NULL;

	RtlZeroMemory(Buffer, sizeof(Buffer));
	AccessReply.ResultListLength = 1;
	AccessReply.GrantedAccessMask = (PACCESS_MASK)(Buffer);
	AccessReply.Error = (PDWORD)(Buffer + sizeof(ACCESS_MASK));


	if (!AuthzAccessCheck(0,
		hAuthzClient,
		&AccessRequest,
		NULL,
		psd,
		NULL,
		0,
		&AccessReply,
		NULL)) {
		fprintf(file, "AuthzAccessCheck failed with %d\n", GetLastError());
	}
	else {
		fprintf(file, "\nEffective Rights to \"%s\" for user %s: ", path, username.c_str());
		printOptions(*(PACCESS_MASK)(AccessReply.GrantedAccessMask));
	}

	return;
}

BOOL acl::GetEffectiveRightsForUser(AUTHZ_RESOURCE_MANAGER_HANDLE hManager, PSECURITY_DESCRIPTOR psd, LPTSTR lpszUserName, char* path)
{
	PSID pSid = NULL;
	BOOL bResult = FALSE;
	LUID unusedId = { 0 };
	AUTHZ_CLIENT_CONTEXT_HANDLE hAuthzClientContext = NULL;

	pSid = ConvertNameToBinarySid(lpszUserName);
	if (pSid != NULL)
	{
		bResult = AuthzInitializeContextFromSid(0,
			pSid,
			hManager,
			NULL,
			unusedId,
			NULL,
			&hAuthzClientContext);
		if (bResult)
		{
			GetAccess(hAuthzClientContext, psd, path);
			AuthzFreeContext(hAuthzClientContext);
		}
		else
			fprintf(file, "AuthzInitializeContextFromSid failed with %d\n", GetLastError());
	}
	if (pSid != NULL)
	{
		LocalFree(pSid);
		pSid = NULL;
	}

	return bResult;
}

void acl::UseAuthzSolution(PSECURITY_DESCRIPTOR psd, char* path)
{
	AUTHZ_RESOURCE_MANAGER_HANDLE hManager;
	BOOL bResult = FALSE;

	bResult = AuthzInitializeResourceManager(AUTHZ_RM_FLAG_NO_AUDIT,
		NULL, NULL, NULL, NULL, &hManager);
	if (bResult)
	{
		bResult = GetEffectiveRightsForUser(hManager, psd, const_cast<char *>(username.c_str()), path);
		AuthzFreeResourceManager(hManager);
	}
	else
		fprintf(file, "AuthzInitializeResourceManager failed with %d\n", GetLastError());
}

PSID acl::ConvertNameToBinarySid(LPTSTR pAccountName)
{
	LPTSTR pDomainName = NULL;
	DWORD dwDomainNameSize = 0;
	PSID pSid = NULL;
	DWORD dwSidSize = 0;
	SID_NAME_USE sidType;
	BOOL fSuccess = FALSE;
	HRESULT hr = S_OK;

	__try
	{
		LookupAccountName(
			NULL,      // look up on local system
			pAccountName,
			pSid,              // buffer to receive name
			&dwSidSize,
			pDomainName,
			&dwDomainNameSize,
			&sidType);

		//  If the Name cannot be resolved, LookupAccountName will fail with
		//  ERROR_NONE_MAPPED.
		if (GetLastError() == ERROR_NONE_MAPPED)
		{
			fprintf(file, "LookupAccountName failed with %d\n", GetLastError());
			__leave;
		}
		else if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			pSid = (LPTSTR)LocalAlloc(LPTR, dwSidSize * sizeof(TCHAR));
			if (pSid == NULL)
			{
				fprintf(file, "LocalAlloc failed with %d\n", GetLastError());
				__leave;
			}

			pDomainName = (LPTSTR)LocalAlloc(LPTR, dwDomainNameSize * sizeof(TCHAR));
			if (pDomainName == NULL)
			{
				fprintf(file, "LocalAlloc failed with %d\n", GetLastError());
				__leave;
			}

			if (!LookupAccountName(
				NULL,      // look up on local system
				pAccountName,
				pSid,              // buffer to receive name
				&dwSidSize,
				pDomainName,
				&dwDomainNameSize,
				&sidType))
			{
				fprintf(file, "LookupAccountName failed with %d\n", GetLastError());
				__leave;
			}
		}

		//  Any other error code
		else
		{
			fprintf(file, "LookupAccountName failed with %d\n", GetLastError());
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

		//  Free pSid only if failed;
		//  otherwise, the caller has to free it after use.
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