#include <errno.h>
#include <fcntl.h>
#include <linux/if_tun.h>
#include <net/if.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

static int fd;

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
		close(device);
		return err;
	}

	strncpy(name, ifr.ifr_name, sizeof(ifr.ifr_name));
	return device;
}

int writeTun(char *buffer, int length) {
	return write(fd, buffer, length);
}

int initTap(char *name) {
	fd = allocTap(name);
	char *command = malloc(250);

	//Set interface up
	sprintf(command, "sudo ip link set dev %s up", name); 
	system(command);

	//Set interface route
	sprintf(command, "sudo ip route add dev %s 10.0.0.0/24", name);
	system(command);	 

	return fd;
}

