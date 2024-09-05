#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#include "arp.h"
#include "arp_log.h"

int arpLogFile;

void openArpLog() {
  arpLogFile = open("./arp_log.txt", O_WRONLY | O_APPEND);
}

void writeArpLog(char *text) {
	write(logFile, text, strlen(text)); 
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

void logArpData(arp_ipv4 *arpData) {
	char *logText = (char *)malloc(1000);

	snprintf(logText, 500, "Incoming ARP request from IP  : %"PRIu32"\n", arpData->sourceIp); 
	writeArpLog(logText);

	snprintf(logText, 500, "Incoming ARP request to IP    : %"PRIu32"\n", arpData->destinationIp);
	writeArpLog(logText);

	snprintf(logText, 500, "Incoming ARP request from MAC : ");
	writeArpLog(logText);
	logArpMac(arpData->sourceMac);

	snprintf(logText, 500, "Incoming ARP request to MAC   : ");
	writeArpLog(logText);
	logArpMac(arpData->destinationMac);

	sprintf(logText, "-------------------------------------------------------------------------\n");
	writeArpLog(logText);
}

