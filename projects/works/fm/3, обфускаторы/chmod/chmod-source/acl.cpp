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

	//При вызове c GA почему-то в правах появилось F, а GW - W, GR - R, только мешают проверкам (при проверке 1 бит, а после вставки записи у неё много бит установлено), сейчас запрещены с помощью deniedOptions
	{ "GA", GENERIC_ALL },
	{ "GR", GENERIC_READ },
	{ "GW", GENERIC_WRITE },
	{ "GE", GENERIC_EXECUTE },

	{ "X", FILE_EXECUTE }, // FILE_EXECUTE или FILE_TRAVERSE
	{ "RD", FILE_READ_DATA }, // FILE_READ_DATA или FILE_LIST_DIRECTORY
	{ "WD", FILE_WRITE_DATA }, // FILE_WRITE_DATA или FILE_ADD_FILE
	{ "AD", FILE_APPEND_DATA }, // FILE_APPEND_DATA или FILE_ADD_SUBDIRECTORY
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

//При вызове c GA почему-то в правах появилось F, а GW - W, GR - R, только мешают проверкам (при проверке 1 бит, а после вставки записи у неё много бит установлено)
map<string, DWORD> acl::deniedOptions = {
	{ "GA", GENERIC_ALL },
	{ "GR", GENERIC_READ },
	{ "GW", GENERIC_WRITE },
	{ "GE", GENERIC_EXECUTE }
};

// Используется в BuildExplicitAccessWithName
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
	PSECURITY_DESCRIPTOR pSecurityDescriptor; // указатель на SD 
	DWORD dwRetCode, mask; // код возврата 
	PACL pDacl = NULL; // DACL
	char lpName[UNLEN], lpDomain[UNLEN];
	string typeAce;
	SID_NAME_USE SidType;
	DWORD dwSize = UNLEN;
	PSID sid;
	BYTE flags;

	printf("\nInfo about \"%s\"\n", path);

	// получаем дескриптор безопасности файла
	dwRetCode = GetNamedSecurityInfo(
		path, // имя файла 
		SE_FILE_OBJECT, // объект файл 
		DACL_SECURITY_INFORMATION, // читаем список DACL, владельца и группу
		NULL, // адрес указателя на SID владельца 
		NULL, // адрес указателя на первичную группу 
		&pDacl, // указатель на DACL
		NULL, // указатель на SACL не нужен 
		&pSecurityDescriptor); // адрес указателя на SD 
	if (dwRetCode != ERROR_SUCCESS) {
		printf("Get named security info failed.\n");
		printf("Error code: %u\n", dwRetCode);
		return dwRetCode;
	}

	void *lpAce = NULL; // указатель на элемент списка
	for (unsigned i = 0; i < pDacl->AceCount; ++i)
	{
		if (!GetAce(
			pDacl, // адрес DACL 
			i, // индекс элемента 
			&lpAce)) // указатель на элемент списка 
		{
			dwRetCode = GetLastError();
			printf("Get ace failed.\n");
			printf("Error code: %d\n", dwRetCode);
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

		// выводим на печать пользователя и права
		dwSize = UNLEN;
		memset(&lpName[0], 0, sizeof(lpName));
		memset(&lpDomain[0], 0, sizeof(lpDomain));
		// получаем имя пользователя
		if (!LookupAccountSid(NULL, sid, lpName, &dwSize, lpDomain, &dwSize, &SidType))
		{
			dwRetCode = GetLastError();
			if (dwRetCode == ERROR_NONE_MAPPED)
				strcpy_s(lpName, dwSize, "NONE_MAPPED");
			else
			{
				printf("LookupAccountSid Error %u\n", GetLastError());
				return dwRetCode;
			}
		}
		printf("For user %s\\%s flags, %s rights: ", lpDomain, lpName, typeAce.c_str());
		// печатаем права и флаги
		printInheritance(flags);
		printOptions(mask);
	}

	// освобождаем память 
	LocalFree(pSecurityDescriptor);
	return 0;
}

