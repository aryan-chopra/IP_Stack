/**
 * @file ip_log.h
 * @author Aryan Chopra
 * @brief This file contains the declaration of the functions required to log
 * the IP headers.
 */

#ifndef IP_LOG_H
#define IP_LOG_H

#include "ip.h"
#include "log_location.h"

/**
 * @brief Opens the log file specified.
 *
 *
 * Opens the log file at the specified location in append mode.
 * In case of an error, it prints an explanatory error message on console
 * and exits the process.
 *
 * @pre Log file exists at the location specified
 */

void openIpLog();

/**
 * @brief Writes the char array in the IP log file.
 *
 * @param[in] char * A character array containing the text to log.
 * @pre The file descriptor is valid.
 */

void writeIpLog(char *);

/**
 * @brief Logs the IP Header in the Log file.
 *
 * Marks the IP Header as incoming or outgoing in the log file.
 * Marking depends on the value of the flag provided.
 * Logs the fields of the IP Header in the Log file.
 * Calls necessary functions to convert the IP address in legible form.
 *
 * @param[in] IpHeader A struct formatted with apt name and sizes of fields in an
 * IP Header.
 * @param[in] uint8_t A flag which marks whether the header provided is
 * incoming or outgoing.
 */

void logIpHeader(IpHeader *, uint8_t);

#endif

