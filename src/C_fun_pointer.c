//
// Created by root on 19-3-22.
//

#include "C_fun_pointer.h"
#include <stdio.h>
void initial(){
    printf("initialization...\n");
}

void process_in(int id, char *name)
{
    printf("process...\n%d\t%s\n",id, name);
}
void destroy()
{
    printf("destroy...\n");
}
