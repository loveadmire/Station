//
// Created by root on 1/5/19.
//

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#ifndef _WIN32
#include <netinet/in.h>
# ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#include <sys/socket.h>
#endif

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/event_struct.h>

#include "libevent.h"
//向输出缓冲添加数据
//int bufferevent_write(struct bufferevent *bufev,const void *data, size_t size);
//int bufferevent_write_buffer(struct bufferevent *bufev, struct evbuffer *buf);
//向输入缓冲添加数据
//size_t bufferevent_read(struct bufferevent *bufev,void *data,size_t size);
//int bufferevent_read_buffer(struct bufferevent*bufev,struct evbuffer *buf);

//添加数据到evbuff中
//int evbuffer_add(struct evbuffer *buf, const void *data, size_t datlen);
//int evbuffer_add_printf(struct evbuffer *buf, const char *fmt, ...)

//两个函数都实现了类似C++中的移动语义，完成操作后src中的数据被转移到dst
//int evbuffer_add_buffer(struct evbuffer*dst,struct evbuffer*src);
//int evbuffer_remove_buffer(struct evbuffer *src,struct evbuffer*dst,size_t datlen);

//头部插入数据，返回新的buff,待测试
//int evbuffer_prepend(struct evbuffer*buf, const void *data,size_t size);
//int evbuffer_prepend_buffer(struct evbuffer *dst,struct evbuffer *src);

//unsigned char * evbuffer_pullup(struct evbuffer *buf, ev_ssize_t size);
////当size<0 将返回所有数据
//unsigned char * p = evbuffer_pullup(tmp,evbuffer_get_length(tmp));
//printf("%s\r\n",p);

//drain直接丢弃len长度的数据
//remove将数据拷贝到data中
//int evbuffer_drain(struct evbuffer *buf, size_t len);
//int evbuffer_remove(struct evbuffer *buf, void *data, size_t datlen);


static const char MESSAGE[]= "hello world\n";
static const int PORT = 9995;

int demo_func(int num,char *data){
    struct event_base *base;
    //base = event_base_new();
    //base add event listener signal;
    //event_base_dispatch(base);
    //event_base_free(base);
    return 0;
}