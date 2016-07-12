#pragma once

using namespace std;

extern CString RegCode[10];

class CRegVerify {
private:
	char RCode[100];
	char lpszCpu[128];
public:
	CRegVerify(CString code);
	CRegVerify();
	~CRegVerify();
	BOOL GetCpuByCmd();
	char* GetMd5() ;
	void SetRCode(CString code);
	BOOL IsRegLocal(char *);
	BOOL IsRegVaild(CString code);
};

class MD5
{
private:
#define uint8  unsigned char
#define uint32 unsigned long int

	struct md5_context
	{
		uint32 total[2];
		uint32 state[4];
		uint8 buffer[64];
	};

	void md5_starts(struct md5_context *ctx);
	void md5_process(struct md5_context *ctx, uint8 data[64]);
	void md5_update(struct md5_context *ctx, uint8 *input, uint32 length);
	void md5_finish(struct md5_context *ctx, uint8 digest[16]);

public:
	//! construct a MD5 from any buffer
	void GenerateMD5(unsigned char* buffer, int bufferlen);

	//! construct a MD5
	MD5();

	//! construct a md5src from char *
	MD5(const char * md5src);

	//! construct a MD5 from a 16 bytes md5
	MD5(unsigned long* md5src);


	//! to a string
	void ToString(char md[]);

	unsigned long m_data[4];
};