#ifndef THREADPOOL
#define THREADPOOL
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

struct job
{
    void *(*callbacl_function)(void *arg);
    void *arg;
    struct job *next;
};

struct threadpool
{
    int thread_num;
    int queue_max_num;
    struct job *head;
    struct job *tail;
    pthread_t *pthreads;     //save every thread tid by array
    pthread_mutex_t mutex;    //
    pthread_cond_t queue_empty;
    pthread_cond_t queue_not_empty;
    pthread_cond_t queue_not_full;
    int queue_cur_num;
    int queue_close;
    int pool_close;

};

//init thread pool
struct threadpool* threadpool_init(int thread_num,int queue_max);

//add a job to thread pool
int threadpool_add_job(struct threadpool *pool,void* (*callback_function)(void *arg),void *arg);

//destroy thread pool
int threadpool_destroy_it(struct threadpool *pool);

void *threadpool_function(void *arg);



#endif // THREADPOLL

