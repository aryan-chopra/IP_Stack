/**
 * @file netdev.c
 * @author Aryan Chopra
 * @brief Intializes a virtual network device.
 *
 * Initializes a virtual network device using the file descriptor, IP and MAC address provided.
 * Writes an ethernet frame to the parent TUN/TAP device.
 */

#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "netdev.h"
#include "ethernet.h"
#include "log.h"
#include "tap.h"

/**
 * @brief Initializes the virtual network device.
 *
 *
 * Initializes the network device using the IP and MAC addresses provided.
 * Checks for the validity of the the IP address provided in the decimal notation.
 * Converts the provided IP address from decimal to binary notation in Network Byte Order(Big Endian).
 * Assigns the provided MAC address to the network device converting it to uint8_t.
 *
 * @param[in, out] netdev A struct respresenting a virtual/emulated network device.
 * @param[in] device An integer containing the file descriptor of the TUN/TAP device.
 * @param[in] ipAddress A character array containing the IP Address in decimal notation.
 * The address is assigned to the virtual network device.
 * @param[in] macAddress A character array containing the MAC Address in legible format.
 * The address is assigned to the virtual network device.
 * @pre macAddress is a valid MAC address.
 */

void initNetdev(Netdev *netdev, int device, char *ipAddress, char *macAddress) {
  memset(netdev, 0, sizeof(netdev));
  netdev->deviceDescriptor = device;
  if (inet_pton(AF_INET, ipAddress, &netdev->address) != 1) {
    printf("Parsing failed\n");
    exit(1);	
  }

  sscanf(macAddress, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", 
      &netdev->macOctets[0],  
      &netdev->macOctets[1], 
      &netdev->macOctets[2], 
      &netdev->macOctets[3], 
      &netdev->macOctets[4], 
      &netdev->macOctets[5]);
}

/**
 * @brief Transmits the ethernet packet through it's TUN/TAP device;
 *
 *
 * Changes the payload type of the ethernet header provided.
 * Changes the source and destination MAC address of the header to be trasnmitted back.
 * Adds the size of the ethernet header to the total length of the packet/frame.
 * Logs the outgoing ethernet header.
 * Writes the ethernet header to the TUN/TAP device of the device provided.
 *
 * @param[in] netdev A struct emulating a network device.
 * The MAC address of netdev is used as source address, as the frame is transmitted from this device.
 * The frame is transmitted/written to the TUN/TAP device of the netdev.
 * @param[in, out] A struct structured with fields of apt sizes to contain the information of the ethernet header.
 * Source and destination MAC addresses and payload type are modified for correct transmission.
 * @param[in] ethertype An integer containing the type of payload of the modified frame which is to be transmitted.
 * @param[in] length An integer containing the length of the ethernet header provided.
 * @param[in] destination A character array containing the MAC address of the device from which the frame was recieved
 * or is to be sent to.
 */

void transmitNetdev(Netdev *netdev, EthernetHeader *ethHeader, uint16_t ethertype, int length, unsigned char *destination) {
  ethHeader->payloadType= htons(ethertype);

  memcpy(ethHeader->destinationMac, destination, 6);
  memcpy(ethHeader->sourceMac, netdev->macOctets, 6);

  length += sizeof(EthernetHeader);

  log(ethHeader, L_ETHERNET);

  write(netdev->deviceDescriptor, (char *)ethHeader, length);
}

