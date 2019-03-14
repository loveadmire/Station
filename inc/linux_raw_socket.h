#ifndef LINUX_RAW_SOCKET_H
#define LINUX_RAW_SOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <poll.h>
#include <linux/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/filter.h>
#include <net/ethernet.h>
#include <fcntl.h>
#include <linux/ip.h>
#include <netdb.h>
#include <netinet/in.h>
#include <net/if.h>
#include <pthread.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/icmp.h>
#include "queue.h"
#include "logger.h"

#define ETH_HDR_LEN 14
#define IP_HDR_LEN 20
#define UDP_HDR_LEN 8
#define TCP_HDR_LEN 20
#define NETDEV_NAME "ens33"
#define NETDEV_SEND "ens33"

#endif // LINUX_RAW_SOCKET_H

