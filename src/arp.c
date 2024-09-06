/**
 * @file arp.c
 * @author Aryan Chopra
 * @brief This file contains the simple implementation of Address Resolution Protocol(ARP).
 *
 * An ethernet header is passed which carries an ARP request in it's payload.
 * The payload is extracted and is stored in an Arp header, which contains information such as device type.
 * ARP query is run on the IP data extracted from the ARP header's payload, containing information about sending and receiving device.
 * If the requested IP address is found in the cache maintained, an ARP reply is sent modifying the contents of the received ethernet packet.
 */

#include <arpa/inet.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "arp.h"
#include "log.h"
#include "netdev.h"

ArpCacheEntry cache[ARP_CACHE_LEN];

/**
 * @brief This function initializes the ArpCache buffer by setting it to zero(0).
 *
 * @pre An array containing ArpCacheEntry of size ARP_CACHE_LEN exists.
 */

void initArp() {
  memset(cache, 0, ARP_CACHE_LEN * sizeof(ArpCacheEntry));
}

/**
 * @brief Creates a new ARP entry for the ARP header passed.
 *
 *
 * The function browses through the already existing cache and finds and an entry which is free, or not initialized yet.
 * Creates a new entry in the cache from, a struct, containing the state of the entry, hardware type of the device,
 * source IP Address and source MAC Address.
 *
 * @param[in] header Struct containing the information about the device sending the ARP request.
 * @param[in] data Struct containing the source and destination IP and MAC address.
 * @return 0, if the entry is inserted successfully, -1, if the cache is full.
 * @pre ARP_CACHE_LEN is initialized, Arp Cache is initialized.
 */

int insertArpEntry(ArpHeader *header, arp_ipv4 *data) {
  int length = ARP_CACHE_LEN;

  ArpCacheEntry *entry;

  for (int index = 0; index < length; index++) {
    entry = &(cache[index]);

    if (entry->state == ARP_FREE) {
      entry->state = ARP_RESOLVED;

      entry->hardwareType = header->hardwareType;
      entry->sourceIp = data->sourceIp;
      memcpy(entry->sourceMac, data->sourceMac, sizeof(entry->sourceMac));

      return 0;
    }
  }

  return -1;
}

/**
 * @brief Updates the ARP cache's mac address for the corresponding IP address contained in the ARP data
 *
 * 
 * Updates the ARP cache browsing through every entry.
 * Confirms whether the entry is resolved, then checks if the hardware type and IP address of the source matches.
 * If the above conditions are satisfied, the entry's corresponding MAC address is updated.
 *
 * @param[in] header Struct containing the information about the device sending the ARP request.
 * @param[in] data Struct containing the source and destination IP and MAC address.
 * @return 0, if the entry is updated successfully, -1 if the entry having the IP address of the source does not exist.
 * @pre ARP_CACHE_LEN is initialized, Arp Cache is initalized.
 */

int updateArpTable(ArpHeader *header, arp_ipv4 *data) {
  int length = ARP_CACHE_LEN;

  ArpCacheEntry *entry;

  for (int index = 0; index < length; index++) {
    entry = &cache[index];

    if (entry->state == ARP_RESOLVED) {
      if (entry->hardwareType == header->hardwareType && entry->sourceIp == data->sourceIp) {
        memcpy(entry->sourceMac, data->sourceMac, 6);
        return 1;
      }
    }
  }
  return 0;
}

/**
 * @brief Handles the incoming ARP request.
 *
 *
 * Extracts the ARP header from the Ethernet header's payload and ARP devices' informtion.
 * Converts the required information from network notation(Big Endian) to host's notation(mostly Little Endian).
 * Logs the incoming ARP packet to a log file.
 * Checks if the hardware type is supported and whether the Internet Protocol is version 4.
 * Updates the MAC address of the IP address in the ARP cache and updates a flag if the operation is successful.
 * Flag conveys whether the IP address exists in the ARP cache or not.
 * Checks whether the requested IP's MAC exists in the ARP cache.
 * Replies with the requested MAC address.
 *
 * @param[in] netdev A struct emulating a network device having IP and MAC address.
 * @param[in] header A struct containing the information of an ethernet header.
 */

void incomingRequest(Netdev *netdev, EthernetHeader *header) {

  ArpHeader *arpHeader;
  arp_ipv4 *arpData;
  int merge = 0;

  arpHeader = (ArpHeader *) header->payload;
  arpHeader->hardwareType = ntohs(arpHeader->hardwareType);
  arpHeader->protype = ntohs(arpHeader->protype);
  arpHeader->opcode = ntohs(arpHeader->opcode);

  arpData = (arp_ipv4 *) arpHeader->data;

  log(arpData, L_ARP | L_INCOMING);

  if (arpHeader->hardwareType != ARP_ETHERNET) {
    printf("Only ethernet is supported\n");
    return;
  }

  if (arpHeader->protype != ARP_IPV4) {
    printf("Only IPv4 is supported\n");
    return;
  }

  merge = updateArpTable(arpHeader, arpData);

  if (netdev->address!= arpData->destinationIp) {
    printf("ARP not for our own address\n");
  }

  if (!merge && insertArpEntry(arpHeader, arpData) != 0) {
    printf("ARP Table full!\n");
  }

  switch (arpHeader-> opcode) {
    case ARP_REQUEST:
      replyArp(netdev, header, arpHeader);
      break;
    default:
      printf("Invalid Request\n");
      break;
  }
}

/**
 * @brief Replies the MAC address back to the sender
 *
 *
 * Updates the ARP devices' header marking the destination(our device) as the source,
 * Fills in the requested MAC address.
 * Converts the fields back to Network notation(Big Endian).
 * Updates the opcode of ARP header from request, to response.
 * Updates the length, logs the outgoing ARP packet, and calls the function required to transmit the packet over the network.
 *
 * @param[in] netdev A struct emulating a network device having IP and MAC address.
 * @param[in] header A struct containing the information of an ethernet header.
 * @param[in, out] header Struct containing the information about the device sending the ARP request.
 */

void replyArp(Netdev *netdev, EthernetHeader *etherHeader, ArpHeader *arpHeader) {
  arp_ipv4 *arpData;
  int length;

  arpData = (arp_ipv4 *) arpHeader->data;

  memcpy(arpData->destinationMac, arpData->sourceMac, 6);
  arpData->destinationIp = arpData->sourceIp;
  memcpy(arpData->sourceMac, netdev->macOctets, 6);
  arpData->sourceIp = netdev->address;

  arpHeader->opcode = ARP_REPLY;

  arpHeader->opcode = htons(arpHeader->opcode);
  arpHeader->hardwareType = htons(arpHeader->hardwareType);
  arpHeader->protype = htons(arpHeader->protype);

  length = sizeof(ArpHeader) + sizeof(arp_ipv4);

  log(arpData, L_ARP);
  transmitNetdev(netdev, etherHeader,  ETH_P_ARP, length, arpData->destinationMac);
}

