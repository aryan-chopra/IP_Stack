#ifndef ETHERNET_LOG_H
#define ETHERNET_LOG_H

#include "log_location.h"

void openEthernetLog();
void writeEtLog(char *);
void logEthernetHeader(EthernetHeader *, uint8_t);

#endif

