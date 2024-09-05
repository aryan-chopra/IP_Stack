#include <stdio.h>

#include "arp_log.h"
#include "ethernet_log.h"
#include "ip_log.h"
#include "log.h"

void openLogFiles() {
  openArpLog();
  openIpLog();
  openEthernetLog();
}

void log(void *header, uint8_t flags) {
  uint8_t headerType = flags & 0x0E;
  uint8_t incoming = flags & 0x01;

  switch(headerType) {
    case L_ARP:
      logArpHeader((arp_ipv4 *)header, incoming);
      break;
    case L_ETHERNET:
      logEthernetHeader((EthernetHeader *)header, incoming);
      break;
    case L_IP:
      logIpHeader((IpHeader *)header, incoming);
      break;
    default:
      printf("Unidentified header type\n");
  }
}

