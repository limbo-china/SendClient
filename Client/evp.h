#ifndef _EVP_H_
#define _EVP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <openssl/evp.h>
#define BF_BLOCK_SIZE 8
#define DES_BLOCK_SIZE 8
#define AES_BLOCK_SIZE 16

uint32_t aes_cfb_encrypt(uint8_t* source_buffer, uint8_t* des_buffer,
	uint32_t len_in, uint8_t* key);

uint32_t aes_cfb_decrypt(uint8_t* source_buffer, uint8_t* des_buffer,
	uint32_t len_in, uint8_t* key);

uint32_t aes_cbc_encrypt(uint8_t* source_buffer, uint8_t* des_buffer,
		uint32_t len_in, uint32_t* len_out, uint8_t* key);

uint32_t aes_cbc_decrypt(uint8_t* source_buffer, uint8_t* des_buffer,
		uint32_t len_in, uint32_t* len_out, uint8_t* key);

uint32_t blowfish_cbc_encrypt(uint8_t* source_buffer, uint8_t* des_buffer,
		uint32_t len_in, uint32_t* len_out, uint8_t* key);

uint32_t blowfish_cbc_decrypt(uint8_t* source_buffer, uint8_t* des_buffer,
		uint32_t len_in, uint32_t* len_out, uint8_t* key);

uint32_t des_cbc_encrypt(uint8_t* source_buffer, uint8_t* des_buffer,
		uint32_t len_in, uint32_t* len_out, uint8_t* key);

uint32_t des_cbc_decrypt(uint8_t* source_buffer, uint8_t* des_buffer,
		uint32_t len_in, uint32_t* len_out, uint8_t* key);

uint32_t des_3_encrypt(uint8_t* source_buffer, uint8_t* des_buffer,
		uint32_t len_in, uint32_t* len_out, uint8_t* key);

uint32_t des_3_decrypt(uint8_t* source_buffer, uint8_t* des_buffer,
		uint32_t len_in, uint32_t* len_out, uint8_t* key);

#endif
