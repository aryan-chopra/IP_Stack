#ifndef PRINT_HEADERS_H
#define PRINT_HEADERS_H

#include "ethernet.h"
#include "ip.h"

void openFile();
void logToFileHeader(char *text);
void logEthernetPacket(EthernetHeader *, int);
void logIpHeader(IpHeader *, int);

#endif

