#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "netdev.h"
#include "ethernet.h"
#include "log.h"
#include "tap.h"

void initNetdev(Netdev *netdev, char *ipAddress, char *macAddress) {
	memset(netdev, 0, sizeof(netdev));
	printf("memset\n");

	if (inet_pton(AF_INET, ipAddress, &netdev->address) != 1) {
		printf("Parsing failed\n");
		exit(1);	
	}

	printf("Parsed\n");

	sscanf(macAddress, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &netdev->macOctets[0],  &netdev->macOctets[1], &netdev->macOctets[2], &netdev->macOctets[3], &netdev->macOctets[4], &netdev->macOctets[5]);
	printf("alloc'd\n");
}

void transmitNetdev(Netdev *netdev, EthernetHeader *ethHeader, uint16_t ethertype, int length, unsigned char *destination) {
	ethHeader->payloadType= htons(ethertype);

	memcpy(ethHeader->destinationMac, destination, 6);
	memcpy(ethHeader->sourceMac, netdev->macOctets, 6);

	length += sizeof(EthernetHeader);

  log(ethHeader, 0);

	int status = writeTun((char *) ethHeader, length);
}

