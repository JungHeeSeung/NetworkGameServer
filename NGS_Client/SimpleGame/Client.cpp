#include "stdafx.h"
#include "Client.h"

Client_Socket::Client_Socket()
{
	int retVal;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) 
	{ 
		return ;
	}

	// socket()
	sock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (sock == INVALID_SOCKET) 
	{
		err_quit("socket()");
	}


	string serverIP;
	cout << "IP�� �Է����ּ���: ";
	cin >> serverIP;

	// connect()
	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.S_un.S_addr = inet_addr(serverIP.c_str());
	serverAddr.sin_port = ntohs(SERVER_PORT);

	retVal = connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (retVal == SOCKET_ERROR)
	{
		err_quit("connect()");
	}
}

Client_Socket::~Client_Socket()
{
	closesocket(sock);

	WSACleanup();
}

void Client_Socket::SendBufToServer(Packet data)
{
	int retVal;

	// ������ ������
	retVal = send(sock, (char*)&data, sizeof(data), 0);
	if (retVal == SOCKET_ERROR) {
		err_display("send()");
	}

	cout << sizeof(data) << "��ŭ ���´�!" << endl;
}

void Client_Socket::RecvBufFromServer(Object* player)
{
	int retVal;

	memset(&buf, 0, sizeof(buf));

	// ���� ���� ������ �ޱ�
	retVal = recv(sock, (char*)buf, sizeof(Object), 0);
	if (retVal == SOCKET_ERROR) {
		err_display("recv()");
	}

	cout << sizeof(Object) << "��ŭ �޾���!" << endl;
	
	/*player = (Object*)buf;*/

	Object* temp = (Object*)buf;

	float x, y, z;
	temp->GetPos(&x, &y, &z);

	cout << "x: " << x << " y: " << y << " z: " << z << endl;
}

void Client_Socket::err_quit(const char* msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void Client_Socket::err_display(const char* msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

	std::cout << "[" << msg << "] " << (char*)lpMsgBuf << std::endl;
	LocalFree(lpMsgBuf);
}

int Client_Socket::recvn(SOCKET s, char* buf, int len, int flags) 
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);

		if (received == SOCKET_ERROR) {
			return SOCKET_ERROR;
		}
		else if (received == 0) {
			break;
		}

		left -= received;
		ptr += received;
	}

	return (len - left);
}