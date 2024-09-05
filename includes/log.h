#ifndef LOG_H
#define LOG_H

#include "arp_log.h"
#include "ip_log.h"
#include "ethernet_log.h"

static inline void openLogs() {
  openArpLog();
  openIpLog();
  openEthernetLog();
}

#endif

