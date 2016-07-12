#ifndef _SESSION_H_
#define _SESSION_H_
#include "Connection.h"

class CClientSession
{
public:
	CClientSession();
	~CClientSession();

	
	bool m_bVPNActive;
	unsigned short m_uVPNServerPort;
	char m_sUsrInfo[256];
	char m_sSerial[64];
	char m_sDataServerIP[20];
	char m_sVPNServerIP[20];
	char m_sBaseDir[256];
	SocketOperation m_ctx;
	unsigned short m_uDataServerPort;
	bool m_bLogin;
	bool m_bKeeplive;
	bool m_bIfAuto;
	bool m_bIfOnline;
};


extern CClientSession session;

#endif //_SESSION_H_