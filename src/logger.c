#include <syslog.h>
#include "logger.h"

#ifndef SYSLOG_USE
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
#endif
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

void test_mylog()
{
    char str[] = "sdqweqweqew";
    int i = 23;
#ifndef SYSLOG_USE   //mylog

    PRINT_MYLOG(LOG_TYPE_ERROR," %s 1  %dd \r\n",str,i);
    PRINT_MYLOG(LOG_TYPE_FATAL," %s 2  %d  \r\n",str,i);
    PRINT_MYLOG(LOG_TYPE_WARN," %s  3 %d  \r\n",str,i);
    PRINT_MYLOG(LOG_TYPE_INFO," %s  4 %d  \r\n",str,i);
    PRINT_MYLOG(LOG_TYPE_DEBUG," %s 5 %d  \r\n",str,i);
#else //syslog
#include <syslog.h>
    PRINT_SYSLOG(LOG_EMERG,"%s 1 %d \n",str,i);
#endif
}
void test_PRINTF(void){
    char *str = "helloworld";
    PRINTF_MESSAGE(strlen(str),str,1,"Test01");
    char stt[] = "helloworld";
    PRINTF_MESSAGE(strlen(stt),stt,1,"Test02");
    PRINTF_FORMAT("%s %s %d %d ",str,stt,strlen(str),strlen(stt));
    PRINTF_FORMAT("%s %s %d %d ",str,stt,sizeof(str),sizeof(stt));
}
