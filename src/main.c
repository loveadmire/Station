#include <stdio.h>
#include <pthread.h>
#include "threadpool_ex.h"
#include "socket_tcp_func.h"
#include "Sort_func.h"
#include "AVLtree.h"
#include "double_list.h"
#include "threadpool.h"

#include <event2/event.h>
#include "searchtree.h"
#include "c_hash.h"
#include "epoll_func.h"
#include "gmssl_debug.h"
#include "cJSON_Utils.h"
#include "cJSON.h"

#include "Hand_pfx.h"
#include "https_curl.h"

#include "String.h"
#include "linklist.h"
#include "../inc/stack.h"

#define debug 1

void sys_err(const char *str){
    perror(str);
    exit(1);
}



int Recode_port_get(int serial,char *file_read) {
    int ret = 0;
    FILE *fp = NULL;
    if (!file_read) {
        return -1;
    }
    fp = fopen(file_read, "rb");
    if (!fp) {
        return -1;
    }
    fseek(fp, 0, SEEK_END);
    int file_len = ftell(fp);
    rewind(fp);
    char *buffer = NULL;
    buffer = (char *)malloc(sizeof(char)* file_len);
    fread(buffer, file_len, 1, fp);
    if (ret < 0) {
        fclose(fp);
        return -1;
    }
    fclose(fp);
    cJSON *root = cJSON_Parse(buffer);
    if (!root) {
        return -2;
    }
    cJSON *array = cJSON_GetObjectItem(root,"port");
    int array_size = 0;
    array_size = cJSON_GetArraySize(array);
    int sock = cJSON_GetArrayItem(array, serial)->valueint;
    if (sock > 0) {
        return sock;
    }
    else {
        return -1;
    }
}

struct threadpool *pool = NULL;

typedef struct __thread_arg{
    int first ;
    int second;
    char str1[32];
}THREADARG,*PTHREADARG;

int  tcp_connect_thread_callbake(void *arg){
    if(arg == NULL)
        printf("This function has no arguments\n");
    else {
        PTHREADARG function_arg = (PTHREADARG) arg;
        int port = function_arg->first;
        int send_data_flag = function_arg->second;
        char *ip = function_arg->str1;

        int sock = CreateConnect(ip, port);  //创建连接
        char buf_01[20] = {0};
        char *send_data = "12345678";
        while (sock > 0) {
            sleep(3);
            memset(buf_01, buf_01, sizeof(buf_01));
            if(send_data_flag){
                write(sock, send_data, strlen(send_data));
                int size = read(sock, buf_01, strlen(buf_01));
                if (size < 0) {
                    printf("client thread %d  exit %d ", pthread_self(),sock);
                }
                else{
                    printf("recv data len is %d \n",size);
                }
            }
        }
        if (sock < 0) {
            printf("connect to %d fail!\n",function_arg->first);
        }
        free(function_arg);
        function_arg = NULL;
        close(sock);
    }
    return 0;

}


void  *thread_callbake(void *arg){
    int ret = 0;
    tcp_connect_thread_callbake(arg);
    if(ret < 0){
        return -1;
    }
    return 0;
}

int Tcp_thread_test(int start_port,int port_number,int send_data_falg,char *ip_in){
    int g_start_port = 0;
    int g_port_num = 0;
    int send_data_tag = 0;

    g_start_port  = start_port;
    g_port_num    = port_number;
    send_data_tag = send_data_falg;

    char ip[32] = {0};
    if(ip){
        memcpy(ip,ip_in,strlen(ip_in));
    }
    else{
        memcpy(ip,"127.0.0.1",strlen("127.0.0.1"));
    }
    pool = threadpool_init(g_port_num,g_port_num);
    int i = 0;
    for( i =0;i < g_port_num;i++){

        PTHREADARG thread_arg = (PTHREADARG) malloc(sizeof(THREADARG));
        thread_arg->first = g_start_port + i;
        thread_arg->second = send_data_tag;
        memcpy(thread_arg->str1,ip,strlen(ip));
        sleep(1);
        threadpool_add_job(pool,thread_callbake,thread_arg);
    }
    while(1){
        sleep(1);
    }

}


