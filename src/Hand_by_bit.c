//
// Created by root on 1/16/19.
//
#include <stdio.h>
#include <stdlib.h>

void f(unsigned n){
    if(n)
        f(n/2);
    else
        return;
    printf("%d",n%2);
}

unsigned getbit(unsigned x ,int p,int n){
    void f(x);
    return (x >> (p-n))& ~(~0 << n);
}
//将x中从第p位开始的n个（二进制）位设置为y中最右边n位的值，x的其余各位保持不变
unsigned setbits(unsigned x,int p,int n, unsigned y){
    return x & ~(~(~0 << n) << ( p - n )) | (y & ~(~ 0 << n)) << (p-n);
}
unsigned invert(unsigned x,int p,int n){
    return      x  ^   ~(~0<<n)  <<  (p-n)  ;
}

int bitsum(int x){
    int count = 0;
    while(x){
        count++;
        x = x&(x-1);
    }
    return count;
}

int my_strlen(const char *str){
    int count = 0;
    if(!str)
        return -1;
    while(*str++ != '\0'){
        count++;
    }
    return count;

}


int main_hand_by_bit(){
    char *str = "helloworld\0";
    printf(" str len is %d ",my_strlen(str));
    unsigned  int x = 175;
    unsigned  int y = 245;

    x = setbits(x,7,1,y);
    f(x);
    printf("  x 1 is %d \n",bitsum(x));
    printf(" x is %d \n",x);
    printf("\n");
    y = invert(y,4,4);
    f(y);
    printf("\n");
    printf("  y 1 is %d \n",bitsum(y));
    printf(" y is %d \n",y);


}