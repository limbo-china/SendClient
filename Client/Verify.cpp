#include "stdafx.h"
#include "Verify.h"
#include <iostream>
#include <string>
#include <winsock2.h>
#include <iphlpapi.h>
#include "openssl/evp.h"
#include "evp.h"
#include "commonfunc.h"
using namespace std;
#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")
#pragma comment(lib,"Iphlpapi.lib")
Verify::Verify(SOCKET fd)
{
	C_Initialize(NULL_PTR);
	m_pSlotList = NULL_PTR;
	m_hSession = NULL_PTR;

	SocketFd = fd;
	OpenSSL_add_all_digests();
}

Verify::~Verify()
{

}

char *Verify::hexstr(unsigned char *bBuf, int nLen)
{
	const char *set = "0123456789abcdef";
	static char str[65], *tmp;
	unsigned char *end;
	if (nLen > 32)
		nLen = 32;
	end = bBuf + nLen;
	tmp = &str[0];
	while (bBuf < end)
	{
		*tmp++ = set[(*bBuf) >> 4];
		*tmp++ = set[(*bBuf) & 0xF];
		bBuf++;
	}
	tmp = NULL;
	return str;
}

int Verify::get_digest(char* bBuf1, char* bBuf2, char* sDig,
	int* pnLen)
{
	unsigned int mdlen;
	unsigned char md[EVP_MAX_MD_SIZE];
	EVP_MD_CTX* ctx = (EVP_MD_CTX*)calloc(sizeof(EVP_MD_CTX), 1);
	const EVP_MD *type = EVP_md5();

	EVP_DigestInit(ctx, type);
	EVP_DigestUpdate(ctx, bBuf1, strlen(bBuf1));
	EVP_DigestUpdate(ctx, bBuf2, strlen(bBuf2));
	EVP_DigestFinal(ctx, md, &mdlen);

	strncpy(sDig, hexstr(md, mdlen), strlen(hexstr(md, mdlen)));
	*pnLen = mdlen;
	if (ctx)
		free(ctx);
	return 0;
}

int Verify::CVerifyFinal()
{
	VERIFY_MESSAGE_BODY tBodySend;
	VERIFY_MESSAGE_BODY tBodyRcv;
	memset(tBodySend.szMessage, 0, VRFY_MAX_MESSAGE_LENGTH);
	memset(tBodyRcv.szMessage, 0, VRFY_MAX_MESSAGE_LENGTH);

	int nLen;
	tBodySend.cType = IA_AUTHENTICATION_DIG;
	get_digest(m_Serial, m_Key, tBodySend.szMessage,
		&nLen);
	tBodySend.unMessageLength = htonl(nLen);
	if (send(SocketFd, (char*)&tBodySend, sizeof(tBodySend), 0) < 0)
	{
		return IA_SOCKET_SEND_ERROR;
	}

	if (recv(SocketFd, (char*)&tBodyRcv, sizeof(tBodyRcv), 0) < 0)
	{
		return IA_SOCKET_RECV_ERROR;
	}

	if (tBodyRcv.cType == IA_AUTHENTICATION_OK)
	{
		return IA_GENETAL;
	}
	else
	{
		return IA_VERIFY_FAILER;
	}

}

int Verify::CLoginToken(char * passwd)
{
	CK_RV rv;

	rv = Connect();
	if (CKR_OK != rv)
	{
		C_Finalize(NULL_PTR);
		printf("connect error\n");
		return IA_UKEY_CONNECT_ERROR;
	}
	rv = Login(passwd);
	if (CKR_OK != rv)
	{
		C_Finalize(NULL_PTR);
		printf("Login error\n");
		return IA_UKEY_LOGIN_ERROR;
	}
	rv = GetUKData();
	if (CKR_OK != rv)
	{
		C_Finalize(NULL_PTR);
		printf("Get usr info error\n");
		return IA_UKEY_LOGIN_ERROR;
	}
	return IA_GENETAL;
}

