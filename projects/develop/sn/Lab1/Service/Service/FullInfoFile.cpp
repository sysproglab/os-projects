#include "stdafx.h"
#include "LibFullInfoFile.h"
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <stdexcept>
#include <fstream>
#include <iomanip>
#include <RestartManager.h>
#pragma comment(lib,"Rstrtmgr.lib")


#define Is2power(x) (!(x & (x - 1)))
#define ALIGN_DOWN(x, align) (x & ~(align - 1))
#define ALIGN_UP(x, align) ((x & (align - 1)) ? ALIGN_DOWN(x, align) + align : x) 
using namespace std;

#define BUFSIZE MAX_PATH

namespace FullInfoFile
{
	int MyFullInfoFile::infoFile(_TCHAR* argv)
	{
		TCHAR Path[BUFSIZE];
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;
		DWORD dwAttrs;
		_TCHAR* temp;
		BY_HANDLE_FILE_INFORMATION lpFileInformation;

		HANDLE hFile = CreateFile(argv,
			//HANDLE hFile = CreateFile(L"D:\\getFileInfo.exe",       // file name 
			GENERIC_READ,          // open for reading 
			0,                     // do not share 
			NULL,                  // default security 
			OPEN_EXISTING,         // existing file only 
			FILE_ATTRIBUTE_NORMAL, // normal file 
			NULL);

		if (GetFileInformationByHandle(hFile, &lpFileInformation))
		{
			printf("File Index : %d%d \n", lpFileInformation.nFileIndexLow, lpFileInformation.nFileIndexHigh);
			printf("Number Of Links : %d \n", lpFileInformation.nNumberOfLinks);
			//printf("Volume Serial Number : %d \n", lpFileInformation.dwVolumeSerialNumber);
		}
		else
			printf("GetFileInformationByHandle ERROR: %s\r\n", strerror(GetLastError()));
		DWORD dwFileType;
		dwFileType = GetFileType(hFile);
		switch (dwFileType)
		{
		case FILE_TYPE_CHAR:	printf("File Type: %s\r\n", "FILE_TYPE_CHAR");	break;
		case FILE_TYPE_DISK:	printf("File Type: %s\r\n", "FILE_TYPE_DISK");	break;
		case FILE_TYPE_PIPE:	printf("File Type: %s\r\n", "FILE_TYPE_PIPE");	break;
		case FILE_TYPE_REMOTE:	printf("File Type: %s\r\n", "FILE_TYPE_REMOTE"); break;
		case FILE_TYPE_UNKNOWN:	printf("File Type: %s\r\n", "FILE_TYPE_UNKNOWN"); break;
		}
		DWORD dwRet;
		dwRet = GetFinalPathNameByHandle(hFile, Path, BUFSIZE, VOLUME_NAME_DOS);
		if (dwRet < BUFSIZE)
		{
			_tprintf(TEXT("\nThe final path is: %s\n"), Path);
		}
		else printf("\nThe required buffer size is %d.\n", dwRet);


		DWORD  retval = 0;
		TCHAR  buffer[BUFSIZE] = TEXT("");
		TCHAR** lppPart = { NULL };
		retval = GetFullPathName(argv,
			//retval = GetFullPathName(L"D:\\getFileInfo.exe",
			BUFSIZE,
			buffer,
			lppPart);

		if (retval == 0)
		{
			// Handle an error condition.
			printf("GetFullPathName failed (%d)\n", GetLastError());
			return 1;
		}
		else
		{
			_tprintf(TEXT("The full path name is:  %s\n"), buffer);
			if (lppPart != NULL && *lppPart != 0)
			{
				_tprintf(TEXT("The final component in the path name is:  %s\n"), *lppPart);
			}
		}



		CloseHandle(hFile);
		hFind = FindFirstFile(argv, &FindFileData);
		//hFind = FindFirstFile(TEXT("D:\\getFileInfo.exe")/*argv[1]*/, &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			printf("FindFirstFile failed (%d)\n", GetLastError());
			return 1;
		}
		else
		{
			dwAttrs = GetFileAttributes(TEXT("d:\\getFileInfo.exe"));
			if (dwAttrs == INVALID_FILE_ATTRIBUTES)
			{
				printf("INVALID_FILE_ATTRIBUTES\n");
			}
			else
			{
				printf("\nFile attributes:");
				if (dwAttrs & FILE_ATTRIBUTE_ARCHIVE)				printf("\nThe attribute FILE_ATTRIBUTE_ARCHIVE is established");
				if (dwAttrs & FILE_ATTRIBUTE_COMPRESSED)			printf("\nThe attribute FILE_ATTRIBUTE_COMPRESSED is established");
				if (dwAttrs & FILE_ATTRIBUTE_DEVICE)				printf("\nThe attribute FILE_ATTRIBUTE_DEVICE is established");
				if (dwAttrs & FILE_ATTRIBUTE_DIRECTORY)				printf("\nThe attribute FILE_ATTRIBUTE_DIRECTORY is established");
				if (dwAttrs & FILE_ATTRIBUTE_ENCRYPTED)				printf("\nThe attribute FILE_ATTRIBUTE_ENCRYPTED is established");
				if (dwAttrs & FILE_ATTRIBUTE_HIDDEN)				printf("\nThe attribute FILE_ATTRIBUTE_HIDDEN is established");
				if (dwAttrs & FILE_ATTRIBUTE_NORMAL)				printf("\nThe attribute FILE_ATTRIBUTE_NORMAL is established");
				if (dwAttrs & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)	printf("\nThe attribute FILE_ATTRIBUTE_NOT_CONTENT_INDEXED is established");
				if (dwAttrs & FILE_ATTRIBUTE_OFFLINE)				printf("\nThe attribute FILE_ATTRIBUTE_OFFLINE is established");
				if (dwAttrs & FILE_ATTRIBUTE_READONLY)				printf("\nThe attribute FILE_ATTRIBUTE_READONLY is established");
				if (dwAttrs & FILE_ATTRIBUTE_REPARSE_POINT)			printf("\nThe attribute FILE_ATTRIBUTE_REPARSE_POINT is established");
				if (dwAttrs & FILE_ATTRIBUTE_SPARSE_FILE)			printf("\nThe attribute FILE_ATTRIBUTE_SPARSE_FILE is established");
				if (dwAttrs & FILE_ATTRIBUTE_SYSTEM)				printf("\nThe attribute FILE_ATTRIBUTE_SYSTEM is established");
				if (dwAttrs & FILE_ATTRIBUTE_TEMPORARY)				printf("\nThe attribute FILE_ATTRIBUTE_ATTRIBUTE_TEMPORARY is established");
			}

			WIN32_FILE_ATTRIBUTE_DATA    fileInfo;
			if (GetFileAttributesEx(argv, GetFileExInfoStandard, &fileInfo))
				//if (GetFileAttributesEx(L"d:\\getFileInfo.exe", GetFileExInfoStandard, &fileInfo))
			{

				unsigned long int fileSize = (fileInfo.nFileSizeHigh * (MAXDWORD + 1)) + fileInfo.nFileSizeLow;
				printf("\nFile size= %d bytes", fileSize);

				SYSTEMTIME stLocal;

				FileTimeToSystemTime(&fileInfo.ftCreationTime, &stLocal);
				printf("\nFile creation time: %02d/%02d/%d  %02d:%02d", stLocal.wMonth, stLocal.wDay, stLocal.wYear, stLocal.wHour, stLocal.wMinute);

				FileTimeToSystemTime(&fileInfo.ftLastAccessTime, &stLocal);
				printf("\nFile last access time: %02d/%02d/%d  %02d:%02d", stLocal.wMonth, stLocal.wDay, stLocal.wYear, stLocal.wHour, stLocal.wMinute);

				FileTimeToSystemTime(&fileInfo.ftLastWriteTime, &stLocal);
				printf("\nFile last write time: %02d/%02d/%d  %02d:%02d", stLocal.wMonth, stLocal.wDay, stLocal.wYear, stLocal.wHour, stLocal.wMinute);
			}
			else
				printf("file not found");
			DWORD fileEncryptionStatus;
			LPDWORD lpFileInformation = &fileEncryptionStatus;

			if (FileEncryptionStatus(argv, &fileEncryptionStatus))
				//if (FileEncryptionStatus(L"d:\\getFileInfo.exe", &fileEncryptionStatus))
			{
				switch (fileEncryptionStatus)
				{
				case 0: printf("\nFILE_ENCRYPTABLE");		break;
				case 1: printf("\nFILE_IS_ENCRYPTED");		break;
				case 8: printf("\nFILE_READ_ONLY");			break;
				case 3: printf("\nFILE_ROOT_DIR");			break;
				case 2: printf("\nFILE_SYSTEM_ATTR");		break;
				case 4: printf("\nFILE_SYSTEM_DIR");		break;
				case 6: printf("\nFILE_SYSTEM_NOT_SUPPORT"); break;
				case 5: printf("\nFILE_UNKNOWN");			break;
				case 7: printf("\nFILE_USER_DISALLOWED");	break;
				}
				printf("\n");
			}
			else
				printf("file not found");
		}


