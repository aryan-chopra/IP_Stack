#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "arp.h"
#include "ethernet.h"
#include "icmp.h"
#include "ip.h"
#include "netdev.h"
#include "tap.h"
#include "printHeaders.h"

void handleFrame(Netdev *netdev, EthernetHeader *header) {
	switch(header->payloadType) {
		case ETH_P_ARP:
      logEthernetPacket(header, 1);
      printf("Got Arp\n");
			incomingRequest(netdev, header);
			break;
		case ETH_P_IP:
      printf("Got ICMP\n");
      ipIncoming(netdev, header);
			break;
		default:
			//printf("Unrecognized\n");
	}
}

int main() {
	Netdev netdev;

  openFile();

	printf("Size: %d\n", sizeof(EthernetHeader));

  /* char* name = calloc(20, 1);
  name = "tap0"; */

	int tapDevice = initTap();
	printf("%d\n", tapDevice);

	printf("tap init\n");
	initNetdev(&netdev, "10.0.0.4", "00:0c:29:6d:50:25");
	printf("netdev init\n");

	initArp();
	printf("init arp\n");

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