int Verify::CKeyGeneration()
{
	VERIFY_MESSAGE_BODY tBodySend;
	VERIFY_MESSAGE_BODY tBodyRcv;

	tBodySend.cType = IA_AUTHENTICATION_START;
	memset(tBodySend.szMessage, 0, VRFY_MAX_MESSAGE_LENGTH);
	memset(tBodyRcv.szMessage, 0, VRFY_MAX_MESSAGE_LENGTH);

	memcpy(tBodySend.szMessage, m_Serial, P_SERIAL_LENGTH);
	memcpy(tBodySend.szMessage + P_SERIAL_LENGTH, m_UsrInfo, strlen(m_UsrInfo));

	tBodySend.unMessageLength = htonl(P_SERIAL_LENGTH + strlen(m_UsrInfo));
	if (send(SocketFd, (char*)&tBodySend, sizeof(tBodySend), 0) < 0)
	{
		return IA_SOCKET_SEND_ERROR;
	}

	if (recv(SocketFd, (char*)&tBodyRcv, sizeof(tBodyRcv), 0) < 0)
	{
		return IA_SOCKET_RECV_ERROR;
	}

	if (tBodyRcv.cType != IA_AUTHENTICATION_METHORD)
	{
		return IA_NOT_VAL_USR;
	}
	int nLen = ntohl(tBodyRcv.unMessageLength);
	if (nLen
		< 1 || nLen > VRFY_MAX_MESSAGE_LENGTH)
	{
		return IA_INTERACTION_MESSAGE_LENGTH_ERROR;
	}

	uint8_t sz_debuf[sizeof(VERIFY_PROTO)];

	CK_ULONG n_deLen = 0;
	CK_RV rv = RSADecrypt((CK_BYTE_PTR)tBodyRcv.szMessage,
		(CK_ULONG)nLen, (CK_BYTE_PTR)sz_debuf, (CK_ULONG_PTR)&n_deLen);

	if (CKR_OK != rv || n_deLen > P_KEY_LENGTH)
	{
		C_Finalize(NULL_PTR);
		printf("decrypt error\n");
		return IA_UKEY_DECRYPT_ERROR;
	}

	VERIFY_PROTO *p = (VERIFY_PROTO*)sz_debuf;


	m_Key = (char*)malloc(sizeof(char)*(AES_KEY_LENGTH + 1));
	memcpy(m_Key, p->cKey, AES_KEY_LENGTH);
	m_Key[AES_KEY_LENGTH] = '\0';
	m_nKeyLen = AES_KEY_LENGTH;

	mPort = ntohs(p->Port);
	mTunnelIP = p->unTunIP;
	return IA_GENETAL;
}


CK_RV Verify::GetUKData()
{
	CK_RV ret = CKR_OK;
	CK_OBJECT_CLASS cObjectClass = CKO_DATA;
	CK_BYTE bTrueValue = TRUE;
	CK_BYTE bFalseValue = FALSE;
	CK_BYTE sDataLab[] = "usr_name";
	CK_BYTE sDataBuf[1024] = { 0 };
	CK_ULONG uDataBuflen = 1024;
	CK_OBJECT_HANDLE hDataHandle = 0;
	CK_ULONG uObjCount = 0;
	CK_ATTRIBUTE DataAttr[] =
	{
		{ CKA_CLASS, &cObjectClass, sizeof(cObjectClass) },
		{ CKA_TOKEN, &bTrueValue, sizeof(bTrueValue) },
		{ CKA_LABEL, sDataLab, sizeof(sDataLab) - 1 },
		{ CKA_VALUE, sDataBuf, uDataBuflen } };
	ret = C_FindObjectsInit(m_hSession, DataAttr, 3);
	if (ret != CKR_OK)
	{
		return ret;
	}
	ret = C_FindObjects(m_hSession, &hDataHandle, 1, &uObjCount);
	if ((ret != CKR_OK) || (uObjCount == 0))
	{
		C_FindObjectsFinal(m_hSession);
		return -1;
	}
	ret = C_GetAttributeValue(m_hSession, hDataHandle, DataAttr, 4);
	C_FindObjectsFinal(m_hSession);
	if (ret != CKR_OK)
	{
		return ret;
	}

	memset(m_UsrInfo, 0, 256);
	memcpy(m_UsrInfo, DataAttr[3].pValue, DataAttr[3].ulValueLen);

	return CKR_OK;
}

