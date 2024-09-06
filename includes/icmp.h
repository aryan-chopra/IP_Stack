/**
 * @file icmp.h
 * @author Aryan Chopra
 * @brief This file contains the definations of the struct to read and edit the
 * ICMP header.
 */

#ifndef ICMPV4_H
#define ICMPV4_H

#include <stdint.h>

#include "ip.h"
#include "netdev.h"

#define ICMP_REPLY 0x00 ///Predefined value which represents the header carries an ICMP reply.
#define ICMP_ECHO 0x08 ///Predefined value which represents the header carries an ICMP request.

/**
 * @struct Icmp
 * @brief A struct to hold various fields in an ICMP header.
 *
 * @var Icmp::type
 * Describes the purpose of payload the header carries.
 *
 * @var Icmp::code
 * Describes the code, or the type of message the header carries.
 *
 * @var Icmp::checksum
 * Holds the checksum for the ICMP header.
 * It is used to verify the integrity of the header.
 *
 * @var Icmp::data
 * Contains the payload the ICMP header carries.
 */

typedef struct {
  //Purpose of message
  uint8_t type;
  //Describes the response
  uint8_t code;
  uint16_t checksum;
  uint8_t data[];
}__attribute((packed)) Icmp;

/**
 * @brief Handles the incoming ICMP Reqeust.
 *
 *
 * Extracts the ICMP information from the incoming IP Header.
 * Checks if the request type is Echo, and calls the appropriate function to
 * modify the incoming header accordingly.
 *
 * @param[in, out] IpHeader A struct containing aptly named and sized
 * fields of an IP Header.
 * If the request type is Echo, calls the appropriate function to modify the
 * incmoing header.
 */

void handleIcmp(IpHeader *); 

/**
 * @brief Changes the info type from Echo to Reply, and recomputes the checksum.
 *
 *
 * @param[in, out] icmpInfo A struct formated with appropriate names and
 * sizes for an ICMP Header.
 * Changes the type from Echo to Reply to reply back to the ICMP(commonly a
 * ping) request.
 * Recomputes the checksum.
 * @param[in] Icmp A struct which is designed to allow us to read and write to
 * the ICMP header.
 * @param[in] int Conatains length of ICMP Header.
 */

void structureIcmpReply(Icmp *, int);

#endif

