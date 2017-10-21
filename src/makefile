SHELL=/bin/sh

CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
LD = $(CROSS_COMPILE)ld

CFLAGS +=

LDFLAGS +=-L.

MAKE_DIR=$(PWD)

CFLAGS += $(INCLS)
CXXFLAGS += $(INCLS)
#find all the sub-directory
VPATH=$(foreach dir,$(shell find . -type d),$(shell echo $(dir):))
#include all the sub-directory
INCLS=$(foreach dir,$(subst :, ,$(VPATH)),$(shell echo -I$(dir)))
#find all .c and produce .o
C_SRC_PATH = $(foreach dir,$(subst :, ,$(VPATH)),$(wildcard $(dir)/*.c))
COBJS = $(subst .c,.o,$(C_SRC_PATH))
#find all .cxx and produce .o
CPP_SRC_PATH = $(foreach dir,$(subst :, ,$(VPATH)),$(wildcard $(dir)/*.cpp))
CPPOBJS = $(subst .cpp,.o,$(CPP_SRC_PATH))

execobjs = $(COBJS) $(CPPOBJS)
#the scripts for compile
.PHONY: all

all: $(execobjs)
	$(CXX) -lstdc++ -o application $(execobjs) $(LDFLAGS)

$(COBJS):%.o:%.c
	$(CC) -c -lstdc $< -o $@ $(CFLAGS)

$(CPPOBJS):%.o:%.cpp
	$(CC) -c $< -o $@ $(CXXFLAGS)
#the scripts for clean
.PHONY: clean

clean:
	rm application $(execobjs)