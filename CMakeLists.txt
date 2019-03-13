cmake_minimum_required(VERSION 3.13)
project(Station C)


#add src file name to value DIRSRCS
AUX_SOURCE_DIRECTORY(./src  DIRSRCS)


INCLUDE_DIRECTORIES( ${catkin_INCLUDE_DIRS} ./inc ./inc/openssl  ./inc/crypto )



LINK_DIRECTORIES(${catkin_LIB_DIRS} ./lib )

FIND_LIBRARY(RUNTIME_LIB rt /usr/lib /usr/local/lib NO_DEFAULT_PATH)

set(CMAKE_C_STANDARD 99)

#
set(GNU_FLAGS "-m64 -static")
set(CMAKE_C_FLAGS "${GNU_FLAGS}")

ADD_EXECUTABLE(Station ${DIRSRCS} inc/String.h src/String.c)
#ADD_EXECUTABLE(Station ${catkin_SRC_DIRS} )
#add_executable(Station src/main.c src/libevent.c inc/libevent.h src/epoll_func.c inc/socket_tcp_func.h src/socket_tcp_func.c inc/cJSON.h  inc/cJSON_Utils.h src/cJSON.c src/cJSON_Utils.c)

TARGET_LINK_LIBRARIES(
     Station ${catkin_LIBRARIES}  curl ssl crypto m dl rt pthread event
)