/**
 * @file arp.h
 * @author Aryan Chopra
 * @brief Contains the declaration of various structs to be used by ARP.
 *
 * This header file declares various structures to be used by ARP functionality, viz
 * representing an ARP header, an ARP data header, which is the payload of an ARP header
 * containing information about sender and host, and an ARP Cache Entry struct which is 
 * a single entry for the ARP cache containing IP address and MAC address of various
 * devices that interacted with the process.
 */

#ifndef ARP_H
#define ARP_H

#include <stdint.h>

#include "ethernet.h"
#include "ethernet.h"
#include "netdev.h"

#define ARP_ETHERNET 0x0001 ///Predefined code for specifying that ARP request was carried over Ethernet.
#define ARP_IPV4 0x0800 ///Predefined code for specifying that the protocol used to carry the request is IPv4.

#define ARP_REQUEST 0x0001 ///Predefined code for specifying that ARP header contains an ARP Request.
#define ARP_REPLY 0x0002 ///Predefined code for specifying that ARP header contains a reply to an ARP Request.

#define ARP_CACHE_LEN 32
#define ARP_FREE 0
#define ARP_RESOLVED 2

/**
 * @struct ArpHeader
 * @brief A structure to hold various fields in an ARP header.
 *
 *
 * @var ArpHeader::hardwareType
 * The type of hardware used to connect the two devices. eg Ethernet.
 *
 * @var ArpHeader::protocol
 * Specifies the protocol for which the request is intended. eg IPv4
 *
 * @var ArpHeader::hardwareSize
 * Number of octates in the hardware address provided.
 * It will represent the number of octates in MAC address in our case.
 *
 * @var ArpHeader::prosize
 * Number of octates in the address of the protocol used.
 * In our case, it will represent the number of octates in an IPv4 address.
 *
 * @var ArpHeader::opcode
 * Specifies the operation the packet is intended for.
 * It is 1 for request, and 2 for reply.
 *
 * @var ArpHeader::data
 * It contains the underlying payload of the ARP header.
 */

typedef struct{
	uint16_t hardwareType;
	uint16_t protocol;
	unsigned char hardwareSize;
	unsigned char prosize;
	uint16_t opcode;
	unsigned char data[];
}__attribute__((packed)) ArpHeader;

/**
 * @struct arp_ipv4
 * @brief The struct contains the IPv4 data of the sender and receiver.
 *
 * @var arp_ipv4::sourceMac
 * It contains the media address of the sender.
 * In our case, it will contain MAC address of the sender.
 *
 * @var arp_ipv4::sourceIP
 * It contains the address used by the protocol for which the request is 
 * intended.
 * In our case, it will contain the source IPv4 address of the sender.
 *
 * @var arp_ipv4::destinationMac
 * It contains the media address of the receiver.
 * In our case, it will contain MAC address of the receiver.
 *
 * @var arp_ipv4::destinationIp
 * It contains the address used by the protocol for which the request is 
 * intended.
 * In our case, it will contain the source IPv4 address of the receiver.
 */

typedef struct{
	unsigned char sourceMac[6];
	uint32_t sourceIp;
	unsigned char destinationMac[6];
	uint32_t destinationIp;
}__attribute__((packed)) arp_ipv4;

/**
 * @struct ArpCacheEntry
 * @brief Structure to contain one entry in the ARP Cache table.
 *
 * An array of Arp Cache Entries make the ARP Cache Table.
 *
 * @var ArpCacheEntry::hardwareType
 * It specifies the tpye of hardware connecting the two devices.
 *
 * @var ArpCacheEntry::sourceIp
 * It specifies the IP address of the device which interacts with the 
 * system.
 * In our case, it will contain the IP address of the device sending the ARP request.
 *
 * @var ArpCacheEntry::sourceMac
 * It specifies the MAC address of the device which interacts with the 
 * system.
 * In our case, it will contain the MAC address of the device sending the 
 * ARP request.
 *
 * @var ArpCacheEntry::state
 * It specifies whether the current entry in ARP Cache is empty(0) or
 * resolved(1).
 * If the entry is empty, it means that there is no information that the
 * current entry has.
 * If the entry is resolved, it means that there the current entry has
 * information about a device.
 * Arp Cache array is cleared(set to zero) when the ARP is initialized, which
 * sets all the states
 * to zero.
 */

typedef struct{
	uint16_t hardwareType;
	uint32_t sourceIp;
	unsigned char sourceMac[6];
	unsigned int state;
} ArpCacheEntry;

/**
 * @brief This function initializes the ArpCache buffer by setting it to zero(0).
 *
 * @pre An array containing ArpCacheEntry of size ARP_CACHE_LEN exists.
 */

void initArp();

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
 * @param[in] Netdev * A pointer to a struct emulating a network device having IP and MAC address.
 * @param[in] EthernetHeader * A pointer to a struct containing the information of an ethernet header.
 */

void incomingRequest(Netdev *, EthernetHeader *);

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
 * @param[in] Netdev A struct emulating a network device having IP and MAC address.
 * @param[in, out] EthernetHeader A struct containing the information of an ethernet header.
 * @param[in, out] ArpHeader A struct containing the information about the device sending the ARP request.
 */

void replyArp(Netdev *, EthernetHeader *, ArpHeader *);

#endif
