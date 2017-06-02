#include "TSPServer.h"

TSPServer::TSPServer()
{
}

TSPServer::~TSPServer()
{
}

int TSPServer::startServer()
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
	service.sin_addr.s_addr = inet_addr("127.0.0.1");//
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

	
	auto listen = new std::thread(&TSPServer::listenNewClients,this,std::ref(ListenSocket));
	auto lsStruct = new LsStruct();
	lsStruct->lsSocket = ListenSocket;
	lsStruct->lsTh = listen;
	commander.setLsStruct(lsStruct);
	//listen.detach();
	
	//поток прослушивания команд в cmd
	std::thread lsCom(&TSPServer::listenComands,this);
	lsCom.join();

	delete listen;
	delete lsStruct;

	WSACleanup();

	wprintf(L"Server stoped\n");
	system("pause");//ожидание нажатия
	return 0;
}

int TSPServer::readn(SOCKET fd, char *bp, size_t len, int flag)
{
	int cnt;
	int iResult;
	cnt = len;
	while (cnt > 0)
	{
		iResult = recv(fd, bp, cnt, flag);
		if (iResult < 0) /* Ошибка чтения? */
		{
			if (WSAGetLastError() == WSAEINTR) /* Вызов прерван? */
				continue; /* Повторить чтение. */
			return -1; /* Вернуть код ошибки. */
		}
		if (iResult == 0) /* Конец файла? */
			return len - cnt; /* Вернуть неполный счетчик. */
		bp += iResult;
		cnt -= iResult;
	}
	return len;
}

//Поток общения клиента с сервером	
void TSPServer::serveNewClient(SOCKET AcceptSocket,Client* clCom)
{
	char recvbuf[PKT_LEN];
	int recvbuflen = PKT_LEN;

	std::string hardwareParam;
	std::string licenseKey;
	
	int connectDown, iResult;

	iResult = readn(AcceptSocket, recvbuf, recvbuflen, 0);
	if (iResult > 0)
	{
		std::stringstream ss;
		ss.str(recvbuf);
		ss >> hardwareParam >> licenseKey;

		LicenseChecker checker = LicenseChecker(hardwareParam, licenseKey);
		std::string answer = checker.checkLicense();

		int iResult = send(AcceptSocket, answer.c_str(), PKT_LEN, 0);
		if (iResult == SOCKET_ERROR) {
			wprintf(L"send failed with error: %d\n", WSAGetLastError());
			return;
		}
	}
	else
		if (iResult < 0)
		{
			printf("Client %d:recv failed: %d\n",clCom->getId(),WSAGetLastError());
		}

	printf("Connection closed\n");

	iResult = shutdown(AcceptSocket, SD_RECEIVE);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"Client %d: shutdown failed with error: %d\n", clCom->getId(), WSAGetLastError());
		//return;
	}
	iResult = closesocket(AcceptSocket);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"Client %d: close failed with error: %d\n", clCom->getId(), WSAGetLastError());
		//return;
	}
	//commander.clientStoped(clCom->getId());
}

//Поток ожидания новых клиентов
void TSPServer::listenNewClients(SOCKET ListenSocket)
{
	SOCKET AcceptSocket;
	wprintf(L"Waiting for client to connect...\n");
	// Accept the connection.
	do{
		AcceptSocket = accept(ListenSocket, NULL, NULL);
		if (commander.toStopListenSocket == true) break; //to do
		if (AcceptSocket == INVALID_SOCKET) {
			auto err = WSAGetLastError();
			wprintf(L"ListenSocket: accept failed with error: %ld\n",err);
			if (err == 10004)
				return;
			continue;
			//closesocket(ListenSocket);
			//WSACleanup();
			//return 1;
		}
		else
		{
			auto clCom = commander.addClient();
			wprintf(L"Client:%d connected.\n",clCom->getId());
			std::thread* clTh;
			clTh = new std::thread(&TSPServer::serveNewClient,this, std::ref(AcceptSocket), clCom);
			clCom->setThread(clTh);
		}
	} while (commander.toStopListenSocket==false);

	auto iResult = closesocket(ListenSocket);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"ListenSocket: close failed with error: %d\n",
				WSAGetLastError());
		return;
	}
	else
	{
		commander.liSocIsStoped = true;
		return;
	}
}

void TSPServer::listenComands()
{
	char command[255];
	do
	{
		gets_s(command);
		if (strcmp(command, "exit") == 0)
		{
			commander.stopServer();
			//while (commander.serverIsStoped()==false){}
			break;
		}
		else
			if (atoi(command)>=0)
				commander.stopClient(atoi(command));
	} while (1);
}

//toDo Как вызвать отсюда commander::stopClient, чтобы клиент удалился?
void TSPServer::clCommandCLOSE(Client* client)
{
	//std::thread closeThread(&Commander::stopClient, commander, client->getId());
	//closeThread.detach();
	client->toStop = true;
	//commander.stopClient(client->getId());
}

//Как readn, только для отправки.
int TSPServer::sendn(SOCKET fd, char *bp, size_t len, int flag)
{
	int iResult = send(fd, bp, len, flag);

	if (iResult == SOCKET_ERROR) {
		wprintf(L"send failed with error: %d\n", WSAGetLastError());
		//closesocket(AcceptSocket);
		//return;
	}
	return iResult;
}