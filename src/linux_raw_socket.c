#include "linux_raw_socket.h"
#include "logger.h"

static int sock;
typedef unsigned short int uint16;
typedef unsigned long  int uint32;

// 短整型大小端互换
#define BigLittleSwap16(A)  ((((uint16)(A) & 0xff00) >> 8) | \
                            (((uint16)(A) & 0x00ff) << 8))
 // 长整型大小端互换

#define BigLittleSwap32(A)  ((((uint32)(A) & 0xff000000) >> 24) | \
                            (((uint32)(A) & 0x00ff0000) >> 8) | \
                            (((uint32)(A) & 0x0000ff00) << 8) | \
                            (((uint32)(A) & 0x000000ff) << 24))



char *GetLocalMac()
{
    int sock_mac;
    char mac_addr[30];
    struct ifreq ifr_mac;
    sock_mac = socket(AF_INET,SOCK_STREAM,0);
    if(sock_mac == -1)
    {
        perror("socket");
        return "";
    }
    memset(&ifr_mac,0,sizeof(ifr_mac));
    strncpy(ifr_mac.ifr_name,NETDEV_SEND,sizeof(ifr_mac.ifr_name) -1);
    if((ioctl(sock_mac,SIOCGIFHWADDR,&ifr_mac)) < 0 )
    {
        perror("ioctl");
        return "";
    }
    sprintf(mac_addr,"%02x%02x%02x%02x%02x%02x",
               (unsigned char)ifr_mac.ifr_hwaddr.sa_data[0],
               (unsigned char)ifr_mac.ifr_hwaddr.sa_data[1],
               (unsigned char)ifr_mac.ifr_hwaddr.sa_data[2],
               (unsigned char)ifr_mac.ifr_hwaddr.sa_data[3],
               (unsigned char)ifr_mac.ifr_hwaddr.sa_data[4],
               (unsigned char)ifr_mac.ifr_hwaddr.sa_data[5]);

      // printf("local mac:%s \n",mac_addr);

       close( sock_mac );
       return (char *)( mac_addr );
}

void sig_handler(int sig)
{
    struct ifreq ethreq;
    if(sig == SIGTERM)
        printf("SIGTERM recieved, exiting.../n");
    else if(sig == SIGINT)
        printf("SIGINT recieved, exiting.../n");
    else if(sig == SIGQUIT)
        printf("SIGQUIT recieved, exiting.../n");
    // turn off the PROMISCOUS mode
    strncpy(ethreq.ifr_name,NETDEV_NAME, IFNAMSIZ);
    if(ioctl(sock, SIOCGIFFLAGS, &ethreq) != -1) {
        ethreq.ifr_flags &= ~IFF_PROMISC;
        ioctl(sock, SIOCSIFFLAGS, &ethreq);
    }
    close(sock);
    exit(0);
}

/*******************************************************
功能：
    校验和函数
参数：
    buf: 需要校验数据的首地址
    nword: 需要校验数据长度的一半
返回值：
    校验和
*******************************************************/

unsigned short checksum(unsigned short *buf,int nword)
{
   unsigned long sum;
   for(sum = 0;nword > 0;nword--)
   {
       sum += htons(*buf);
       buf++;
   }
   sum = (sum>>16) + (sum&0xffff);
   sum += (sum>>16);
   return ~sum;
}

