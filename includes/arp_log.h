#ifndef ARP_LOG_H
#define ARP_LOG_H

#include "arp.h"
#include "log_location.h"

void openArpLog();
void writeArpLog(char *);
void logArpMac(unsigned char [5]);
void logArpHeader(arp_ipv4 *, int);

#endif

