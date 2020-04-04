#pragma once
#include "Object.h"

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000

class Client_Socket
{
public:
	SOCKET sock;
	string buf;
	int bufSize;

	Client_Socket();
	~Client_Socket();

	void err_quit(const char* msg);
	void err_display(const char* msg);
	
	int recvn(SOCKET s, char* buf, int len, int flags);

	void SendBufToServer(SendData data);
	Object* RecvBufFromServer();
};


