//
// Created by root on 1/5/19.
//

#include "socket_tcp_func.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>

#include "cJSON.h"
#include "cJSON_Utils.h"

#ifdef _WIN32
#else
#endif

#define NET_CON_TIMEOUT       5   //连接超时
#define NET_REC_TIMEOUT       5   //接收超时
//#define DEFAULT_THREADNUM     1
//#define DEFAULT_QUEUENUM      10

int File_write(char *file,char *buf,char *mode){
    if( !mode ||  !file || !buf ){
        return -1;
    }
    FILE  *fp = NULL;
    fp = fopen(file,mode);
    if(!fp){
        return -2;
    }
    int ret = 0;
    ret = fwrite(buf,strlen(buf),1,fp);
    if(ret < 0){
        return -3;
    }
    fclose(fp);
    return 0;
}

int Recode_port_test(int min_port,int max_port,int number,char *file){
    if(min_port > max_port){
        return -1;
    }
    cJSON *port_root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(port_root, "use_port",array);
    int calcute = 0;
    int i = 0;
    for(i = min_port;i < max_port;i++){
        struct sockaddr_in serveraddr;
        int sockfd,serverport,ret;
        sockfd = socket(AF_INET,SOCK_STREAM,0);
        bzero(&serveraddr,sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        //inet_pton(AF_INET,"127.0.0.1",&serveraddr.sin_addr);
        serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        serveraddr.sin_port = htons(i);
        if(bind(sockfd,(struct sockaddr *)&serveraddr,sizeof( struct sockaddr_in)) < 0)
        {
            //这个端口已经被占用，不加入配置文件；
            close(sockfd);
        }
        if(listen(sockfd,5) < 0){
            close(sockfd);
        }
        else{
            close(sockfd);
            calcute += 1;
            cJSON_AddNumberToObject(array,"port",i);
            if(calcute == number ){
                break;

            }
            //这个端口可用记录即可
        }

    }
    int ret = File_write(file,cJSON_Print(port_root),"w");
    cJSON_Delete(port_root);

}



int CreateConnect(char *IP,unsigned short port)
{
    struct sockaddr_in addr;
    int sock;

#ifdef WIN32
    WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 0);
	if(WSAStartup(wVersionRequested, &wsaData)!= 0)
	{
		return -1;
	}
#endif

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock != INVALID_SOCKET)
    {
        int  ret;
        unsigned long  optval;
        struct timeval tm;

        memset(&addr, 0, sizeof(struct sockaddr_in));
        addr.sin_family   = AF_INET;
        addr.sin_addr.s_addr = inet_addr(IP);
        addr.sin_port     = htons(port);

        optval = 1;
        ioctlsocket(sock, FIONBIO, &optval);

        ret = connect(sock, (struct sockaddr *)&addr, sizeof(addr));

        if(ret < 0)
        {
            int error = -1;
            fd_set set;
            socklen_t len;

#ifndef WIN32
            if (errno != EWOULDBLOCK && errno != EINPROGRESS)
            {
                closesocket(sock);
                return -1;
            }
#endif

            //设置连接超时
            tm.tv_sec  = NET_CON_TIMEOUT;
            tm.tv_usec = 0;
            FD_ZERO(&set);
            FD_SET(sock, &set);
            if(select(sock + 1, NULL, &set, NULL, &tm) > 0)
            {
                len = sizeof(int);
                getsockopt(sock, SOL_SOCKET, SO_ERROR, (char*)&error, &len);
                if(error != 0)
                {
                    closesocket(sock);
                    return -1;
                }
            }
            else
            {
                closesocket(sock);
                return -1;
            }
        }
        //设置SOCK为阻塞
        optval = 0;
        ioctlsocket(sock, FIONBIO, &optval);

        tm.tv_usec = 0;
#ifdef WIN32
        tm.tv_sec = m_iRecvTimeout * 1000;
#else
        tm.tv_sec = m_iRecvTimeout;
#endif
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&tm, sizeof(tm));
        setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&tm, sizeof(tm));
    }

    return sock;
}

int create_listen(char *IP,unsigned short port)
{
    struct sockaddr_in addr;
    int sock;

#ifdef WIN32
    WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 0);
	if(WSAStartup(wVersionRequested, &wsaData)!= 0)
	{
		return -1;
	}
