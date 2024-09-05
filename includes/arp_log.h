#ifndef ARP_LOG_H
#define ARP_LOG_H

void openArpLog();
void writeArpLog(char *);
void logArpMac(unsigned char [5]);
void logArpData(arp_ipv4 *);

#endif

