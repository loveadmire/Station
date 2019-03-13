//
// Created by root on 1/9/19.
//

#ifndef STATION_THREAD_FUNC_H
#define STATION_THREAD_FUNC_H

#include "queue_my.h"

typedef struct _report_thread_info{
    long int invalid;
    long int valid;
    Queue *queue;
}r_thread_info_t,*p_r_thread_info_t;


void *thread_func_example(void *arg);

//add data in buffer head
int build_web_data(char *data, int len, char *buff);


#endif //STATION_THREAD_FUNC_H
