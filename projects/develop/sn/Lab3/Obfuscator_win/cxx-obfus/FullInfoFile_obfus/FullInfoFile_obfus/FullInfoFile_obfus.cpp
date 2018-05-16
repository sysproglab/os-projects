// FullInfoFile_obfus.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <tchar.h>
#include <fstream>
#include <iomanip>
#include <RestartManager.h>
#pragma comment(lib,"Rstrtmgr.lib")
#define ReplacementFor_Is2power(ReplacementFor_x) (!(ReplacementFor_x & (\
ReplacementFor_x - 1)))
#define ReplacementFor_ALIGN_DOWN(ReplacementFor_x, align) (ReplacementFor_x & ~\
(align - 1))
#define ReplacementFor_ALIGN_UP(ReplacementFor_x, align) ((ReplacementFor_x & (\
align - 1)) ? ReplacementFor_ALIGN_DOWN(ReplacementFor_x, align) + align : \
ReplacementFor_x) 
using namespace std;
#define ReplacementFor_BUFSIZE ReplacementFor_MAX_PATH


int _tmain(int ReplacementFor_argc,ReplacementFor__TCHAR*
ReplacementFor_argv[])
{
ReplacementFor_TCHAR ReplacementFor_Path[
ReplacementFor_BUFSIZE];ReplacementFor_WIN32_FIND_DATA 
ReplacementFor_FindFileData;ReplacementFor_HANDLE ReplacementFor_hFind;
ReplacementFor_DWORD ReplacementFor_dwAttrs;ReplacementFor__TCHAR*
ReplacementFor_temp;ReplacementFor_BY_HANDLE_FILE_INFORMATION 
ReplacementFor_lpFileInformation;ReplacementFor__TCHAR*ReplacementFor_fileName=
L"D:\\getFileInfo.exe";ReplacementFor_HANDLE ReplacementFor_hFile=
ReplacementFor_CreateFile(ReplacementFor_fileName,
ReplacementFor_GENERIC_READ,
0,
ReplacementFor_NULL,
ReplacementFor_OPEN_EXISTING,
ReplacementFor_FILE_ATTRIBUTE_NORMAL,
ReplacementFor_NULL);if(ReplacementFor_GetFileInformationByHandle(
ReplacementFor_hFile,&ReplacementFor_lpFileInformation)){ReplacementFor_printf(
"File Index : %d%d \n",ReplacementFor_lpFileInformation.
ReplacementFor_nFileIndexLow,ReplacementFor_lpFileInformation.
ReplacementFor_nFileIndexHigh);ReplacementFor_printf("Number Of Links : %d \n",
ReplacementFor_lpFileInformation.ReplacementFor_nNumberOfLinks);
}else ReplacementFor_printf("GetFileInformationByHandle ERROR: %s\r\n",
ReplacementFor_strerror(ReplacementFor_GetLastError()));ReplacementFor_DWORD 
ReplacementFor_dwFileType;ReplacementFor_dwFileType=ReplacementFor_GetFileType(
ReplacementFor_hFile);switch(ReplacementFor_dwFileType){case 
ReplacementFor_FILE_TYPE_CHAR:ReplacementFor_printf("File Type: %s\r\n",
"FILE_TYPE_CHAR");break;case ReplacementFor_FILE_TYPE_DISK:ReplacementFor_printf
("File Type: %s\r\n","FILE_TYPE_DISK");break;case ReplacementFor_FILE_TYPE_PIPE:
ReplacementFor_printf("File Type: %s\r\n","FILE_TYPE_PIPE");break;case 
ReplacementFor_FILE_TYPE_REMOTE:ReplacementFor_printf("File Type: %s\r\n",
"FILE_TYPE_REMOTE");break;case ReplacementFor_FILE_TYPE_UNKNOWN:
ReplacementFor_printf("File Type: %s\r\n","FILE_TYPE_UNKNOWN");break;}
ReplacementFor_DWORD ReplacementFor_dwRet;ReplacementFor_dwRet=
ReplacementFor_GetFinalPathNameByHandle(ReplacementFor_hFile,ReplacementFor_Path
,ReplacementFor_BUFSIZE,ReplacementFor_VOLUME_NAME_DOS);if(ReplacementFor_dwRet<
ReplacementFor_BUFSIZE){ReplacementFor__tprintf(ReplacementFor_TEXT(
"\nThe final path is: %s\n"),ReplacementFor_Path);}else ReplacementFor_printf(
"\nThe required buffer size is %d.\n",ReplacementFor_dwRet);ReplacementFor_DWORD
 ReplacementFor_retval=0;ReplacementFor_TCHAR ReplacementFor_buffer[
ReplacementFor_BUFSIZE]=ReplacementFor_TEXT("");ReplacementFor_TCHAR**
ReplacementFor_lppPart={ReplacementFor_NULL};ReplacementFor_retval=
ReplacementFor_GetFullPathName(L"D:\\getFileInfo.exe",ReplacementFor_BUFSIZE,
ReplacementFor_buffer,ReplacementFor_lppPart);if(ReplacementFor_retval==0){
ReplacementFor_printf("GetFullPathName failed (%d)\n",
ReplacementFor_GetLastError());return 1;}else{ReplacementFor__tprintf(
ReplacementFor_TEXT("The full path name is:  %s\n"),ReplacementFor_buffer);if(
ReplacementFor_lppPart!=ReplacementFor_NULL&&*ReplacementFor_lppPart!=0){
ReplacementFor__tprintf(ReplacementFor_TEXT(
"The final component in the path name is:  %s\n"),*ReplacementFor_lppPart);}}
ReplacementFor_CloseHandle(ReplacementFor_hFile);ReplacementFor_hFind=
ReplacementFor_FindFirstFile(ReplacementFor_TEXT("D:\\getFileInfo.exe") ,&
ReplacementFor_FindFileData);if(ReplacementFor_hFind==
ReplacementFor_INVALID_HANDLE_VALUE){ReplacementFor_printf(
"FindFirstFile failed (%d)\n",ReplacementFor_GetLastError());return 1;}else{
ReplacementFor_dwAttrs=ReplacementFor_GetFileAttributes(ReplacementFor_TEXT(
"d:\\getFileInfo.exe"));if(ReplacementFor_dwAttrs==
ReplacementFor_INVALID_FILE_ATTRIBUTES){ReplacementFor_printf(
"INVALID_FILE_ATTRIBUTES\n");}else{ReplacementFor_printf("\nFile attributes:");
if(ReplacementFor_dwAttrs&ReplacementFor_FILE_ATTRIBUTE_ARCHIVE)
ReplacementFor_printf("\nThe attribute FILE_ATTRIBUTE_ARCHIVE is established");
if(ReplacementFor_dwAttrs&ReplacementFor_FILE_ATTRIBUTE_COMPRESSED)
ReplacementFor_printf("\nThe attribute FILE_ATTRIBUTE_COMPRESSED is established"
);if(ReplacementFor_dwAttrs&ReplacementFor_FILE_ATTRIBUTE_DEVICE)
ReplacementFor_printf("\nThe attribute FILE_ATTRIBUTE_DEVICE is established");if
(ReplacementFor_dwAttrs&ReplacementFor_FILE_ATTRIBUTE_DIRECTORY)
ReplacementFor_printf("\nThe attribute FILE_ATTRIBUTE_DIRECTORY is established")
;if(ReplacementFor_dwAttrs&ReplacementFor_FILE_ATTRIBUTE_ENCRYPTED)
ReplacementFor_printf("\nThe attribute FILE_ATTRIBUTE_ENCRYPTED is established")
;if(ReplacementFor_dwAttrs&ReplacementFor_FILE_ATTRIBUTE_HIDDEN)
ReplacementFor_printf("\nThe attribute FILE_ATTRIBUTE_HIDDEN is established");if
(ReplacementFor_dwAttrs&ReplacementFor_FILE_ATTRIBUTE_NORMAL)
ReplacementFor_printf("\nThe attribute FILE_ATTRIBUTE_NORMAL is established");if
(ReplacementFor_dwAttrs&ReplacementFor_FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)
ReplacementFor_printf(
"\nThe attribute FILE_ATTRIBUTE_NOT_CONTENT_INDEXED is established");if(
ReplacementFor_dwAttrs&ReplacementFor_FILE_ATTRIBUTE_OFFLINE)
ReplacementFor_printf("\nThe attribute FILE_ATTRIBUTE_OFFLINE is established");
if(ReplacementFor_dwAttrs&ReplacementFor_FILE_ATTRIBUTE_READONLY)
ReplacementFor_printf("\nThe attribute FILE_ATTRIBUTE_READONLY is established");
if(ReplacementFor_dwAttrs&ReplacementFor_FILE_ATTRIBUTE_REPARSE_POINT)
ReplacementFor_printf(
"\nThe attribute FILE_ATTRIBUTE_REPARSE_POINT is established");if(
ReplacementFor_dwAttrs&ReplacementFor_FILE_ATTRIBUTE_SPARSE_FILE)
ReplacementFor_printf(
"\nThe attribute FILE_ATTRIBUTE_SPARSE_FILE is established");if(
ReplacementFor_dwAttrs&ReplacementFor_FILE_ATTRIBUTE_SYSTEM)
ReplacementFor_printf("\nThe attribute FILE_ATTRIBUTE_SYSTEM is established");if
(ReplacementFor_dwAttrs&ReplacementFor_FILE_ATTRIBUTE_TEMPORARY)
ReplacementFor_printf(
"\nThe attribute FILE_ATTRIBUTE_ATTRIBUTE_TEMPORARY is established");}
ReplacementFor_WIN32_FILE_ATTRIBUTE_DATA ReplacementFor_fileInfo;if(
ReplacementFor_GetFileAttributesEx(L"d:\\getFileInfo.exe",
ReplacementFor_GetFileExInfoStandard,&ReplacementFor_fileInfo)){unsigned long 
int ReplacementFor_fileSize=(ReplacementFor_fileInfo.
ReplacementFor_nFileSizeHigh*(ReplacementFor_MAXDWORD+1))+
ReplacementFor_fileInfo.ReplacementFor_nFileSizeLow;ReplacementFor_printf(
"\nFile size= %d bytes",ReplacementFor_fileSize);ReplacementFor_SYSTEMTIME 
ReplacementFor_stLocal;ReplacementFor_FileTimeToSystemTime(&
ReplacementFor_fileInfo.ReplacementFor_ftCreationTime,&ReplacementFor_stLocal);
ReplacementFor_printf("\nFile creation time: %02d/%02d/%d  %02d:%02d",
ReplacementFor_stLocal.ReplacementFor_wMonth,ReplacementFor_stLocal.
ReplacementFor_wDay,ReplacementFor_stLocal.ReplacementFor_wYear,
ReplacementFor_stLocal.ReplacementFor_wHour,ReplacementFor_stLocal.
ReplacementFor_wMinute);ReplacementFor_FileTimeToSystemTime(&
ReplacementFor_fileInfo.ReplacementFor_ftLastAccessTime,&ReplacementFor_stLocal)
;ReplacementFor_printf("\nFile last access time: %02d/%02d/%d  %02d:%02d",
ReplacementFor_stLocal.ReplacementFor_wMonth,ReplacementFor_stLocal.
ReplacementFor_wDay,ReplacementFor_stLocal.ReplacementFor_wYear,
ReplacementFor_stLocal.ReplacementFor_wHour,ReplacementFor_stLocal.
ReplacementFor_wMinute);ReplacementFor_FileTimeToSystemTime(&
ReplacementFor_fileInfo.ReplacementFor_ftLastWriteTime,&ReplacementFor_stLocal);
ReplacementFor_printf("\nFile last write time: %02d/%02d/%d  %02d:%02d",
ReplacementFor_stLocal.ReplacementFor_wMonth,ReplacementFor_stLocal.
ReplacementFor_wDay,ReplacementFor_stLocal.ReplacementFor_wYear,
ReplacementFor_stLocal.ReplacementFor_wHour,ReplacementFor_stLocal.
ReplacementFor_wMinute);}else ReplacementFor_printf("file not found");
ReplacementFor_DWORD ReplacementFor_fileEncryptionStatus;ReplacementFor_LPDWORD 
ReplacementFor_lpFileInformation=&ReplacementFor_fileEncryptionStatus;if(
ReplacementFor_FileEncryptionStatus(L"d:\\getFileInfo.exe",&
ReplacementFor_fileEncryptionStatus)){switch(ReplacementFor_fileEncryptionStatus
){case 0:ReplacementFor_printf("\nFILE_ENCRYPTABLE");break;case 1:
ReplacementFor_printf("\nFILE_IS_ENCRYPTED");break;case 8:ReplacementFor_printf(
"\nFILE_READ_ONLY");break;case 3:ReplacementFor_printf("\nFILE_ROOT_DIR");break;
case 2:ReplacementFor_printf("\nFILE_SYSTEM_ATTR");break;case 4:
ReplacementFor_printf("\nFILE_SYSTEM_DIR");break;case 6:ReplacementFor_printf(
"\nFILE_SYSTEM_NOT_SUPPORT");break;case 5:ReplacementFor_printf("\nFILE_UNKNOWN"
);break;case 7:ReplacementFor_printf("\nFILE_USER_DISALLOWED");break;}
ReplacementFor_printf("\n");}else ReplacementFor_printf("file not found");}
ReplacementFor_DWORD ReplacementFor_fileBinaryType;ReplacementFor_LPDWORD 
ReplacementFor_lpBinaryType=&ReplacementFor_fileBinaryType;
if(ReplacementFor_GetBinaryType(L"D:\\getFileInfo.exe",&
ReplacementFor_fileBinaryType)){
std::ReplacementFor_ifstream ReplacementFor_pefile;ReplacementFor_pefile.
ReplacementFor_open(L"D:\\getFileInfo.exe",std::ReplacementFor_ios::
ReplacementFor_in|std::ReplacementFor_ios::ReplacementFor_binary);if(!
ReplacementFor_pefile.ReplacementFor_is_open()){
std::ReplacementFor_cout<<"\nCan't open file. Not .exe"<<std::
ReplacementFor_endl;goto ReplacementFor_label;}
ReplacementFor_pefile.ReplacementFor_seekg(0,std::ReplacementFor_ios::
ReplacementFor_end);
std::ReplacementFor_streamoff ReplacementFor_filesize=ReplacementFor_pefile.
ReplacementFor_tellg();

ReplacementFor_pefile.ReplacementFor_seekg(0);ReplacementFor_IMAGE_DOS_HEADER 
ReplacementFor_dos_header;ReplacementFor_pefile.ReplacementFor_read(
reinterpret_cast<char*>(&ReplacementFor_dos_header),sizeof(
ReplacementFor_IMAGE_DOS_HEADER));if(ReplacementFor_pefile.ReplacementFor_bad()
||ReplacementFor_pefile.ReplacementFor_eof()){
std::ReplacementFor_cout<<"Unable to read IMAGE_DOS_HEADER"<<std::
ReplacementFor_endl;goto ReplacementFor_label;}

if(ReplacementFor_dos_header.ReplacementFor_e_magic!='ZM'){std::
ReplacementFor_cout<<"IMAGE_DOS_HEADER signature is incorrect"<<std::
ReplacementFor_endl;goto ReplacementFor_label;}


if((ReplacementFor_dos_header.ReplacementFor_e_lfanew%sizeof(
ReplacementFor_DWORD))!=0){
std::ReplacementFor_cout<<"PE header is not DWORD-aligned"<<std::
ReplacementFor_endl;goto ReplacementFor_label;}
ReplacementFor_pefile.ReplacementFor_seekg(ReplacementFor_dos_header.
ReplacementFor_e_lfanew);if(ReplacementFor_pefile.ReplacementFor_bad()||
ReplacementFor_pefile.ReplacementFor_fail()){std::ReplacementFor_cout<<
"Cannot reach IMAGE_NT_HEADERS"<<std::ReplacementFor_endl;goto 
ReplacementFor_label;}



ReplacementFor_IMAGE_NT_HEADERS32 ReplacementFor_nt_headers;
ReplacementFor_pefile.ReplacementFor_read(reinterpret_cast<char*>(&
ReplacementFor_nt_headers),sizeof(ReplacementFor_IMAGE_NT_HEADERS32)-sizeof(
ReplacementFor_IMAGE_DATA_DIRECTORY)*16);if(ReplacementFor_pefile.
ReplacementFor_bad()||ReplacementFor_pefile.ReplacementFor_eof()){std::
ReplacementFor_cout<<"Error reading IMAGE_NT_HEADERS32"<<std::
ReplacementFor_endl;goto ReplacementFor_label;}


if(ReplacementFor_nt_headers.ReplacementFor_Signature!='EP'){std::
ReplacementFor_cout<<"Incorrect PE signature"<<std::ReplacementFor_endl;goto 
ReplacementFor_label;}
if(ReplacementFor_nt_headers.ReplacementFor_OptionalHeader.ReplacementFor_Magic
!=0x10b){std::ReplacementFor_cout<<"This PE is not PE32"<<std::
ReplacementFor_endl;goto ReplacementFor_label;}

ReplacementFor_DWORD ReplacementFor_first_section=ReplacementFor_dos_header.
ReplacementFor_e_lfanew+ReplacementFor_nt_headers.ReplacementFor_FileHeader.
ReplacementFor_SizeOfOptionalHeader+sizeof(ReplacementFor_IMAGE_FILE_HEADER)+
sizeof(ReplacementFor_DWORD) ;
ReplacementFor_pefile.ReplacementFor_seekg(ReplacementFor_first_section);if(
ReplacementFor_pefile.ReplacementFor_bad()||ReplacementFor_pefile.
ReplacementFor_fail()){std::ReplacementFor_cout<<"Cannot reach section headers"
<<std::ReplacementFor_endl;goto ReplacementFor_label;}std::ReplacementFor_cout<<
std::ReplacementFor_hex<<std::ReplacementFor_showbase<<std::ReplacementFor_left;
for(int ReplacementFor_i=0;ReplacementFor_i<ReplacementFor_nt_headers.
ReplacementFor_FileHeader.ReplacementFor_NumberOfSections;ReplacementFor_i++){
ReplacementFor_IMAGE_SECTION_HEADER ReplacementFor_header;
ReplacementFor_pefile.ReplacementFor_read(reinterpret_cast<char*>(&
ReplacementFor_header),sizeof(ReplacementFor_IMAGE_SECTION_HEADER));if(
ReplacementFor_pefile.ReplacementFor_bad()||ReplacementFor_pefile.
ReplacementFor_eof()){std::ReplacementFor_cout<<"Error reading section header"<<
std::ReplacementFor_endl;goto ReplacementFor_label;}

if(!ReplacementFor_header.ReplacementFor_SizeOfRawData&&!ReplacementFor_header.
ReplacementFor_Misc.ReplacementFor_VirtualSize){std::ReplacementFor_cout<<
"Virtual and Physical sizes of section can't be 0 at the same time"<<std::
ReplacementFor_endl;goto ReplacementFor_label;}
if(ReplacementFor_header.ReplacementFor_SizeOfRawData!=0){
if(ReplacementFor_ALIGN_DOWN(ReplacementFor_header.
ReplacementFor_PointerToRawData,ReplacementFor_nt_headers.
ReplacementFor_OptionalHeader.ReplacementFor_FileAlignment)+
ReplacementFor_header.ReplacementFor_SizeOfRawData>ReplacementFor_filesize){std
::ReplacementFor_cout<<"Incorrect section address or size"<<std::
ReplacementFor_endl;goto ReplacementFor_label;}
ReplacementFor_DWORD ReplacementFor_virtual_size_aligned;
if(ReplacementFor_header.ReplacementFor_Misc.ReplacementFor_VirtualSize==0)

ReplacementFor_virtual_size_aligned=ReplacementFor_ALIGN_UP(
ReplacementFor_header.ReplacementFor_SizeOfRawData,ReplacementFor_nt_headers.
ReplacementFor_OptionalHeader.ReplacementFor_SectionAlignment);else

ReplacementFor_virtual_size_aligned=ReplacementFor_ALIGN_UP(
ReplacementFor_header.ReplacementFor_Misc.ReplacementFor_VirtualSize,
ReplacementFor_nt_headers.ReplacementFor_OptionalHeader.
ReplacementFor_SectionAlignment);
if(ReplacementFor_header.ReplacementFor_VirtualAddress+
ReplacementFor_virtual_size_aligned>ReplacementFor_ALIGN_UP(
ReplacementFor_nt_headers.ReplacementFor_OptionalHeader.
ReplacementFor_SizeOfImage,ReplacementFor_nt_headers.
ReplacementFor_OptionalHeader.ReplacementFor_SectionAlignment)){std::
ReplacementFor_cout<<"Incorrect section address or size"<<std::
ReplacementFor_endl;goto ReplacementFor_label;}}
char ReplacementFor_name[9]={0};ReplacementFor_memcpy(ReplacementFor_name,
ReplacementFor_header.ReplacementFor_Name,8);
std::ReplacementFor_cout<<std::ReplacementFor_setw(20)<<"Section: "<<
ReplacementFor_name<<std::ReplacementFor_endl<<"======================="<<std::
ReplacementFor_endl;
std::ReplacementFor_cout<<std::ReplacementFor_setw(20)<<"Virtual size:"<<
ReplacementFor_header.ReplacementFor_Misc.ReplacementFor_VirtualSize<<std::
ReplacementFor_endl;std::ReplacementFor_cout<<std::ReplacementFor_setw(20)<<
"Raw size:"<<ReplacementFor_header.ReplacementFor_SizeOfRawData<<std::
ReplacementFor_endl;std::ReplacementFor_cout<<std::ReplacementFor_setw(20)<<
"Virtual address:"<<ReplacementFor_header.ReplacementFor_VirtualAddress<<std::
ReplacementFor_endl;std::ReplacementFor_cout<<std::ReplacementFor_setw(20)<<
"Raw address:"<<ReplacementFor_header.ReplacementFor_PointerToRawData<<std::
ReplacementFor_endl;
std::ReplacementFor_cout<<std::ReplacementFor_setw(20)<<"Characteristics: ";if(
ReplacementFor_header.ReplacementFor_Characteristics&
ReplacementFor_IMAGE_SCN_MEM_READ)std::ReplacementFor_cout<<"R ";if(
ReplacementFor_header.ReplacementFor_Characteristics&
ReplacementFor_IMAGE_SCN_MEM_WRITE)std::ReplacementFor_cout<<"W ";if(
ReplacementFor_header.ReplacementFor_Characteristics&
ReplacementFor_IMAGE_SCN_MEM_EXECUTE)std::ReplacementFor_cout<<"X ";if(
ReplacementFor_header.ReplacementFor_Characteristics&
ReplacementFor_IMAGE_SCN_MEM_DISCARDABLE)std::ReplacementFor_cout<<
"discardable ";if(ReplacementFor_header.ReplacementFor_Characteristics&
ReplacementFor_IMAGE_SCN_MEM_SHARED)std::ReplacementFor_cout<<"shared";std::
ReplacementFor_cout<<std::ReplacementFor_endl<<std::ReplacementFor_endl;}
switch(ReplacementFor_fileBinaryType){case 0:ReplacementFor_printf(
"\nSCS_32BIT_BINARY");break;case 6:ReplacementFor_printf("\nSCS_64BIT_BINARY");
break;case 1:ReplacementFor_printf("\nSCS_DOS_BINARY");break;case 5:
ReplacementFor_printf("\nSCS_OS216_BINARY");break;case 3:ReplacementFor_printf(
"\nSCS_PIF_BINARY");break;case 4:ReplacementFor_printf("\nSCS_POSIX_BINARY");
break;case 2:ReplacementFor_printf("\nSCS_WOW_BINARY");break;
}}else ReplacementFor_printf("\nFile is not executable");ReplacementFor_label:








ReplacementFor_FindClose(ReplacementFor_hFind);
ReplacementFor_printf("\n");ReplacementFor_system("pause");return 0;
}

