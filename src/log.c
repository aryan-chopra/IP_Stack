/**
 * @file log.c
 * @author Aryan Chopra
 * @brief A unified interface to log the supported headers.
 *
 * One can use the function provided to open and close all the log files at
 * once.
 * Provides the ability to log the supported headers in correct files.
 * Takes away the burden to remember various function names to log different headers.
 * Logs the header provided on the basis of the flags passed.
 * Ability to specify the type of header using bit flags already defined in
 * a macro.
 * Ability to mark the provided packet as incmoming or outgoing using a flag
 * defined in a macro.
 */

#include <stdio.h>

#include "arp_log.h"
#include "ethernet_log.h"
#include "ip_log.h"
#include "log.h"

/**
 * @brief Opens all the log files at once.
 */

void openLogFiles() {
  openArpLog();
  openIpLog();
  openEthernetLog();
}

/**
 * @brief Logs the header identifying various flags passed.
 *
 *
 * Identifies the type of header using the flags passed.
 * Different flags toggle different bits of an 8-bit mask.
 * The LSB of the mask specifies whether the packet is incoming or outgoing.
 * Extracts the toggled bits from the 8-bit mask and calls the appropriate
 * logging function accordingly.
 *
 * @param[in] header A pointer which points to the Header of any type.
 * @param[in] flags A mask whose different bits represent unique type of
 * header passed.
 * LSB reserved for marking the header as incoming/outgoing.
 */

void log(void *header, uint8_t flags) {
  uint8_t headerType = flags & 0x0E;
  uint8_t incoming = flags & 0x01;

  switch(headerType) {
    case L_ARP:
      logArpHeader((arp_ipv4 *)header, incoming);
      break;
    case L_ETHERNET:
      logEthernetHeader((EthernetHeader *)header, incoming);
      break;
    case L_IP:
      logIpHeader((IpHeader *)header, incoming);
      break;
    default:
      printf("Unidentified header type\n");
  }
}

