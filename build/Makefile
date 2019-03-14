
#***************************************************************************
#
#   Copyright (c) 2017 by colin hans.
#
#   Name        : libclp-test
#
#   Purpose     : 
#
#   Additional  : 
#
#--------------------------------------------------------------------------
#
#   Maintenance History:
#
#   2005-09-08-002: colin hans - 增加了对静态库编译的支持static=on;
#                                  增加了make选项release=on和debug=on分别对应Release和Debug版本的编译;
#   2005-09-08-001: colin hans - 创建
#
#**************************************************************************/

# 目标程序名称
TARGET := Station

# 需要链接的库路径，以空格隔开
LIB_PATHS := ../lib/

# 需要连接的库名，以空格隔开LIBS := 
LIBS := ssl crypto m  rt  pthread dl event curl
# 包含头文件所在目录INCLUDES := 

# 头文件所在目录INCLUDE_PATH := ../include/

INCLUDE_PATH :=  ../inc/openssl  ../inc

# 源码所在根目录SRC_PATH := ../src/
SRC_PATH := ../src/
$(warning, $(SRC_PATH) )
$(warning, $(INCLUDE_PATH) )
# 中间文件目录
OBJ_PATH := ../obj

# 输出文件目录
OUT_DIR := ../bin

CC := gcc
CFLAGS := -g -static -m64 -fPIC -Wall -O2 -MMD -DLINUX_PRG
LDFLAGS := 
CXXFLAGS = $(CFLAGS)

static=off
release=off
debug=off

BASE_TARGET := $(TARGET)
BASE_OBJ_PATH := $(OBJ_PATH)

ifneq ($(release), on)
 ifneq ($(debug), on)
  release=on
 endif
endif

ifeq ($(release), on)
 DEFINES += NDEBUG
 OBJ_PATH := $(OBJ_PATH)/release/
 ifeq ($(wildcard $(OBJ_PATH)),)
  $(shell mkdir $(OBJ_PATH))
 endif
else
 ifeq ($(debug), on)
  DEFINES += DEBUG
  CFLAGS += -g
  OBJ_PATH := $(OBJ_PATH)/debug/
  TARGET := $(TARGET)D
  ifeq ($(wildcard $(OBJ_PATH)),)
   $(shell mkdir $(OBJ_PATH))
  endif
 endif
endif

ifeq ($(static), on)
 TARGET := $(TARGET).a
endif
RM-F := rm -f -r 

SOURCE := $(shell find $(SRC_PATH) -name *.c)

OBJS := $(patsubst %.c,$(OBJ_PATH)%.o,$(notdir $(SOURCE)))

DEPS := $(patsubst %.o,%.d,$(OBJS)) 

MISSING_DEPS := $(filter-out $(wildcard $(DEPS)),$(DEPS))

MISSING_DEPS_SOURCES := $(wildcard $(patsubst %.d,%.c,$(MISSING_DEPS)))

.PHONY : default clean rebuild

default : $(OUT_DIR)$(TARGET) 

deps : $(DEPS) 

objs : $(OBJS) 

clean : 
	#$(RM-F) $(OBJ_PATH)/*.d
	#$(RM-F) $(OBJ_PATH)/*.o
	$(RM-F) $(BASE_OBJ_PATH)/release
	$(RM-F) $(BASE_OBJ_PATH)/debug
	$(RM-F) $(OUT_DIR)/$(TARGET)
	#$(RM-F) $(OUT_DIR)/$(BASE_TARGET)%.a
rebuild: clean all

ifneq ($(MISSING_DEPS),) 
$(MISSING_DEPS) :
	$(RM-F) $(patsubst %.d,%.o,$@)
endif 
-include $(DEPS)
$(OBJ_PATH)%.o:
	$(CC) $(addprefix -D, $(DEFINES)) $(addprefix -I,$(INCLUDES) $(INCLUDE_PATH)) -c $(CXXFLAGS) -o $@ $(filter $(subst .o,.c,%/$(notdir $@) ),$(SOURCE))
$(OUT_DIR)$(TARGET) : $(OBJS) 
 ifneq ($(static), on)
	$(CC) $(LDFLAGS) -o $(OUT_DIR)$(TARGET) $(OBJS) $(addprefix -L,$(LIB_PATHS)) $(addprefix -l,$(LIBS))	
 else
	ar rvs $(OUT_DIR)$(TARGET) $(OBJS)
 endif

