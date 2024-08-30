#ifndef NETDEV_H
#define NETDEV_H

#include "ethernet.h"

typedef struct{
	uint32_t address;
	unsigned char macOctets[6];
}Netdev;

void initNetdev(Netdev *device, char *address, char *hardwareAddress);
void transmit(Netdev *device, EthernetHeader *ethHeader, uint16_t ethernetType, int length, unsigned char *destination);

#endif

