#include <arpa/inet.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "arp.h"
#include "netdev.h"

int logFile;
ArpCacheEntry cache[ARP_CACHE_LEN];

void initArp() {
	memset(cache, 0, ARP_CACHE_LEN * sizeof(ArpCacheEntry));
	logFile = open("../log.txt", O_WRONLY | O_APPEND);	
}

int insertArpEntry(ArpHeader *header, arp_ipv4 *data) {
	int length = ARP_CACHE_LEN;

	ArpCacheEntry *entry;

	for (int index = 0; index < length; index++) {
		entry = &(cache[index]);

		if (entry->state == ARP_FREE) {
			entry->state = ARP_RESOLVED;

			entry->hardwareType = header->hardwareType;
			entry->sourceIp = data->sourceIp;
			memcpy(entry->sourceMac, data->sourceMac, sizeof(entry->sourceMac));

			return 0;
		}
	}

	return -1;
}

int updateArpTable(ArpHeader *header, arp_ipv4 *data) {
	int length = ARP_CACHE_LEN;

	ArpCacheEntry *entry;

	for (int index = 0; index < length; index++) {
		entry = &cache[index];

		if (entry->state == ARP_RESOLVED) {
			if (entry->hardwareType == header->hardwareType && entry->sourceIp == data->sourceIp) {
				memcpy(entry->sourceMac, data->sourceMac, 6);
				return 1;
			}
		}
	}
	return 0;
}

void logToFile(char *text) {
	write(logFile, text, strlen(text)); 
}

void logMac(unsigned char mac[6]) {
	unsigned char logText[4];

	  for (int index = 0; index < 5; index++) {
	  snprintf(logText, 4, "%02x:", mac[index]);
	  logToFile(logText); 
	  }
	  snprintf(logText, 3, "%02x", mac[5]);
	  logToFile(logText);
	  snprintf(logText, 3, "\n");
	  logToFile(logText);
}

void logArpData(arp_ipv4 *arpData) {
	char *logText = (char *)malloc(1000);

	snprintf(logText, 500, "Incoming ARP request from IP  : %"PRIu32"\n", arpData->sourceIp); 
	logToFile(logText);

	snprintf(logText, 500, "Incoming ARP request to IP    : %"PRIu32"\n", arpData->destinationIp);
	logToFile(logText);

	snprintf(logText, 500, "Incoming ARP request from MAC : ");
	logToFile(logText);
	logMac(arpData->sourceMac);

	snprintf(logText, 500, "Incoming ARP request to MAC   : ");
	logToFile(logText);
	logMac(arpData->destinationMac);

	sprintf(logText, "-------------------------------------------------------------------------\n");
	logToFile(logText);
}

void incomingRequest(Netdev *netdev, EthernetHeader *header) {

	ArpHeader *arpHeader;
	arp_ipv4 *arpData;
	int merge = 0;

	logArpData(arpData);

	arpHeader = (ArpHeader *) header->payload;

	arpHeader->hardwareType = ntohs(arpHeader->hardwareType);
	arpHeader->protype = ntohs(arpHeader->protype);
	arpHeader->opcode = ntohs(arpHeader->opcode);

	arpData = (arp_ipv4 *) arpHeader->data;


	if (arpHeader->hardwareType != ARP_ETHERNET) {
		printf("Only ethernet is supported\n");
		return;
	}

	if (arpHeader->protype != ARP_IPV4) {
		printf("Only IPv4 is supported\n");
		return;
	}

	merge = updateArpTable(arpHeader, arpData);

	if (netdev->address!= arpData->destinationIp) {
		printf("ARP not for our own address\n");
	}

	if (!merge && insertArpEntry(arpHeader, arpData) != 0) {
		printf("ARP Table full!\n");
	}

	switch (arpHeader-> opcode) {
		case ARP_REQUEST:
			replyArp(netdev, header, arpHeader);
			break;
		default:
			printf("Invalid Request\n");
			break;
	}
}

void replyArp(Netdev *netdev, EthernetHeader *etherHeader, ArpHeader *arpHeader) {
	arp_ipv4 *arpData;
	int length;

	arpData = (arp_ipv4 *) arpHeader->data;

	memcpy(arpData->destinationMac, arpData->sourceMac, 6);
	arpData->destinationIp = arpData->sourceIp;
	memcpy(arpData->sourceMac, netdev->macOctets, 6);
	arpData->sourceIp = netdev->address;

	arpHeader->opcode = ARP_REPLY;

	arpHeader->opcode = htons(arpHeader->opcode);
	arpHeader->hardwareType = htons(arpHeader->hardwareType);
	arpHeader->protype = htons(arpHeader->protype);

	length = sizeof(ArpHeader) + sizeof(arp_ipv4);
	transmitNetdev(netdev, etherHeader,  ETH_P_ARP, length, arpData->destinationMac);
}

