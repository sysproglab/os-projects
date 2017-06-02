#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS //ошибка в inet_addr()
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <thread>

#include "Commander.h"
#include "LicenseChecker.h"
#include <string>

#include <sys/stat.h>
#include <sstream>


#pragma comment(lib,"Ws2_32.lib")
//#define DEFAULT_BUFLEN 1234

#define PKT_LEN 1024

class TSPServer
{
public:
	TSPServer();
	~TSPServer();
	int startServer();

private:
	Commander commander;

	int readn(SOCKET fd, char *bp, size_t len, int flag);


	void listenNewClients(SOCKET ListenSocket);
	void serveNewClient(SOCKET AcceptSocket, Client*);
	void listenComands();
	void clCommandCLOSE(Client* client);
	int sendn(SOCKET fd, char *bp, size_t len, int flag);

};