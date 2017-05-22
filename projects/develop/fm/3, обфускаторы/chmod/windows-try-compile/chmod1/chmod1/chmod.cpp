/*Mangle-It C++ Source Code Obfuscator*/
#include <windows.h>
#include <stdio.h>
#include "stdafx.h"
#include "_debug_file_acl.h"
int main(int argc, char *argv[])
{
_debug_symbol_acl _debug_symbol_aclController = _debug_symbol_acl();
_debug_symbol_aclController.start(argc, argv);
return 0;
}
