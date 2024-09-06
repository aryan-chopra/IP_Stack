/**
 * @file ip_log.c
 * @author Aryan Chopra
 * @brief Logs the IP packet in IP log file.
 *
 * Opens the file at the location in append mode.
 * Logs the IP address in decimal notation after converting it.
 * Logs other necessary information in an IP packet.
 */

#include <arpa/inet.h>
#include <errno.h>
#include <inttypes.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ip.h"
#include "log_location.h"

#define SIZE 100

#define FIRST_OCTATE 0xFF000000
#define SECOND_OCTATE 0xFF0000
#define THIRD_OCTATE 0xFF00
#define FOURTH_OCTATE 0xFF

/**
 * @brief Contains the file descirptor of the Log file opened.
 */

int ipLogFile; 

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

void openIpLog() {
  ipLogFile = open(LOG_LOCATION "ip_log.txt", O_WRONLY | O_APPEND);
  if (ipLogFile < 0) {
    printf("Error opening ip log file: %s\n", strerror(errno));
    exit(1);
  }
}

/**
 * @brief Writes the char array in the IP log file.
 *
 * @param[in] text A character array containing the text to log.
 * @pre The file descriptor is valid.
 */

void writeIpLog(char *text) {
  write(ipLogFile, text, strlen(text));
}

/**
 * @brief Converts the provided address from binary, network notation(Big
 * Endian) to the legible decimal notation.
 *
 *
 * The function is static to avoid linking errors with the logIpAddress
 * function in a different file.
 * The function writes the formated string using snprintf to a buffer which
 * is then logged to the log file.
 * inet_atoa requires in_addr struct which contains a single field
 * equivalent to uint32_t.
 * The address provided is casted to the struct which is then used to covert
 * the 
 * IP address in commonl understandable format.
 *
 * @param[in] address 32 bit unsigned int which contains the IP address in
 * binary, Big Endian notation.
 */

static void logIpAddress(uint32_t address) {
  char *text = calloc(SIZE, 1);

  snprintf(text, SIZE, "%s\n", inet_ntoa(*((struct in_addr *)&address)));
        
  writeIpLog(text);

  free(text);
}

/**
 * @brief Logs the IP Header in the Log file.
 *
 * Marks the IP Header as incoming or outgoing in the log file.
 * Marking depends on the value of the flag provided.
 * Logs the fields of the IP Header in the Log file.
 * Calls necessary functions to convert the IP address in legible form.
 *
 * @param[in] hdr A struct formatted with apt name and sizes of fields in an
 * IP Header.
 * @param[in] incoming A flag which marks whether the header provided is
 * incoming or outgoing.
 */

void logIpHeader(IpHeader *hdr, uint8_t incoming) {
  char *text = calloc(SIZE, 1);

  if (incoming) {
    snprintf(text, SIZE, "Incoming IP Header:\n\n");   
  }

  else {
    snprintf(text, SIZE, "Outgoing IP Header:\n\n");
  }

  writeIpLog(text);

  snprintf(text, SIZE, "Header Length        : %"PRIu8"\n", hdr->headerLength);
  writeIpLog(text);

  snprintf(text, SIZE, "Version              : %"PRIu8"\n", hdr->version);
  writeIpLog(text);

  snprintf(text, SIZE, "tos                  : %"PRIu8"\n", hdr->tos);
  writeIpLog(text);

  snprintf(text, SIZE, "totalLength          : %"PRIu16"\n", hdr->totalLength);
  writeIpLog(text);

  snprintf(text, SIZE, "ID                   : %"PRIu16"\n", hdr->id);
  writeIpLog(text);

  snprintf(text, SIZE, "flags                : %"PRIu16"\n", hdr->flags);
  writeIpLog(text);

  snprintf(text, SIZE, "fragmentOffset       : %"PRIu16"\n", hdr->fragmentOffset);
  writeIpLog(text);

  snprintf(text, SIZE, "ttl                  : %"PRIu8"\n", hdr->ttl);
  writeIpLog(text);

  snprintf(text, SIZE, "protocol             : %"PRIu8"\n", hdr->protocol);
  writeIpLog(text);

  snprintf(text, SIZE, "checksum             : %"PRIu16"\n", hdr->checksum);
  writeIpLog(text);

  snprintf(text, SIZE, "Sender Address       : ");
  writeIpLog(text);
  logIpAddress(hdr->sourceAddress);

  snprintf(text, SIZE, "Destination Address  : ");
  writeIpLog(text);
  logIpAddress(hdr->destinationAddress);

  snprintf(text, SIZE, "\n-------------------------------------------------\n\n");
  writeIpLog(text);

  free(text);
}

