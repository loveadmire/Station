1. test.sh 为测试脚本
#!/bin/bash

i=0
             
sum=0

while [ $i -le 100 ]


(100 为次数减一的结果，，按照实际需要的并发数修改即可，测试时，可以先从小递增测试，100次则设置101；设置预测数字加一即可
 )     
  do
   
    let sum=sum+$i
     为测试脚本是否可以正常运行的案例，在使用时，可以在前加#屏蔽
    let i+=2
                为测试脚本是否可以正常运行的案例，在使用时，可以在前加#屏蔽
    echo $sum
        为测试脚本是否可以正常运行的案例，在使用时，可以在前加#屏蔽
    ./hello&

 #./hello&为实际要调用的，在你使用时，可更改为 ./TLS_app&
#echo $sum


2. hello 为测试程序，执行一句打印以及睡眠等待，模拟客户端一直在运行的这种状态

3. test.c 为测试程序源代码，可以在你放置这个程序的目录，直接执行
   gcc test.c -o  hello 生成相应的hello程序进行测试脚本是否可以正常运行