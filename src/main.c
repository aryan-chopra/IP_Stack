/**
 * @file main.c
 * @author Aryan Chopra
 * @brief Entry point of the program, initializing various entities.
 *
 * Opens all the log files.
 * Initializes the network device(virtual/emulated) with IP and MAC address.
 * Initializes the TAP device to be used.
 * Receives ethernet frames over the network.
 * Calls various functions to handle the frame based on the type of request.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "arp.h"
#include "ethernet.h"
#include "icmp.h"
#include "ip.h"
#include "log.h"
#include "netdev.h"
#include "tap.h"

/*
 * @brief Handles the incoming frame.
 *
 *
 * Handles the incoming ethernet frame based on the type of payload.
 * Calls various functions designed to handle the ethernet packet.
 * Logs the incoming ethernet header for an ARP type payload.
 * Only ARP and IP type payloads are supported yet.
 *
 * @param[in] netdev A struct emulating a network device having an IP and a MAC address.
 * @param[in, out] header A struct having an apt structure using correct sizes to represent an ethernet header.
 * Various fields, such as source and destination address, underlying payload is modified depending on the type of request.
 */

void handleFrame(Netdev *netdev, EthernetHeader *header) {

  switch(header->payloadType) {
    case ETH_P_ARP:
      log(header, L_ETHERNET | L_INCOMING);
      incomingRequest(netdev, header);
      break;
    case ETH_P_IP:
      ipIncoming(netdev, header);
      break;
    default:
      return;
  }
}

/**
 * @brief Entry point of the program.
 *
 *
 * Opens the log files.
 * Initializes a TAP device, using a hardcoded name.
 * Initializes a vertual network device using hardcoded IP and MAC address.
 * Initializes the ARP cache.
 * Continually reads ethernet packets from the TAP device and initializes an ethernet header from it.
 * Handles every incoming frame.
 */

int main() {
  openLogFiles();

  Netdev netdev;

  char *name = calloc(20, 1);
  strcpy(name, "tap0");

  int tapDevice = initTap(name);

  initNetdev(&netdev, tapDevice, "10.0.0.4", "00:0c:29:6d:50:25");
  initArp();

  int size = 2500;
  char *buffer = malloc(size);

  while (1) {
    if (read(tapDevice, buffer, size) < 0) {
      printf("Error reading: %s\n", strerror(errno));
      exit(1);
    }

    EthernetHeader *header = initializeEthernet(buffer);	

    handleFrame(&netdev, header);
  }
}

