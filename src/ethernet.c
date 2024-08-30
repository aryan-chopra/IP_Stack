#include <arpa/inet.h>

#include "ethernet.h"

EthernetHeader* initializeEthernet(char *buffer) {
	EthernetHeader* header = (EthernetHeader *) buffer;
	
	header->payloadType = htons(header->payloadType);

	return header;
}

