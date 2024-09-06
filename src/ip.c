/**
 * @file ip.c
 * @author Aryan Chopra
 * @brief Handles the incoming IP Packet.
 *
 * Extracts the IP Packet from the incoming Ethernet Header.
 * Checks whether the implementations of the packet's protocols exist.
 * Calls appropriate functions for handling an ICMP request, if the payload is ICMP.
 * Contains the utility for computing checksum.
 * Replies to the source with an appropriate message(not always).
 */

#include <arpa/inet.h>
#include <inttypes.h>
#include <stdio.h>

#include "arp.h"
#include "ethernet.h"
#include "icmp.h"
#include "ip.h"
#include "log.h"
#include "netdev.h"

/**
 * @brief Handles the incoming IP request.
 *
 *
 * Extracts the payload, an IP Packet from the incoming Ethernet Packet.
 * Computes the checksum to verify the integrity of the packet.
 * Checks various parameters of the IP Header to verify the integrity.
 * Checks the type of request the packet is carrying.
 * In case of an ICMP request, calls the appropriate functions to deal with the ICMP request.
 * Replies back to the source with a modified IP/Ethernet Packet.
 *
 * @param[in] netdev A struct emulating a network device. The IP request is directed to the device.
 * @param[in, out] ethHeader A struct containing formated fields for an Ethrnet Header.
 * The payload and the sender and receiver address of the packet is modified while relaying back.
 * @pre The ethernet header contains an IP Packet;
 */

void ipIncoming(Netdev *netdev, EthernetHeader *ethHeader) {
  IpHeader *ipHeader = (IpHeader *) ethHeader->payload;
  uint16_t checksumValue = -1;

  if (ipHeader->version != IPV4) {
    printf("Version not IPV4 while intercepting got = &0x02\n", ipHeader->version);
    return;
  }

  printf("Version and length: %"PRIu8"\n", ipHeader->version);

  if (ipHeader->headerLength < 5) {
    printf("Header length must be 5\n");
    return;
  }

  if (ipHeader->ttl == 0) {
    //todo send ICMP error
    printf("Packet ttl = 0\n");
    return;
  }

  checksumValue = checksum(ipHeader, ipHeader->headerLength * 4);

  if (checksumValue != 0) {
    printf("Checksum failed to verify in incoming before icmp\n");
    return;
  }

  ipHeader->totalLength = ntohs(ipHeader->totalLength);

  switch (ipHeader->protocol) {
    case ICMP:
      log(ipHeader, L_IP | L_INCOMING);
      handleIcmp(ipHeader);
      ipReply(netdev, ethHeader);
      break;
    default:
      printf("Got protocol: %"PRIu8"\n", ipHeader->protocol);
      //diff between uit8 and uint8_t?
      return;
  }
}

/**
 * @brief Relays the ethernet packet back to the sender.
 *
 *
 * Swaps the source and destination IP Addresses, as the Packet is to be send back to the sender.
 * Recomputes the checksum to verify the integrity of the IP Packet.
 * Logs the outgoing Packet to a log file.
 *
 * @param[in] netdev A struct emulating a device on the network, through which the request is received
 * and is to be sent back from.
 * @param[in, out] ethHeader A struct containing formated fields to contain an ethernet header/packet.
 * The payload(Ip Packet) is modified with the appropriate information and is transmitted back to the sender.
 */

void ipReply(Netdev *netdev, EthernetHeader *ethHeader){
  IpHeader *ipHeader = (IpHeader *) ethHeader->payload;
  uint8_t length = ipHeader->totalLength;

  ipHeader->destinationAddress = ipHeader->sourceAddress;
  ipHeader->sourceAddress = netdev->address;

  ipHeader->totalLength = htons(ipHeader->totalLength);

  ipHeader->checksum = 0;
  ipHeader->checksum = checksum(ipHeader, ipHeader->headerLength * 4);

  log(ipHeader, L_IP);
  transmitNetdev(netdev, ethHeader, ETH_P_IP, length, ethHeader->sourceMac); 
}

/**
 * @brief Computes the checksum for the IP Header provided.
 *
 * @param[in] address The IP header on which the checksum is to be computed.
 * @param[in] length The length of the IP Header provided.
 * @return Checksum computed.
 */

uint16_t checksum(void *address, int count) {
  int sum = 0;
  uint16_t *ptr = address;

  while (count > 1) {
    sum += * ptr++;
    count -= 2;
  }
  
  if (count > 0) {
    sum += * (uint8_t *) address;
  }

  while (sum >> 16) {
    sum = (sum & 0xffff) + (sum >> 16);
  }

  return ~sum;
}

