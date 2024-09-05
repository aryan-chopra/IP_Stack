#ifndef LOG_H
#define LOG_H

#include "arp.h"
#include "ethernet.h"
#include "ip.h"

void openLogFiles();
void log(arp_ipv4 *, int);
void log(EthernetHeader *, int);
void log(IpHeader *, int);

#endif

