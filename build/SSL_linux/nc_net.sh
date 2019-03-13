#!/bin/bash
i=0
sum=0
#带参数的，创建新的获取可用端口的文件
#./TLS_app create&
#killall -9 TLS_app

#while [ $i -le 1 ]
for  ((i = "$1" ;i < "$1+$2";i++));
do
  # let sum=sum+$i
  # let i+=2
  #  echo "connection to server is $i "
    nc 127.0.0.1 $i &
    sleep 1
done
#   echo connection to server is $i 