int main(int argc,char *argv[]) {
    int ret = 0;
    if(argc == 1 ) {
        printf("Please input \" --help \" to get information\n");
        return 0;
    }
    if(argc >= 2){
        if(strcmp(argv[1],"--help") == 0){
            printf("epoll server      : program epoll_server port     (./Station epoll_server 8080)\n");
            printf("gmssl_client_demo : program gmssl_client port ip  (./Station gmssl_client 443 127.0.0.1)\n");
            printf("gmssl_server_demo : program gmssl_server port ip  (./Station gmssl_server 443 127.0.0.1)\n");
            printf("tcp_thread        : program tcp_thread port port_num flag ip  (./Station tcp_thread 10000 300 1  127.0.0.1)\n");
            return 0;
        }
        if(strcmp(argv[1],"epoll_server") == 0) {
            main_epoll((unsigned short) atoi(argv[2]));
        }
        if(strcmp(argv[1],"gmssl_client") == 0)
            gmssl_client_demo((unsigned short )atoi(argv[2]),argv[3]);
        if(strcmp(argv[1],"gmssl_server") == 0)
            gmssl_server_demo((unsigned short)atoi(argv[2]),argv[3]);
        if(strcmp(argv[1],"tcp_thread") == 0)
            Tcp_thread_test(atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),argv[5]);
        if(strcmp(argv[1],"AVLTREE") == 0){
            AVL_tree_test();
            searchtree_test();
        }
        if(strcmp(argv[1],"sort_fun") == 0){
            int a[] = {10,30,20,40,100,80,60,34};
            int a_len = sizeof(a)/sizeof(a[0]);
            quick_sort_int(a,0,a_len -1);
            int i = 0;
            for(i = 0;i<a_len;i++){
                printf(" %d ",a[i]);
            }
        }
        if(strcmp(argv[1],"testdlist") == 0){
            testdlist();
        }
        if(strcmp(argv[1],"libevent") == 0){
            struct  event_base *base = event_base_new();
            const char **buf;
            buf = event_get_supported_methods();
            int i = 0;
            for(i = 0;i< 10;i++){
                if(buf) {
                    printf("buf[%d] = %s \n", i, buf[i]);
                }
                else{
                    break;
                }
            }
            char *buff = NULL;
            buff = event_base_get_method(base);
            printf("buff si %s \n",buff);
        }
        if(strcmp(argv[1],"pfx") == 0){
            char *pri_sig = NULL ;        //私钥
            char *cert_sig = NULL;        //证书
            int FORMAT_PEM = 1;           //证书类型PEM
            int FORMAT_DER = 0;           //证书类型DER
            ret = PraseP12Cert("./sm2test015.pfx",NULL,cert_sig,pri_sig,FORMAT_DER);
            if(ret){
                printf("Analyse cert correct!\n");
            }
            else{
                printf("Cert error please debug it \n");
            }
        }
        if(strcmp(argv[1],"https") == 0){
            Https_get_test(3,NULL);
        }
        if(strcmp(argv[1],"string") ==  0){
           // char str[] = "98364358713249123461349814";
           char str[] = "abasjhdqjeddskdl";
            printf("src %s \n",str);
            ret = char_exchange(str,strlen(str));
            printf("exc %s \n",str);
            Insert_sort_string(str);
            printf("Insert_sort_string %s \n",str);
            Signal_list_test();
            STACK * stack = createStack(8);
            if (stack){
                printf("ok\n");
            }
        }
        if(strcmp(argv[1],"deabin") == 0) {
            int a = 2;
            printf("int is %d byte \n", sizeof(a));
            int i = sizeof(a);
            while (i) {
                printf(" the %d byte is %d \n", i, *(char *) (&a + i));
                i--;
            }
            union test {
                int a;
                char b[4];
            };
            union test test1;
            test1.a = 0x12345678;
            char byte[4] = {0};
            printf(" %d byte is byte4[]\n", sizeof(byte));
            printf("%p\n%p\t%p\n", &test1.a, &test1.b[0], &test1.b[1]);
            printf(" %x %x %x %x \n", test1.b[0], test1.b[1], test1.b[2], test1.b[3]);
            printf("test1.b = %d \n", test1.b);

            int c = 1;
            int *p = &c;
            printf("c is %08x *p = %d ,p = %08x p = %p  &(*p) = %08x\n", &c, *p, p, &p, &(*p));

            unsigned short int x = 12;
            int e = (x & 0xff00 >> 8);//| (x & 0x00ff << 8);
            printf("e is %x \n", e);
        }



    }
//    getchar();

    return 0;

}
