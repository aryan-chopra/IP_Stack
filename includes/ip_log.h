#ifndef IP_LOG_H
#define IP_LOG_H

#include "ip.h"
#include "log_location.h"

void openIpLog();
void writeIpLog(char *);
void logIpHeader(IpHeader *, uint8_t);

#endif

