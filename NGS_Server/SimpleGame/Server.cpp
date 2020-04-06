#include "stdafx.h"
#include "Server.h"

Server::Server()
{
	int retVal;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { return ; }

	// socket()
	// listen ������ OVERLAPPED ��������
	listenSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (listenSock == INVALID_SOCKET) { err_quit("socket()"); }


	// bind()
	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = ntohs(SERVER_PORT);

	retVal = bind(listenSock, (struct sockaddr*) & serverAddr, sizeof(SOCKADDR_IN));
	if (retVal == SOCKET_ERROR) { err_quit("bind()"); }


	// listen()
	retVal = listen(listenSock, 10);
	if (retVal == SOCKET_ERROR) { err_quit("listen()"); }


	// accept()
	// ������ ��ſ� ����� ����
	SOCKADDR_IN clientAddr;
	int addrLen;

	addrLen = sizeof(clientAddr);
	clientSock = accept(listenSock, (SOCKADDR*)&clientAddr, &addrLen);
	if (clientSock == INVALID_SOCKET)
	{
		err_display("accept()");
	}

	g_clients[clientSock] = SOCKETINFO{};
	g_clients[clientSock].socket = clientSock;
	g_clients[clientSock].dataBuffer.len = MAX_BUFFER;
	g_clients[clientSock].dataBuffer.buf = g_clients[clientSock].messageBuffer;
	memset(&g_clients[clientSock].overlapped, 0, sizeof(WSAOVERLAPPED));	
	g_clients[clientSock].overlapped.hEvent = (HANDLE)g_clients[clientSock].socket;	
																					
	DWORD flags = 0;
	WSARecv(g_clients[clientSock].socket, &g_clients[clientSock].dataBuffer, 1, NULL,
		&flags, &(g_clients[clientSock].overlapped), recv_callback);

	// ������ Ŭ���̾�Ʈ ���� ���
	std::cout << "\n[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ� = " << inet_ntoa(clientAddr.sin_addr)
		<< " ��Ʈ ��ȣ = " << ntohs(clientAddr.sin_port) << std::endl;
}


void Server::SendBufToClient(Object data)
{
	int retVal;

	bufSize = sizeof(data);

	// ���� ����
	retVal = send(clientSock, (char*)&bufSize, sizeof(int), 0);
	if (retVal == SOCKET_ERROR) {
		err_display("send()");
	}

	// ���� ����
	retVal = send(clientSock, (char*)&data, sizeof(data), 0);
	if (retVal == SOCKET_ERROR)
	{
		err_display("send()");
	}
		
}

SendData* Server::RecvBufFromClient()
{
	int retVal;
	
	buf.clear();

	// ������ �ޱ�(���� ����)
		// ���� �̸� ũ��
	retVal = recvn(clientSock, (char*)&bufSize, sizeof(int), 0);
	if (retVal == SOCKET_ERROR) {
		err_display("recv()");
	}
	

	// ������ �ޱ�(���� ����)
	// ���� �̸� �ޱ�
	retVal = recvn(clientSock, (char*)buf.c_str(), bufSize, 0);
	if (retVal == SOCKET_ERROR) {
		err_display("recv()");
	}
	
	SendData * data = (SendData*)buf.c_str();

	return data;
}


Server::~Server()
{
	closesocket(clientSock);
	closesocket(listenSock);

	WSACleanup();
}

void Server::err_quit(const char * msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void Server::err_display(const char * msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

	std::cout << "[" << msg << "] " << (char*)lpMsgBuf << std::endl;
	LocalFree(lpMsgBuf);
}

int Server::recvn(SOCKET s,  char * buf, int len, int flags) {
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);

		if (received == SOCKET_ERROR) { return SOCKET_ERROR; }
		else if (received == 0) { break; }

		left -= received;
		ptr += received;
	}

	return (len - left);
}

void CALLBACK recv_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags)
{
	SOCKET client_s = reinterpret_cast<int>(overlapped->hEvent);	// ���� ���´°�?�� �˾Ƴ�

	if (dataBytes == 0)
	{
		closesocket(g_clients[client_s].socket);
		g_clients.erase(client_s);
		return;
	}  // Ŭ���̾�Ʈ�� closesocket�� ���� ���

	g_clients[client_s].messageBuffer[dataBytes] = 0;	// �����͸� �������� ��Ʈ���� ������ ������ 0�� �־ ������ �ɷ���
	cout << "From client : " << g_clients[client_s].messageBuffer << " (" << dataBytes << ") bytes)\n";
	g_clients[client_s].dataBuffer.len = dataBytes;
	memset(&(g_clients[client_s].overlapped), 0, sizeof(WSAOVERLAPPED));	// �������� ����ü ������ ���� �ʱ�ȭ
	g_clients[client_s].overlapped.hEvent = (HANDLE)client_s;
	WSASend(client_s, &(g_clients[client_s].dataBuffer), 1, NULL, 0, &(g_clients[client_s].overlapped), send_callback);
}

void CALLBACK send_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags)
{
	DWORD receiveBytes = 0;
	DWORD flags = 0;

	SOCKET client_s = reinterpret_cast<int>(overlapped->hEvent);

	if (dataBytes == 0) {
		closesocket(g_clients[client_s].socket);
		g_clients.erase(client_s);
		return;
	}  // Ŭ���̾�Ʈ�� closesocket�� ���� ���

	// WSASend(���信 ����)�� �ݹ��� ���
	g_clients[client_s].dataBuffer.len = MAX_BUFFER;

	cout << "TRACE - Send message : " << g_clients[client_s].messageBuffer << " (" << dataBytes << " bytes)\n";
	memset(&(g_clients[client_s].overlapped), 0, sizeof(WSAOVERLAPPED));
	g_clients[client_s].overlapped.hEvent = (HANDLE)client_s;
	WSARecv(client_s, &g_clients[client_s].dataBuffer, 1, 0, &flags, &(g_clients[client_s].overlapped), recv_callback);
}