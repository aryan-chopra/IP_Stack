/**
 * @file ethernet.c
 * @author Aryan Chopra
 * @brief Converts the incoming buffer to an Ethernet Header.
 */

#include <arpa/inet.h>

#include "ethernet.h"

/**
 * @brief Converts the incoming buffer to a struct, having various fields marked 
 * and named with appropriate name and size.
 *
 *
 * @param[in] buffer An incoming Ethernet Packet from the network.
 * @return header A struct which has various fields of the Ethernet Header marked 
 * and named which appropriate name and size
 * @pre buffer is a formated packet received over the network and has not been modified.
 */

EthernetHeader* initializeEthernet(char *buffer) {
	EthernetHeader* header = (EthernetHeader *) buffer;
	
	header->payloadType = htons(header->payloadType);

	return header;
}

