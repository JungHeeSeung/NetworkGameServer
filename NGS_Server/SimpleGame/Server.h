#pragma once

#include "Object.h"
#include "Globals.h"

struct SOCKETINFO	
{
	WSAOVERLAPPED overlapped;	

	WSABUF dataBuffer;	
						
	SOCKET socket;		
	char messageBuffer[MAX_BUFFER];	
									
};

extern map <SOCKET, SOCKETINFO> g_clients;

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

void CALLBACK recv_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags);
void CALLBACK send_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags);
