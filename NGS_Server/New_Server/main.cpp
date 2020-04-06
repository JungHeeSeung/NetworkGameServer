#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <map>	
using namespace std;
#include <WS2tcpip.h>
#include "Object.h"
#pragma comment(lib, "Ws2_32.lib")	

#define MAX_MAP_X			8
#define MAX_MAP_Y			8
#define HERO_ID				0
#define MAX_BUFFER			1024
#define SERVER_PORT			3500	

struct Packet
{
	bool m_KeyUp;
	bool m_KeyDown;
	bool m_KeyLeft;
	bool m_KeyRight;
};

struct SOCKETINFO	
{					
	WSAOVERLAPPED overlapped;	
	WSABUF dataBuffer; 
	SOCKET socket;	
	
	Packet packData;
	short player_ID;
};									
						
short numOfPlayer = 1;
map <SOCKET, SOCKETINFO> clients;	
Packet packet;
Object * g_Map[MAX_MAP_X][MAX_MAP_Y];
Object * g_Player[10];

							
void Init_Game();
void Update(Packet pack, Object Player);

void err_quit(const char* msg);
void err_display(const char* msg);

void CALLBACK recv_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags);
void CALLBACK send_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags);

void Init_Game()
{
	// Initialize Map
	for (int x = 0; x < MAX_MAP_X /* Map_X : 8*/; x++)
	{
		for (int y = 0; y < MAX_MAP_Y /* Map_Y : 8*/; y++)
		{
			g_Map[x][y] = new Object();
			g_Map[x][y]->SetPos(x - 4.f, y - 4.f, 0);	// posX : -4 ~ 4
			g_Map[x][y]->SetVol(1, 1, 1);				// posY : -4 ~ 4
		
			if ((x + y + 1) % 2 != 1) // 1이 아니면 흰색 타일
			{
				g_Map[x][y]->SetColor(1, 1, 1, 1);
			}
			else					// 검은색 타일일 경우
			{
				g_Map[x][y]->SetColor(0, 0, 0, 1);
			}
		}
	}

	// Initialize Player
	for (int k = 0; k < 10; ++k)
	{
		g_Player[k] = new Object();
		g_Player[k]->SetPos(-4, -4, 0);
		g_Player[k]->SetVol(0.5, 0.5, 0.5);
	}

	// Player Color
	{
		g_Player[0]->SetColor(1, 0, 0, 1);			// Red
		g_Player[1]->SetColor(0, 0, 1, 1);			// Blue
		g_Player[2]->SetColor(0, 1, 0, 1);			// Green
		g_Player[3]->SetColor(1, 1, 0, 1);			// Yellow
		g_Player[4]->SetColor(0.5, 0, 0.5, 1);		// Purple
		g_Player[5]->SetColor(1, 0.65, 0, 1);		// Orange
		g_Player[6]->SetColor(0, 0, 0.5, 1);		// Dark Blue
		g_Player[7]->SetColor(1, 0.4, 0.7, 1);		// Hot Pink
		g_Player[8]->SetColor(0, 1, 1, 1);			// Cyan
		g_Player[9]->SetColor(0.65, 0.15, 0.15, 1); // Brown
	}
}

void Update(Packet pack, Object Player)
{
	float x, y, z;

	if (Player != NULL)
	{
		Player.GetPos(&x, &y, &z);

		if (pack.m_KeyUp)
		{
			if (y > -4)
			{
				y += 1;
			}
		}
		if (pack.m_KeyDown)
		{
			if (y < 4)
			{
				y -= 1;
			}
		}
		if (pack.m_KeyLeft)
		{
			if (x > -4)
			{
				x += 1;
			}
		}
		if (pack.m_KeyRight)
		{
			if (x < 4)
			{
				x -= 1;
			}
		}

		Player.SetPos(x, y, z);
	}
}

void CALLBACK recv_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags)
{
	SOCKET client_s = reinterpret_cast<int>(overlapped->hEvent);	

	if (dataBytes == 0)	
	{
		closesocket(clients[client_s].socket);
		clients.erase(client_s);
		return;
	}  
	
	cout << "From client : " << sizeof(clients[client_s].packData) << " (" << dataBytes << ") bytes)\n";

	// Game Logic Update
	Update(clients[client_s].packData, *(g_Player[clients[client_s].player_ID - 1]));


	clients[client_s].dataBuffer.len = dataBytes;
	memset(&(clients[client_s].overlapped), 0, sizeof(WSAOVERLAPPED));	
	clients[client_s].overlapped.hEvent = (HANDLE)client_s;


	WSASend(client_s, &(clients[client_s].dataBuffer), 1, NULL, 0, &(clients[client_s].overlapped), send_callback);
}

void CALLBACK send_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags)
{
	DWORD receiveBytes = 0;
	DWORD flags = 0;

	SOCKET client_s = reinterpret_cast<int>(overlapped->hEvent);

	if (dataBytes == 0) {
		closesocket(clients[client_s].socket);
		clients.erase(client_s);
		return;
	}  

	clients[client_s].dataBuffer.len = MAX_BUFFER;

	clients[client_s].dataBuffer.buf = (char*)&(g_Player[ clients[client_s].player_ID - 1 /* Client ID */ ]);

	cout << "TRACE - Send message : " << sizeof(clients[client_s].packData) << " (" << dataBytes << " bytes)\n";
	memset(&(clients[client_s].overlapped), 0, sizeof(WSAOVERLAPPED));
	clients[client_s].overlapped.hEvent = (HANDLE)client_s;


	WSARecv(client_s, &clients[client_s].dataBuffer, 1, 0, &flags, &(clients[client_s].overlapped), recv_callback);
}

int main()
{
	int retVal;

	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);	

	// Bind 
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	
	retVal = ::bind(listenSocket, (struct sockaddr*) & serverAddr, sizeof(SOCKADDR_IN));
	if (retVal == SOCKET_ERROR) { err_quit("bind()"); }

	// Listen
	retVal = listen(listenSocket, 10);
	if (retVal == SOCKET_ERROR) { err_quit("listen()"); }

	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(SOCKADDR_IN);
	memset(&clientAddr, 0, addrLen);


	// Accept
	// 다중 접속이니 Accept은 계속되어야 한다

	while (true) {
		SOCKET clientSocket = accept(listenSocket, (struct sockaddr*) & clientAddr, &addrLen);
		clients[clientSocket] = SOCKETINFO{};
		clients[clientSocket].socket = clientSocket;
		clients[clientSocket].dataBuffer.len = MAX_BUFFER;
		clients[clientSocket].dataBuffer.buf = (char*)&(clients[clientSocket].packData);
		clients[clientSocket].player_ID = numOfPlayer;
		numOfPlayer++;

		memset(&clients[clientSocket].overlapped, 0, sizeof(WSAOVERLAPPED));	// 쓰기전에 반드시 0으로 초기화
		clients[clientSocket].overlapped.hEvent = (HANDLE)clients[clientSocket].socket;	
																						
		DWORD flags = 0;
		WSARecv(clients[clientSocket].socket, &clients[clientSocket].dataBuffer, 1, NULL,	
			&flags, &(clients[clientSocket].overlapped), recv_callback);
	}
	closesocket(listenSocket);
	WSACleanup();
}

void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void err_display(const char* msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

	std::cout << "[" << msg << "] " << (char*)lpMsgBuf << std::endl;
	LocalFree(lpMsgBuf);
}

// Recv -> Send -> Recv -> ... 서로 주고받음