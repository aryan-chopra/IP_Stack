/**
 * @file tap.h
 * @author Aryan Chopra
 * @brief Contains the declarations of the functions used to initialize a
 * TAP/TUN device.
 */

#ifndef TAP_H
#define TAP_H

/**
 * @brief Allocates a TAP device.
 *
 * 
 * Opens the /dev/net/tun file therefore, creating a communication interface on kernel's internal network.
 * Writes the error to the console and exits the process in case of an error.
 * Initializes the device with the name provided.
 * If no name is provided, the default name provided by the kernel is used.
 * If no name is provided, the name provided by the kernel is copied to the
 * buffer provided.
 * Clears the configuration struct, to avoid any random ones(1).
 * Configures the interface as a TAP device.
 * IFF_NO_PI signifies that the version of IP protocol will be deduced from
 * IP version number in the packet.
 * Writes the error to the console, and exits the process if device's
 * configuration fails.
 *
 * @param[in, out] char * A character array containing the name to be assigned
 * to the interface
 * In case the buffer is empty, the default name assigned is copied to the
 * buffer.
 * @return int An integer containing the file descriptor of the TUN/TAP
 * device.
 * @pre The char array has sufficient capacity to hold the default name,
 * if empty.
 */

int initTap(char *);

#endif

