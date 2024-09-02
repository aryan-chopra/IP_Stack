main.out: src/main.c
	gcc -I./includes src/main.c src/arp.c src/netdev.c src/tun.c src/ethernet.c -o main
