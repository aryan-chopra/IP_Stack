#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "netdev.h"
#include "ethernet.h"
//#include "tuntap.h"

void initNetdev(Netdev *netdev, char *ipAddress, char *macAddress) {
	memset(netdev, 0, sizeof(netdev));

	if (inet_pton(AF_INET, ipAddress, &netdev->address) != 1) {
		printf("Parsing failed\n");
		exit(1);	
	}

	for (int octet = 0; octet < 6; octet++) {
		sscanf(macAddress, "%hhx:", netdev->macOctets[octet]);
	}
}

void transmitNetdev(Netdev *netdev, EthernetHeader *ethHeader, uint16_t ethertype, int length, unsigned char *destination) {
ethHeader->payloadType= htons(ethertype);

memcpy(ethHeader->sourceMac, netdev->macOctets, 6);
memcpy(ethHeader->destinationMac, destination, 6);

length += sizeof(EthernetHeader);

//writeTunnel((char *)ethHeader, len);
}

