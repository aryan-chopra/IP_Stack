/**
 * @file icmp.c
 * @author Aryan Chopra
 * @brief Replies to an incoming ICMP Request over a network
 */

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "icmp.h"
#include "ip.h"

/**
 * @brief Handles the incoming ICMP Reqeust.
 *
 *
 * Extracts the ICMP information from the incoming IP Header.
 * Checks if the request type is Echo, and calls the appropriate function to modify the incoming header accordingly.
 *
 * @param[in, out] ipHeader A struct containing aptly named and sized fields of an IP Header.
 * If the request type is Echo, calls the appropriate function to modify the incmoing header.
 */

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

/**
 * @brief Changes the info type from Echo to Reply, and recomputes the checksum.
 *
 *
 * @param[in, out] icmpInfo A struct formated with appropriate names and sizes for an ICMP Header.
 * Changes the type from Echo to Reply to reply back to the ICMP(commonly a ping) request.
 * Recomputes the checksum.
 * @param[in] icmpLength Conatains length of ICMP Header.
 */

void structureIcmpReply(Icmp *icmpInfo, int icmpLength) {
  icmpInfo->type = ICMP_REPLY;
  icmpInfo->checksum = 0;
  icmpInfo->checksum = checksum(icmpInfo, icmpLength);
}

