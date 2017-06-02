#include "Commander.h"

Client::Client() : toStop(false), id(-1)
{
	//clTh = new std::thread();
}

Client::~Client()
{
	delete clTh;
}

void Client::setId(int id)
{
	this->id = id;
}

int Client::getId()
{
	return id;
}

void Client::setThread(std::thread* th)
{
	clTh = th;
}


std::thread* Client::getThread()
{
	return clTh;
}


Commander::Commander() : toStopServer(false), toStopListenSocket(false),
						liSocIsStoped(false),clients(), freeId(0)
{
	map_lock = new std::recursive_mutex();
}

Commander::~Commander()
{
	for (std::map<int, Client*>::iterator it = clients.begin();
		it != clients.end(); ++it)
	{
		delete it->second;
	}

	delete map_lock;
}

void Commander::setLsStruct(LsStruct* lsSt)
{
	lsStruct = lsSt;
}

void Commander::stopServer()
{
	auto iResult = closesocket(lsStruct->lsSocket);
	//в цикле происходит удаление элемента map со всеми последствиями
	for (std::map<int, Client*>::iterator it = clients.begin();
		it != clients.end();)
	{
		auto client = it->second;
		it++;
		stopClient(client->getId());
	}
	lsStruct->lsTh->join();

	toStopListenSocket = true;
	toStopServer = true;
}

bool Commander::stopClient(int id)
{
	Client* client;
	try{
		client = clients.at(id);
	}
	catch (const std::out_of_range& oor)
	{
		return false;
	}
	client->toStop = true;


	client->getThread()->join();
	clientStoped(client->getId());
	return true;
}

void Commander::stopListenSocket()
{
	toStopListenSocket = true;
}

//Возвращает ссылку на команды для клиента
Client* Commander::addClient()
{
	int id = getNextId();
	auto newClient = new Client();
	newClient->setId(id);
	map_lock->lock();
	clients.insert(std::pair<int, Client*>(id, newClient));
	map_lock->unlock();
	return newClient;
}


void Commander::clientStoped(int id)
{
	map_lock->lock();
	auto client = clients.at(id);
	delete client;
	clients.erase(id);
	map_lock->unlock();
}

bool Commander::serverIsStoped()
{
	if (clients.size() == 0 && liSocIsStoped == true)
		return true;
	return false;
}

//Возвращает свободный id для клиента
//toDo простоя увелечение ид - это фигня
int Commander::getNextId()
{
	return freeId++;
}