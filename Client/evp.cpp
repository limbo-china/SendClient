#include "stdafx.h"
#include "evp.h"

uint32_t aes_cfb_encrypt(uint8_t* source_buffer, uint8_t* des_buffer,
	uint32_t len_in, uint8_t* key)
{
	int32_t outl;
	int32_t outl2;
	int32_t rv;
	uint8_t iv[AES_BLOCK_SIZE];

	EVP_CIPHER_CTX ctx;

	EVP_CIPHER_CTX_init(&ctx);
	memset(iv, 0, AES_BLOCK_SIZE);

	rv = EVP_EncryptInit_ex(&ctx, EVP_aes_128_cfb(), NULL, key, iv);
	if (rv != 1)
	{
		printf("Err\n");
		return 1;
	}

	rv = EVP_EncryptUpdate(&ctx, des_buffer, &outl, source_buffer, len_in);
	if (rv != 1)
	{
		printf("Err\n");
		return 2;
	}

	rv = EVP_EncryptFinal_ex(&ctx, des_buffer + outl, &outl2);
	if (rv != 1)
	{
		EVP_CIPHER_CTX_cleanup(&ctx);
		return 3;
	}

	EVP_CIPHER_CTX_cleanup(&ctx);
	return 0;
}

uint32_t aes_cfb_decrypt(uint8_t* source_buffer, uint8_t* des_buffer,
	uint32_t len_in, uint8_t* key)
{
	int32_t outl;
	int32_t outl2;
	int32_t rv;
	uint8_t iv[AES_BLOCK_SIZE];

	EVP_CIPHER_CTX ctx;

	EVP_CIPHER_CTX_init(&ctx);
	memset(iv, 0, AES_BLOCK_SIZE);

	rv = EVP_DecryptInit_ex(&ctx, EVP_aes_128_cfb(), NULL, key, iv);
	if (rv != 1)
	{
		printf("Err\n");
		return 1;
	}

	rv = EVP_DecryptUpdate(&ctx, des_buffer, &outl, source_buffer, len_in);
	if (rv != 1)
	{
		printf("Err\n");
		return 2;
	}

	rv = EVP_DecryptFinal_ex(&ctx, des_buffer + outl, &outl2);
	if (rv != 1)
	{
		EVP_CIPHER_CTX_cleanup(&ctx);
		return 3;
	}

	EVP_CIPHER_CTX_cleanup(&ctx);
	return 0;
}

uint32_t aes_cbc_encrypt(uint8_t* source_buffer, uint8_t* des_buffer,
		uint32_t len_in, uint32_t* len_out, uint8_t* key)
{
	int32_t outl;
	int32_t outl2;
	int32_t rv;
	uint8_t iv[AES_BLOCK_SIZE];

	EVP_CIPHER_CTX ctx;

	EVP_CIPHER_CTX_init(&ctx);
	memset(iv, 0, AES_BLOCK_SIZE);

	rv = EVP_EncryptInit_ex(&ctx, EVP_aes_128_cbc(), NULL, key, iv);
	if (rv != 1)
	{
		printf("Err\n");
		return 1;
	}

	rv = EVP_EncryptUpdate(&ctx, des_buffer, &outl, source_buffer, len_in);
	if (rv != 1)
	{
		printf("Err\n");
		return 2;
	}

	rv = EVP_EncryptFinal_ex(&ctx, des_buffer + outl, &outl2);
	if (rv != 1)
	{
		EVP_CIPHER_CTX_cleanup(&ctx);
		return 3;
	}

	*len_out = outl + outl2;
	EVP_CIPHER_CTX_cleanup(&ctx);
	return 0;

}

