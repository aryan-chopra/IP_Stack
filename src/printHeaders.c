#include <errno.h>
#include <inttypes.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ethernet.h"
#include "printHeaders.h"

int logFileHeader; 
char *text;

void openFile() {
  logFileHeader = open("./packetLog.txt", O_WRONLY | O_APPEND);
  if (logFileHeader < 0) {
    printf("Error opening log: %s\n", strerror(errno));
    exit(1);
  }
  text = calloc(500, 1);
}

void logToFileHeader(char *text) {
  write(logFileHeader, text, strlen(text));
}

void logMacHeader(unsigned char mac[6]) {
  unsigned char logText[4];

  for (int index = 0; index < 5; index++) {
    snprintf(logText, 4, "%02x:", mac[index]);
    logToFileHeader(logText); 
  }
  snprintf(logText, 3, "%02x", mac[5]);
  logToFileHeader(logText);
  snprintf(logText, 3, "\n");
  logToFileHeader(logText);
}


void logEthernetPacket(EthernetHeader *ethHeader, int incoming) {
  if (incoming) {
    snprintf(text, 200, "Ethernet header incoming:\n");
  }

  else {
    snprintf(text, 200, "Ethernet header outgoing:\n");
  }

  logToFileHeader(text);

  snprintf(text, 200, "Destination Mac: ");
  logToFileHeader(text);
  logMacHeader(ethHeader->destinationMac);

  snprintf(text, 200, "Source Mac     : ");
  logToFileHeader(text);
  logMacHeader(ethHeader->sourceMac);

  snprintf(text, 200, "Payload type   : %"PRIu16"\n", ethHeader->payloadType);
  logToFileHeader(text);

  snprintf(text, 200, "\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\n");
  logToFileHeader(text);
}

void logIpHeader(IpHeader *hdr, int incoming) {
  if (incoming) {
    snprintf(text, 200, "Incoming IP Header:\n");   
  }

  else {
    snprintf(text, 200, "Outgoing IP Header:\n");
  }

  logToFileHeader(text);

  snprintf(text, 200, "Header Length: %"PRIu8"\n", hdr->headerLength);
  logToFileHeader(text);

  snprintf(text, 200, "Version        : %"PRIu8"\n", hdr->version);
  logToFileHeader(text);

  snprintf(text, 200, "tos            : %"PRIu8"\n", hdr->tos);
  logToFileHeader(text);

  snprintf(text, 200, "totalLength    : %"PRIu16"\n", hdr->totalLength);
  logToFileHeader(text);

  snprintf(text, 200, "ID             : %"PRIu16"\n", hdr->id);
  logToFileHeader(text);

  snprintf(text, 200, "flags          : %"PRIu16"\n", hdr->flags);
  logToFileHeader(text);

  snprintf(text, 200, "fragmentOffset : %"PRIu16"\n", hdr->fragmentOffset);
  logToFileHeader(text);

  snprintf(text, 200, "ttl            : %"PRIu8"\n", hdr->ttl);
  logToFileHeader(text);

  snprintf(text, 200, "protocol       : %"PRIu8"\n", hdr->protocol);
  logToFileHeader(text);

  snprintf(text, 200, "checksum       : %"PRIu16"\n", hdr->checksum);
  logToFileHeader(text);

  snprintf(text, 200, "Sender Address : %"PRIu32"\n", hdr->sourceAddress);
  logToFileHeader(text);

  snprintf(text, 200, "destination    : %"PRIu32"\n", hdr->destinationAddress);
  logToFileHeader(text);

  snprintf(text, 200, "\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\n");
  logToFileHeader(text);
}

