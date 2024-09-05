#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ethernet.h"
#include "ethernet_log.h"

#define SIZE 100

int ethLogFile;

void openEthernetLog() {
  ethLogFile = open(LOG_LOCATION"ethernet_log.txt", O_WRONLY | O_APPEND);
  if (ethLogFile < 0) {
    printf("Error opening ethernet log: %s\n", strerror(errno));
    exit(1);
  }
}

void writeEthLog(char *text) {
  write(ethLogFile, text, strlen(text));
}

void logEthMac(unsigned char mac[6]) {
  unsigned char logText[4];

  for (int index = 0; index < 5; index++) {
    snprintf(logText, 4, "%02x:", mac[index]);
    writeEthLog(logText); 
  }
  snprintf(logText, 3, "%02x", mac[5]);
  writeEthLog(logText);
  snprintf(logText, 3, "\n");
  writeEthLog(logText);
}

void logEthernetHeader(EthernetHeader *ethHeader, int incoming) {
  char *text = calloc(SIZE, 1);

  if (incoming) {
    snprintf(text, SIZE, "Incoming Ethernet Header:\n\n");
  }

  else {
    snprintf(text, SIZE, "Outgoing Ethernet Header:\n\n");
  }

  writeEthLog(text);

  snprintf(text, SIZE, "Destination Mac          : ");
  writeEthLog(text);
  logEthMac(ethHeader->destinationMac);

  snprintf(text, SIZE, "Source Mac               : ");
  writeEthLog(text);
  logEthMac(ethHeader->sourceMac);

  snprintf(text, SIZE, "Payload type             : %"PRIu16"\n", ethHeader->payloadType);
  writeEthLog(text);

  snprintf(text, SIZE, "\n-------------------------------------------------\n\n");
  writeEthLog(text);

  free(text);
}