int create_raw_sock()
{
    int n;   //check package whether right
    int ret = 0;
    char recv_buff[2048] = {0};
    unsigned char * ethhead;
    struct ifreq ethreq;
    struct sigaction signheandle;
    struct sockaddr_in cli_addr;
    int cli_addr_len = sizeof(struct sockaddr_in);

    // tcpdump -i ens33 '((udp) and (port 8080) and (host 192.168.11.133))' -dd
        struct sock_filter bpf_code[] = {
        { 0x28, 0, 0, 0x0000000c },
        { 0x15, 15, 0, 0x000086dd },
        { 0x15, 0, 14, 0x00000800 },
        { 0x30, 0, 0, 0x00000017 },
        { 0x15, 0, 12, 0x00000011 },
        { 0x28, 0, 0, 0x00000014 },
        { 0x45, 10, 0, 0x00001fff },
        { 0xb1, 0, 0, 0x0000000e },
        { 0x48, 0, 0, 0x0000000e },
        { 0x15, 2, 0, 0x00001f90 },
        { 0x48, 0, 0, 0x00000010 },
        { 0x15, 0, 5, 0x00001f90 },
        { 0x20, 0, 0, 0x0000001a },
        { 0x15, 2, 0, 0xc0a80b85 },
        { 0x20, 0, 0, 0x0000001e },
        { 0x15, 0, 1, 0xc0a80b85 },
        { 0x6, 0, 0, 0x00040000 },
        { 0x6, 0, 0, 0x00000000 }

        };
        struct sock_filter bpf01_code[] = {

       { 0x6, 0, 0, 0x0000ffff }

        };
    struct sock_fprog filter;
    filter.len = sizeof(bpf_code)/sizeof(bpf_code[0]);
    filter.filter = bpf_code;

    signheandle.sa_flags = 0;
    signheandle.sa_handler = sig_handler;
    sigemptyset(&signheandle.sa_mask);
    sigaction(SIGTERM,&signheandle,NULL);
    sigaction(SIGINT,&signheandle,NULL);
    sigaction(SIGQUIT,&signheandle,NULL);

    if((sock = socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL))) == -1)
    {
        PRINT_MYLOG(LOG_TYPE_ERROR,"socket fail");
    }
    // set Net I/O device to promiscous mode, so we can recieve all packets of the network
    strncpy(ethreq.ifr_name, NETDEV_NAME, IFNAMSIZ);
    if(ioctl(sock, SIOCGIFFLAGS, &ethreq) == -1) {
        perror("ioctl");
        close(sock);
        exit(1);
    }

    ethreq.ifr_flags |= IFF_PROMISC;
    if(ioctl(sock, SIOCSIFFLAGS, &ethreq) == -1) {
        perror("ioctl");
        close(sock);
        exit(1);
    }
    // attach the bpf filter
    if(setsockopt(sock, SOL_SOCKET, SO_ATTACH_FILTER, &filter, sizeof(filter)) == -1) {
        perror("setsockopt");
        close(sock);
        exit(1);
    }
    while(1) {
        n = recvfrom(sock, recv_buff, sizeof(recv_buff), 0, (struct sockaddr *)&cli_addr, &cli_addr_len);
        if(n < (ETH_HDR_LEN+IP_HDR_LEN+UDP_HDR_LEN)) {
            printf("invalid packet\n");
            continue;
        }


        ethhead = (struct ethhdr *)recv_buff;
        char *udp_data = &ethhead+32;
        //create udp  package to send
        //2.根据各种协议首部格式构建发送数据报
        unsigned char send_msg[1024] = {
            //--------------组MAC--------14------
            0x2c, 0x4d, 0x54, 0xc5, 0x47, 0xac, //dst_mac: 74-27-EA-B5-FF-D8
            0xc8, 0x9c, 0xdc, 0xb7, 0x0f, 0x19, //src_mac: c8:9c:dc:b7:0f:19
            0x08, 0x00,                         //类型：0x0800 IP协议
            //--------------组IP---------20------
            0x45, 0x00, 0x00, 0x00,             //版本号：4, 首部长度：20字节, TOS:0, --总长度--：
            0x00, 0x00, 0x00, 0x00,				//16位标识、3位标志、13位片偏移都设置0
            0x80, 17,   0x00, 0x00,				//TTL：128、协议：UDP（17）、16位首部校验和
            192,  168,   11,  133,				//src_ip: 10.221.20.11
            172,  16,    2,  44,					//dst_ip: 10.221.20.10
            //--------------组UDP--------8+78=86------
            0x1f, 0x90, 0x1f, 0x90,             //src_port:0x1f90(8080), dst_port:0x1f90(8080)
            0x00, 0x00, 0x00, 0x00,               //#--16位UDP长度--30个字节、#16位校验和
        };

        int len = sprintf(send_msg+42, "%s", udp_data);
        if(len % 2 == 1)//判断len是否为奇数
        {
            len++;//如果是奇数，len就应该加1(因为UDP的数据部分如果不为偶数需要用0填补)
        }

        *((unsigned short *)&send_msg[16]) = htons(20+8+len);//IP总长度 = 20 + 8 + len
        *((unsigned short *)&send_msg[14+20+4]) = htons(8+len);//udp总长度 = 8 + len
        //3.UDP伪头部
        unsigned char pseudo_head[1024] = {
            //------------UDP伪头部--------12--
            192,  168,   11,  133,				//src_ip: 10.221.20.11
            172,  16,   2,  44,					//dst_ip: 10.221.20.10
            0x00, 17,   0x00, 0x00,             	//0,17,#--16位UDP长度--20个字节
        };

        *((unsigned short *)&pseudo_head[10]) = htons(8 + len);//为头部中的udp长度（和真实udp长度是同一个值）
        //4.构建udp校验和需要的数据报 = udp伪头部 + udp数据报
        memcpy(pseudo_head+12, send_msg+34, 8+len);//--计算udp校验和时需要加上伪头部--
        //5.对IP首部进行校验
        *((unsigned short *)&send_msg[24]) = htons(checksum((unsigned short *)(send_msg+14),20/2));
        //6.--对UDP数据进行校验--
        *((unsigned short *)&send_msg[40]) = htons(checksum((unsigned short *)pseudo_head,(12+8+len)/2));

        if(ethhead[23] == IPPROTO_TCP)
            printf("[TCP]");
        else if(ethhead[23] == IPPROTO_UDP)
            printf("[UDP]");
        else if(ethhead[23] == IPPROTO_ICMP)
            printf("[ICMP]");
        else if(ethhead[23] == IPPROTO_IGMP)
            printf("[IGMP]");
        else
            printf("[OTHERS]");

        if(ethhead[23] == IPPROTO_UDP)
       {


           //create socketbuf
           int sock_raw_send_fd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
           if(sock_raw_send_fd < 0)
           {
               perror("socket");
               exit(-1);
               close(sock_raw_send_fd);
           }
           struct ifreq s_ethreq;
           strncpy(s_ethreq.ifr_name,NETDEV_SEND,IFNAMSIZ);
           ret =  ioctl(sock_raw_send_fd,SIOCGIFINDEX,&s_ethreq);
           if(ret == -1)
           {
               perror("ioctl");
               close(sock_raw_send_fd);
               exit(-1);
           }
           //sockaddr_ll 第三个成员赋值
           struct sockaddr_ll sll;
           bzero(&sll,sizeof(sll));
           sll.sll_ifindex = s_ethreq.ifr_ifindex;
           //sll.sll_addr = 0;
           int send_len = sizeof(send_msg);
           ret = sendto(sock_raw_send_fd,send_msg,14+20+8+len/*send_len*/,0,(struct sockaddr *)&sll,sizeof(sll));
           if(ret > 0)
               close(sock_raw_send_fd);
           else
           {
               PRINT_MYLOG(LOG_TYPE_ERROR,"send data fail");
               close(sock_raw_send_fd);
           }

           memset(send_msg,0,sizeof(send_msg));

         }
        close(sock);
    }
        return 0;

}







































