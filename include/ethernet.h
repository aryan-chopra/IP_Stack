/**
 * @file ethernet.h
 * @author Aryan Chopra
 * @brief This file contains the defination for the
 * struct that holds the ethernet header.
 */

#ifndef ETHERNET_H
#define ETHERNET_H

#include <linux/if_ether.h>
#include <stdint.h>

/**
 * @struct EthernetHeader
 * @brief This struct contains the various fields in an ethernet header.
 *
 * @var EthernetHeader::destinationMac
 * It contains the MAC address of the device which sent the ethernet
 * frame.
 *
 * @var EthernetHeader::sourceMac
 * It contains the MAC address of the device to which the ethernet
 * frame is sent.
 *
 * @var EthernetHeader::payloadType
 * Usually contains the length of the payload that it carries.
 * If the value is greater than some value, then it contains the type of
 * payload. In our case it will contains either an ARP
 * payload(0x806), or an IP payload(0x800).
 *
 * @var EthernetHeader::payload
 * It contains the payload or encapsulated headers carried by
 * the ethernet frame.
 */

typedef struct{
	unsigned char destinationMac[6];
	unsigned char sourceMac[6];
	uint16_t payloadType;
	unsigned char payload[];
}__attribute__((packed)) EthernetHeader;

/**
 * @brief Converts the incoming buffer to a struct, having various fields
 * marked and named with appropriate name and size.
 *
 *
 * @param[in] buffer An incoming Ethernet Packet from the network.
 * @return header A struct which has various fields of the Ethernet Header
 * marked 
 * and named which appropriate name and size
 * @pre char * is a formated packet received over the network and has not
 * been modified.
 */

EthernetHeader* initializeEthernet(char *);

#endif