uint32_t aes_cbc_decrypt(uint8_t* source_buffer, uint8_t* des_buffer,
		uint32_t len_in, uint32_t* len_out, uint8_t* key)
{
	int32_t outl;
	int32_t outl2;
	int32_t rv;
	uint8_t iv[AES_BLOCK_SIZE];

	EVP_CIPHER_CTX ctx;

	EVP_CIPHER_CTX_init(&ctx);
	memset(iv, 0, AES_BLOCK_SIZE);

	rv = EVP_DecryptInit_ex(&ctx, EVP_aes_128_cbc(), NULL, key, iv);
	if (rv != 1)
	{
		printf("Err\n");
		return 1;
	}

	rv = EVP_DecryptUpdate(&ctx, des_buffer, &outl, source_buffer, len_in);
	if (rv != 1)
	{
		printf("Err\n");
		return 2;
	}

	rv = EVP_DecryptFinal_ex(&ctx, des_buffer + outl, &outl2);
	if (rv != 1)
	{
		EVP_CIPHER_CTX_cleanup(&ctx);
		return 3;
	}

	*len_out = outl + outl2;
	EVP_CIPHER_CTX_cleanup(&ctx);
	return 0;
}

uint32_t blowfish_cbc_encrypt(uint8_t* source_buffer, uint8_t* des_buffer,
		uint32_t len_in, uint32_t* len_out, uint8_t* key)
{
	int32_t outl;
	int32_t outl2;
	int32_t rv;
	uint8_t iv[BF_BLOCK_SIZE];

	EVP_CIPHER_CTX ctx;

	EVP_CIPHER_CTX_init(&ctx);
	memset(iv, 0, BF_BLOCK_SIZE);

	rv = EVP_EncryptInit_ex(&ctx, EVP_bf_cbc(), NULL, key, iv);
	if (rv != 1)
	{
		printf("Err\n");
		return 1;
	}

	rv = EVP_EncryptUpdate(&ctx, des_buffer, &outl, source_buffer, len_in);
	if (rv != 1)
	{
		printf("Err\n");
		return 2;
	}

	rv = EVP_EncryptFinal_ex(&ctx, des_buffer + outl, &outl2);
	if (rv != 1)
	{
		EVP_CIPHER_CTX_cleanup(&ctx);
		return 3;
	}

	*len_out = outl + outl2;
	EVP_CIPHER_CTX_cleanup(&ctx);
	return 0;
}

uint32_t blowfish_cbc_decrypt(uint8_t* source_buffer, uint8_t* des_buffer,
		uint32_t len_in, uint32_t* len_out, uint8_t* key)
{
	int32_t outl;
	int32_t outl2;
	int32_t rv;
	uint8_t iv[BF_BLOCK_SIZE];

	EVP_CIPHER_CTX ctx;

	EVP_CIPHER_CTX_init(&ctx);
	memset(iv, 0, BF_BLOCK_SIZE);

	rv = EVP_DecryptInit_ex(&ctx, EVP_bf_cbc(), NULL, key, iv);
	if (rv != 1)
	{
		printf("Err\n");
		return 1;
	}

	rv = EVP_DecryptUpdate(&ctx, des_buffer, &outl, source_buffer, len_in);
	if (rv != 1)
	{
		printf("Err\n");
		return 2;
	}

	rv = EVP_DecryptFinal_ex(&ctx, des_buffer + outl, &outl2);
	if (rv != 1)
	{
		EVP_CIPHER_CTX_cleanup(&ctx);
		return 3;
	}

	*len_out = outl + outl2;
	EVP_CIPHER_CTX_cleanup(&ctx);
	return 0;
}

uint32_t des_cbc_encrypt(uint8_t* source_buffer, uint8_t* des_buffer,
		uint32_t len_in, uint32_t* len_out, uint8_t* key)
{
	int32_t outl;
	int32_t outl2;
	int32_t rv;
	uint8_t iv[DES_BLOCK_SIZE];

	EVP_CIPHER_CTX ctx;

	EVP_CIPHER_CTX_init(&ctx);
	memset(iv, 0, DES_BLOCK_SIZE);

	rv = EVP_EncryptInit_ex(&ctx, EVP_des_cbc(), NULL, key, iv);
	if (rv != 1)
	{
		printf("Err\n");
		return 1;
	}

	rv = EVP_EncryptUpdate(&ctx, des_buffer, &outl, source_buffer, len_in);
	if (rv != 1)
	{
		printf("Err\n");
		return 2;
	}

	rv = EVP_EncryptFinal_ex(&ctx, des_buffer + outl, &outl2);
	if (rv != 1)
	{
		EVP_CIPHER_CTX_cleanup(&ctx);
		return 3;
	}

	*len_out = outl + outl2;
	EVP_CIPHER_CTX_cleanup(&ctx);
	return 0;
}

