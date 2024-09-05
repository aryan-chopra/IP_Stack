#include <arpa/inet.h>
#include <errno.h>
#include <inttypes.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ip.h"
#include "log_location.h"

#define SIZE 100

#define FIRST_OCTATE 0xFF000000
#define SECOND_OCTATE 0xFF0000
#define THIRD_OCTATE 0xFF00
#define FOURTH_OCTATE 0xFF

int ipLogFile; 

void openIpLog() {
  ipLogFile = open(LOG_LOCATION "ip_log.txt", O_WRONLY | O_APPEND);
  if (ipLogFile < 0) {
    printf("Error opening ip log file: %s\n", strerror(errno));
    exit(1);
  }
}

void writeIpLog(char *text) {
  write(ipLogFile, text, strlen(text));
}

static void logIpAddress(uint32_t address) {
  char *text = calloc(SIZE, 1);

  snprintf(text, SIZE, "%s\n", inet_ntoa(*((struct in_addr *)&address)));
        
  writeIpLog(text);

  free(text);
}

void logIpHeader(IpHeader *hdr, int incoming) {
  char *text = calloc(SIZE, 1);

  if (incoming) {
    snprintf(text, SIZE, "Incoming IP Header:\n\n");   
  }

  else {
    snprintf(text, SIZE, "Outgoing IP Header:\n\n");
  }

  writeIpLog(text);

  snprintf(text, SIZE, "Header Length        : %"PRIu8"\n", hdr->headerLength);
  writeIpLog(text);

  snprintf(text, SIZE, "Version              : %"PRIu8"\n", hdr->version);
  writeIpLog(text);

  snprintf(text, SIZE, "tos                  : %"PRIu8"\n", hdr->tos);
  writeIpLog(text);

  snprintf(text, SIZE, "totalLength          : %"PRIu16"\n", hdr->totalLength);
  writeIpLog(text);

  snprintf(text, SIZE, "ID                   : %"PRIu16"\n", hdr->id);
  writeIpLog(text);

  snprintf(text, SIZE, "flags                : %"PRIu16"\n", hdr->flags);
  writeIpLog(text);

  snprintf(text, SIZE, "fragmentOffset       : %"PRIu16"\n", hdr->fragmentOffset);
  writeIpLog(text);

  snprintf(text, SIZE, "ttl                  : %"PRIu8"\n", hdr->ttl);
  writeIpLog(text);

  snprintf(text, SIZE, "protocol             : %"PRIu8"\n", hdr->protocol);
  writeIpLog(text);

  snprintf(text, SIZE, "checksum             : %"PRIu16"\n", hdr->checksum);
  writeIpLog(text);

  snprintf(text, SIZE, "Sender Address       : ");
  writeIpLog(text);
  logIpAddress(hdr->sourceAddress);

  snprintf(text, SIZE, "Destination Address  : ");
  writeIpLog(text);
  logIpAddress(hdr->destinationAddress);

  snprintf(text, SIZE, "\n-------------------------------------------------\n\n");
  writeIpLog(text);

  free(text);
}

