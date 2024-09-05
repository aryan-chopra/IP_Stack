main.out: src/main.c
	gcc -I./includes src/main.c src/arp.c src/netdev.c src/tun.c src/ethernet.c src/ip.c src/icmp.c src/ip_log.c src/ethernet_log.c src/arp_log.c -o main
