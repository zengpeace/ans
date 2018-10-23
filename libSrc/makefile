TARGET_SO = ./libans.so
TARGET_LIB = ./libans.a
PLATFORM = arm-arago-linux-gnueabi-
DEBUG_MODE = -g

CC = $(PLATFORM)gcc
XX = $(PLATFORM)g++
AR = $(PLATFORM)ar

EXTERN_MACRO = -DWEBRTC_LINUX -DWEBRTC_NS_FIXED -DWEBRTC_ARCH_ARM_V7
COMMON_FLAGS = -march=armv7-a -mtune=cortex-a8  -mcpu=cortex-a8 -Wall $(DEBUG_MODE)
C_COMMON_FLAGS = $(COMMON_FLAGS) -Wunused -Wsign-compare -Wunreachable-code -DDEBUG_SYS_INFO -fPIC -fstack-protector-all $(EXTERN_MACRO) 
CFLAGS = $(C_COMMON_FLAGS) -std=gnu99
CXXFLAGS = $(C_COMMON_FLAGS)
LDFLAGS = 
LIBS =

INC =	-I./ \
		-I./modules/audio_processing/ns/main/interface \
		-I./modules/audio_processing/ns/main/source \
		-I./modules/audio_processing/utility \
		-I./common_audio/signal_processing_library/main/interface \
		-I./common_audio/resampler/main/interface \
		-I./common_audio/vad/main/interface \
		-I./common_audio/vad/main/source
		

DIR = ./

CSRC = $(shell find $(DIR) -name "*.c")
CXXSRC = $(shell find $(DIR) -name "*.cc")

OBJS := $(CSRC:%.c=%.o) $(CXXSRC:%.cc=%.o) $(SSRC:%.S=%.o) $(SSRC:%.s=%.o)

so:$(TARGET_SO)
$(TARGET_SO): $(OBJS)
	rm -f $@
	$(XX) -shared -o $@ $(OBJS)
	cp $(TARGET_SO) ../lib
	cp $(TARGET_SO) /usr/local/nginx/html/
	../createHeadFile.sh

lib:$(TARGET_LIB)
$(TARGET_LIB): $(OBJS)
	$(AR) cr $@ $(OBJS)
	cp $(TARGET_LIB) ../lib
	cp $(TARGET_LIB) /usr/local/nginx/html/
	../createHeadFile.sh

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $(@D)/$(<F) -o $(@D)/$(@F)

%.o: %.cc
	$(XX) $(CXXFLAGS) $(INC) -c $(@D)/$(<F) -o $(@D)/$(@F)

clean:
	$(RM) $(TARGET_SO) $(TARGET_LIB) $(OBJS)
