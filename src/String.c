//
// Created by root on 19-3-6.
//

#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include "String.h"
#include "Sort_func.h"

/*-NO.1-----------------------------
 *  功能： 将一个字符串逆序
 *  说明 ： 这个函数只能将字符串逆序，不能逆序内存单元
 *-----------------------------*/

int char_exchange(char *str_in,int ll)
{
    char *str = NULL;
   // printf(" %p %p ",&str,&str_in);
    str = str_in;
   // printf(" %p %p ",str,str_in);
    int len,i,j;
    len = ll;
    for(i = 0,j=len-1;i!=len/2; i++,j--) {
        str[i] = str[i] ^ str[j];
        str[j] = str[j] ^ str[i];
        str[i] = str[i] ^ str[j];
    }
    return 0;
}
/* NO.2------------------------------
 * 功能：将传入的字符串按照ASCII码进行排序
 * 说明：算法部分暂且选用冒泡
/**************************************/

int  Sort_string(char *src){
    if(!src ){
        return -1;
    }
    return Bubble_sort_string(src);
}


/*-NO.n-----------------------------
 *  功能： 获得一个文件的大小,单位 byte
 *  说明 ： 传入的字符串为文件的绝对路径
 *			不能获得文件夹的大小
 *-----------------------------*/
long GetFileSize(char *path)
{
    struct stat buf;
    int iRet = stat(path,&buf);
    if(iRet == -1)
        return 0;
    return buf.st_size;
}