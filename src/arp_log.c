#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "arp.h"
#include "arp_log.h"

#define SIZE 100

int arpLogFile;

void openArpLog() {
  arpLogFile = open(LOG_LOCATION"arp_log.txt", O_WRONLY | O_APPEND);
  if (arpLogFile < 0) {
    printf("Error opening arp log file: %s\n", strerror(errno));
    exit(1);
  }
}

void writeArpLog(char *text) {
	write(arpLogFile, text, strlen(text)); 
}

static void logIpAddress(uint32_t address) {
  char *text = calloc(SIZE, 1);

  snprintf(text, SIZE, "%s\n", inet_ntoa(*((struct in_addr *)&address)));
        
  writeArpLog(text);

  free(text);
}


void logArpMac(unsigned char mac[6]) {
	unsigned char text[4];

	  for (int index = 0; index < 5; index++) {
	  snprintf(text, 4, "%02x:", mac[index]);
	  writeArpLog(text); 
	  }

	  snprintf(text, 3, "%02x", mac[5]);
	  writeArpLog(text);

	  snprintf(text, 3, "\n");
	  writeArpLog(text);
}

void logArpHeader(arp_ipv4 *arpData, int incoming) {
	char *text = (char *)calloc(SIZE, 1);

  if (incoming) {
    snprintf(text, SIZE, "Incoming ARP Header:\n\n");
  }

  else {
    snprintf(text, SIZE, "Outgoing ARP Header:\n\n");
  }

  writeArpLog(text);

	snprintf(text, SIZE, "Source IP         : "); 
	writeArpLog(text);
  logIpAddress(arpData->sourceIp);

	snprintf(text, SIZE, "Destination IP    : ");
	writeArpLog(text);
  logIpAddress(arpData->destinationIp);

	snprintf(text, SIZE, "Source MAC        : ");
	writeArpLog(text);
	logArpMac(arpData->sourceMac);

	snprintf(text, SIZE, "Destination MAC   : ");
	writeArpLog(text);
	logArpMac(arpData->destinationMac);

	sprintf(text, "\n-------------------------------------------------------------------------\n\n");
	writeArpLog(text);

  free(text);
}

