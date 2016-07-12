#include "stdafx.h"
#include "Client.h"
#include "afxdialogex.h"
#include <string.h>
#include <iostream>
#include <string>
#include "RegVerify.h"

#define CodeNum 10

CString RegCode[CodeNum] = { _T("96BAC-049D8-59DC1-CD406-A3D1F"),
_T("E4887-77200-C6044-1C2B5-E29DB"),
_T("5B1C2-5AB48-9761E-497C4-2F4F9"),
_T("D6C01-21020-C3C78-ACE11-07730"),
_T("48F97-77280-15F22-B8C8E-B4663"),
_T("38A61-21020-02A93-15C65-F193D"),
_T("2B572-DABA8-0A10F-AB1A2-CFCED"),
_T("FFC19-69890-E8646-CE138-7B1EF"),
_T("C4F09-94858-FDEC3-BB55C-FC69B"),
_T("FC531-DC0C8-A6486-D54A0-AD7A5")};


CRegVerify::CRegVerify(CString code) {
	int nLength = code.GetLength();
	int nBytes = WideCharToMultiByte(CP_ACP, 0, code, nLength, NULL, 0, NULL, NULL);
	char* temp = new char[nBytes + 1];
	memset(temp, 0, nLength + 1);
	WideCharToMultiByte(CP_OEMCP, 0, code, nLength, temp, nBytes, NULL, NULL);
	temp[nBytes] = 0;
	strcpy(RCode, temp);
}
CRegVerify::CRegVerify() {

}
CRegVerify::~CRegVerify() {

}
BOOL CRegVerify::GetCpuByCmd()
{
	const long MAX_COMMAND_SIZE = 10000; // ��������������С	
	WCHAR szFetCmd[] = L"wmic cpu get processorid"; // ��ȡCPU���к�������	
	const std::string strEnSearch = "ProcessorId"; // CPU���кŵ�ǰ����Ϣ

	BOOL   bret = FALSE;
	HANDLE hReadPipe = NULL; //��ȡ�ܵ�
	HANDLE hWritePipe = NULL; //д��ܵ�	
	PROCESS_INFORMATION pi;   //������Ϣ	
	STARTUPINFO			si;	  //���������д�����Ϣ
	SECURITY_ATTRIBUTES sa;   //��ȫ����

	char			szBuffer[MAX_COMMAND_SIZE + 1] = { 0 }; // ���������н�������������
	std::string			strBuffer;
	unsigned long	count = 0;
	long			ipos = 0;

	memset(&pi, 0, sizeof(pi));
	memset(&si, 0, sizeof(si));
	memset(&sa, 0, sizeof(sa));

	pi.hProcess = NULL;
	pi.hThread = NULL;
	si.cb = sizeof(STARTUPINFO);
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	//1.0 �����ܵ�
	bret = CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
	if (!bret)
	{
		goto END;
	}

	//2.0 ���������д��ڵ���ϢΪָ���Ķ�д�ܵ�
	GetStartupInfo(&si);
	si.hStdError = hWritePipe;
	si.hStdOutput = hWritePipe;
	si.wShowWindow = SW_HIDE; //���������д���
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	//3.0 ������ȡ�����еĽ���
	bret = CreateProcess(NULL, szFetCmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
	if (!bret)
	{
		goto END;
	}

	//4.0 ��ȡ���ص�����
	WaitForSingleObject(pi.hProcess, 500/*INFINITE*/);
	bret = ReadFile(hReadPipe, szBuffer, MAX_COMMAND_SIZE, &count, 0);
	if (!bret)
	{
		goto END;
	}

	//5.0 ����CPU���к�
	bret = FALSE;
	strBuffer = szBuffer;
	ipos = strBuffer.find(strEnSearch);

	if (ipos < 0) // û���ҵ�
	{
		goto END;
	}
	else
	{
		strBuffer = strBuffer.substr(ipos + strEnSearch.length());
	}

	memset(szBuffer, 0x00, sizeof(szBuffer));
	strcpy_s(szBuffer, strBuffer.c_str());

	//ȥ���м�Ŀո� 


	int j = 0;
	for (int i = 0; i < strlen(szBuffer); i++)
	{
		if (szBuffer[i] != ' ' && szBuffer[i] != '\r' && szBuffer[i] != '\n')
		{
			lpszCpu[j] = szBuffer[i];
			j++;
		}
	}
	lpszCpu[j] = 0;
	bret = TRUE;

END:
	//�ر����еľ��
	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return(bret);
}
void CRegVerify::SetRCode(CString code) {
	int nLength = code.GetLength();
	int nBytes = WideCharToMultiByte(CP_ACP, 0, code, nLength, NULL, 0, NULL, NULL);
	char* VoicePath = new char[nBytes + 1];
	memset(VoicePath, 0, nLength + 1);
	memset(RCode, 0, 100);
	WideCharToMultiByte(CP_OEMCP, 0, code, nLength, VoicePath, nBytes, NULL, NULL);
	VoicePath[nBytes] = 0;
	strcpy(RCode, VoicePath);
}
char* CRegVerify::GetMd5()  {
	strcat(RCode, lpszCpu);
	MD5 md5;
	md5.GenerateMD5((unsigned char *)RCode, strlen(RCode));
	char md[33];
	md5.ToString(md);
	return md;
}
BOOL CRegVerify::IsRegLocal(char * md) {
	CRegVerify rv;
	rv.GetCpuByCmd();
	char md_[33];
	for (int i = 0; i < CodeNum; i++)
	{
		rv.SetRCode(RegCode[i]);
		strcpy(md_, rv.GetMd5());
		if (strcmp(md, md_) == 0)
			return true;
	}
	return false;
}
BOOL CRegVerify::IsRegVaild(CString code) 
{
	for (int i = 0; i < CodeNum; i++)
	{
		if (code == RegCode[i])
			return true;
	}
	return false;
}
/*
* RFC 1321 compliant MD5 implementation,
* by Christophe Devine <devine@cr0.net>;
* this program is licensed under the GPL.
*/

#define GET_UINT32(n,b,i)                                      \
{                                                                                      \
    (n) = (uint32) ((uint8 *) b)[(i)]                                 \
      | (((uint32) ((uint8 *) b)[(i)+1]) <<  8)                  \
      | (((uint32) ((uint8 *) b)[(i)+2]) << 16)                \
      | (((uint32) ((uint8 *) b)[(i)+3]) << 24);               \
}

#define PUT_UINT32(n,b,i)                                              \
{                                                                                              \
    (((uint8 *) b)[(i)]  ) = (uint8) (((n)      ) & 0xFF);             \
    (((uint8 *) b)[(i)+1]) = (uint8) (((n) >>  8) & 0xFF);      \
    (((uint8 *) b)[(i)+2]) = (uint8) (((n) >> 16) & 0xFF);     \
    (((uint8 *) b)[(i)+3]) = (uint8) (((n) >> 24) & 0xFF);     \
}

void MD5::md5_starts(struct md5_context *ctx)
{
	ctx->total[0] = 0;
	ctx->total[1] = 0;
	ctx->state[0] = 0x67452301;
	ctx->state[1] = 0xEFCDAB89;
	ctx->state[2] = 0x98BADCFE;
	ctx->state[3] = 0x10325476;
}

void MD5::md5_process(struct md5_context *ctx, uint8 data[64])
{
	uint32 A, B, C, D, X[16];

	GET_UINT32(X[0], data, 0);
	GET_UINT32(X[1], data, 4);
	GET_UINT32(X[2], data, 8);
	GET_UINT32(X[3], data, 12);
	GET_UINT32(X[4], data, 16);
	GET_UINT32(X[5], data, 20);
	GET_UINT32(X[6], data, 24);
	GET_UINT32(X[7], data, 28);
	GET_UINT32(X[8], data, 32);
	GET_UINT32(X[9], data, 36);
	GET_UINT32(X[10], data, 40);
	GET_UINT32(X[11], data, 44);
	GET_UINT32(X[12], data, 48);
	GET_UINT32(X[13], data, 52);
	GET_UINT32(X[14], data, 56);
	GET_UINT32(X[15], data, 60);

#define S(x,n) ((x << n) | ((x & 0xFFFFFFFF) >> (32 - n)))

#define P(a,b,c,d,k,s,t)                                    \
{                                                                           \
    a += F(b,c,d) + X[k] + t; a = S(a,s) + b;      \
}

	A = ctx->state[0];
	B = ctx->state[1];
	C = ctx->state[2];
	D = ctx->state[3];

#define F(x,y,z) (z ^ (x & (y ^ z)))

	P(A, B, C, D, 0, 7, 0xD76AA478);
	P(D, A, B, C, 1, 12, 0xE8C7B756);
	P(C, D, A, B, 2, 17, 0x242070DB);
	P(B, C, D, A, 3, 22, 0xC1BDCEEE);
	P(A, B, C, D, 4, 7, 0xF57C0FAF);
	P(D, A, B, C, 5, 12, 0x4787C62A);
	P(C, D, A, B, 6, 17, 0xA8304613);
	P(B, C, D, A, 7, 22, 0xFD469501);
	P(A, B, C, D, 8, 7, 0x698098D8);
	P(D, A, B, C, 9, 12, 0x8B44F7AF);
	P(C, D, A, B, 10, 17, 0xFFFF5BB1);
	P(B, C, D, A, 11, 22, 0x895CD7BE);
	P(A, B, C, D, 12, 7, 0x6B901122);
	P(D, A, B, C, 13, 12, 0xFD987193);
	P(C, D, A, B, 14, 17, 0xA679438E);
	P(B, C, D, A, 15, 22, 0x49B40821);

#undef F

#define F(x,y,z) (y ^ (z & (x ^ y)))

	P(A, B, C, D, 1, 5, 0xF61E2562);
	P(D, A, B, C, 6, 9, 0xC040B340);
	P(C, D, A, B, 11, 14, 0x265E5A51);
	P(B, C, D, A, 0, 20, 0xE9B6C7AA);
	P(A, B, C, D, 5, 5, 0xD62F105D);
	P(D, A, B, C, 10, 9, 0x02441453);
	P(C, D, A, B, 15, 14, 0xD8A1E681);
	P(B, C, D, A, 4, 20, 0xE7D3FBC8);
	P(A, B, C, D, 9, 5, 0x21E1CDE6);
	P(D, A, B, C, 14, 9, 0xC33707D6);
	P(C, D, A, B, 3, 14, 0xF4D50D87);
	P(B, C, D, A, 8, 20, 0x455A14ED);
	P(A, B, C, D, 13, 5, 0xA9E3E905);
	P(D, A, B, C, 2, 9, 0xFCEFA3F8);
	P(C, D, A, B, 7, 14, 0x676F02D9);
	P(B, C, D, A, 12, 20, 0x8D2A4C8A);

#undef F

#define F(x,y,z) (x ^ y ^ z)

	P(A, B, C, D, 5, 4, 0xFFFA3942);
	P(D, A, B, C, 8, 11, 0x8771F681);
	P(C, D, A, B, 11, 16, 0x6D9D6122);
	P(B, C, D, A, 14, 23, 0xFDE5380C);
	P(A, B, C, D, 1, 4, 0xA4BEEA44);
	P(D, A, B, C, 4, 11, 0x4BDECFA9);
	P(C, D, A, B, 7, 16, 0xF6BB4B60);
	P(B, C, D, A, 10, 23, 0xBEBFBC70);
	P(A, B, C, D, 13, 4, 0x289B7EC6);
	P(D, A, B, C, 0, 11, 0xEAA127FA);
	P(C, D, A, B, 3, 16, 0xD4EF3085);
	P(B, C, D, A, 6, 23, 0x04881D05);
	P(A, B, C, D, 9, 4, 0xD9D4D039);
	P(D, A, B, C, 12, 11, 0xE6DB99E5);
	P(C, D, A, B, 15, 16, 0x1FA27CF8);
	P(B, C, D, A, 2, 23, 0xC4AC5665);

#undef F

#define F(x,y,z) (y ^ (x | ~z))

	P(A, B, C, D, 0, 6, 0xF4292244);
	P(D, A, B, C, 7, 10, 0x432AFF97);
	P(C, D, A, B, 14, 15, 0xAB9423A7);
	P(B, C, D, A, 5, 21, 0xFC93A039);
	P(A, B, C, D, 12, 6, 0x655B59C3);
	P(D, A, B, C, 3, 10, 0x8F0CCC92);
	P(C, D, A, B, 10, 15, 0xFFEFF47D);
	P(B, C, D, A, 1, 21, 0x85845DD1);
	P(A, B, C, D, 8, 6, 0x6FA87E4F);
	P(D, A, B, C, 15, 10, 0xFE2CE6E0);
	P(C, D, A, B, 6, 15, 0xA3014314);
	P(B, C, D, A, 13, 21, 0x4E0811A1);
	P(A, B, C, D, 4, 6, 0xF7537E82);
	P(D, A, B, C, 11, 10, 0xBD3AF235);
	P(C, D, A, B, 2, 15, 0x2AD7D2BB);
	P(B, C, D, A, 9, 21, 0xEB86D391);

#undef F

	ctx->state[0] += A;
	ctx->state[1] += B;
	ctx->state[2] += C;
	ctx->state[3] += D;
}

void MD5::md5_update(struct md5_context *ctx, uint8 *input, uint32 length)
{
	uint32 left, fill;

	if (!length) return;

	left = (ctx->total[0] >> 3) & 0x3F;
	fill = 64 - left;

	ctx->total[0] += length << 3;
	ctx->total[1] += length >> 29;

	ctx->total[0] &= 0xFFFFFFFF;
	ctx->total[1] += ctx->total[0] < length << 3;

	if (left && length >= fill)
	{
		memcpy((void *)(ctx->buffer + left), (void *)input, fill);
		md5_process(ctx, ctx->buffer);
		length -= fill;
		input += fill;
		left = 0;
	}

	while (length >= 64)
	{
		md5_process(ctx, input);
		length -= 64;
		input += 64;
	}

	if (length)
	{
		memcpy((void *)(ctx->buffer + left), (void *)input, length);
	}
}

static uint8 md5_padding[64] =
{
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void MD5::md5_finish(struct md5_context *ctx, uint8 digest[16])
{
	uint32 last, padn;
	uint8 msglen[8];

	PUT_UINT32(ctx->total[0], msglen, 0);
	PUT_UINT32(ctx->total[1], msglen, 4);

	last = (ctx->total[0] >> 3) & 0x3F;
	padn = (last < 56) ? (56 - last) : (120 - last);

	md5_update(ctx, md5_padding, padn);
	md5_update(ctx, msglen, 8);

	PUT_UINT32(ctx->state[0], digest, 0);
	PUT_UINT32(ctx->state[1], digest, 4);
	PUT_UINT32(ctx->state[2], digest, 8);
	PUT_UINT32(ctx->state[3], digest, 12);
}

void MD5::GenerateMD5(unsigned char* buffer, int bufferlen)
{
	struct md5_context context;
	md5_starts(&context);
	md5_update(&context, buffer, bufferlen);
	md5_finish(&context, (unsigned char*)m_data);
}

MD5::MD5()
{
	for (int i = 0; i<4; i++)
		m_data[i] = 0;
}

MD5::MD5(unsigned long* md5src)
{
	memcpy(m_data, md5src, 16);
}
int _httoi(const char *value)
{
	struct CHexMap
	{
		char chr;
		int value;
	};
	const int HexMapL = 16;
	CHexMap HexMap[HexMapL] =
	{
		{ '0', 0 },{ '1', 1 },
		{ '2', 2 },{ '3', 3 },
		{ '4', 4 },{ '5', 5 },
		{ '6', 6 },{ '7', 7 },
		{ '8', 8 },{ '9', 9 },
		{ 'a', 10 },{ 'b', 11 },
		{ 'c', 12 },{ 'd', 13 },
		{ 'e', 14 },{ 'f', 15 }
	};
	//pthread_mutex_lock(&mutexMemory);
	char *mstr = strdup(value);
	//pthread_mutex_unlock(&mutexMemory);
	char *s = mstr;
	int result = 0;
	if (*s == '0' && *(s + 1) == 'X') s += 2;
	bool firsttime = true;
	while (*s != '/0')
	{
		bool found = false;
		for (int i = 0; i < HexMapL; i++)
		{
			if (*s == HexMap[i].chr)
			{
				if (!firsttime) result <<= 4;
				result |= HexMap[i].value;
				found = true;
				break;
			}
		}
		if (!found) break;
		s++;
		firsttime = false;
	}
	//pthread_mutex_lock(&mutexMemory);
	free(mstr);
	//pthread_mutex_unlock(&mutexMemory);
	return result;
}


MD5::MD5(const char* md5src)
{
	if (strcmp(md5src, "") == 0)
	{
		for (int i = 0; i<4; i++)
			m_data[i] = 0;
		return;
	}
	for (int j = 0; j < 16; j++)
	{
		char buf[10];
		strncpy(buf, md5src, 2);
		md5src += 2;
		((unsigned char*)m_data)[j] = _httoi(buf);
	}
}


void MD5::ToString(char md[])
{
	for (int j = 0; j < 16; j++)
	{
		sprintf(md + j * 2, "%02x", ((unsigned char*)m_data)[j]);
	}
}