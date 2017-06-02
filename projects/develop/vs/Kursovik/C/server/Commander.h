#pragma once
#include <map>
#include <thread>
#include <winsock2.h>
#include <mutex>

class Client
{
public:
	Client();
	~Client();
	void setId(int id);
	int getId();
	void setThread(std::thread* th);
	std::thread* getThread();

	bool toStop;

private:
	int id;
	std::thread* clTh;
};

struct LsStruct
{
	SOCKET lsSocket;
	std::thread* lsTh;
};

class Commander
{
public:
	Commander();
	~Commander();
	Client* addClient();
	void setLsStruct(LsStruct* lsSt);

	//command
	void stopServer();
	bool stopClient(int id);
	void stopListenSocket();

	//answer
	void clientStoped(int id);
	void LiSocStoped();

	//??
	bool serverIsStoped();

	bool toStopServer;
	bool toStopListenSocket;
	bool liSocIsStoped;

private:
	int getNextId();
	int freeId;
	LsStruct* lsStruct;
	std::map<int, Client*> clients;
	std::recursive_mutex* map_lock;
};
