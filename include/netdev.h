/**
 * @file netdev.h
 * @author Aryan Chopra
 * @brief Contains the defination of a struct which is represents an emulated
 * network device.
 */

#ifndef NETDEV_H
#define NETDEV_H

#include "ethernet.h"

/**
 * @struct Netdev
 * @brief A struct which represents an emulated network device.
 *
 * @var Netdev::deviceDescriptor
 * File descriptor of the TUN/TAP device.
 *
 * @var Netdev::address
 * IP Address of the network device in bianry, Network Notation(Big Endian).
 *
 * @var Netdev::macOctates
 * MAC Address of the network device.
 */

typedef struct{
  int deviceDescriptor;
	uint32_t address;
	unsigned char macOctets[6];
}Netdev;

/**
 * @brief Initializes the virtual network device.
 *
 *
 * Initializes the network device using the IP and MAC addresses provided.
 * Checks for the validity of the the IP address provided in the decimal
 * notation.
 * Converts the provided IP address from decimal to binary notation in
 * Network Byte Order(Big Endian).
 * Assigns the provided MAC address to the network device converting it to
 * uint8_t.
 *
 * @param[in, out] Netdev A struct respresenting a virtual/emulated network
 * device.
 * @param[in] int An integer containing the file descriptor of the TUN/TAP
 * device.
 * @param[in] char * A character array containing the IP Address in
 * decimal notation.
 * The address is assigned to the virtual network device.
 * @param[in] char * A character array containing the MAC Address in
 * legible format.
 * The address is assigned to the virtual network device.
 * @pre macAddress is a valid MAC address.
 */

void initNetdev(Netdev *, int,  char *, char *);

/**
 * @brief Transmits the ethernet packet through it's TUN/TAP device;
 *
 *
 * Changes the payload type of the ethernet header provided.
 * Changes the source and destination MAC address of the header to be
 * trasnmitted back.
 * Adds the size of the ethernet header to the total length of the
 * packet/frame.
 * Logs the outgoing ethernet header.
 * Writes the ethernet header to the TUN/TAP device of the device provided.
 *
 * @param[in] Netdev A struct emulating a network device.
 * The MAC address of netdev is used as source address, as the frame is
 * transmitted from this device.
 * The frame is transmitted/written to the TUN/TAP device of the netdev.
 * @param[in, out] EthernetHeader A struct structured with fields of apt sizes to contain
 * the information of the ethernet header.
 * Source and destination MAC addresses and payload type are modified for
 * correct transmission.
 * @param[in] uint16_t An integer containing the type of payload of the
 * modified frame which is to be transmitted.
 * @param[in] int An integer containing the length of the ethernet header
 * provided.
 * @param[in] unsigned char * A character array containing the MAC address of
 * the device from which the frame was recieved or is to be sent to.
 */

void transmitNetdev(Netdev *, EthernetHeader *, uint16_t , int , unsigned char *);

#endif

