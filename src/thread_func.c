//
// Created by root on 1/9/19.
//

#include <stdio.h>
#include <pthread.h>

#include "logger.h"
#include "thread_func.h"
#include "cJSON.h"
#include "cJSON_Utils.h"
#include "queue_my.h"

pthread_mutex_t thread_queue_lock;


int build_web_data(char *data, int len, char *buff)
{
    buff[0] = 0xff & (len >> 24);
    buff[1] = 0xff & (len >> 16);
    buff[2] = 0xff & (len >> 8);
    buff[3] = 0xff & (len >> 0);

    //int len = buffer[0] << 24 | buffer[1] << 16 | buffer[2] << 8 | buffer[3] << 0;

    if (len > 0)
    {
        memcpy(&buff[4], data, len);
    }
    return (4 + len);
}

void *thread_func_example(void *arg){
    pthread_mutex_lock(&thread_queue_lock);
    //pthread_detach(pthread_self());
    pthread_join(pthread_self(),NULL);

    r_thread_info_t *r_info;
    r_info = (r_thread_info_t *)arg;
    Queue *r_thread_queue;

    r_thread_queue = r_info->queue;
    pthread_mutex_lock(&thread_queue_lock);
    while(1) {
        printf("hello it is thread!\n");
    }
    pthread_mutex_unlock(&thread_queue_lock);

    return NULL;
}