int acl::printDaclInfoSDDL(char* path)
{
	PSECURITY_DESCRIPTOR pSecurityDescriptor; // указатель на SD 
	LPSTR StringSecurityDescriptor; // строка с SD 
	DWORD StringSecurityDescriptorLen; // длина строки с SD 
	DWORD dwRetCode; // код возврата 
	PACL pDacl = NULL; // DACL
	PSID pSidOwner; // указатель на SID владельца объекта
	PSID pSidGroup; // указатель на SID первичной группы объекта 
	LPTSTR lpStringSid; // указатель на строку SID 

	printf("\Info about \"%s\"\n", path);

	// получаем дескриптор безопасности файла
	dwRetCode = GetNamedSecurityInfo(
		path, // имя файла 
		SE_FILE_OBJECT, // объект файл 
		OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION |
		DACL_SECURITY_INFORMATION, // читаем список DACL, владельца и группу
		&pSidOwner, // адрес указателя на SID владельца 
		&pSidGroup, // адрес указателя на первичную группу 
		&pDacl, // указатель на DACL
		NULL, // указатель на SACL не нужен 
		&pSecurityDescriptor); // адрес указателя на SD 
	if (dwRetCode != ERROR_SUCCESS) {
		printf("Get named security info failed.\n");
		printf("Error code: %u\n", dwRetCode);
		return dwRetCode;
	}

	// преобразуем дескриптор безопасности в строку 
	if (!ConvertSecurityDescriptorToStringSecurityDescriptor(
		pSecurityDescriptor, // адрес дескриптора безопасности 
		SDDL_REVISION_1, // версия языка описания 
		OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION |
		DACL_SECURITY_INFORMATION | SACL_SECURITY_INFORMATION,
		&StringSecurityDescriptor, // буфер для строки 
		&StringSecurityDescriptorLen)) // длина буфера 
	{
		dwRetCode = GetLastError();
		printf("Convert security descriptor to string security descriptor failed. Error code: %u\n", dwRetCode);
		return dwRetCode;
	}

	//printf("String security descriptor length: %u\n", StringSecurityDescriptorLen);
	//printf("String security desriptor: %s\n", StringSecurityDescriptor);
	// Форматируем дескриптор безопасности отступами и переносами
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
	printf("String security desriptor: %s\n", result.c_str());

	char lpName[UNLEN], lpDomain[UNLEN];;
	SID_NAME_USE SidType;
	DWORD dwSize = UNLEN;
	PSID sid;

	// Получаем и выводим имя владельца файла
	if (!LookupAccountSid(NULL, pSidOwner, lpName, &dwSize, lpDomain, &dwSize, &SidType))
	{
		dwRetCode = GetLastError();
		if (dwRetCode == ERROR_NONE_MAPPED)
			strcpy_s(lpName, dwSize, "NONE_MAPPED");
		else
		{
			printf("LookupAccountSid Error %u\n", GetLastError());
			return dwRetCode;
		}
	}
	printf("Owner is a member of the %s\\%s group\n", lpDomain, lpName);

	// освобождаем память 
	LocalFree(pSecurityDescriptor);
	LocalFree(StringSecurityDescriptor);
	return 0;
}

