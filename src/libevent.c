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