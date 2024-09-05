#ifndef LOG_H
#define LOG_H

#define L_INCOMING 0x01
#define L_ARP 0x02
#define L_ETHERNET 0x04
#define L_IP 0x08

#include "arp.h"
#include "ethernet.h"
#include "ip.h"

void openLogFiles();
void log(void *, uint8_t);

#endif

