#include "arp_log.h"
#include "ethernet_log.h"
#include "ip_log.h"
#include "log.h"

void openLogFiles() {
  openArpLog();
  openIpLog();
  openEthernetLog();
}

void log(arp_ipv4 *arpData, int incoming) {
  logArpHeader(arpData, incoming);
}

void log(EthernetHeader *ethHeader, int incoming) {
  logEthernetHeader(ethHeader, incoming);
}

void log(IpHeader *ipHeader, int incoming) {
  logIpHeader(ipHeader, incoming);
}

