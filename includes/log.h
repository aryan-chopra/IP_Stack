/**
 * @file log.h
 * @author Aryan Chopra
 * @brief This header files declares the functions required to open log files
 * and log to the correct log file.
 *
 * Allows the user to use a single log function to log all kinds of headers to
 * the correct file.
 */

#ifndef LOG_H
#define LOG_H

#define L_INCOMING 0x01 ///Represents whether the header is incoming(1) or outgoing(0)
#define L_ARP 0x02 ///Represents whether the header is an ARP header.
#define L_ETHERNET 0x04 ///Represents whether the header is an ethernet header.
#define L_IP 0x08 ///Represents whether the header is an IP header.

#include "arp.h"
#include "ethernet.h"
#include "ip.h"

/**
 * @brief Opens all the log files at once.
 */

void openLogFiles();

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
 * @param[in] void * A pointer which points to the Header of any type.
 * @param[in] uint8_t A mask whose different bits represent unique type of
 * header passed.
 * LSB reserved for marking the header as incoming/outgoing.
 */

void log(void *, uint8_t);

#endif

