#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "arp.h"
#include "arp_log.h"

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

void logArpMac(unsigned char mac[6]) {
	unsigned char logText[4];

	  for (int index = 0; index < 5; index++) {
	  snprintf(logText, 4, "%02x:", mac[index]);
	  writeArpLog(logText); 
	  }

	  snprintf(logText, 3, "%02x", mac[5]);
	  writeArpLog(logText);

	  snprintf(logText, 3, "\n");
	  writeArpLog(logText);
}

void logArpHeader(arp_ipv4 *arpData, int incoming) {
	char *logText = (char *)calloc(50, 1);

  if (incoming) {
    snprintf(logText, 50, "Incoming ARP Header:\n");
  }

  else {
    smprintf(logText, 50, "Outgoing ARP Header:\n");
  }

  writeArpLog(logText);

	snprintf(logText, 50, "Source IP         : %"PRIu32"\n", arpData->sourceIp); 
	writeArpLog(logText);

	snprintf(logText, 50, "Destination IP    : %"PRIu32"\n", arpData->destinationIp);
	writeArpLog(logText);

	snprintf(logText, 50, "Source MAC        : ");
	writeArpLog(logText);
	logArpMac(arpData->sourceMac);

	snprintf(logText, 50, "Destination MAC   : ");
	writeArpLog(logText);
	logArpMac(arpData->destinationMac);

	sprintf(logText, "\n-------------------------------------------------------------------------\n\n");
	writeArpLog(logText);
}

