/**
 * @file ethernet_log.c
 * @author Aryan Chopra
 * @brief Logs the ethernet packet in a log file.
 */

#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ethernet.h"
#include "ethernet_log.h"

#define SIZE 100

/**
 * @brief contains the file descriptor of the log file opened.
 */

int ethLogFile;

/**
 * @brief Opens the log file.
 *
 *
 * Opens the log file to log the ethernet headers, in append mode.
 * Prints an error to the consols and exits the process in case of an error.
 *
 * @pre Log file exists at the location.
 */

void openEthernetLog() {
  ethLogFile = open(LOG_LOCATION"ethernet_log.txt", O_WRONLY | O_APPEND);
  if (ethLogFile < 0) {
    printf("Error opening ethernet log: %s\n", strerror(errno));
    exit(1);
  }
}

/**
 * @brief Writes the provided character array to the log.
 *
 *
 * @pre File descriptor provided is valid.
 */

void writeEthLog(char *text) {
  write(ethLogFile, text, strlen(text));
}

/**
 * @brief Logs mac address to the log file
 *
 *
 * The function is static to prevent it's symbol being exported and clashin with similar function from other files.
 * Converts the given address to the commonly legible format and logs it to a file.
 *
 * @param[in] mac[6] a character array containing the mac address.
 */

static void logMacAddress(unsigned char mac[6]) {
  unsigned char logText[4];

  for (int index = 0; index < 5; index++) {
    snprintf(logText, 4, "%02x:", mac[index]);
    writeEthLog(logText); 
  }
  snprintf(logText, 3, "%02x", mac[5]);
  writeEthLog(logText);
  snprintf(logText, 3, "\n");
  writeEthLog(logText);
}

/**
 * @brief Logs the provided ethernet header.
 *
 *
 * Parses the provided ethernet header and logs the various fields.
 * Marks the packet as incoming or outgoing based on the flag provided
 * Calls the appropriate functions to format and log MAC address in legible format.
 *
 * @param[in] ethHeader Struct formated with apt names and sizes for various fields of an ethernet header.
 * @param[in] incoming Flag that marks the packet as incoming or outgoing
 */

void logEthernetHeader(EthernetHeader *ethHeader, uint8_t incoming) {
  char *text = calloc(SIZE, 1);

  if (incoming) {
    snprintf(text, SIZE, "Incoming Ethernet Header:\n\n");
  }

  else {
    snprintf(text, SIZE, "Outgoing Ethernet Header:\n\n");
  }

  writeEthLog(text);

  snprintf(text, SIZE, "Destination Mac          : ");
  writeEthLog(text);
  logMacAddress(ethHeader->destinationMac);

  snprintf(text, SIZE, "Source Mac               : ");
  writeEthLog(text);
  logMacAddress(ethHeader->sourceMac);

  snprintf(text, SIZE, "Payload type             : %"PRIu16"\n", ethHeader->payloadType);
  writeEthLog(text);

  snprintf(text, SIZE, "\n-------------------------------------------------\n\n");
  writeEthLog(text);

  free(text);
}