uint32_t des_cbc_decrypt(uint8_t* source_buffer, uint8_t* des_buffer,
		uint32_t len_in, uint32_t* len_out, uint8_t* key)
{
	int32_t outl;
	int32_t outl2;
	int32_t rv;
	uint8_t iv[DES_BLOCK_SIZE];

	EVP_CIPHER_CTX ctx;

	EVP_CIPHER_CTX_init(&ctx);
	memset(iv, 0, DES_BLOCK_SIZE);

	rv = EVP_DecryptInit_ex(&ctx, EVP_des_cbc(), NULL, key, iv);
	if (rv != 1)
	{
		printf("Err\n");
		return 1;
	}

	rv = EVP_DecryptUpdate(&ctx, des_buffer, &outl, source_buffer, len_in);
	if (rv != 1)
	{
		printf("Err\n");
		return 2;
	}

	rv = EVP_DecryptFinal_ex(&ctx, des_buffer + outl, &outl2);
	if (rv != 1)
	{
		EVP_CIPHER_CTX_cleanup(&ctx);
		return 3;
	}

	*len_out = outl + outl2;
	EVP_CIPHER_CTX_cleanup(&ctx);
	return 0;
}

uint32_t des_3_encrypt(uint8_t* source_buffer, uint8_t* des_buffer,
		uint32_t len_in, uint32_t* len_out, uint8_t* key)
{
	int32_t outl;
	int32_t outl2;
	int32_t rv;
	uint8_t iv[DES_BLOCK_SIZE];

	EVP_CIPHER_CTX ctx;

	EVP_CIPHER_CTX_init(&ctx);
	memset(iv, 0, DES_BLOCK_SIZE);

	rv = EVP_EncryptInit_ex(&ctx, EVP_des_ede3_cbc(), NULL, key, iv);
	if (rv != 1)
	{
		printf("Err\n");
		return 1;
	}

	rv = EVP_EncryptUpdate(&ctx, des_buffer, &outl, source_buffer, len_in);
	if (rv != 1)
	{
		printf("Err\n");
		return 2;
	}

	rv = EVP_EncryptFinal_ex(&ctx, des_buffer + outl, &outl2);
	if (rv != 1)
	{
		EVP_CIPHER_CTX_cleanup(&ctx);
		return 3;
	}

	*len_out = outl + outl2;
	EVP_CIPHER_CTX_cleanup(&ctx);
	return 0;
}

uint32_t des_3_decrypt(uint8_t* source_buffer, uint8_t* des_buffer,
		uint32_t len_in, uint32_t* len_out, uint8_t* key)
{
	int32_t outl;
	int32_t outl2;
	int32_t rv;
	uint8_t iv[DES_BLOCK_SIZE];

	EVP_CIPHER_CTX ctx;

	EVP_CIPHER_CTX_init(&ctx);
	memset(iv, 0, DES_BLOCK_SIZE);

	rv = EVP_DecryptInit_ex(&ctx, EVP_des_ede3_cbc(), NULL, key, iv);
	if (rv != 1)
	{
		printf("Err\n");
		return 1;
	}

	rv = EVP_DecryptUpdate(&ctx, des_buffer, &outl, source_buffer, len_in);
	if (rv != 1)
	{
		printf("Err\n");
		return 2;
	}

	rv = EVP_DecryptFinal_ex(&ctx, des_buffer + outl, &outl2);
	if (rv != 1)
	{
		EVP_CIPHER_CTX_cleanup(&ctx);
		return 3;
	}

	*len_out = outl + outl2;
	EVP_CIPHER_CTX_cleanup(&ctx);
	return 0;
}
