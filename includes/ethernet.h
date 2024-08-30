#ifndef ETHERNET_H
#define ETHERNET_H

#include <linux/if_ether.h>
#include <stdint.h>

typedef struct{
	unsigned char destinationMac[6];
	unsigned char sourceMac[6];
	uint16_t payloadType;
	unsigned char payload[];
}__attribute__((packed)) EthernetHeader;

#endif

