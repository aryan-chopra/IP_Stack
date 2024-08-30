#include <stdint.h>

typedef struct {
	uint8_t version : 4;
	uint8_t internetHeaderLength: 4;
	uint8_t tos;
	uint16_t len;
	uint16_t id;
	uint16_t flags : 3;
	uint16_t frag_offset : 13;
	uint8_t ttl;
	uint8_t proto;
	uint16_t csum;
	uint32_t senderAddress;
	uint32_t destinationAddress;
} __attribute__((packed)) IpHeader;