		//printf("GetFileInformationByHandle ERROR: %s\r\n", strerror(GetLastError()));
		DWORD fileBinaryType;
		LPDWORD lpBinaryType = &fileBinaryType;
		//printf("\ntest: %d", GetBinaryType(L"d:\\getFileInfo.exe", &fileBinaryType));
		if (GetBinaryType(argv, &fileBinaryType))
			//if (GetBinaryType(L"D:\\getFileInfo.exe", &fileBinaryType))
		{
			//������� ���� ������� PE � �������� ������
			std::ifstream pefile;
			pefile.open(argv, std::ios::in | std::ios::binary);
			//pefile.open(L"D:\\getFileInfo.exe", std::ios::in | std::ios::binary);
			if (!pefile.is_open())
			{
				//���� ����� ��� ������� �� �������, �� ������� ������ � ������
				std::cout << "\nCan't open file. Not .exe" << std::endl;
				goto label;
			}

			//��������� ������ �����, �� ��� ���������� ������
			pefile.seekg(0, std::ios::end);
			//��� ����� ��������� �������� ��������� ������ � ����� ����� �����, ������� ��� �������
			std::streamoff filesize = pefile.tellg();
			//��� � ����� ������ ����� � ������
			//����� ������ �������� ��������� � ������ �����
			pefile.seekg(0);
			IMAGE_DOS_HEADER dos_header;
			pefile.read(reinterpret_cast<char*>(&dos_header), sizeof(IMAGE_DOS_HEADER));
			if (pefile.bad() || pefile.eof())
			{
				//���� ����� ������� �� �������...
				std::cout << "Unable to read IMAGE_DOS_HEADER" << std::endl;
				goto label;
			}

			//������ ��� ����� ��������� ������ ���� MZ, ��, ��� ��� � x86 � ��� �������� ������� ���������� ������,
			//�� ���������� ��� ����� �� ��������� 'ZM'
			if (dos_header.e_magic != 'ZM')
			{
				std::cout << "IMAGE_DOS_HEADER signature is incorrect" << std::endl;
				goto label;
			}

			//������ ��������� ������ PE-����� (IMAGE_NT_HEADERS) ������ ����
			//��������� �� �������� �������� ����� (DWORD)
			//��������, ��� ��� ���
			if ((dos_header.e_lfanew % sizeof(DWORD)) != 0)
			{
				//� ����� ��� PE-���� �����������
				std::cout << "PE header is not DWORD-aligned" << std::endl;
				goto label;
			}
			//��������� �� ��������� IMAGE_NT_HEADERS � ��������� ������� ��
			pefile.seekg(dos_header.e_lfanew);
			if (pefile.bad() || pefile.fail())
			{
				std::cout << "Cannot reach IMAGE_NT_HEADERS" << std::endl;
				goto label;
			}

			//������
			//������ ����� ������ ����� ��������� IMAGE_NT_HEADERS
			//��� ���� ����������
			//��� ��� � �� ����������� ������
			IMAGE_NT_HEADERS32 nt_headers;
			pefile.read(reinterpret_cast<char*>(&nt_headers), sizeof(IMAGE_NT_HEADERS32)-sizeof(IMAGE_DATA_DIRECTORY)* 16);
			if (pefile.bad() || pefile.eof())
			{
				std::cout << "Error reading IMAGE_NT_HEADERS32" << std::endl;
				goto label;
			}
			//���������, ��� ��� ���� - PE
			//��������� � ���� ������ ���� "PE\0\0"
			//������ ��� �������� ������� ������ � ���������...
			if (nt_headers.Signature != 'EP')
			{
				std::cout << "Incorrect PE signature" << std::endl;
				goto label;
			}

			//���������, ��� ��� PE32
			if (nt_headers.OptionalHeader.Magic != 0x10B)
			{
				std::cout << "This PE is not PE32" << std::endl;
				goto label;
			}
			//������� � ����� ������� ������ - ��� ������ ���� ���������� ���������
			//(������� ���-����, ���� �� ���� � ��� ���� ����������, ���� ��� ����)
			DWORD first_section = dos_header.e_lfanew + nt_headers.FileHeader.SizeOfOptionalHeader + sizeof(IMAGE_FILE_HEADER)+sizeof(DWORD) /* Signature */;

			//��������� �� ������ ������ � ������� ������
			pefile.seekg(first_section);
			if (pefile.bad() || pefile.fail())
			{
				std::cout << "Cannot reach section headers" << std::endl;
				goto label;
			}
			std::cout << std::hex << std::showbase << std::left;
			for (int i = 0; i < nt_headers.FileHeader.NumberOfSections; i++)
			{
				//������� ��������� ������
				IMAGE_SECTION_HEADER header;
				//� ������ ���
				pefile.read(reinterpret_cast<char*>(&header), sizeof(IMAGE_SECTION_HEADER));
				if (pefile.bad() || pefile.eof())
				{
					std::cout << "Error reading section header" << std::endl;
					goto label;
				}
				//��-������, "�����" ������ ������ � ����������� ������ ������
				//�� ����� ���� ������������ ��������
				if (!header.SizeOfRawData && !header.Misc.VirtualSize)
				{
					std::cout << "Virtual and Physical sizes of section can't be 0 at the same time" << std::endl;
					goto label;
				}

				//���� ������ ������������������ ������ ("�����") �� ����� ����...
				if (header.SizeOfRawData != 0)
				{
					//��������, ��� ������������������ ������ ������ ����� �� �������� �� ������� ������ PE-�����
					if (ALIGN_DOWN(header.PointerToRawData, nt_headers.OptionalHeader.FileAlignment) + header.SizeOfRawData > filesize)
					{
						std::cout << "Incorrect section address or size" << std::endl;
						goto label;
					}

					//� ���� ���������� �� �������� ����������� ����������� ������ ������
					DWORD virtual_size_aligned;

					//���� ����������� ������ ������ ��� ��������� � ����,
					if (header.Misc.VirtualSize == 0)
						//�� �� ����������� ����������� ������ ����� �� ��������� ������� ������������������ ������,
						//������������ �� ������� SectionAlignment
						virtual_size_aligned = ALIGN_UP(header.SizeOfRawData, nt_headers.OptionalHeader.SectionAlignment);
					else
						//� ����� �� ����� �� ������������ �������,
						//������������ �� ������� SectionAlignment
						virtual_size_aligned = ALIGN_UP(header.Misc.VirtualSize, nt_headers.OptionalHeader.SectionAlignment);
					//��������, ��� ����������� ������������ ������ �� �������� �� ������� ������������ ������������ ����� PE-�����
					if (header.VirtualAddress + virtual_size_aligned > ALIGN_UP(nt_headers.OptionalHeader.SizeOfImage, nt_headers.OptionalHeader.SectionAlignment))
					{
						std::cout << "Incorrect section address or size" << std::endl;
						goto label;
					}
				}
				//��� ������ ����� ����� ������ �� 8 ��������
				char name[9] = { 0 };
				memcpy(name, header.Name, 8);
				//������� ��� ������
				std::cout << std::setw(20) << "Section: " << name << std::endl << "=======================" << std::endl;
				//�� �������, ������
				std::cout << std::setw(20) << "Virtual size:" << header.Misc.VirtualSize << std::endl;
				std::cout << std::setw(20) << "Raw size:" << header.SizeOfRawData << std::endl;
				std::cout << std::setw(20) << "Virtual address:" << header.VirtualAddress << std::endl;
				std::cout << std::setw(20) << "Raw address:" << header.PointerToRawData << std::endl;

				//� ����� ������ ��������������
				std::cout << std::setw(20) << "Characteristics: ";
				if (header.Characteristics & IMAGE_SCN_MEM_READ)
					std::cout << "R ";
				if (header.Characteristics & IMAGE_SCN_MEM_WRITE)
					std::cout << "W ";
				if (header.Characteristics & IMAGE_SCN_MEM_EXECUTE)
					std::cout << "X ";
				if (header.Characteristics & IMAGE_SCN_MEM_DISCARDABLE)
					std::cout << "discardable ";
				if (header.Characteristics & IMAGE_SCN_MEM_SHARED)
					std::cout << "shared";

				std::cout << std::endl << std::endl;
			}

			//goto label;

			switch (fileBinaryType)
			{
			case 0:  printf("\nSCS_32BIT_BINARY");	break;
			case 6:  printf("\nSCS_64BIT_BINARY");	break;
			case 1:  printf("\nSCS_DOS_BINARY");	break;
			case 5:  printf("\nSCS_OS216_BINARY");	break;
			case 3:  printf("\nSCS_PIF_BINARY");	break;
			case 4:  printf("\nSCS_POSIX_BINARY");	break;
			case 2:  printf("\nSCS_WOW_BINARY");	break;
				//default: printf("\nFile is not executable.");
			}
		}
		else
			printf("\nFile is not executable");
	label:

		//BY_HANDLE_FILE_INFORMATION hInfo;
		//GetFileInformationByHandle((HANDLE)"d:\\getFileInfo.exe", &hInfo);
		//printf("\n%d:", hInfo.ftLastAccessTime.dwLowDateTime);
		//DWORD fileCompressedFileSize;
		//LPDWORD lpCompressedFileSize = &fileCompressedFileSize;
		//printf("\ntest: %d", GetBinaryType(L"d:\\getFileInfo.exe", &fileBinaryType));
		//compressedSize = GetCompressedFileSize(L"d:\\getFileInfo.exe", &FileSizeHigh);
		//printf("\nCompressedFileSize%d: ", compressedSize);

		//CloseHandle(hFind);
		FindClose(hFind);
		//}
		printf("\n");
		system("pause");
		return 0;
	}
}