int Verify::cVerify(char* passwd, char* ServerIP)
{
	int ret = IA_GENETAL;
	do
	{
		ret = CLoginToken(passwd);
		if (ret != IA_GENETAL)
		{
			break;
		}

		ret = CKeyGeneration();
		if (ret != IA_GENETAL)
		{
			break;
		}
	} while (0);

	if (m_hSession)
	{
		C_CloseSession(m_hSession);
		m_hSession = NULL_PTR;
	}
	if (m_pSlotList)
	{
		delete[] m_pSlotList;
		m_pSlotList = NULL_PTR;
	}
	C_Finalize(NULL_PTR);
	memset(m_sServerIP, 0, 20);
	strcpy(m_sServerIP, ServerIP);
	if (ret == IA_GENETAL)
		return CVerifyFinal();
	else
		return ret;
}

int Verify::cVerifyDataServer(char * sPasswd)
{
	int ret = CLoginToken(sPasswd);
	C_Finalize(NULL_PTR);
	return ret;
}

int Verify::CryptoEncrypt(char * pbSourceBuffer, char * pbDesBuffer, size_t unLenth)
{
	int ret = aes_cfb_encrypt((uint8_t*)pbSourceBuffer, (uint8_t*)pbDesBuffer, unLenth, (uint8_t*)m_Key);
	return ret;
}

int Verify::CryptoDecrypt(char * pbSourceBuffer, char * pbDesBuffer, size_t unLenth)
{
	int ret = aes_cfb_decrypt((uint8_t*)pbSourceBuffer, (uint8_t*)pbDesBuffer, unLenth, (uint8_t*)m_Key);
	return ret;
}

int Verify::GenerateCnfFile()
{
	FILE* fpCnfFile = fopen("client.conf1", "w");
	char* ori_name = (char*)calloc(100, 1);
	char* intf_name = (char*)calloc(100, 1);
	int ret = 0;
	if (fpCnfFile == NULL)
	{
		//..

	}
	do 
	{
		char Line[100] = { 0 };
		sprintf(Line, "#VPN config file\r\n");
		fwrite(Line, sizeof(char), strlen(Line), fpCnfFile);
		memset(Line, 0, 100);

		sprintf(Line, "server=%s\r\n", m_sServerIP);
		fwrite(Line, sizeof(char), strlen(Line), fpCnfFile);
		memset(Line, 0, 100);

		sprintf(Line, "port=%u\r\n", mPort);
		fwrite(Line, sizeof(char), strlen(Line), fpCnfFile);
		memset(Line, 0, 100);

		sprintf(Line, "password=%s\r\n", m_Key);
		fwrite(Line, sizeof(char), strlen(Line), fpCnfFile);
		memset(Line, 0, 100);

		sprintf(Line, "mode=client\r\n");
		fwrite(Line, sizeof(char), strlen(Line), fpCnfFile);
		memset(Line, 0, 100);

		sprintf(Line, "concurrency=1\r\n");
		fwrite(Line, sizeof(char), strlen(Line), fpCnfFile);
		memset(Line, 0, 100);

		sprintf(Line, "mtu=1440\r\n");
		fwrite(Line, sizeof(char), strlen(Line), fpCnfFile);
		memset(Line, 0, 100);
		

		if (GetAdapterName(intf_name, ori_name) != 0)
		{
			ret = 1;
			break;
		}
		if (strlen(ori_name) == 0
			|| strlen(intf_name) == 0)
		{
			ret = 2;
			break;
		}
		sprintf(Line, "orig_intf=%s\r\n", ori_name);
		fwrite(Line, sizeof(char), strlen(Line), fpCnfFile);
		memset(Line, 0, 100);

		sprintf(Line, "intf=%s\r\n", intf_name);
		fwrite(Line, sizeof(char), strlen(Line), fpCnfFile);
		memset(Line, 0, 100);

		struct in_addr addr;
		addr.s_addr = ntohl(mTunnelIP + 2);
		sprintf(Line, "tunip=%s\r\n", inet_ntoa(addr));
		fwrite(Line, sizeof(char), strlen(Line), fpCnfFile);
		memset(Line, 0, 100);

		addr.s_addr = ntohl(mTunnelIP + 1);
		sprintf(Line, "remote_tun_ip=%s\r\n", inet_ntoa(addr));
		fwrite(Line, sizeof(char), strlen(Line), fpCnfFile);
		memset(Line, 0, 100);

		sprintf(Line, "up=client_up.bat\r\n");
		fwrite(Line, sizeof(char), strlen(Line), fpCnfFile);
		memset(Line, 0, 100);


		sprintf(Line, "down=client_down.bat\r\n");
		fwrite(Line, sizeof(char), strlen(Line), fpCnfFile);
		memset(Line, 0, 100);
	} while (0);
	
	free(ori_name);
	free(intf_name);
	fflush(fpCnfFile);
	fclose(fpCnfFile);
	return ret;
}

