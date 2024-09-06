/**
 * @file ethernet_log.c
 * @author Aryan Chopra
 * @brief This header files contains the necassary functions to log the
 * ethernet header to the log file.
 */

#ifndef ETHERNET_LOG_H
#define ETHERNET_LOG_H

#include "log_location.h"

/**
 * @brief Opens the log file.
 *
 *
 * Opens the log file to log the ethernet headers, in append mode.
 * Prints an error to the consols and exits the process in case of an error.
 *
 * @pre Log file exists at the location.
 */

void openEthernetLog();

/**
 * @brief Writes the provided character array to the log.
 *
 *
 * @pre File descriptor provided is valid.
 */

void writeEtLog(char *);

/**
 * @brief Logs the provided ethernet header.
 *
 *
 * Parses the provided ethernet header and logs the various fields.
 * Marks the packet as incoming or outgoing based on the flag provided
 * Calls the appropriate functions to format and log MAC address in legible format.
 *
 * @param[in] EthernedHeader Struct formated with apt names and sizes for various fields of an ethernet header.
 * @param[in] uint8_t Flag that marks the packet as incoming or outgoing
 */

void logEthernetHeader(EthernetHeader *, uint8_t);

#endif

