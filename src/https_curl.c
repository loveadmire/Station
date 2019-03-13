//
// Created by root on 19-3-5.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <curl/curl.h>
#include "https_curl.h"

#define false 0
#define true 1

#define POSTURL   "https://blog.csdn.net/samzlb/article/details/80688906"
#define POSTFIELDS "email=myemail@163.com&password=mypassword&autologin=1&submit=登 录&type

size_t Process_data(void *buffer,size_t size,size_t nmemb,void *user_p){
    char bufR[4096]  = {0};
    char msg[1024] = {0};
    size_t return_size = 0;
    return_size = fwrite(buffer,size,nmemb,user_p);
    fflush(user_p);
    printf("%s>%d-%s\n", __func__, return_size, buffer);
    return (size*nmemb);
}

int Https_get_test(int number ,char *argv){
    CURLcode return_code;   //初始化curl
    char *pCaPath = NULL;
    return_code = curl_global_init(CURL_GLOBAL_ALL);
    if(CURLE_OK != return_code){
        printf("init libcurl failed!\n");
        return -1;
    }
    CURL *easy_handle = curl_easy_init();
    if(NULL == easy_handle){
        printf("Get easy handle failed \n");
        return -1;
    }
    struct curl_slist* headers = NULL;    //Curl 头部结构体
    headers = curl_slist_append(headers, "Accept:application/json");            //表示本地cURL接受报文为json
    headers = curl_slist_append(headers, "Content-Type:application/json");      //请求我们发送的报文为json，注意这里一定要说明自己发送的信息为JSON类型的，否则对方使用的应用层函数可能无法正确的识别解析
    headers = curl_slist_append(headers, "charset:utf-8");                      //表示我们发送的报文的编码格式为utf-8类型的格式
    curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, headers);            //给cURL结构体添加我们刚刚组成好的头部
    FILE *fp ;

    if(!(fp = fopen("data.html", "ab+")))
    {
        printf("fopen erro\n");
        return -1;
    }
    fseek(fp, 0 , SEEK_SET);
    // 设置easy handle属性
    curl_easy_setopt(easy_handle, CURLOPT_URL, POSTURL);
    //curl_easy_setopt(easy_handle,CURLOPT_POSTFIELDS,POSTFIELDS); //post参数
//    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, &Process_data);
//    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, fp);
//    curl_easy_setopt(easy_handle,CURLOPT_POST,1); //设置问非0表示本次操作为post
    //curl_easy_setopt(easy_handle,CURLOPT_VERBOSE,1); //打印调试信息
//    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 30); // 设置超时限制防止死循环
//    curl_easy_setopt(easy_handle,CURLOPT_HEADER,1); //将响应头信息和相应体一起传给write_data
//    curl_easy_setopt(easy_handle,CURLOPT_FOLLOWLOCATION,1); //设置为非0,响应头信息location
//    curl_easy_setopt(easy_handle,CURLOPT_COOKIEFILE,"curlposttest.txt");

    if(NULL == pCaPath)
    {
        curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYPEER, 0);//设定为不验证证书和HOST
        curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYHOST, 0);
    }
    else
    {
        curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYPEER, 1);
        curl_easy_setopt(easy_handle, CURLOPT_CAINFO, pCaPath);
    }

    // 执行数据请求
    printf("hello https---------------\n");
    while(1)
    {
        char testbuf[100];
        static testCnn = 0;

        testCnn++;
        snprintf(testbuf,sizeof(testbuf), "test cnn = %d", testCnn);
        curl_easy_setopt(easy_handle,CURLOPT_POSTFIELDS,testbuf); //post参数
        return_code = curl_easy_perform(easy_handle);
        if (return_code != CURLE_OK)
        {
            switch(return_code)
            {
                case CURLE_UNSUPPORTED_PROTOCOL:
                    fprintf(stderr,"不支持的协议,由URL的头部指定\n");
                case CURLE_COULDNT_CONNECT:
                    fprintf(stderr,"不能连接到remote主机或者代理\n");
                case CURLE_HTTP_RETURNED_ERROR:
                    fprintf(stderr,"http返回错误\n");
                case CURLE_READ_ERROR:
                    fprintf(stderr,"读本地文件错误\n");
                default:
                    fprintf(stderr,"返回值:%d\n",return_code);
            }
            return -1;
        }
        sleep(10);
    }
    // 释放资源
    sleep(10);
    fclose(fp);
    curl_easy_cleanup(easy_handle);
    curl_global_cleanup();


    return 0;

}