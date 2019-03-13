#!/bin/bash
# $1  起始连接的端口号
# $2  需要连接的端口数量，一个进程启动的线程数目与此一致
# $3  是否需要发送数据的标志，默认为0，可不传入，0，只建立连接，不发送数据，1，发送并接受数据
# $4  添加需要连接的ip地址，默认为localhost,可不传入
if [ $# = 0 ];then
	echo "Please read Station.sh to ues it "
fi
if [ $# = 2 ];then
	echo "create connect 127.0.0.1 "$1" to "$1" + "$2" port"
	./Station $1 $2 &
fi
if [ $# = 3 ];then
     echo "create connect 127.0.0.1 "$1" to "$1 + $2" port"
     ./Station $1 $2 $3 &
fi
if [ $# = 4 ];then
      echo "create connect "$3" "$1" to "$1 + $2" port"
     ./Station $1 $2 $3 $4
fi
