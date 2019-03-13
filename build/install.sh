#!/bin/sh
word_bit=`getconf WORD_BIT`
long_bit=`getconf LONG_BIT`
#echo " "$word_bit" "$long_bit" "
if [ $1 = "unstall" ];then
    echo "unstall"
     if [ ! -d "/usr/local/sslvpn/" ];then
        echo "dir not delete "
    else
        echo "dir has delete"
    fi
elif [ $1 = "install" ];then
    echo "install"
    if [ ! -d "/usr/local/sslvpn/" ];then
        echo "dir not exit "
    else
        echo "dir has exit"
    fi
else
    echo "unknow commd"
fi
if [ "$word_bit" -eq 32 ] && [ "$long_bit" -eq 64 ];
then
	echo "x86_64_bit system"
else
	echo "x86_i686 bit system"
fi
