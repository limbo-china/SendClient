#ifndef _VERIFY_H_
#define _VERIFY_H_

#include <pkcs11/cryptoki_ext.h>
#include <string>
#include<windows.h>

#define FLAG_SERVER 0x01
#define FLAG_CLIENT 0x02


#define IA_BASE 0

#define IA_GENETAL (IA_BASE + 0)
#define IA_SOCKET_SEND_ERROR (IA_BASE + 1)
#define IA_SOCKET_RECV_ERROR (IA_BASE +2)
#define IA_UKEY_CONNECT_ERROR (IA_BASE + 3)
#define IA_UKEY_LOGIN_ERROR (IA_BASE +4)
#define IA_UKEY_LOAD_LIB_ERROR (IA_BASE +5)
#define IA_UKEY_DECRYPT_ERROR (IA_BASE +6)
#define IA_UKEY_GET_TOKEN_ERROR (IA_BASE +7)
#define IA_INTERACTION_VERSION_ERROR (IA_BASE +8)
#define IA_INTERACTION_MESSAGE_LENGTH_ERROR (IA_BASE +9)
#define IA_VERIFY_FAILER (IA_BASE + 10)
#define IA_NOT_VAL_USR (IA_BASE + 11)
#define IA_GET_MEMORY_ERROR (IA_BASE + 12)
#define IA_RSA_INIT_ERROR (IA_BASE + 13)
#define IA_RSA_EN_ERROR (IA_BASE + 14)
#define IA_INIT_ADEPTER_INFO_ERROR (IA_BASE + 15)

#define IA_TOTAL_METHORD 4
#define P_KEY_LENGTH 128
#define P_SERIAL_LENGTH 16
#define VRFY_MAX_MESSAGE_LENGTH 1025
#define EN_MAX_BUFFER_SIZE 1024

#define IA_AUTHENTICATION_START 0x01
#define IA_AUTHENTICATION_METHORD 0x02
#define IA_AUTHENTICATION_UNREG 0x03
#define IA_AUTHENTICATION_DIG 0x04

#define IA_AUTHENTICATION_OK 0x00
#define IA_AUTHENTICATION_FAILED 0x0A

#define AES_KEY_LENGTH 16

#pragma pack (1)
typedef struct _VERIFY_MESSAGE_BODY_
{
	char cType;
	int unMessageLength;
	char szMessage[VRFY_MAX_MESSAGE_LENGTH];
} VERIFY_MESSAGE_BODY;

typedef struct _VERIFY_PROTO_
{
	unsigned short Port;
	char cKey[AES_KEY_LENGTH];
	unsigned int unTunIP;
	unsigned int unRemoteTunIP;
} VERIFY_PROTO;

#pragma pack()

class Verify
{
public:
	Verify(SOCKET fd);
	~Verify();
	int cVerify(char* sPasswd, char* sServerIP);
	int cVerifyDataServer(char* sPasswd);

	int CryptoEncrypt(char* pbSourceBuffer, char* pbDesBuffer, size_t unLenth);

	int CryptoDecrypt(char* pbSourceBuffer, char* pbDesBuffer, size_t unLenth);

	CK_RV GetUKData();
	int GenerateCnfFile();
	unsigned short GetPort();
	unsigned int GetTunnelIP();
	char *GetUsrInfo();
	char *GetSerial();
protected:

	char *hexstr(unsigned char *bBuffer, int nLen);
	int get_digest(char* bBuf1, char* bBuf2, char* sDig,
		int* pnLen);
	int CVerifyFinal();
	int CLoginToken(char* sPasswd);
	int CKeyGeneration();
	void ShowMsg(std::string strInfo);
	void ShowErr(std::string strInfo, CK_RV rv);
	int Connect();
	int Login(char* strPasswd);
	int RSAEncrypt(CK_BYTE_PTR pbSourceBuffer, CK_ULONG uLenIN, CK_BYTE_PTR pbDesBuffer, CK_ULONG_PTR puLenOut);
	
	int RSADecrypt(CK_BYTE_PTR pbSourceBuffer, CK_ULONG uLenIN, CK_BYTE_PTR pbDesBuffer, CK_ULONG_PTR puLenOut);

	CK_SLOT_ID_PTR m_pSlotList;
	CK_SESSION_HANDLE m_hSession;
	CK_VOID_PTR m_pApplication;

	SOCKET SocketFd;
	char* m_Key;
	int m_nKeyLen;
	char m_Serial[P_SERIAL_LENGTH + 1];
	char m_UsrInfo[256];
	unsigned short mPort;
	unsigned int mTunnelIP;
	char m_sServerIP[20];
private:
};
#endif/* _VERIFY_H_ */