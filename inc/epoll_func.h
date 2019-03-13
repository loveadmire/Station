//
// Created by root on 19-2-19.
//

#ifndef STATION_EPOLL_FUNC_H
#define STATION_EPOLL_FUNC_H

int main_epoll( unsigned short );

//设置socket为非阻塞
int set_sock_noblock(int sockfd);

#endif //STATION_EPOLL_FUNC_H
