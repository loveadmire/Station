//
// Created by root on 1/5/19.
//

#ifndef STATION_SOCKET_TCP_FUNC_H
#define STATION_SOCKET_TCP_FUNC_H

#ifdef WIN32      // windows system
#include <io.h>
#include <Winsock2.h>
#pragma comment(lib,"Ws2_32.lib")

#define MSG_NOSIGNAL 0
#define socklen_t int
#define CALLBACK   __stdcall
#define pthread_t  HANDLE

#else             // linux system
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <net/if.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>


#define ioctlsocket ioctl
#define closesocket close
//#define O_BINARY 0
#define INVALID_SOCKET (-1)

#endif

int m_iListenRunning;
int m_iConTimeout;
int m_iRecvTimeout;

int m_iThreadNum;
int m_iQueueNum;

void SetTimeout(int connectimeout, int recvtimeout, int sendtimeout);
int AddListen(char * ip, unsigned short port);
int CreateConnect(char *IP,unsigned short port);  //创建连接
int RecvData(int sock, void * pdata, int len);
int SendData(int sock, void * pdata, int len);
int create_listen(char *IP,unsigned short port);
int SetKeepAlive(int sock);

int File_write(char *,char *,char *);
int Recode_port_test(int ,int ,int ,char *);

//int initlistensocket(int ,int);
int SetSockNoblock(int sockfd);

int Analysis_DNS(char *);
#endif //STATION_SOCKET_TCP_FUNC_H
