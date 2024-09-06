/**
 * @file tun.c
 * @author Aryan Chopra
 * @brief Initializes the TUN/TAP device.
 *
 * Initializes the TUN/TAP device by opening the tun file
 * Initializes the TUN/TAP device as a TAP device.
 * Assigns IP address to the device and activates it's state.
 */

#include <errno.h>
#include <fcntl.h>
#include <linux/if_tun.h>
#include <net/if.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

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
 * @param[in, out] name A character array containing the name to be assigned
 * to the interface
 * In case the buffer is empty, the default name assigned is copied to the
 * buffer.
 * @return device An integer containing the file descriptor of the TUN/TAP
 * device.
 * @pre The "name" array has sufficient capacity to hold the default name,
 * if empty.
 */

int allocTap(char *name) {
	struct ifreq ifr;
	int device, err;

	device = open("/dev/net/tun", O_RDWR);
	if (device < 0) {
		printf("Error opening TUN/TAP: %s\n", strerror(errno));
		exit(1);
	}

	//Set ifr's memory to be zero
	memset(&ifr, 0, sizeof(ifr));

	ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
	if (*name) {
		strncpy(ifr.ifr_name, name, IFNAMSIZ);
	} 
  printf("Name not empty\n");

	err = ioctl(device, TUNSETIFF, (void *) &ifr);
	if (err < 0) {
		printf("Could not ioctl tun: %s\n", strerror(errno));
    exit(1);
	}

	strncpy(name, ifr.ifr_name, sizeof(ifr.ifr_name));
	return device;
}

/**
 * @brief Activates the TAP device and assigns an IP route to it.
 *
 *
 * Calls the appropriate function to create a TAP device.
 * Sets the device created as UP.
 * Assigns a hardcorded route to TAP device.
 *
 * @param[in, out] A character array containing the name of the device, used for configuring it.
 * Could be modified by the allocTap function if empty.
 * @return fd An integer containing the file descriptor of the TAP device.
 */

int initTap(char *name) {
  int	fd = allocTap(name);
	char *command = malloc(250);

	//Set interface up
	sprintf(command, "sudo ip link set dev %s up", name); 
	system(command);

	//Set interface route
	sprintf(command, "sudo ip route add dev %s 10.0.0.0/24", name);
	system(command);	 

  free(command);

	return fd;
}

