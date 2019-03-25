//
// Created by root on 19-3-23.
//
#include "../../inc/algorithm/judge_num.h"

int Judge_num_isOdd(int num){
    if (num > 0)
        return num&1;
    else{
        return -1;
    }
}

int Judge_num_isPrime(int num){
    if (num <= 3){
        return num >1;
    }
    if (num %6 != 1 && num %6 != 5){
        return 0;
    }
    float ss = (float)sqrtf128(num);
    printf(" ss is %02f ",ss);
    int temp = (int)sqrtf(num);
    int i = 0;
    for(i = 5;i<= temp;i+=6){
        if(num%i == 0 || num%(i+2) == 0){
            return 0;
        }
    }
    return 1;
}