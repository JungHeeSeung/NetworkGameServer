#pragma once

#include "Object.h"

#define SERVERPORT 9000

class Server
{
public:
	Server();
	~Server();

	SOCKET clientSock;
	SOCKET listenSock;
	string buf;
	int bufSize;

	void err_quit(const char* msg);
	void err_display(const char* msg);
	int recvn(SOCKET s, char* buf, int len, int flags);


	void SendBufToClient(Object data);
	SendData* RecvBufFromClient();
};