unsigned short Verify::GetPort()
{
	return mPort;
}

unsigned int Verify::GetTunnelIP()
{
	return mTunnelIP;
}

char * Verify::GetUsrInfo()
{
	return m_UsrInfo;
}

char *Verify::GetSerial()
{
	return m_Serial;
}

void Verify::ShowMsg(string strInfo)
{

	cout << "[INFO]: " << strInfo << endl;
}

void Verify::ShowErr(string strInfo, CK_RV rv)
{
	printf("[ERROR] %s\t\tErrCode = 0x%08X.", strInfo.c_str(), rv);
}

int Verify::Connect()
{
	if (m_hSession)
		return -1;
	CK_RV rv;
	CK_ULONG ulCount = 0;
	rv = C_GetSlotList(TRUE, NULL_PTR, &ulCount);
	if (CKR_OK != rv)
	{
		ShowErr("Can't acquire information of slot", rv);
		return rv;
	}
	if (0 >= ulCount)
	{
		ShowMsg("Can't connect to token, make sure one token has been inserted.");
		return rv;
	}

	m_pSlotList = (CK_SLOT_ID_PTR)new CK_SLOT_ID[ulCount];
	if (!m_pSlotList)
	{
		ShowMsg("Not enough memory!");
		return rv;
	}

	rv = C_GetSlotList(TRUE, m_pSlotList, &ulCount);
	if (CKR_OK != rv)
	{
		ShowErr("Can't acquire information of slot", rv);
		return rv;
	}
	if (0 >= ulCount)
	{
		ShowMsg("Can't connect to token, make sure one token has been inserted.");
		return rv;
	}
	CK_TOKEN_INFO t_info;

	rv = C_GetTokenInfo(m_pSlotList[0], &t_info);
	sprintf(m_Serial, "%s", t_info.serialNumber);
	m_Serial[P_SERIAL_LENGTH] = '\0';

	rv = C_OpenSession(
		m_pSlotList[0], CKF_RW_SESSION | CKF_SERIAL_SESSION,
		&m_pApplication, NULL_PTR, &m_hSession);
	if (CKR_OK != rv)
	{
		ShowErr("Can't Acquire information of slot", rv);
		delete[] m_pSlotList;
		m_pSlotList = NULL_PTR;
	}
	else
	{
		ShowMsg("Connect to token Successfully !");
	}
	return CKR_OK;
}

int Verify::Login(char* passwd)
{
	CK_RV rv;
	rv = C_Login(m_hSession, CKU_USER, (CK_BYTE_PTR)passwd,
		strlen(passwd));
	if (CKR_OK != rv)
	{
		ShowErr("Can't use your pin login to Token\n", rv);
		return CKR_GENERAL_ERROR;
	}
	else
	{
		ShowMsg("Success login to Token\n");
	}

	return CKR_OK;
}