int acl::addACEs(char* path)
{
	EXPLICIT_ACCESS ea; // информация для элемента списка DACL 
	PACL lpOldDacl; // указатель на старый список DACL 
	PACL lpNewDacl; // указатель на новый список DACL 
	PSECURITY_DESCRIPTOR lpSD; // указатель на дескриптор безопасности 

	DWORD dwErrCode; // код возврата
	EXPLICIT_ACCESS s [2]; // массив с информацией об элементах списка DACL 
	int countAddedACEs = 0;

	// получаем SD
	dwErrCode = GetNamedSecurityInfo(
		path, // имя файла или каталога
		SE_FILE_OBJECT, // объект файл 
		DACL_SECURITY_INFORMATION, // получаем DACL 
		NULL, // адрес указателя на SID владельца 
		NULL, // адрес указателя на первичную группу 
		&lpOldDacl, // указатель на DACL 
		NULL, // указатель на SACL не нужен 
		&lpSD); // адрес указателя на дескриптор безопасности 
	if (dwErrCode != ERROR_SUCCESS)
	{
		printf("Get named security info failed.\n");
		printf("Error code: %u\n", dwErrCode);
		return dwErrCode;
	}

	dwErrCode = cleanDACL(lpOldDacl);
	if (dwErrCode != ERROR_SUCCESS)
	{
		perror("Cleaning DACL failed.\n");
		printf("The last error code: %u\n", dwErrCode);
		return dwErrCode;
	}

	// строим структуру EXPLICIT_ACCESS по имени
	if (grantRights > 0)
	{
		BuildExplicitAccessWithName(
			&ea, // адрес структуры ExplicitAccess 
			const_cast<char *>(username.c_str()), // имя учетной записи
			grantRights, // разрешенные права
			GRANT_ACCESS, // разрешить доступ 
			inheritanceFlags // флаги наследования 
		);
		s[countAddedACEs] = ea;
		countAddedACEs++;
	}
	if (denyRights > 0)
	{
		BuildExplicitAccessWithName(
			&ea, // адрес структуры ExplicitAccess 
			const_cast<char *>(username.c_str()), // имя учетной записи
			denyRights, // запрещённые права
			DENY_ACCESS, // запретить доступ
			inheritanceFlags // флаги наследования 
		);
		s[countAddedACEs] = ea;
		countAddedACEs++;
	}

	// создаем список DACL 
	dwErrCode = SetEntriesInAcl(countAddedACEs, // добавляем от одного до двух элементов в список DACL 
		s, // массив структур
		lpOldDacl, // адрес старого списка DACL 
		&lpNewDacl); // адрес указателя на новый список DACL

	if (dwErrCode != ERROR_SUCCESS)
	{
		perror("Set entries in DACL failed.\n");
		printf("The last error code: %u\n", dwErrCode); 
		return dwErrCode;
	}

	// устанавливаем новый список DACL 
	dwErrCode = SetNamedSecurityInfo(
		path, // имя файла 
		SE_FILE_OBJECT, // объект файл 
		DACL_SECURITY_INFORMATION, // изменяем список DACL 
		NULL, // владельца не изменяем 
		NULL, // первичную группу не изменяем 
		lpNewDacl, // новый DACL 
		NULL); // SACL не изменяем 
	if (dwErrCode != ERROR_SUCCESS)
	{
		printf("Set named security info failed.\n");
		printf("Error code: %u\n", dwErrCode);
		return dwErrCode;
	}

	// освобождаем память 
	LocalFree(lpSD);
	LocalFree(lpNewDacl);
	if (grantRights > 0 || denyRights > 0) {
		printf("The rights for user %s to \"%s\" were modified.\n", username.c_str(), path);
	}
	else {
		printf("The rights for user %s to \"%s\" have not been modified.\n", username.c_str(), path);
	}
	return 0;
}

int acl::cleanDACL(PACL lpOldDacl) 
{
	void *lpAce = NULL; // указатель на элемент списка 
	char lpName[UNLEN], lpDomain[UNLEN];
	SID_NAME_USE SidType;
	DWORD dwSize = UNLEN, dwErrCode, rights, flags;
	DWORD *mask;
	PSID sid;
	vector<int> deleteIndex; // Индексы элементов, которые нужно удалить
	vector<int>::reverse_iterator it;

	// удаляем или изменяем записи, противоречащие rights для пользоватля
	for (unsigned i = 0; i < lpOldDacl->AceCount; ++i)
	{
		// получить элемент списка DACL 
		if (!GetAce(
			lpOldDacl, // адрес DACL 
			i, // индекс элемента
			&lpAce)) // указатель на элемент списка
		{ 
			dwErrCode = GetLastError();
			printf("Get асе failed.\n");
			printf("Error code: %d\n", dwErrCode);
			return dwErrCode;
		}
		
		flags = ((ACE_HEADER*)lpAce)->AceFlags;
		if (((ACE_HEADER*)lpAce)->AceType == ACCESS_ALLOWED_ACE_TYPE) {
			if (denyRights == 0) continue;
			sid = &((ACCESS_ALLOWED_ACE*)lpAce)->SidStart;
			mask = &((ACCESS_ALLOWED_ACE*)lpAce)->Mask;
			// Если хотим запретить права, то убираем их из ALLOWED записи
			rights = denyRights;
		}
		else {
			if (grantRights == 0) continue;
			sid = &((ACCESS_DENIED_ACE*)lpAce)->SidStart;
			mask = &((ACCESS_DENIED_ACE*)lpAce)->Mask;
			// Если хотим разрешить права, то убираем их из DENIED записи
			rights = grantRights;
		}

		// Изменяем записи, связанные с пользователем для которого меняем права
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
				printf("LookupAccountSid Error %u\n", GetLastError());
				//continue;
				return dwErrCode;
			}
		}
		//printf("User of ACE is a member of the %s\\%s group\n", lpDomain, lpName);
		if (strcmp(lpName, username.c_str()) != 0) {
			continue;
		}

		// Если существует противоречащая по правам наследуемая запись, то мы не может её удалить, следовательно, не следует пытаться добавлять противоречивую запись
		if ((flags & INHERITED_ACE) == INHERITED_ACE && ((*mask & ~rights) == 0 || (*mask & ~rights) != *mask)) {
			printf("Sorry, but found problem with inherited ACE (crossed rights). Try at first to change rights of parent directories.\n");
			return - 1;
		}

		if ((*mask & ~rights) == 0) {
			deleteIndex.push_back(i);
		}
		else if ((*mask & ~rights) != *mask) {
			// Изменяем права, чтобы права в ALLOWED(DENIED) записи не противоречили правам, которые мы добавляем в DENIED(ALLOWED) запись
			*mask = (*mask & ~rights);
			//printf("Changed old ACE\n");
		}
	}
	for (it = deleteIndex.rbegin(); it != deleteIndex.rend(); ++it)
	{
		// удаляем элемент из списка DACL, потому что все права из этого элемента мы будет добавлять в DENIED(ALLOWED) запись
		if (!DeleteAce(lpOldDacl, *it))
		{
			dwErrCode = GetLastError();
			printf("Delete ace failed.\n");
			printf("Error code: %d\n", dwErrCode);
			return dwErrCode;
		}
		//printf("Deleted old ACE at index %d\n", *it);
	}
	return 0;
}

