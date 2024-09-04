#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "icmp.h"
#include "ip.h"

void handleIcmp(IpHeader *ipHeader) {
  Icmp *icmpInfo = (Icmp *) ipHeader->data;

  switch (icmpInfo->type) {
    case ICMP_ECHO:
      printf("Is ICMP_ECHO\n");
      int length = ipHeader->totalLength - (ipHeader->headerLength * 4);
      structureIcmpReply(icmpInfo, length);
      break;
    default:
      printf("Got ICMP type = %"PRIu8"\n", icmpInfo->type);
      return;
  }
}

void structureIcmpReply(Icmp *icmpInfo, int icmpLength) {
  icmpInfo->type = ICMP_REPLY;
  icmpInfo->checksum = 0;
  icmpInfo->checksum = checksum(icmpInfo, icmpLength);
}

