#!/bin/bash
i=0
sum = 0
# 此版本用于测试并发时的使用说明
# 
# $1 开始监听的本地起始端口
# $2 一个进程监听的端口数量
#  监听的端口为$1 到   $1+$2为止
#while [ $i -le 1 ]
#for  ((i = "$1" ;i < "$1+$2";i++));
#do
  #  echo "connection to server is $i "
    ./TLS_app $1 $2 & 
    sleep 1
#done
#   echo connection to server is $i 
