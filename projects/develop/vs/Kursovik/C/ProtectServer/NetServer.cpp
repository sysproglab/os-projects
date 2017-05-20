#include "NetServer.h"


NetServer::NetServer()
{

}

NetServer::~NetServer()
{

}

bool NetServer::startServer()
{
	SOCKET ListenSocket = INVALID_SOCKET;
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);	//старт
	if (iResult != NO_ERROR) {
		wprintf(L"WSAStartup failed with error: %ld\n", iResult);
		return 1;
	}
	//создание сокета прослушивания
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//AF_INET - IPv4;
	//AF_inet - IPV6;
	//AF_UNSPEC - не важена версия IP
	// The socket address to be passed to bind
	if (ListenSocket == INVALID_SOCKET) {
		wprintf(L"socket function failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("192.168.56.1");//
	service.sin_port = htons(27015);

	//Привязывание сокета
	iResult = bind(ListenSocket, (SOCKADDR*)&service, sizeof(service));
	if (iResult == SOCKET_ERROR) {
		wprintf(L"bind failed with error %u\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	else
		wprintf(L"bind returned success\n");

	// Listen for incoming connection requests 
	// on the created socket
	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
		wprintf(L"listen function failed with error: %d\n", WSAGetLastError());


	auto listen = new std::thread(&TSPServer::listenNewClients, this, std::ref(ListenSocket));
	auto lsStruct = new LsStruct();
	lsStruct->lsSocket = ListenSocket;
	lsStruct->lsTh = listen;
	commander.setLsStruct(lsStruct);
	//listen.detach();

	//поток прослушивания команд в cmd
	std::thread lsCom(&TSPServer::listenComands, this);
	lsCom.join();

	WSACleanup();

	wprintf(L"Server stoped\n");
	system("pause");//ожидание нажатия
	return 0;
	return true;
}