int Verify::RSAEncrypt(CK_BYTE_PTR pbSourceBuffer,
	CK_ULONG uLenIN, CK_BYTE_PTR pbDesBuffer, CK_ULONG_PTR puLenOut)
{
	CK_OBJECT_CLASS dataClass = CKO_PUBLIC_KEY;
	bool IsToken = true;
	CK_MECHANISM ckMechanism = { CKM_RSA_PKCS, NULL_PTR, 0 };
	CK_ATTRIBUTE pTempl[] =
	{
		{ CKA_CLASS, &dataClass, sizeof(CKO_PUBLIC_KEY) },
		{ CKA_TOKEN, &IsToken, sizeof(true) }
	};


	C_FindObjectsInit(m_hSession, pTempl, 2);

	CK_OBJECT_HANDLE hCKObj;
	CK_ULONG ulRetCount = 0;
	CK_ULONG m_ulCipherLen = 0;
	CK_RV rv;
	rv = C_FindObjects(m_hSession, &hCKObj, 1, &ulRetCount);
	C_FindObjectsFinal(m_hSession);
	if (CKR_OK != rv)
	{
	}

	rv = C_EncryptInit(m_hSession,
		&ckMechanism,
		hCKObj);
	if (CKR_OK != rv)
	{
		ShowErr("Failed to call EncryptInit !", rv);
		return rv;
	}

	rv = C_Encrypt(m_hSession, pbSourceBuffer, uLenIN, NULL_PTR, &m_ulCipherLen);
	if (CKR_OK != rv)
	{
		ShowErr("Can't acquire the size of Data After encrypt", rv);
		return rv;
	}

	rv = C_Encrypt(m_hSession, pbSourceBuffer, uLenIN, pbDesBuffer, &m_ulCipherLen);
	if (CKR_OK != rv)
	{
		ShowErr("Failed to encrypt!", rv);
		return rv;
	}
	*puLenOut = m_ulCipherLen;
	return CKR_OK;
}


int Verify::RSADecrypt(CK_BYTE_PTR pbSourceBuffer,
	CK_ULONG uLenIN, CK_BYTE_PTR pbDesBuffer, CK_ULONG_PTR puLenOut)
{
	CK_OBJECT_CLASS dataClass = CKO_PRIVATE_KEY;
	CK_MECHANISM ckMechanism = { CKM_RSA_PKCS, NULL_PTR, 0 };
	bool IsToken = true;
	CK_ATTRIBUTE pTempl[] =
	{
		{ CKA_CLASS, &dataClass, sizeof(CKO_PRIVATE_KEY) },
		{ CKA_TOKEN, &IsToken, sizeof(true) }
	};

	C_FindObjectsInit(m_hSession, pTempl, 2);

	CK_OBJECT_HANDLE hCKObj;
	CK_ULONG ulRetCount = 0;
	CK_ULONG ulRestoredMsgLen;
	CK_RV rv;
	int numObj = 0;

	rv = C_FindObjects(m_hSession, &hCKObj, 1, &ulRetCount);
	C_FindObjectsFinal(m_hSession);
	if (CKR_OK != rv)
	{
		return rv;
	}

	rv = C_DecryptInit(m_hSession,
		&ckMechanism,
		hCKObj);
	if (CKR_OK != rv)
	{
		ShowErr("Failed to call DecryptInit!", rv);
		return rv;
	}

	rv = C_Decrypt(m_hSession, pbSourceBuffer, uLenIN, NULL_PTR, &ulRestoredMsgLen);
	if (CKR_OK != rv)
	{
		ShowErr("Can't acquire size of Data after Decrypt", rv);
		return rv;
	}

	rv = C_Decrypt(m_hSession, pbSourceBuffer, uLenIN, pbDesBuffer, &ulRestoredMsgLen);
	if (CKR_OK != rv)
	{
		ShowErr("Failed to call decrypt.", rv);
		return rv;
	}
	*puLenOut = ulRestoredMsgLen;
	return CKR_OK;
}
