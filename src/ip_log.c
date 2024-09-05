#include <errno.h>
#include <inttypes.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ip.h"
#include "log_location.h"

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

void logIpHeader(IpHeader *hdr, int incoming) {
  char *text = calloc(50, 1);

  if (incoming) {
    snprintf(text, 50, "Incoming IP Header:\n");   
  }

  else {
    snprintf(text, 50, "Outgoing IP Header:\n");
  }

  writeIpLog(text);

  snprintf(text, 50, "Header Length: %"PRIu8"\n", hdr->headerLength);
  writeIpLog(text);

  snprintf(text, 50, "Version        : %"PRIu8"\n", hdr->version);
  writeIpLog(text);

  snprintf(text, 50, "tos            : %"PRIu8"\n", hdr->tos);
  writeIpLog(text);

  snprintf(text, 50, "totalLength    : %"PRIu16"\n", hdr->totalLength);
  writeIpLog(text);

  snprintf(text, 50, "ID             : %"PRIu16"\n", hdr->id);
  writeIpLog(text);

  snprintf(text, 50, "flags          : %"PRIu16"\n", hdr->flags);
  writeIpLog(text);

  snprintf(text, 50, "fragmentOffset : %"PRIu16"\n", hdr->fragmentOffset);
  writeIpLog(text);

  snprintf(text, 50, "ttl            : %"PRIu8"\n", hdr->ttl);
  writeIpLog(text);

  snprintf(text, 50, "protocol       : %"PRIu8"\n", hdr->protocol);
  writeIpLog(text);

  snprintf(text, 50, "checksum       : %"PRIu16"\n", hdr->checksum);
  writeIpLog(text);

  snprintf(text, 50, "Sender Address : %"PRIu32"\n", hdr->sourceAddress);
  writeIpLog(text);

  snprintf(text, 50, "destination    : %"PRIu32"\n", hdr->destinationAddress);
  writeIpLog(text);

  snprintf(text, 50, "\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\n");
  writeIpLog(text);

  free(text);
}

