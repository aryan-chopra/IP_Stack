#include <errno.h>
#include <inttypes.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ethernet.h"
#include "printHeaders.h"

int ipLogFile; 
char *text;

void openIpLog() {
  ipLogFile = open("./ip_log.txt", O_WRONLY | O_APPEND);
  if (ipLogFile < 0) {
    printf("Error opening log: %s\n", strerror(errno));
    exit(1);
  }
  text = calloc(500, 1);
}

void writeIpLog(char *text) {
  write(ipLogFile, text, strlen(text));
}

void logIpMac(unsigned char mac[6]) {
  unsigned char logText[4];

  for (int index = 0; index < 5; index++) {
    snprintf(logText, 4, "%02x:", mac[index]);
    writeIpLog(logText); 
  }
  snprintf(logText, 3, "%02x", mac[5]);
  writeIpLog(logText);
  snprintf(logText, 3, "\n");
  writeIpLog(logText);
}


void logEthernetHeader(EthernetHeader *ethHeader, int incoming) {
  if (incoming) {
    snprintf(text, 200, "Ethernet header incoming:\n");
  }

  else {
    snprintf(text, 200, "Ethernet header outgoing:\n");
  }

  writeIpLog(text);

  snprintf(text, 200, "Destination Mac: ");
  writeIpLog(text);
  logIpMac(ethHeader->destinationMac);

  snprintf(text, 200, "Source Mac     : ");
  writeIpLog(text);
  logIpMac(ethHeader->sourceMac);

  snprintf(text, 200, "Payload type   : %"PRIu16"\n", ethHeader->payloadType);
  writeIpLog(text);

  snprintf(text, 200, "\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\n");
  writeIpLog(text);
}

void logIpHeader(IpHeader *hdr, int incoming) {
  if (incoming) {
    snprintf(text, 200, "Incoming IP Header:\n");   
  }

  else {
    snprintf(text, 200, "Outgoing IP Header:\n");
  }

  writeIpLog(text);

  snprintf(text, 200, "Header Length: %"PRIu8"\n", hdr->headerLength);
  writeIpLog(text);

  snprintf(text, 200, "Version        : %"PRIu8"\n", hdr->version);
  writeIpLog(text);

  snprintf(text, 200, "tos            : %"PRIu8"\n", hdr->tos);
  writeIpLog(text);

  snprintf(text, 200, "totalLength    : %"PRIu16"\n", hdr->totalLength);
  writeIpLog(text);

  snprintf(text, 200, "ID             : %"PRIu16"\n", hdr->id);
  writeIpLog(text);

  snprintf(text, 200, "flags          : %"PRIu16"\n", hdr->flags);
  writeIpLog(text);

  snprintf(text, 200, "fragmentOffset : %"PRIu16"\n", hdr->fragmentOffset);
  writeIpLog(text);

  snprintf(text, 200, "ttl            : %"PRIu8"\n", hdr->ttl);
  writeIpLog(text);

  snprintf(text, 200, "protocol       : %"PRIu8"\n", hdr->protocol);
  writeIpLog(text);

  snprintf(text, 200, "checksum       : %"PRIu16"\n", hdr->checksum);
  writeIpLog(text);

  snprintf(text, 200, "Sender Address : %"PRIu32"\n", hdr->sourceAddress);
  writeIpLog(text);

  snprintf(text, 200, "destination    : %"PRIu32"\n", hdr->destinationAddress);
  writeIpLog(text);

  snprintf(text, 200, "\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\n");
  writeIpLog(text);
}

