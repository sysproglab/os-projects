#include "NetClient.h"

NetClient::NetClient(string aparam, string alicenseKey)
{
	hardwareParam = aparam;
	licenseKey = alicenseKey;
}

NetClient::~NetClient()
{

}

string NetClient::getActivateKey()
{
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);	//старт
	if (iResult != NO_ERROR) {
		wprintf(L"WSAStartup failed with error: %ld\n", iResult);
		return "";
	}
	//создание сокета для подключения
	SOCKET ConnectSocket = INVALID_SOCKET;
	ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//AF_INET - IPv4;
	if (ConnectSocket == INVALID_SOCKET) {
		wprintf(L"socket function failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return "";
	}
	//СОздание соединения с сервером
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
	clientService.sin_port = htons(27015);

	//----------------------
	// Connect to server.
	iResult = connect(ConnectSocket, (SOCKADDR *)& clientService, sizeof(clientService));
	if (iResult == SOCKET_ERROR) {
		wprintf(L"connect function failed with error: %ld\n", WSAGetLastError());
		iResult = closesocket(ConnectSocket);
		if (iResult == SOCKET_ERROR)
			wprintf(L"closesocket function failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return "";
	}
	wprintf(L"Connected to server.\n");

	string answer = commun(ConnectSocket);

	//Завершение соединения
	iResult = shutdown(ConnectSocket, SD_RECEIVE);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"shutdown failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return "";
	}
	//закрытие сокета
	iResult = closesocket(ConnectSocket);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"close failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return "";
	}
	WSACleanup();

	return answer;
}

string NetClient::commun(SOCKET socket)
{
	string data = hardwareParam + " " + licenseKey;

	int sendBufLen = DEFAULT_BUFLEN;
	int iResult = send(socket, data.c_str(), DEFAULT_BUFLEN, 0);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"send failed with error: %d\n", WSAGetLastError());
		closesocket(socket);
		WSACleanup();
		return "";
	}


	char recvbuf[DEFAULT_BUFLEN + 1];
	int recvbuflen = DEFAULT_BUFLEN;

	iResult = readn(socket, recvbuf, recvbuflen, 0);
	if (iResult > 0)
	{
		return string(recvbuf);
	}
	else
	{
		if (iResult == 0)
			printf("Connection closed\n");
		else
			if (iResult < 0)
				printf("recv failed: %d\n", WSAGetLastError());
		return "";
	}
	
}

int NetClient::readn(SOCKET fd, char *bp, size_t len, int flag)
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