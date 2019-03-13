//
// Created by root on 1/22/19.
//
#include <stdio.h>
#include "Sort_func.h"

int ChangeAB(int *a,int *b){
    *a = *a ^ *b;
    *b = *b ^ *a;
    *a =  *a ^ *b;
    return 0;
}
void prin_bin(unsigned n){
    if(n)
        prin_bin(n/2);
    else
        return;
    printf("%d",n%2);
}

int  ChangeA_B(int a,int b){
    //异或操作满足结合律和交换律，
    a = a ^ b;
    b = b ^ a;   //b = b ^ a ^ b = b ^ b ^ a ; reason b ^ b = 0;  so 0 ^ a = a; b =a;
    a = a ^ b;

    printf(" %d %d \n",a,b);
}

int main_change(){
    int i = 0;
    int a= 12;
    int b = 23;
    ChangeA_B(a,b);
    printf(" %d %d \n",a,b);



    int *x = 13;
    int *y = 14;
    ChangeAB(x,y);
    printf("%d %d ",x,y);
    return 0;
}