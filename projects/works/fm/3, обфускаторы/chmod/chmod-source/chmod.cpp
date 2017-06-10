#include <windows.h>
#include <stdio.h>
#include "stdafx.h"
#include "acl.h"

int main(int argc, char *argv[])
{
	acl aclController = acl();
	aclController.start(argc, argv);
    return 0;
}
