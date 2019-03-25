//
// Created by root on 19-3-22.
//

#ifndef STATION_C_FUN_POINTER_H
#define STATION_C_FUN_POINTER_H
#ifdef  __cplusplus
extern "C" {
#endif
//ec_pmeth.c gmssl
typedef  struct __tag_math_fun{
    int id;
    char name[50];
    void (*initial)();
    void (*process_in)(int id,char *name);
    void (*destroy)();
}math_fun,*Pmath_fun;

 void initial();

 void process_in(int id, char *name);

 void destroy();

#ifdef  __cplusplus
}
#endif
#endif //STATION_C_FUN_POINTER_H
