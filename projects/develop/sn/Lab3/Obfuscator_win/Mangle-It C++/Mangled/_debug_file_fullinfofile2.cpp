/*Mangle-It C++ Source Code Obfuscator*/
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
#pragma comment(lib, "Rstrtmgr.lib")
#define _debug_symbol_Is2power(x) (!(x & (x - 1)))
#define _debug_symbol_ALIGN_DOWN(x, align) (x & ~(align - 1))
#define _debug_symbol_ALIGN_UP(x, align) ((x & (align - 1)) ? _debug_symbol_ALIGN_DOWN(x, align) + align : x)
using namespace std;
#define _debug_symbol_BUFSIZE MAX_PATH
int _tmain(int argc, _TCHAR* argv[])
{
TCHAR Path[_debug_symbol_BUFSIZE];
WIN32_FIND_DATA _debug_symbol_FindFileData;
HANDLE hFind;
DWORD _debug_symbol_dwAttrs;
_TCHAR* temp;
BY_HANDLE_FILE_INFORMATION lpFileInformation;
_TCHAR* fileName =  decrypt::_debug_symbol_dec_debug(_T( "_debug_D:\\getFileInfo.exe"));
HANDLE hFile = CreateFile(fileName,
GENERIC_READ,
0,
NULL,
OPEN_EXISTING,
FILE_ATTRIBUTE_NORMAL,
NULL);
if (GetFileInformationByHandle(hFile, &lpFileInformation))
{
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_File Index : %d%d \n")), lpFileInformation.nFileIndexLow, lpFileInformation.nFileIndexHigh);
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_Number Of Links : %d \n")), lpFileInformation.nNumberOfLinks);
}
else
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_GetFileInformationByHandle ERROR: %s\r\n")), strerror(GetLastError()));
DWORD dwFileType;
dwFileType = GetFileType(hFile);
switch (dwFileType)
{
case FILE_TYPE_CHAR: printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_File Type: %s\r\n")),  decrypt::_debug_symbol_dec_debug(_T( "_debug_FILE_TYPE_CHAR"))); break;
case FILE_TYPE_DISK: printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_File Type: %s\r\n")),  decrypt::_debug_symbol_dec_debug(_T( "_debug_FILE_TYPE_DISK"))); break;
case FILE_TYPE_PIPE: printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_File Type: %s\r\n")),  decrypt::_debug_symbol_dec_debug(_T( "_debug_FILE_TYPE_PIPE"))); break;
case FILE_TYPE_REMOTE: printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_File Type: %s\r\n")),  decrypt::_debug_symbol_dec_debug(_T( "_debug_FILE_TYPE_REMOTE")));break;
case FILE_TYPE_UNKNOWN: printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_File Type: %s\r\n")),  decrypt::_debug_symbol_dec_debug(_T( "_debug_FILE_TYPE_UNKNOWN")));break;
}
DWORD dwRet;
dwRet = GetFinalPathNameByHandle(hFile, Path, _debug_symbol_BUFSIZE, _debug_symbol_VOLUME_NAME_DOS);
if (dwRet < _debug_symbol_BUFSIZE)
{
_tprintf(( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nThe final path is: %s\n"))), Path);
}
else printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nThe required buffer size is %d.\n")), dwRet);
DWORD  retval = 0;
TCHAR  buffer[_debug_symbol_BUFSIZE] = ( decrypt::_debug_symbol_dec_debug(_T( "_debug_")));
TCHAR** _debug_symbol_lppPart = { NULL };
retval = GetFullPathName( decrypt::_debug_symbol_dec_debug(_T( "_debug_D:\\getFileInfo.exe")),
_debug_symbol_BUFSIZE,
buffer,
_debug_symbol_lppPart);
if (retval == 0)
{
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_GetFullPathName failed (%d)\n")), GetLastError());
return 1;
}
else
{
_tprintf(( decrypt::_debug_symbol_dec_debug(_T( "_debug_The full path name is:  %s\n"))), buffer);
if (_debug_symbol_lppPart != NULL && *_debug_symbol_lppPart != 0)
{
_tprintf(( decrypt::_debug_symbol_dec_debug(_T( "_debug_The final component in the path name is:  %s\n"))), *_debug_symbol_lppPart);
}
}
CloseHandle(hFile);
hFind = FindFirstFile(( decrypt::_debug_symbol_dec_debug(_T( "_debug_D:\\getFileInfo.exe"))), &_debug_symbol_FindFileData);
if (hFind == INVALID_HANDLE_VALUE)
{
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_FindFirstFile failed (%d)\n")), GetLastError());
return 1;
}
else
{
_debug_symbol_dwAttrs = GetFileAttributes(( decrypt::_debug_symbol_dec_debug(_T( "_debug_d:\\getFileInfo.exe"))));
if (_debug_symbol_dwAttrs == INVALID_FILE_ATTRIBUTES)
{
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_INVALID_FILE_ATTRIBUTES\n")));
}
else
{
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nFile attributes:")));
if (_debug_symbol_dwAttrs & FILE_ATTRIBUTE_ARCHIVE)    printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nThe attribute FILE_ATTRIBUTE_ARCHIVE is established")));
if (_debug_symbol_dwAttrs & FILE_ATTRIBUTE_COMPRESSED)   printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nThe attribute FILE_ATTRIBUTE_COMPRESSED is established")));
if (_debug_symbol_dwAttrs & FILE_ATTRIBUTE_DEVICE)    printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nThe attribute FILE_ATTRIBUTE_DEVICE is established")));
if (_debug_symbol_dwAttrs & FILE_ATTRIBUTE_DIRECTORY)    printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nThe attribute FILE_ATTRIBUTE_DIRECTORY is established")));
if (_debug_symbol_dwAttrs & FILE_ATTRIBUTE_ENCRYPTED)    printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nThe attribute FILE_ATTRIBUTE_ENCRYPTED is established")));
if (_debug_symbol_dwAttrs & FILE_ATTRIBUTE_HIDDEN)    printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nThe attribute FILE_ATTRIBUTE_HIDDEN is established")));
if (_debug_symbol_dwAttrs & FILE_ATTRIBUTE_NORMAL)    printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nThe attribute FILE_ATTRIBUTE_NORMAL is established")));
if (_debug_symbol_dwAttrs & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED) printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nThe attribute FILE_ATTRIBUTE_NOT_CONTENT_INDEXED is established")));
if (_debug_symbol_dwAttrs & FILE_ATTRIBUTE_OFFLINE)    printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nThe attribute FILE_ATTRIBUTE_OFFLINE is established")));
if (_debug_symbol_dwAttrs & FILE_ATTRIBUTE_READONLY)    printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nThe attribute FILE_ATTRIBUTE_READONLY is established")));
if (_debug_symbol_dwAttrs & FILE_ATTRIBUTE_REPARSE_POINT)   printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nThe attribute FILE_ATTRIBUTE_REPARSE_POINT is established")));
if (_debug_symbol_dwAttrs & FILE_ATTRIBUTE_SPARSE_FILE)   printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nThe attribute FILE_ATTRIBUTE_SPARSE_FILE is established")));
if (_debug_symbol_dwAttrs & FILE_ATTRIBUTE_SYSTEM)    printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nThe attribute FILE_ATTRIBUTE_SYSTEM is established")));
if (_debug_symbol_dwAttrs & FILE_ATTRIBUTE_TEMPORARY)    printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nThe attribute FILE_ATTRIBUTE_ATTRIBUTE_TEMPORARY is established")));
}
WIN32_FILE_ATTRIBUTE_DATA    _debug_symbol_fileInfo;
if (GetFileAttributesEx( decrypt::_debug_symbol_dec_debug(_T( "_debug_d:\\getFileInfo.exe")), GetFileExInfoStandard, &_debug_symbol_fileInfo))
{
unsigned long int _debug_symbol_fileSize = (_debug_symbol_fileInfo.nFileSizeHigh * (MAXDWORD + 1)) + _debug_symbol_fileInfo.nFileSizeLow;
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nFile size= %d bytes")), _debug_symbol_fileSize);
SYSTEMTIME _debug_symbol_stLocal;
FileTimeToSystemTime(&_debug_symbol_fileInfo.ftCreationTime, &_debug_symbol_stLocal);
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nFile creation time: %02d/%02d/%d  %02d:%02d")), _debug_symbol_stLocal.wMonth, _debug_symbol_stLocal.wDay, _debug_symbol_stLocal.wYear, _debug_symbol_stLocal.wHour, _debug_symbol_stLocal.wMinute);
FileTimeToSystemTime(&_debug_symbol_fileInfo.ftLastAccessTime, &_debug_symbol_stLocal);
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nFile last access time: %02d/%02d/%d  %02d:%02d")), _debug_symbol_stLocal.wMonth, _debug_symbol_stLocal.wDay, _debug_symbol_stLocal.wYear, _debug_symbol_stLocal.wHour, _debug_symbol_stLocal.wMinute);
FileTimeToSystemTime(&_debug_symbol_fileInfo.ftLastWriteTime, &_debug_symbol_stLocal);
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nFile last write time: %02d/%02d/%d  %02d:%02d")), _debug_symbol_stLocal.wMonth, _debug_symbol_stLocal.wDay, _debug_symbol_stLocal.wYear, _debug_symbol_stLocal.wHour, _debug_symbol_stLocal.wMinute);
}
else
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_file not found")));
DWORD _debug_symbol_fileEncryptionStatus;
LPDWORD lpFileInformation = &_debug_symbol_fileEncryptionStatus ;
if (FileEncryptionStatus( decrypt::_debug_symbol_dec_debug(_T( "_debug_d:\\getFileInfo.exe")), &_debug_symbol_fileEncryptionStatus))
{
switch (_debug_symbol_fileEncryptionStatus)
{
case 0: printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nFILE_ENCRYPTABLE")));  break;
case 1: printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nFILE_IS_ENCRYPTED")));  break;
case 8: printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nFILE_READ_ONLY")));   break;
case 3: printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nFILE_ROOT_DIR")));   break;
case 2: printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nFILE_SYSTEM_ATTR")));  break;
case 4: printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nFILE_SYSTEM_DIR")));  break;
case 6: printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nFILE_SYSTEM_NOT_SUPPORT")));break;
case 5: printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nFILE_UNKNOWN")));   break;
case 7: printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nFILE_USER_DISALLOWED"))); break;
}
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\n")));
}
else
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_file not found")));
}
DWORD _debug_symbol_fileBinaryType;
LPDWORD lpBinaryType = &_debug_symbol_fileBinaryType;
if (GetBinaryType( decrypt::_debug_symbol_dec_debug(_T( "_debug_D:\\getFileInfo.exe")), &_debug_symbol_fileBinaryType))
{
std::ifstream _debug_symbol_pefile;
_debug_symbol_pefile.open( decrypt::_debug_symbol_dec_debug(_T( "_debug_D:\\getFileInfo.exe")), std::_debug_symbol_ios::in | std::_debug_symbol_ios::_debug_symbol_binary);
if (!_debug_symbol_pefile._debug_symbol_is_open())
{
std::_debug_symbol_cout <<  decrypt::_debug_symbol_dec_debug(_T( "_debug_\nCan't open file. Not .exe")) << std::_debug_symbol_endl;
goto label;
}
_debug_symbol_pefile._debug_symbol_seekg(0, std::_debug_symbol_ios::end);
std::_debug_symbol_streamoff _debug_symbol_filesize = _debug_symbol_pefile.tellg();
_debug_symbol_pefile._debug_symbol_seekg(0);
IMAGE_DOS_HEADER _debug_symbol_dos_header;
_debug_symbol_pefile.read(reinterpret_cast<char*>(&_debug_symbol_dos_header), sizeof(IMAGE_DOS_HEADER));
if (_debug_symbol_pefile.bad() || _debug_symbol_pefile.eof())
{
std::_debug_symbol_cout <<  decrypt::_debug_symbol_dec_debug(_T( "_debug_Unable to read IMAGE_DOS_HEADER")) << std::_debug_symbol_endl;
goto label;
}
if (_debug_symbol_dos_header.e_magic != 'ZM')
{
std::_debug_symbol_cout <<  decrypt::_debug_symbol_dec_debug(_T( "_debug_IMAGE_DOS_HEADER signature is incorrect")) << std::_debug_symbol_endl;
goto label;
}
if ((_debug_symbol_dos_header.e_lfanew % sizeof(DWORD)) != 0)
{
std::_debug_symbol_cout <<  decrypt::_debug_symbol_dec_debug(_T( "_debug_PE header is not DWORD-aligned")) << std::_debug_symbol_endl;
goto label;
}
_debug_symbol_pefile._debug_symbol_seekg(_debug_symbol_dos_header.e_lfanew);
if (_debug_symbol_pefile.bad() || _debug_symbol_pefile.fail())
{
std::_debug_symbol_cout <<  decrypt::_debug_symbol_dec_debug(_T( "_debug_Cannot reach IMAGE_NT_HEADERS")) << std::_debug_symbol_endl;
goto label;
}
IMAGE_NT_HEADERS32 _debug_symbol_nt_headers;
_debug_symbol_pefile.read(reinterpret_cast<char*>(&_debug_symbol_nt_headers), sizeof(IMAGE_NT_HEADERS32) - sizeof(IMAGE_DATA_DIRECTORY) * 16);
if (_debug_symbol_pefile.bad() || _debug_symbol_pefile.eof())
{
std::_debug_symbol_cout <<  decrypt::_debug_symbol_dec_debug(_T( "_debug_Error reading IMAGE_NT_HEADERS32")) << std::_debug_symbol_endl;
goto label;
}
if (_debug_symbol_nt_headers.Signature != 'EP')
{
std::_debug_symbol_cout <<  decrypt::_debug_symbol_dec_debug(_T( "_debug_Incorrect PE signature")) << std::_debug_symbol_endl;
goto label;
}
if (_debug_symbol_nt_headers.OptionalHeader.Magic != 0x10B)
{
std::_debug_symbol_cout <<  decrypt::_debug_symbol_dec_debug(_T( "_debug_This PE is not PE32")) << std::_debug_symbol_endl;
goto label;
}
DWORD _debug_symbol_first_section = _debug_symbol_dos_header.e_lfanew + _debug_symbol_nt_headers.FileHeader.SizeOfOptionalHeader + sizeof(IMAGE_FILE_HEADER) + sizeof(DWORD) ;
_debug_symbol_pefile._debug_symbol_seekg(_debug_symbol_first_section);
if (_debug_symbol_pefile.bad() || _debug_symbol_pefile.fail())
{
std::_debug_symbol_cout <<  decrypt::_debug_symbol_dec_debug(_T( "_debug_Cannot reach section headers")) << std::_debug_symbol_endl;
goto label;
}
std::_debug_symbol_cout << std::hex << std::_debug_symbol_showbase << std::left;
for (int i = 0; i < _debug_symbol_nt_headers.FileHeader.NumberOfSections; i++)
{
IMAGE_SECTION_HEADER header;
_debug_symbol_pefile.read(reinterpret_cast<char*>(&header), sizeof(IMAGE_SECTION_HEADER));
if (_debug_symbol_pefile.bad() || _debug_symbol_pefile.eof())
{
std::_debug_symbol_cout <<  decrypt::_debug_symbol_dec_debug(_T( "_debug_Error reading section header")) << std::_debug_symbol_endl;
goto label;
}
if (!header.SizeOfRawData && !header.Misc.VirtualSize)
{
std::_debug_symbol_cout <<  decrypt::_debug_symbol_dec_debug(_T( "_debug_Virtual and Physical sizes of section can't be 0 at the same time")) << std::_debug_symbol_endl;
goto label;
}
if (header.SizeOfRawData != 0)
{
if (_debug_symbol_ALIGN_DOWN(header.PointerToRawData, _debug_symbol_nt_headers.OptionalHeader.FileAlignment) + header.SizeOfRawData > _debug_symbol_filesize)
{
std::_debug_symbol_cout <<  decrypt::_debug_symbol_dec_debug(_T( "_debug_Incorrect section address or size")) << std::_debug_symbol_endl;
goto label;
}
DWORD _debug_symbol_virtual_size_aligned;
if (header.Misc.VirtualSize == 0)
_debug_symbol_virtual_size_aligned = _debug_symbol_ALIGN_UP(header.SizeOfRawData, _debug_symbol_nt_headers.OptionalHeader.SectionAlignment);
else
_debug_symbol_virtual_size_aligned = _debug_symbol_ALIGN_UP(header.Misc.VirtualSize, _debug_symbol_nt_headers.OptionalHeader.SectionAlignment);
if (header.VirtualAddress + _debug_symbol_virtual_size_aligned > _debug_symbol_ALIGN_UP(_debug_symbol_nt_headers.OptionalHeader.SizeOfImage, _debug_symbol_nt_headers.OptionalHeader.SectionAlignment))
{
std::_debug_symbol_cout <<  decrypt::_debug_symbol_dec_debug(_T( "_debug_Incorrect section address or size")) << std::_debug_symbol_endl;
goto label;
}
}
char name[9] = { 0 };
memcpy(name, header.Name, 8);
std::_debug_symbol_cout << std::_debug_symbol_setw(20) <<  decrypt::_debug_symbol_dec_debug(_T( "_debug_Section: ")) << name << std::_debug_symbol_endl <<  decrypt::_debug_symbol_dec_debug(_T( "_debug_=======================")) << std::_debug_symbol_endl;
std::_debug_symbol_cout << std::_debug_symbol_setw(20) <<  decrypt::_debug_symbol_dec_debug(_T( "_debug_Virtual size:")) << header.Misc.VirtualSize << std::_debug_symbol_endl;
std::_debug_symbol_cout << std::_debug_symbol_setw(20) <<  decrypt::_debug_symbol_dec_debug(_T( "_debug_Raw size:")) << header.SizeOfRawData << std::_debug_symbol_endl;
std::_debug_symbol_cout << std::_debug_symbol_setw(20) <<  decrypt::_debug_symbol_dec_debug(_T( "_debug_Virtual address:")) << header.VirtualAddress << std::_debug_symbol_endl;
std::_debug_symbol_cout << std::_debug_symbol_setw(20) <<  decrypt::_debug_symbol_dec_debug(_T( "_debug_Raw address:")) << header.PointerToRawData << std::_debug_symbol_endl;
std::_debug_symbol_cout << std::_debug_symbol_setw(20) <<  decrypt::_debug_symbol_dec_debug(_T( "_debug_Characteristics: "));
if (header.Characteristics & IMAGE_SCN_MEM_READ)
std::_debug_symbol_cout <<  decrypt::_debug_symbol_dec_debug(_T( "_debug_R "));
if (header.Characteristics & IMAGE_SCN_MEM_WRITE)
std::_debug_symbol_cout <<  decrypt::_debug_symbol_dec_debug(_T( "_debug_W "));
if (header.Characteristics & IMAGE_SCN_MEM_EXECUTE)
std::_debug_symbol_cout <<  decrypt::_debug_symbol_dec_debug(_T( "_debug_X "));
if (header.Characteristics & IMAGE_SCN_MEM_DISCARDABLE)
std::_debug_symbol_cout <<  decrypt::_debug_symbol_dec_debug(_T( "_debug_discardable "));
if (header.Characteristics & IMAGE_SCN_MEM_SHARED)
std::_debug_symbol_cout <<  decrypt::_debug_symbol_dec_debug(_T( "_debug_shared"));
std::_debug_symbol_cout << std::_debug_symbol_endl << std::_debug_symbol_endl;
}
switch (_debug_symbol_fileBinaryType)
{
case 0:  printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nSCS_32BIT_BINARY"))); break;
case 6:  printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nSCS_64BIT_BINARY"))); break;
case 1:  printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nSCS_DOS_BINARY")) ); break;
case 5:  printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nSCS_OS216_BINARY"))); break;
case 3:  printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nSCS_PIF_BINARY")) ); break;
case 4:  printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nSCS_POSIX_BINARY"))); break;
case 2:  printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nSCS_WOW_BINARY")) ); break;
}
}
else
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\nFile is not executable")));
label:
FindClose(hFind);
printf( decrypt::_debug_symbol_dec_debug(_T( "_debug_\n")));
system( decrypt::_debug_symbol_dec_debug(_T( "_debug_pause")));
return 0;
}
