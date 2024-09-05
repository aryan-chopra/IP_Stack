#ifndef IP_LOG_H
#define IP_LOG_H

void openIpLog();
void writeIpLog(char *);
void logIpMac(unsigned char [6]);
void logIpHeader(IpHeader *, int);

#endif

