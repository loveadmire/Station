cmake_minimum_required(VERSION 3.13)
project(Station C)


#add src file name to value DIRSRCS   实现文件目录需要逐个添加
AUX_SOURCE_DIRECTORY(./src   DIRSRCS)
AUX_SOURCE_DIRECTORY(./src/algorithm   DIRSRCS)

#添加头文件目录
INCLUDE_DIRECTORIES( ${catkin_INCLUDE_DIRS} ./inc ./inc/openssl  ./inc/crypto ./inc/algorithm)


#添加库目录
LINK_DIRECTORIES(${catkin_LIB_DIRS} ./lib )

#添加系统库目录
FIND_LIBRARY(RUNTIME_LIB rt /usr/lib /usr/local/lib NO_DEFAULT_PATH)

set(CMAKE_C_STANDARD 99)

#
set(GNU_FLAGS "-m64 -static -fPIC -Wall -O2 -MMD -DLINUX_PRG")
set(CMAKE_C_FLAGS "${GNU_FLAGS}")

ADD_EXECUTABLE(Station ${DIRSRCS} )
#ADD_EXECUTABLE(Station ${catkin_SRC_DIRS} )
#add_executable(Station src/main.c src/libevent.c inc/libevent.h src/epoll_func.c inc/socket_tcp_func.h src/socket_tcp_func.c inc/cJSON.h  inc/cJSON_Utils.h src/cJSON.c src/cJSON_Utils.c)

TARGET_LINK_LIBRARIES(
     Station ${catkin_LIBRARIES}  curl ssl crypto m dl rt pthread event
)