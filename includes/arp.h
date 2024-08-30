#ifndef ARP_H
#define ARP_H

#include <stdint.h>

#include "ethernet.h"
#include "ethernet.h"
#include "netdev.h"

#define ARP_ETHERNET 0x0001
#define ARP_IPV4 0x0800

#define ARP_REQUEST 0x0001
#define ARP_REPLY 0x002

#define ARP_CACHE_LEN 32
#define ARP_FREE 0
#define ARP_WAITING 1
#define ARP_RESOLVED 2

typedef struct{
	uint16_t hardwareType;
	uint16_t protype;
	unsigned char hardwareSize;
	unsigned char prosize;
	uint16_t opcode;
	unsigned char data[];
}__attribute__((packed)) ArpHeader;

typedef struct{
	unsigned char sourceMac[6];
	uint32_t sourceIp;
	unsigned char destinationMac[6];
	uint32_t destinationIp;
}__attribute__((packed)) arp_ipv4;

typedef struct{
	uint16_t hardwareType;
	uint32_t sourceIp;
	unsigned char sourceMac[6];
	unsigned int state;
} ArpCacheEntry;

void initArp();
void incomingRequest(Netdev *netdev, EthernetHeader *header);
void replyArp(Netdev *netdev, EthernetHeader *ethHeader, ArpHeader *arpHeader);

#endif
