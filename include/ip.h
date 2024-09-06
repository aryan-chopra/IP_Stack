/**
 * @file ip.h
 * @author Aryan Chopra
 * @brief Contains the defination of the struct which holds the IP Header.
 */

#ifndef IPV4_H
#define IPV4_H

#include <stdint.h>

#include "ethernet.h"
#include "netdev.h"

#define IPV4 0x04 ///Predefined value which represents that the header has the version for of Internet Protocol
#define ICMP 0x01 ///Predefined value whihc represents that the payload carries an ICMP Echo or Reply

/**
 * @struct IpHeader
 * @brief A struct which is designed to hold the IP header.
 *
 * The headerLength and the version are swapped to account for Network
 * Notation(Big Endian).
 *
 * @var IpHeader::headerLength
 * A 4 bit field which contains the total number of octates the header consists
 * of.
 *
 * @var IpHeader::version
 * A 4 bit field which contains the version of IP the header uses.
 *
 * @var Ipheader::tos
 * Represents the type of service of the header.
 *
 * @var IpHeader::totalLength
 * Contains the total length of the header, including the header and the
 * payload.
 *
 * @var IpHeader::id
 * This field is used to order fragments of a frame.
 * Fragments of the same frame will have same ID.
 *
 * @var IpHeader::flags
 * A 3 bit field which contains information such as whether further packets are
 * arriving or not.
 *
 * @var IpHeader::fragmentOffset
 * A 13 bit field which defines the offset of the current fragment in the
 * frame.
 *
 * @var IpHeader::ttl
 * Contains the Time To Live field of the header.
 *
 * @var IpHeader::protocol
 * Contains the protocol or the type of payload, such as TCP, ICMP.
 *
 * @var IpHeader::checksum
 * Contains the checksum field of IP Header used to verify the integrity of the
 * header.
 *
 * @var IpHeader::sourceAddress
 * Contains the IP Address of the source, in binary, Network Notation(Big
 * Endian).
 * 
 * @var IpHeader::destinationAddress
 * Contains the IP Address of the destination device, in binary, Netwok
 * Notation(Big Endian).
 *
 * @var IpHeader::data
 * Contains the payload the header carries.
 */

typedef struct {
	uint8_t headerLength : 4;
	uint8_t version : 4;
	uint8_t tos;
	uint16_t totalLength;
	uint16_t id;
	uint16_t flags : 3;
	uint16_t fragmentOffset: 13;
	uint8_t ttl;
	uint8_t protocol;
	uint16_t checksum;
	uint32_t sourceAddress;
	uint32_t destinationAddress;
  uint8_t data[];
} __attribute__((packed)) IpHeader;

/**
 * @brief Handles the incoming IP request.
 *
 *
 * Extracts the payload, an IP Packet from the incoming Ethernet Packet.
 * Computes the checksum to verify the integrity of the packet.
 * Checks various parameters of the IP Header to verify the integrity.
 * Checks the type of request the packet is carrying.
 * In case of an ICMP request, calls the appropriate functions to deal with
 * the ICMP request.
 * Replies back to the source with a modified IP/Ethernet Packet.
 *
 * @param[in] Netdev A struct emulating a network device. The IP request is
 * directed to the device.
 * @param[in, out] EthernetHeader A struct containing formated fields for
 * an Ethrnet Header.
 * The payload and the sender and receiver address of the packet is
 * modified while relaying back.
 * @pre The ethernet header contains an IP Packet;
 */

void ipIncoming(Netdev *, EthernetHeader *);

/**
 * @brief Relays the ethernet packet back to the sender.
 *
 *
 * Swaps the source and destination IP Addresses, as the Packet is to be
 * send back to the sender.
 * Recomputes the checksum to verify the integrity of the IP Packet.
 * Logs the outgoing Packet to a log file.
 *
 * @param[in] Netdev A struct emulating a device on the network, through
 * which the request is received
 * and is to be sent back from.
 * @param[in, out] EthernetHeader A struct containing formated fields to
 * contain an ethernet header/packet.
 * The payload(Ip Packet) is modified with the appropriate information and
 * is transmitted back to the sender.
 */

void ipReply(Netdev *, EthernetHeader *);

/**
 * @brief Computes the checksum for the IP Header provided.
 *
 * @param[in] void * The IP header on which the checksum is to be computed.
 * @param[in] int The length of the IP Header provided.
 * @return Checksum computed.
 */

uint16_t checksum(void *, int);

#endif

