#pragma once
#include "string"

#define _WINSOCK_DEPRECATED_NO_WARNINGS //ошибка в inet_addr()
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <memory>

#define DEFAULT_BUFLEN 1024

#pragma comment(lib,"Ws2_32.lib")

using namespace std;

class NetClient
{
public:
	NetClient(string hardwareParam, string licenseKey);
	~NetClient();
	string getActivateKey();
	string commun(SOCKET socket);

private:
	string hardwareParam;
	string licenseKey;
	int readn(SOCKET fd, char *bp, size_t len, int flag);
};