#endif

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock != INVALID_SOCKET)
    {
        int  ret;

        socklen_t len;
        int optval;


        optval = 1;
        len = sizeof(optval);
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, len);

        memset(&addr, 0, sizeof(struct sockaddr_in));
        addr.sin_family   = AF_INET;
        addr.sin_addr.s_addr = inet_addr(IP);
        addr.sin_port     = htons(port);

        ret = bind(sock,(struct sockaddr *)&addr, sizeof(addr));
        if(ret == INVALID_SOCKET)
        {
            closesocket(sock);
            return -1;
        }

        ret = listen(sock, 8);
        if(ret==INVALID_SOCKET)
        {
            closesocket(sock);
            return -1;
        }
        return sock;

    }

    return -1;
}

int RecvData(int sock, void * pdata, int len)
{
    int ret, count;

    if(!pdata)
    {
        return -1;
    }

    count = 0;
    if(len == 0)
    {
        return 0;
    }

    while(len)
    {
        if((ret = recv(sock, (char*)pdata + count, len, 0)) <= 0)
        {
            return -1;
        }
        len  -= ret;
        count += ret;
    }
    return count;
}

int SendData(int sock, void * pdata, int len)
{

    int ret, count;

    if(!pdata)
    {
        return -1;
    }

    count = 0;
    if(len == 0)
    {
        return 0;
    }

    while(len > 0)
    {
        ret = send(sock, (char*)pdata + count, len, MSG_NOSIGNAL);
        if(ret > 0)
        {
            len -= ret;
            count += ret;
        }
        else
        {
            return -1;
        }
    }

    return count;
}

int SetKeepAlive(int sock)
{
    int   keepAlive  =   1;//设定KeepAlive
    int   keepIdle   =   5;//开始首次KeepAlive探测前的TCP空闭时间
    int   keepCount    = 3;//判定断开前的KeepAlive探测次数
    int   keepInterval = 5;//两次KeepAlive探测间的时间间隔

    setsockopt(sock,SOL_SOCKET,SO_KEEPALIVE,(char*)&keepAlive,sizeof(keepAlive));
    setsockopt(sock,SOL_TCP,TCP_KEEPIDLE,(char*)&keepIdle,sizeof(keepIdle));
    setsockopt(sock,SOL_TCP,TCP_KEEPINTVL,(char*)&keepInterval,sizeof(keepInterval));
    setsockopt(sock,SOL_TCP,TCP_KEEPCNT,(char*)&keepCount,sizeof(keepCount));

    return 1;
}

int AddListen(char * ip, unsigned short port)
{
    if(!ip || port == 0 || port == 0xFFFF)
    {
        return -1;
    }


    int listen_sock =  create_listen(ip, port);

    if(listen_sock < 0)
    {
        return -1;
    }

    return listen_sock;
}

void SetTimeout(int connectimeout, int recvtimeout, int sendtimeout)
{
    if(connectimeout == 0)
    {
        m_iConTimeout = NET_CON_TIMEOUT;
    }
    else
    {
        m_iConTimeout = connectimeout;
    }

    if(recvtimeout == 0)
    {
        m_iRecvTimeout = NET_REC_TIMEOUT;

    }
    else
    {
        m_iRecvTimeout = recvtimeout;
    }
}

int SetSockNoblock(int sockfd){
    int flags = fcntl(sockfd,F_GETFL,0);
    if(flags == -1){
        return -1;
    }
    flags = fcntl(sockfd,F_GETFL,flags | O_NONBLOCK);
    if (flags == -1){
        return -1;
    }
    return 0;
}

int Analysis_DNS(char *dns){
    char g_ssl_serverIp[32] = {0};
    struct hostent  *my_addr = NULL;
    my_addr = gethostbyname(dns);
    if (my_addr) {
        switch (my_addr->h_addrtype) {
            case AF_INET:
                inet_ntop(my_addr->h_addrtype, my_addr->h_addr_list[0], g_ssl_serverIp, sizeof(g_ssl_serverIp));
                printf("Get ip [%s ]  from [%s ]   \n", g_ssl_serverIp, dns);
                break;
            default:
                printf("unknown address type [ %s ]!\n", dns);
                return -33;
        }
    }
    else {
        printf("unknow address [%s] \n", dns);
        return -33;
    }
    return 0;

}