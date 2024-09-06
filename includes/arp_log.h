/**
 * @file arp_log.h
 * @author Aryan Chopra
 * @breif This header file contains the declaration of the
 * functions necassary to log thr ARP Header.
 */

#ifndef ARP_LOG_H
#define ARP_LOG_H

#include "arp.h"
#include "log_location.h"

/*
 * @brief Opens the log file.
 * 
 *
 * Opens the log file to log the provided ARP Data headers, in append mode.
 * Prints an error to the console and quits the process in case of an error.
 *
 * @pre Log file exists at the location.
 */

void openArpLog();

/**
 * @brief Writes to the log file, the character array provided.
 *
 *
 * @pre File descriptor provided is valid.
 */

void writeArpLog(char *);

/**
 * @brief Parses the incoming Arp Data header and logs the fields to a file.
 *
 *
 * Logs the packet as incoming or outgoing based on the "incoming" flag.
 * Logs the various fields of the ARP Data Header.
 * Calls the corresponding functions for logging MAC and IP addresses in commonly legible format.
 *
 * @param[in] arp_ipv4 A struct containing the IP and MAC address of the sender and receiver.
 * @param[in] uint8_t Flag which marks the packet as incoming or outgoing in the log.
 */

void logArpHeader(arp_ipv4 *, uint8_t);

#endif

