#include "stdafx.h"
#include "Client.h"

Client_Socket::Client_Socket() {
	int retVal;

	bufSize = 0;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { return ; }

	// socket()
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) { err_quit("socket()"); }

	string serverIP;
	cout << "IP를 입력해주세요: ";
	cin >> serverIP;

	// connect()
	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.S_un.S_addr = inet_addr(serverIP.c_str());
	serverAddr.sin_port = ntohs(SERVERPORT);

	retVal = connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (retVal == SOCKET_ERROR) { err_quit("connect()"); }
}

Client_Socket::~Client_Socket()
{
	closesocket(sock);

	WSACleanup();
}

void Client_Socket::SendBufToServer(SendData data)
{
	int retVal;

	int bufSize = sizeof(data);

	// 데이터의 길이 가르쳐주기
	retVal = send(sock, (char*)&bufSize, sizeof(int), 0);
	if (retVal == SOCKET_ERROR) {
		err_display("send()");
	}

	// 데이터 보내기
	retVal = send(sock, (char*)&data, sizeof(data), 0);
	if (retVal == SOCKET_ERROR) {
		err_display("send()");
	}
}

Object* Client_Socket::RecvBufFromServer()
{
	int retVal;

	buf.clear();

	// 고정 길이 데이터 받기
	retVal = recvn(sock, (char*)&bufSize, sizeof(int), 0);
	if (retVal == SOCKET_ERROR) {
		err_display("recv()");
	}

	// 가변 길이 데이터 받기
	retVal = recvn(sock, (char*)buf.c_str(), bufSize, 0);
	if (retVal == SOCKET_ERROR) {
		err_display("recv()");
	}

	Object* player = (Object*)buf.c_str();

	return player;
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