LOCAL_PATH := $(call my-dir)

#	litlsgateway
#####################################
include $(CLEAR_VARS)
LOCAL_MODULE    := androidTLS
#SRC_INC		:=/home/admin/fartec/ichange/otherprg/Tlsgateway/libSslJni/TLS_client/src/
SRC_INC		:=../TLS_client/src/
#LOCAL_C_INCLUDES += -I ../TLS_client/include/  ../TLS_client/crypto  ../TLS_client/ssl   ../TLS_client/src
LOCAL_C_INCLUDES +=../TLS_client/include/
LOCAL_C_INCLUDES +=../TLS_client/crypto
LOCAL_C_INCLUDES +=../TLS_client/ssl
LOCAL_C_INCLUDES +=../TLS_client/src
LOCAL_SRC_FILES := $(SRC_INC)/app_main.cpp $(SRC_INC)/cJSON.cpp $(SRC_INC)/cJSON_Utils.cpp $(SRC_INC)/tcpnet.cpp  $(SRC_INC)/threadpool.cpp $(SRC_INC)/sm4.cpp $(SRC_INC)/sms4.cpp

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    :=ssl
LOCAL_SRC_FILES :=./perbuild/libssl_static.a
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE    :=crypto
LOCAL_SRC_FILES :=./perbuild/libcrypto_static.a
include $(PREBUILT_STATIC_LIBRARY)



include $(CLEAR_VARS)

LOCAL_MODULE    :=tlsgateway
LOCAL_C_INCLUDES +=../TLS_client/src
LOCAL_C_INCLUDES +=../TLS_client/include/

LOCAL_SRC_FILES := com_hzlz_tlsgateway.cpp

LOCAL_LDLIBS	:=  -llog


LOCAL_STATIC_LIBRARIES  := androidTLS
LOCAL_STATIC_LIBRARIES  += ssl
LOCAL_STATIC_LIBRARIES  += crypto
include $(BUILD_SHARED_LIBRARY)


