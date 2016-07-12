#ifndef _CONNECTION_H_
#define _CONNECTION_H_


#include <stdio.h>
#include<windows.h>


class SocketOperation
{
public:
	int Connect(const char* IP, int Port);
	
	void Close();

	SOCKET m_sock;
};
#endif