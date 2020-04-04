#include "stdafx.h"
#include "Server.h"

Server::Server()
{
	int retVal;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { return ; }

	// socket()

	listenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSock == INVALID_SOCKET) { err_quit("socket()"); }


	// connect()
	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = ntohs(SERVERPORT);

	retVal = bind(listenSock, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
	if (retVal == SOCKET_ERROR) { err_quit("bind()"); }


	// listen()
	retVal = listen(listenSock, SOMAXCONN);
	if (retVal == SOCKET_ERROR) { err_quit("listen()"); }


	// ������ ��ſ� ����� ����
	SOCKADDR_IN clientAddr;
	int addrLen;

	addrLen = sizeof(clientAddr);
	clientSock = accept(listenSock, (SOCKADDR*)&clientAddr, &addrLen);

	if (clientSock == INVALID_SOCKET)
	{
		err_display("accept()");
	}

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