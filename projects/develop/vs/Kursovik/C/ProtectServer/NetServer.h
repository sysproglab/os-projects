#pragma once

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <thread>

#include <string>

class NetServer
{
public:
	NetServer();
	~NetServer();
	bool startServer();
};