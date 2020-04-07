#pragma once
#include "Object.h"
#include "Globals.h"


class Client_Socket
{
public:
	SOCKET sock;
	char buf[BUFSIZ];
	
	Client_Socket();
	~Client_Socket();

	void err_quit(const char* msg);
	void err_display(const char* msg);
	
	int recvn(SOCKET s, char* buf, int len, int flags);

	void SendBufToServer(Packet data);
	void RecvBufFromServer(Object* player);
};


