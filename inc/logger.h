#ifndef LOGGER_H
#define LOGGER_H
#include <string.h>
#include <stdio.h>

//choice mode for syslog
//#define SYSLOG_USE

#ifdef SYSLOG_USE
#include <sys/syscall.h>
#include <syslog.h>


//make log to syslog
extern const char* TypeToString_syslog(int type);
/*
const char* TypeToString_syslog(int type)
{
    switch (type) {
    case LOG_ERR:
        return "LOG_ERR    ";
    case LOG_DEBUG:
        return "LOG_DEBUG  ";
    case LOG_WARNING:
        return "LOG_WARNING";
    case LOG_NOTICE:
        return "LOG_NOTICE ";
    case LOG_ALERT:
        return "LOG_ALERT  ";
    case LOG_INFO:
        return "LOG_INFO   ";
    case LOG_EMERG:
        return "LOG_EMERG  ";
    default:
        break;

    }
}
*/


//change by every process 
#define LOG_TAG "TLS_SM2"
#if 0
#define PRINT_SYSLOG(type,format,...)\
{\
    do{\
       char msg[128]={0};\
       sprintf(msg,"%s {%s:%d} "format,TypeToString_syslog(type),__FILE__,__LINE__,##__VA_ARGS__);\
        printf("type is %s \n",TypeToString_syslog(type));\
       openlog(LOG_TAG, LOG_CONS | LOG_PID,0);\
       syslog(type, msg);\
       closelog();\
       }while(0);\
}


#else
#define PRINT_SYSLOG(type,format,...)\
{\
    do{\
       char msg[128]={0};\
       sprintf(msg,"%s {%s:%d} "format,TypeToString_syslog(type),__FILE__,__LINE__,##__VA_ARGS__);\      
       openlog(LOG_TAG, LOG_CONS | LOG_PID,0);\
       syslog(type, msg);\
       closelog();\
       }while(0);\
}
#endif
#else

//make log to other file
typedef enum Type{
    LOG_TYPE_FATAL,
    LOG_TYPE_ERROR,
    LOG_TYPE_WARN,
    LOG_TYPE_INFO,
    LOG_TYPE_DEBUG,
}Type;

extern const char* TypeToString_self(const Type type);
/*
const char* TypeToString_self ( const Type type )
{
    switch( type ) {
    case LOG_TYPE_FATAL:
        return "FATAL";
    case LOG_TYPE_ERROR:
        return "ERROR";
    case LOG_TYPE_WARN:
        return "WARN ";
    case LOG_TYPE_INFO:
        return "INFO ";
    case LOG_TYPE_DEBUG:
        return "DEBUG";
    default:
        break;
    }
    return "UNKWN";
}
*/
#define LOG_FILE "/var/log/test_process.log"           //log location
#define LOG_TAG "test_process"                      //log tag
/*
 *
printf(msg);\


*/
#define PRINT_MYLOG(type,format,...)\
{\
    do{\
       FILE *fp;\
       char msg[128] = {0};\
       fp = fopen(LOG_FILE,"a+");\
       sprintf(msg," %s : %s {%s:%d}  " format,LOG_TAG,TypeToString_self(type),__FILE__,__LINE__,##__VA_ARGS__);\
       fwrite(msg,1,strlen(msg),fp);\
       fclose(fp);\
       }while(0);\
}

#endif




#endif // LOGGER_H

