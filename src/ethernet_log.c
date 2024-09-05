#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ethernet.h"
#include "ethernet_log.h"

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
  char *text = calloc(50, 1);

  if (incoming) {
    snprintf(text, 50, "Incoming Ethernet Header:\n");
  }

  else {
    snprintf(text, 50, "Outgoing Ethernet Header:\n");
  }

  writeEthLog(text);

  snprintf(text, 50, "Destination Mac          : ");
  writeEthLog(text);
  logEthMac(ethHeader->destinationMac);

  snprintf(text, 50, "Source Mac               : ");
  writeEthLog(text);
  logEthMac(ethHeader->sourceMac);

  snprintf(text, 50, "Payload type             : %"PRIu16"\n", ethHeader->payloadType);
  writeEthLog(text);

  snprintf(text, 50, "\n-----------------------------------------------------------------------------------------------------\n\n");
  writeEthLog(text);

  free(text);
}

