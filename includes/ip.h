#ifndef IPV4_H
#define IPV4_H

#include "ethernet.h"
#include "netdev.h"

#define IPV4 0x04

#define ICMP 0x01

#include <stdint.h>

typedef struct {
	uint8_t headerLength : 4;
	uint8_t version : 4;
	uint8_t tos;
	uint16_t totalLength;
  //ID for ordering fragmented frames, fragments of the same frame will have same ID
	uint16_t id;
  //flags like, if fragmentation is allowed, or if further packets are incoming or not
	uint16_t flags : 3;
  //position of fragment in datagram
	uint16_t fragmentOffset: 13;
	uint8_t ttl;
  //protocol suck as TCP/UDP
	uint8_t protocol;
	uint16_t checksum;
	uint32_t sourceAddress;
	uint32_t destinationAddress;
  uint8_t data[];
} __attribute__((packed)) IpHeader;

void ipIncoming(Netdev *, EthernetHeader *);
void ipReply(Netdev *, EthernetHeader *);
uint16_t checksum(void *, int);

#endif

