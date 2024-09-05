# IP Stack Implementation (ARP & ICMP)

This project implements core functionalities of the IP Stack, focusing on the Address Resolution Protocol (ARP) and the Internet Control Message Protocol (ICMP). These protocols are essential for network communication and are part of the Internet Protocol (IP) suite.

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)

## Overview

The project demonstrates the functionality of an IP stack by implementing ARP and ICMP. These protocols handle basic network communication operations:

- **ARP**: Resolves IP addresses to MAC addresses for local network communication.
- **ICMP**: Rsponds to the ICMP request of the 'ping' utility

## Features

- **ARP (Address Resolution Protocol)**
  - ARP request and reply handling.
  - Maintenance of an ARP cache to store mappings of IP to MAC addresses.
  
- **ICMP (Internet Control Message Protocol)**
  - ICMP Echo (Ping) request and reply.
  
## Installation

To get a local copy up and running, follow these steps.

### Steps

1. Clone the repository:
    ```bash
    git clone https://github.com/yourusername/ip-stack-implementation.git
    ```

2. Navigate to the project directory:
    ```bash
    cd IP_Stack
    ```

3. Build the project (if necessary):
    ```bash
    make
    ```

4. Run the project:
    ```bash
    ./main
    ```

## Usage
### ARP Implementation (ARP Reply)

The program responds to incoming ARP requests. When a user on the network sends an ARP request using the `arping` command, the program detects the request and responds with an ARP reply, providing the MAC address associated with its IP address.

#### Usage:

1. From a terminal, use the `arping` command targeting the interface device running this program:
    ```bash
    arping -I <interface_name> <program_host_ip>
    ```

    For example:
    ```bash
    ❯ arping -c4 -I tap0 10.0.0.4
    ```

2. The program running on `<program_host_ip>` will receive the ARP request and automatically send back an ARP reply with its MAC address.

3. The `arping` command on the sender’s machine will display the reply, similar to:
```bash
❯ arping -c4 -I tap0 10.0.0.4
ARPING 10.0.0.4 from 192.168.86.61 tap0
Unicast reply from 10.0.0.4 [00:0C:29:6D:50:25]  0.890ms
Unicast reply from 10.0.0.4 [00:0C:29:6D:50:25]  1.033ms
Unicast reply from 10.0.0.4 [00:0C:29:6D:50:25]  1.013ms
Unicast reply from 10.0.0.4 [00:0C:29:6D:50:25]  0.782ms
Sent 4 probes (1 broadcast(s))
Received 4 response(s)
```



### ICMP implementation
The program acts as a responder to incoming ICMP Echo Requests. When a user on the network sends a ping request using the standard Linux ping command, the program detects the request and responds with an appropriate ICMP Echo Reply.
From a terminal, use the ping command targeting the machine running this program:

```bash
ping <program_host_ip>
```

For example:
```bash
❯ ping -c3 10.0.0.4
```

Response:

```bash
❯ ping -c3 10.0.0.4
PING 10.0.0.4 (10.0.0.4) 56(84) bytes of data.
64 bytes from 10.0.0.4: icmp_seq=1 ttl=64 time=0.562 ms
64 bytes from 10.0.0.4: icmp_seq=2 ttl=64 time=0.400 ms
64 bytes from 10.0.0.4: icmp_seq=3 ttl=64 time=0.122 ms

--- 10.0.0.4 ping statistics ---
3 packets transmitted, 3 received, 0% packet loss, time 2030ms
rtt min/avg/max/mdev = 0.122/0.361/0.562/0.181 ms
```
