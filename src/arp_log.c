/**
 * @file arp_log.c
 * @author Aryan Chopra
 * @brief This file logs ARP packets in a log file.
 *
 * An ARP data packet is passed to the log function.
 * The function logs the MAC and IP address of the sender and receiver in the log file.
 */

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "arp.h"
#include "arp_log.h"

#define SIZE 100

/**
 * Contains the file descriptor of the log file to write to.
 */

int arpLogFile;

/*
 * @brief This functions opens the file to log to, or prints an error and quits the process if unsuccessful.
 */

void openArpLog() {
  arpLogFile = open(LOG_LOCATION"arp_log.txt", O_WRONLY | O_APPEND);
  if (arpLogFile < 0) {
    printf("Error opening arp log file: %s\n", strerror(errno));
    exit(1);
  }
}

/**
 * @brief Writes to the log file, the character array provided.
 */

void writeArpLog(char *text) {
	write(arpLogFile, text, strlen(text)); 
}

/**
 * @brief Converts the provided address from binary, network notation(Big Endian) 
 * to the legible decimal notation.
 *
 *
 * The function is static to avoid linking errors with the logIpAddress function in a different file.
 * The function writes the formated string using snprintf to a buffer which is then logged to the log file.
 * inet_atoa requires in_addr struct which contains a single field equivalent to uint32_t.
 * The address provided is casted to the struct which is then used to covert the 
 * IP address in commonl understandable format.
 *
 * @param[in] address 32 bit unsigned int which contains the IP address in binary, Big Endian notation.
 */

static void logIpAddress(uint32_t address) {
  char *text = calloc(SIZE, 1);

  snprintf(text, SIZE, "%s\n", inet_ntoa(*((struct in_addr *)&address)));
        
  writeArpLog(text);

  free(text);
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
	unsigned char text[4];

	  for (int index = 0; index < 5; index++) {
	  snprintf(text, 4, "%02x:", mac[index]);
	  writeArpLog(text); 
	  }

	  snprintf(text, 3, "%02x", mac[5]);
	  writeArpLog(text);

	  snprintf(text, 3, "\n");
	  writeArpLog(text);
}

/**
 * @brief Parses the incoming Arp Data header and logs the fields to a file.
 *
 *
 * Logs the packet as incoming or outgoing based on the "incoming" flag.
 * Logs the various fields of the ARP Data Header.
 * Calls the corresponding functions for logging MAC and IP addresses in commonly legible format.
 *
 * @param[in] arpData A struct containing the IP and MAC address of the sender and receiver.
 * @param[in] incoming Flag which marks the packet as incoming or outgoing in the log.
 */

void logArpHeader(arp_ipv4 *arpData, uint8_t incoming) {
	char *text = (char *)calloc(SIZE, 1);

  if (incoming) {
    snprintf(text, SIZE, "Incoming ARP Header:\n\n");
  }

  else {
    snprintf(text, SIZE, "Outgoing ARP Header:\n\n");
  }

  writeArpLog(text);

	snprintf(text, SIZE, "Source IP         : "); 
	writeArpLog(text);
  logIpAddress(arpData->sourceIp);

	snprintf(text, SIZE, "Destination IP    : ");
	writeArpLog(text);
  logIpAddress(arpData->destinationIp);

	snprintf(text, SIZE, "Source MAC        : ");
	writeArpLog(text);
	logMacAddress(arpData->sourceMac);

	snprintf(text, SIZE, "Destination MAC   : ");
	writeArpLog(text);
	logMacAddress(arpData->destinationMac);

	sprintf(text, "\n-------------------------------------------------------------------------\n\n");
	writeArpLog(text);

  free(text);
}

