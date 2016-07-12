#include "stdafx.h"
#include "Connection.h"
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
int SocketOperation::Connect(const char* IP, int Port)
{
	int rlt = 0;

	int iErrMsg;
	WSAData wsaData;
	iErrMsg = WSAStartup(MAKEWORD(2,2),&wsaData);
	if (iErrMsg != NO_ERROR)
	{
		printf("failed with wsaStartup error : %d\n",iErrMsg);

		rlt = 1;
		return rlt;
	}

	m_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (m_sock == INVALID_SOCKET)
	{
		printf("socket failed with error : %d\n",WSAGetLastError());

		rlt = 2;
		return rlt;
	}

	sockaddr_in sockaddrServer;
	sockaddrServer.sin_family = AF_INET;
	sockaddrServer.sin_port = htons(Port);
	sockaddrServer.sin_addr.s_addr = inet_addr(IP);

	iErrMsg = connect(m_sock,(sockaddr*)&sockaddrServer,sizeof(sockaddrServer));
	if (iErrMsg < 0)
	{
		printf("connect failed with error : %d\n",iErrMsg);
		std::cout << GetLastError() << std::endl;
		rlt = 3;
		return rlt;
	}

	return rlt;
}


void SocketOperation::Close()
{
	closesocket(m_sock);
}