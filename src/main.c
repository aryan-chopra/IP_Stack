#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "arp.h"
#include "ethernet.h"
#include "icmp.h"
#include "ip.h"
#include "log.h"
#include "netdev.h"
#include "tap.h"

void handleFrame(Netdev *netdev, EthernetHeader *header) {

  switch(header->payloadType) {
    case ETH_P_ARP:
      log(header, L_ETHERNET | L_INCOMING);
      printf("Got Arp\n");
      incomingRequest(netdev, header);
      break;
    case ETH_P_IP:
      printf("Got ICMP\n");
      ipIncoming(netdev, header);
      break;
    default:
      return;
  }
}

int main() {
  openLogFiles();

  Netdev netdev;
  int tapDevice = initTap();

  initNetdev(&netdev, "10.0.0.4", "00:0c:29:6d:50:25");
  initArp();

  int size = 2500;
  char *buffer = malloc(size);

  while (1) {
    if (read(tapDevice, buffer, size) < 0) {
      printf("Error reading\n");
    }

    EthernetHeader *header = initializeEthernet(buffer);	

    handleFrame(&netdev, header);
  }
}

