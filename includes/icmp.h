#ifndef ICMPV4_H
#define ICMPV4_H

#include "ip.h"
#include "netdev.h"

#define ICMP_REPLY 0x00
#define ICMP_DST_UNREACHABLE 0x03
#define ICMP_SRC_QUENCH 0x04
#define ICMP_REDIRECT 0x05
#define ICMP_ECHO 0x08
#define ICMP_ROUTER_ADV 0x09
#define ICMP_ROUTER_SOL 0x0a
#define ICMP_TIMEOUT 0x0b
#define ICMP_MALFORMED 0x0c

#include <stdint.h>

typedef struct {
  //Purpose of message
  uint8_t type;
  //Describes the response
  uint8_t code;
  uint16_t checksum;
  uint8_t data[];
}__attribute((packed)) Icmp;

typedef struct {
  //set by sender, the process for which the req is intended (eg pid)
  uint16_t id;
  //consecutive sequence numbers of echo requests to check for loss or reordering
  uint16_t requestSequence;
  //timestamp, etc
  uint8_t data[];
}__attribute__((packed)) IcmpEcho;

typedef struct {
  //dummy octet
  uint8_t unused;
  //length of OG datagram
  uint8_t length;
  //depends on ICMP field
  uint16_t var;
  //Packet that caused the destination unreachable
  uint8_t data[];
}__attribute__((packed)) IcmpDestUnreachable;

void handleIcmp(IpHeader *); 
void structureIcmpReply(Icmp *, int);

#endif

