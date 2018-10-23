TARGET = ../texe

PLATFORM = arm-arago-linux-gnueabi-

CC = $(PLATFORM)gcc
XX = $(PLATFORM)g++
AR = $(PLATFORM)ar

EXTERN_MACRO = -DWEBRTC_LINUX -DWEBRTC_NS_FIXED -DWEBRTC_ARCH_ARM_V7
COMMON_FLAGS = -march=armv7-a -mtune=cortex-a8  -mcpu=cortex-a8 -Wall
C_COMMON_FLAGS = $(COMMON_FLAGS) -Wunused -Wsign-compare -Wunreachable-code -DDEBUG_SYS_INFO -fPIC -fstack-protector-all $(EXTERN_MACRO)
CFLAGS = $(C_COMMON_FLAGS) -std=gnu99
CXXFLAGS = $(C_COMMON_FLAGS)
LDFLAGS = -L../lib
#LIBS = -lans
LIBS = ../lib/libans.a

INC =	-I../inc \
		-I../inc/modules/audio_processing/ns/main/interface \
		-I../inc/modules/audio_processing/ns/main/source \
		-I../inc/modules/audio_processing/utility \
		-I../inc/common_audio/signal_processing_library/main/interface \
		-I../inc/common_audio/resampler/main/interface \
		-I../inc/common_audio/vad/main/interface \
		-I../inc/common_audio/vad/main/source \
		-I./test


DIR = ./

CSRC = $(shell find $(DIR) -name "*.c")
CXXSRC = $(shell find $(DIR) -name "*.cc")

OBJS := $(CSRC:%.c=%.o) $(CXXSRC:%.cc=%.o) $(SSRC:%.S=%.o) $(SSRC:%.s=%.o)

$(TARGET): $(OBJS)
	$(XX) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIBS)
	cp $(TARGET) /usr/local/nginx/html/

##$(TARGET): $(OBJS)
##	rm -f $@
##	$(XX) -shared -o $@ $(OBJS)
##	rm -f $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $(@D)/$(<F) -o $(@D)/$(@F)

%.o: %.cc
	$(XX) $(CXXFLAGS) $(INC) -c $(@D)/$(<F) -o $(@D)/$(@F)

clean:
	$(RM) $(TARGET) $(OBJS)


