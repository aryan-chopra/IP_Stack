#include <arpa/inet.h>
#include <inttypes.h>
#include <stdio.h>

#include "arp.h"
#include "ethernet.h"
#include "icmp.h"
#include "ip.h"
#include "netdev.h"

void ipIncoming(Netdev *netdev, EthernetHeader *ethHeader) {
  IpHeader *ipHeader = (IpHeader *) ethHeader->payload;
  uint16_t checksumValue = -1;

  if (ipHeader->version != IPV4) {
    printf("Version not IPV4 while intercepting got = &0x02\n", ipHeader->version);
    return;
  }

  printf("Version and length: %"PRIu8"\n", ipHeader->version);

  if (ipHeader->headerLength < 5) {
    printf("Header length must be 5\n");
    return;
  }

  if (ipHeader->ttl == 0) {
    //todo send ICMP error
    printf("Packet ttl = 0\n");
    return;
  }

  checksumValue = checksum(ipHeader, ipHeader->headerLength * 4);

  if (checksumValue != 0) {
    printf("Checksum failed to verify in incoming before icmp\n");
    return;
  }

  ipHeader->totalLength = ntohs(ipHeader->totalLength);

  switch (ipHeader->protocol) {
    case ICMP:
      handleIcmp(ipHeader);
      ipReply(netdev, ethHeader);
      break;
    default:
      printf("Got protocol: %"PRIu8"\n", ipHeader->protocol);
      //diff between uit8 and uint8_t?
      return;
  }
}

void ipReply(Netdev *netdev, EthernetHeader *ethHeader){
  IpHeader *ipHeader = (IpHeader *) ethHeader->payload;
  uint8_t length = ipHeader->totalLength;

  printf("IP was meant for %"PRIu32" from %"PRIu32"\n", ipHeader->destinationAddress, ipHeader->sourceAddress);

  ipHeader->destinationAddress = ipHeader->sourceAddress;
  ipHeader->sourceAddress = netdev->address;

  printf("IP wil go to %"PRIu32" from %"PRIu32"\n", ipHeader->destinationAddress, ipHeader->sourceAddress);

  ipHeader->totalLength = htons(ipHeader->totalLength);

  ipHeader->checksum = 0;
  ipHeader->checksum = checksum(ipHeader, ipHeader->headerLength * 4);

  transmitNetdev(netdev, ethHeader, ETH_P_IP, length, ethHeader->sourceMac); 
}

uint16_t checksum(void *address, int count) {
  int sum = 0;
  uint16_t *ptr = address;

  while (count > 1) {
    sum += * ptr++;
    count -= 2;
  }
  
  if (count > 0) {
    sum += * (uint8_t *) address;
  }

  while (sum >> 16) {
    sum = (sum & 0xffff) + (sum >> 16);
  }

  return ~sum;
}