int acl::revokeACEs(char* path)
{
	EXPLICIT_ACCESS ea; // информация для элемента списка DACL 
	PACL lpOldDacl; // указатель на старый список DACL 
	PSECURITY_DESCRIPTOR lpSD; // указатель на дескриптор безопасности 
	DWORD dwErrCode; // код возврата
	vector<int> deleteIndexes; // Индексы элементов, которые нужно удалить
	vector<int>::reverse_iterator it;

	DWORD dwLengthOfSID = 0; // длина SID 
	DWORD dwLengthOfDomainName = 0; // длина имени домена 
	SID *userSID = NULL; // указатель на SID пользователя
	LPTSTR lpDomainName = NULL; // указатель на имя домена 
	SID_NAME_USE type_of_SID; // тип учетной записи 

	void *lpAce = NULL; // указатель на элемент списка 
	DWORD type; // тип ACE записи
	PSID sid; // указатель на SID записи

	// определяем длину SID пользователя 
	if (!LookupAccountName(
		NULL, // ищем имя на локальном компьютере 
		username.c_str(), // имя пользователя 
		NULL, // определяем длину SID 
		&dwLengthOfSID, // длина SID
		NULL, // определяем имя домена 
		&dwLengthOfDomainName, // длина имени домена 
		&type_of_SID)) // тип учетной записи 
		dwErrCode = GetLastError();
	if (dwErrCode == ERROR_INSUFFICIENT_BUFFER)
	{
		// распределяем память для SID и имени домена 
		userSID = (SID*) new char[dwLengthOfSID];
		lpDomainName = (LPTSTR) new wchar_t[dwLengthOfDomainName];
	}
	else
	{
		// выходим из программы 
		printf("Lookup account name failed.\n");
		printf("Error code: %d\n", dwErrCode);
		return dwErrCode;
	}

	// определяем SID и имя домена пользователя 
	if (!LookupAccountName(
		NULL, // ищем имя на локальном компьютере 
		username.c_str(), // имя пользователя 
		userSID, // указатель на SID 
		&dwLengthOfSID, // длина SID 
		lpDomainName, // указатель на имя домена 
		&dwLengthOfDomainName, // длина имени домена 
		&type_of_SID)) // тип учетной записи 
	{
		dwErrCode = GetLastError();
		printf("Lookup account name failed.\n");
		printf("Error code: %d\n", dwErrCode);
		return dwErrCode;
	}
	/*
	// выводим SID на консоль 
	printf("SID revision: %u\n", userSID->Revision);
	printf("SubAuthorityCount: %u\n", userSID->SubAuthorityCount);
	printf("IdentifierAuthority: ");
	for (int i = 0; i < 6; ++i)
		printf("%u ", userSID->IdentifierAuthority.Value[i]);
	printf("\n");
	printf("SubAuthorities: ");
	for (int i = 0; i < userSID->SubAuthorityCount; ++i)
		printf("%u ", userSID->SubAuthority[i]);
	printf("\n");
	// выводим имя домена 
	printf("Domain name: %s\n", lpDomainName);
	*/

	// получаем SD
	dwErrCode = GetNamedSecurityInfo(
		path, // имя файла или каталога
		SE_FILE_OBJECT, // объект файл 
		DACL_SECURITY_INFORMATION, // получаем DACL 
		NULL, // адрес указателя на SID владельца 
		NULL, // адрес указателя на первичную группу 
		&lpOldDacl, // указатель на DACL 
		NULL, // указатель на SACL не нужен 
		&lpSD); // адрес указателя на дескриптор безопасности 
	if (dwErrCode != ERROR_SUCCESS)
	{
		printf("Get named security info failed.\n");
		printf("Error code: %u\n", dwErrCode);
		return dwErrCode;
	}

	// удаляем записи, связанные с пользователем
	for (unsigned i = 0; i < lpOldDacl->AceCount; ++i)
	{
		// получить элемент списка DACL 
		if (!GetAce(
			lpOldDacl, // адрес DACL 
			i, // индекс элемента
			&lpAce)) // указатель на элемент списка
		{
			dwErrCode = GetLastError();
			printf("Get асе failed.\n");
			printf("Error code: %d\n", dwErrCode);
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

		// Если sid у записи равен SID пользователя, то добавляемый индекс записи в список удаляемых
		deleteIndexes.push_back(i);
	}
	for (it = deleteIndexes.rbegin(); it != deleteIndexes.rend(); ++it)
	{
		// удаляем элемент из списка DACL, которые связаны с пользователем
		if (!DeleteAce(lpOldDacl, *it))
		{
			dwErrCode = GetLastError();
			printf("Delete ace failed.\n");
			printf("Error code: %d\n", dwErrCode);
			return dwErrCode;
		}
		//printf("Deleted old ACE at index %d\n", *it);
	}

	// устанавливаем новый список DACL 
	dwErrCode = SetNamedSecurityInfo(
		path, // имя файла 
		SE_FILE_OBJECT, // объект файл 
		DACL_SECURITY_INFORMATION, // изменяем список DACL 
		NULL, // владельца не изменяем 
		NULL, // первичную группу не изменяем 
		lpOldDacl, // новый DACL 
		NULL); // SACL не изменяем 
	if (dwErrCode != ERROR_SUCCESS)
	{
		printf("Set named security info failed.\n");
		printf("Error code: %u\n", dwErrCode);
		return dwErrCode;
	}

	// освобождаем память 
	LocalFree(lpSD);
	LocalFree(lpOldDacl);
	delete[] lpDomainName;
	delete[] userSID;
	if (deleteIndexes.size() > 0) {
		printf("The revoke of \"%s\" for user %s was made.\n", path, username.c_str());
	}
	return 0;
}

int acl::getEffectiveRightsFromAcl(char* path)
{
	TRUSTEE Trustee; // информация об учетной записи 
	PSECURITY_DESCRIPTOR lpSd; // указатель на дескриптор безопасности 
	PACL lpDacl; // указатель на список DACL 
	ACCESS_MASK dwAccessRights = 0; // права доступа 
	DWORD dwErrCode; // код возврата 

	dwErrCode = GetNamedSecurityInfo(
		path, // имя файла 
		SE_FILE_OBJECT, // объект файл 
		OWNER_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION, // получаем DACL 
		NULL, // адрес указателя на SID владельца 
		NULL, // адрес указателя на первичную группу 
		&lpDacl, // указатель на DACL 
		NULL, // указатель на SACL не нужен 
		&lpSd); // адрес указателя на дескриптор безопасности 
	if (dwErrCode != ERROR_SUCCESS)
	{
		printf("Get named security info failed.\n");
		printf("Error code: %u\n", dwErrCode);
		return dwErrCode;
	}

	// строим структуру TRUSTEE по имени
	BuildTrusteeWithName(&Trustee, const_cast<char *>(username.c_str()));

	// получаем права доступа для заданного имени 
	dwErrCode = GetEffectiveRightsFromAcl(
		lpDacl, // указатель на список DACL 
		&Trustee, // адрес структуры TRUSTEE 
		&dwAccessRights); // адрес маски с флагами 
	if (dwErrCode != ERROR_SUCCESS)
	{
		printf("\nGet effective rights from ACL failed.\n");
		printf("Error code: %u\n", dwErrCode);
		// Вторая попытка вывести эффективные права
		UseAuthzSolution(lpSd, path);
		return dwErrCode;
	}

	// выводим на консоль права доступа 
	/*printf("Effective ACCESS MASK: %x\nBinary Effective ACCESS MASK: ", dwAccessRights);
	cout << bitset<32>(dwAccessRights) << endl;*/
	printf("\nEffective Rights to \"%s\" for user %s: ", path, username.c_str());
	printOptions(dwAccessRights);

	// освобождаем память 
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
	inheritanceFlags = NO_INHERITANCE; // нет наследования

	for (int i = 1; i < argc; i++)
	{
		//printf("Arg: %d\nValue: %s\n", i, argv[i]);
		// Разбираем опции переданные после /grant и /deny
		if (strcmp(argv[i], "/grant") == 0 || strcmp(argv[i], "/deny") == 0) {
			isGrant = strcmp(argv[i], "/grant") == 0;
			if (i == (argc - 1)) {
				printf("After /grant or /deny must be options\n");
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
				//printf("Option %s\n", option.c_str());

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
					printf("Bad option %s\n", option.c_str());
					result = false;
				}

				if (pos == string::npos) {
					break;
				}
				temp.erase(0, pos + 1);
				pos = temp.find(",");
			}
		}
		// Устанавливаем флаг, обозначающий, что будут удаляться записи
		else if (strcmp(argv[i], "/revoke") == 0) {
			isRevoke = true;
		}
		// Получаем переданное имя пользователя
		else if (strcmp(argv[i], "/u") == 0) {
			if (username.length() > 0) {
				printf("You can specify just one username\n");
				return false;
			}
			if (i == (argc - 1)) {
				printf("After /u must be username\n");
				return false;
			}
			username = argv[i+1];
			if (!isExistsUser(argv[i + 1])) {
				printf("There are not found user with username %s!\n", argv[i + 1]);
				result = false;
			}
			i++;
		}
		// Устанавливаем флаг, обозначающий, что нужно выводить информацию до и после изменений
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
	//printf("Username - %s\n", username.c_str());

	/*if (paths.size() > 0) {
		printf("The contents of paths are:\n");
		for (vector<char*>::iterator it = paths.begin(); it != paths.end(); ++it)
			printf("%s\n", *it);
	}*/

	//Проверки на противоречия и ошибки в параметрах
	if (paths.size() == 0 && (isRevoke || isChangingRights)) {
		printf("You must select the files or directories. Please, fix it.\n");
		result = false;
	}

	if (isRevoke && isChangingRights) {
		printf("You must use grant and deny options separately from revoke. Please, fix it.\n");
		result = false;
	}

	if ((grantRights & denyRights) > 0) {
		printf("There are crossed options. Please, fix it.\n");
		printOptions(grantRights & denyRights);
		result = false;
	}

	if (isChangingRights && inheritanceFlags > 0 && grantRights == 0 && denyRights == 0) {
		printf("There are just inheritance flags. Please, specify rights.\n");
		result = false;
	}

	if (inheritanceFlags > 0) {
		for (vector<char*>::iterator it = paths.begin(); it != paths.end(); ++it) {
			if (!isDirectory(*it)) {
				result = false;
				printf("You must use inheritance flags with directories, but \"%s\" is not directory.\n", *it);
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

	// Получаем полный путь к файлу
	retval = GetFullPathName(name,
		BUFSIZE,
		buffer,
		lppPart);

	if (retval == 0)
	{
		printf("GetFullPathName failed (%d)\n", GetLastError());
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

	// Проверяем, что файл или директория существует
	int res = PathFileExists(buffer);
	if (res == 1)
	{
		//printf("The requested \"%s\" is a valid file or directory\n", buffer);
		isExists = true;
	}
	else
	{
		printf("The requested \"%s\" is not a valid file or directory\n", buffer);
	}
	return isExists;
}

bool acl::isDirectory(char* name)
{
	DWORD dwAttrs = GetFileAttributes(name);

	// Проверка, что это директория
	if ((dwAttrs & FILE_ATTRIBUTE_DIRECTORY))
	{
		return true;
	}
	return false;
}

bool acl::isExistsUser(char* name)
{
	DWORD dwLengthOfSID = 0; // длина SID 
	DWORD dwLengthOfDomainName = 0; // длина имени домена 
	SID *userSID = NULL; // указатель на SID пользователя
	LPTSTR lpDomainName = NULL; // указатель на имя домена 
	SID_NAME_USE type_of_SID; // тип учетной записи 
	DWORD dwErrCode;

	// определяем длину SID пользователя 
	if (!LookupAccountName(
		NULL, // ищем имя на локальном компьютере 
		username.c_str(), // имя пользователя 
		NULL, // определяем длину SID 
		&dwLengthOfSID, // длина SID
		NULL, // определяем имя домена 
		&dwLengthOfDomainName, // длина имени домена 
		&type_of_SID)) // тип учетной записи 
		dwErrCode = GetLastError();
	if (dwErrCode == ERROR_INSUFFICIENT_BUFFER)
	{
		// распределяем память для SID и имени домена 
		userSID = (SID*) new char[dwLengthOfSID];
		lpDomainName = (LPTSTR) new wchar_t[dwLengthOfDomainName];
	}
	else
	{
		// выходим из программы 
		printf("Lookup account name failed.\n");
		printf("Error code: %d\n", dwErrCode);
		return false;
	}

	// определяем SID и имя домена пользователя 
	if (!LookupAccountName(
		NULL, // ищем имя на локальном компьютере 
		username.c_str(), // имя пользователя 
		userSID, // указатель на SID 
		&dwLengthOfSID, // длина SID 
		lpDomainName, // указатель на имя домена 
		&dwLengthOfDomainName, // длина имени домена 
		&type_of_SID)) // тип учетной записи 
	{
		dwErrCode = GetLastError();
		printf("Lookup account name failed.\n");
		printf("Error code: %d\n", dwErrCode);
		return false;
	}

	delete[] lpDomainName;
	delete[] userSID;
	return true;
}

void acl::printHelp()
{
	printf("Examples of commands:\n");
	printf("\t/grant d,wdac /u User1 file1 file2 - grant rights for User1\n");
	printf("\t/deny d,wdac /u User1 file1 file2 - deny rights for User1\n");
	printf("\t/grant d,wdac file1 file2 - grant rights for current user\n");
	printf("\t/deny d,wdac file1 file2 - deny rights for current user\n");
	printf("\t/revoke /u User1 file1 file2 - revoke rights for User1\n");
	printf("\t/revoke file1 file2 - revoke rights for current user\n");
	printf("\t/u User1 file1 file2 - print effective rights for User1\n");
	printf("\tfile1 file2 - print effective rights for current user and full info\n");
	printf("/grant and /deny can be used together\n");
	printf("/d can be used with /grant, /deny and /revoke for output rights before and after changes\n");
	printf("Inheritance flags are applied only to directories, do not try use them with files.\n");
	printf("File names with spaces must be in quotes: \"file with space\"\n");
}

void acl::printOptions(DWORD mask)
{
	// Эти векторы используются, для того чтобы исключить из вывода права, которые включены в более сложные комбинации (к примеру, FILE_GENERIC_READ)
	vector<DWORD> complex;
	vector<DWORD> variants = { FILE_ALL_ACCESS, FILE_GENERIC_EXECUTE | FILE_GENERIC_READ, FILE_GENERIC_READ, FILE_GENERIC_WRITE};
	vector<DWORD>::iterator it1, it2;
	map<string, DWORD>::iterator it;
	bool noPrint = false;

	// Накапливаем сложные сочетания прав, присутствующие в переданной маске
	for (it1 = variants.begin(); it1 != variants.end(); ++it1)
	{
		if (*it1 == FILE_GENERIC_READ && find(complex.begin(), complex.end(), FILE_GENERIC_EXECUTE | FILE_GENERIC_READ) != complex.end()) continue;
		if ((mask & *it1) == *it1) {
			complex.push_back(*it1);
			if (*it1 == FILE_ALL_ACCESS) break;
		}
	}

	//cout << bitset<32>(mask) << endl;

	// Перебираем все возможные опции прав
	for (it = optionsRights.begin(); it != optionsRights.end(); it++)
	{
		noPrint = false;
		// Проверка на то, что в маске содержатся права
		if ((mask & it->second) == it->second) {
			// Если опция присутствует в complex, то сразу печатаем
			if (!(find(complex.begin(), complex.end(), it->second) != complex.end())) {
				for (it1 = complex.begin(); it1 != complex.end(); ++it1) {
					// Если право присутствует в одном из сочетаний прав, то его не надо печатать
					if ((it->second & *it1) != *it1 && (it->second & *it1) != 0) {
						noPrint = true;
						break;
					}
				}
			}
			if (noPrint) {
				continue;
			}
			printf("%s ", it->first.c_str());
		}
	}
	printf("\n");
}

void acl::printInheritance(BYTE flags)
{
	map<string, DWORD>::iterator it;
	bool printed = false;

	// Перебираем все флаги наследования
	for (it = inheritanceOptions.begin(); it != inheritanceOptions.end(); it++)
	{
		if ((flags & it->second) == it->second) {
			printf("%s ", it->first.c_str());
			printed = true;
		}
	}

	// Выводим, если наследуемая запись
	if ((flags & INHERITED_ACE) == INHERITED_ACE) {
		printf("ID ");
		printed = true;
	}

	if (printed) {
		printf(", ");
	}
}

void acl::start(int argc, char *argv[])
{
	setlocale(LC_ALL, "Russian");

	if (argc == 1) {
		printHelp();
	}
	else if (parseParameters(argc, argv)) {
		if (paths.size() > 0) {
			for (vector<char*>::iterator it = paths.begin(); it != paths.end(); ++it)
			{
				// Печатаем права до изменений
				if (printDebug && (isChangingRights || isRevoke)) {
					//printDaclInfoSDDL(*it);
					printDaclInfo(*it);
					getEffectiveRightsFromAcl(*it);
				}

				// Изменяем права
				if (isChangingRights) {
					addACEs(*it);
				}
				// Удаляем записи о правах
				else if (isRevoke) {
					revokeACEs(*it);
				}
				// Печатаем полную информацию
				else if (printFullInfo) {
					//printDaclInfoSDDL(*it);
					printDaclInfo(*it);
					getEffectiveRightsFromAcl(*it);
				}
				// Печатаем эффективные права пользователя
				else {
					getEffectiveRightsFromAcl(*it);
				}

				// Печатаем права после изменений
				if (printDebug && (isChangingRights || isRevoke)) {
					//printDaclInfoSDDL(*it);
					printDaclInfo(*it);
					getEffectiveRightsFromAcl(*it);
				}
			}
		}
	}
	else {
		printf("Not valid command. Please, change it.\n\n");
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
		wprintf_s(L"AuthzAccessCheck failed with %d\n", GetLastError());
	}
	else {
		printf("\nEffective Rights to \"%s\" for user %s: ", path, username.c_str());
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
			wprintf_s(L"AuthzInitializeContextFromSid failed with %d\n", GetLastError());
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
		wprintf_s(L"AuthzInitializeResourceManager failed with %d\n", GetLastError());
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
			wprintf_s(L"LookupAccountName failed with %d\n", GetLastError());
			__leave;
		}
		else if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			pSid = (LPTSTR)LocalAlloc(LPTR, dwSidSize * sizeof(TCHAR));
			if (pSid == NULL)
			{
				wprintf_s(L"LocalAlloc failed with %d\n", GetLastError());
				__leave;
			}

			pDomainName = (LPTSTR)LocalAlloc(LPTR, dwDomainNameSize * sizeof(TCHAR));
			if (pDomainName == NULL)
			{
				wprintf_s(L"LocalAlloc failed with %d\n", GetLastError());
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
				wprintf_s(L"LookupAccountName failed with %d\n", GetLastError());
				__leave;
			}
		}

		//  Any other error code
		else
		{
			wprintf_s(L"LookupAccountName failed with %d\n", GetLastError());
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