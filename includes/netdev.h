#ifndef NETDEV_H
#define NETDEV_H

#include "ethernet.h"

typedef struct{
  int deviceDescriptor;
	uint32_t address;
	unsigned char macOctets[6];
}Netdev;

void initNetdev(Netdev *, int,  char *, char *);
void transmitNetdev(Netdev *, EthernetHeader *, uint16_t , int , unsigned char *);

#endif

