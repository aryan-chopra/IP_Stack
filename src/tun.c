#include <errno.h>
#include <fcntl.h>
#include <linux/if_tun.h>
#include <net/if.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

int tun_alloc(char *dev) {
	struct ifreq ifr;
	int device, err;

	device = open("/dev/net/tap", O_RDWR);
	if (device < 0) {
		printf("Error opening TUN/TAP: %s\n", strerror(errno));
		exit(1);
	}

	//Set ifr's memory to be zero
	memset(&ifr, 0, sizeof(ifr));

	ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
	if (*dev) {
		strncpy(ifr.ifr_name, dev, IFNAMSIZ);
	}

	err = ioctl(device, TUNSETIFF, (void *) &ifr);
	if (err < 0) {
		printf("Could not ioctl tun: %s\n", strerror(errno));
		close(device);
		return err;
	}

	strcpy(dev, ifr.ifr_name);
	return device;
}

int main() {
    char c;
    int dev = tun_alloc("tun1");
    printf("%d\n", dev);
    scanf("%c", &c);
}
