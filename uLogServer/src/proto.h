#ifndef _PROTO_H_
#define _PROTO_H_

#include <stdint.h>

#define DDS_VERSION			0x01
#define DDS_MAGIC			0xaabbccdd
#define PKT_MAX 1024


#define ITEM_TIME 0x02
#define ITEM_SIP 0x03
#define ITEM_DIP 0x04
#define ITEM_SPORT 0x05
#define ITEM_DPORT 0x06
#define ITEM_PROTOCOL 0x07
#define ITEM_TCP_TAG 0x08

#define ITEM_IP 0x03
#define ITEM_DOMAIN 0x04

#define ITEM_CLIENT_IP 0x03
#define ITEM_DOMAIN_IP 0x04
#define ITEM_DOMAIN_NEW 0x05
#define ITEM_RESPONSE_ADDRESS 0x06


#define ACK_SUCCESS				0x00
#define ACK_DITTO				0x01
#define ACK_ABSENCE				0x02
#define ACK_CONFLICT			0x03
#define ACK_VERSION_ERROR		0x04
#define ACK_MAGIC_ERROR			0x05
#define ACK_CMD_ERROR			0x06
#define ACK_LENGTH_ERROR		0x07
#define ACK_USERID_ERROR		0x08
#define ACK_PAYLOAD_ERROR		0x09
#define ACK_QUERY_ERROR			0x0a
#define ACK_AUTH_ERROR			0x0b
#define ACK_FIELD_TIME_ERROR	0x20
#define ACK_FIELD_IP_ERROR		0x21
#define ACK_FIELD_MASK_ERROR	0x22
#define ACK_UNKNOWN				0x99

enum flags_req
{
	F_DOMAIN	=	1 << 0,
	F_IP		=	1 << 1,
	F_PROTO		=	1 << 2
};

enum flags_gen_domain
{
	F_GEN_DOMAIN	=	1 << 0,
	F_GEN_IP		=	1 << 1,
	F_GEN_TIME		=	1 << 2
};

#pragma pack(1)
struct cnfhdr
{
	uint8_t version;
	uint8_t cnf_type;
	uint16_t tuple_num;
	uint32_t magic;
	uint32_t pkt_len;
	uint32_t cnf_id;
	uint32_t total_num;
	uint32_t offset;
	unsigned char content[0];
};

#pragma pack()

#endif /*_PROTO_H_*/
