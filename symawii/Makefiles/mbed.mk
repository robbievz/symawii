#
# embedXcode+
# ----------------------------------
# Embedded Computing on Xcode
#
# Copyright © Rei VILO, 2010-2016
# http://embedxcode.weebly.com
# All rights reserved
#
#
# Last update: Feb 08, 2016 release 4.2.4


include $(MAKEFILE_PATH)/About.mk

# mbed specifics with pre-compiled library
# ----------------------------------
#
# lib_eXmbed.a available under mbed/build folder
#
#
PLATFORM         := mbed
PLATFORM_TAG      = MBED EMBEDXCODE=$(RELEASE_NOW) $(GCC_PREPROCESSOR_DEFINITIONS)
APPLICATION_PATH := $(MBED_PATH)
BOARDS_TXT       := $(APPLICATION_PATH)/boards.txt
FAST_LIB_PATH     = $(APPLICATION_PATH)/libraries/mbed

ifeq ($(wildcard $(MBED_APP)/*),) # */
    $(error Error: mbed framework not found)
endif

APP_TOOLS_PATH  = $(EMBEDXCODE_APP)/gcc-arm-none-eabi-4_9-2015q2/bin

# Release check
#
PLATFORM_VERSION = $(shell cat $(APPLICATION_PATH)/version.txt)
#ifeq ($(shell if [[ '$(PLATFORM_VERSION)' -ge '$(MBED_SDK_RELEASE)' ]] ; then echo 1 ; else echo 0 ; fi ),0)
#    WARNING_MESSAGE = mbed SDK $(MBED_SDK_RELEASE) or later is required.
#endif

# Uploader and debug server
#
ifeq ($(UPLOADER),stlink)
    UPLOADER_EXEC   = st-flash --reset 
    UPLOADER_PATH   = /usr/local/bin
    UPLOADER_OPTS   = 0x8000000
    DEBUG_SERVER    = stlink
else
    UPLOADER        = cp
    UPLOADER_EXEC   = cp
    UPLOADER_OPTS   = -f
    DEBUG_SERVER    = openocd
#UPLOADER_RESET  =
endif


# Rules for making a c++ file from the main sketch (.pde)
#
PDEHEADER      = \\\#include \"mbed.h\"


# Common and specific flags for gcc, g++, linker, objcopy
#
MCU_FLAG_NAME   = mcpu


# Tool-chain names
#
CC          = $(APP_TOOLS_PATH)/arm-none-eabi-gcc
CXX         = $(APP_TOOLS_PATH)/arm-none-eabi-g++
AR          = $(APP_TOOLS_PATH)/arm-none-eabi-ar
OBJDUMP     = $(APP_TOOLS_PATH)/arm-none-eabi-objdump
OBJCOPY     = $(APP_TOOLS_PATH)/arm-none-eabi-objcopy
SIZE        = $(APP_TOOLS_PATH)/arm-none-eabi-size
NM          = $(APP_TOOLS_PATH)/arm-none-eabi-nm
GDB         = $(APP_TOOLS_PATH)/arm-none-eabi-gdb


# Specific OBJCOPYFLAGS for objcopy only
# objcopy uses OBJCOPYFLAGS only
#
ifneq ($(SOFTDEVICE),)
    SOFTDEVICE_HEX  = $(shell find $(FAST_LIB_PATH)/targets/hal/$(LEVEL1)/$(LEVEL2)/Lib/$(SOFTDEVICE) -name \*.hex)
    MERGE_EXEC      = srec_cat
    MERGE_PATH      = /usr/local/bin
    MERGE_OPTS      = -intel --line-length=44
    OBJCOPYFLAGS    = -Oihex
    TARGET_HEXBIN   = $(TARGET_HEX)
else
    OBJCOPYFLAGS    = -Obinary
    TARGET_HEXBIN   = $(TARGET_BIN)
endif

# Assembler extension
# mbed 92, 100 = .s
# mbed 104 = .S
#
ifeq ($(shell if [[ '$(MBED_SDK_RELEASE)' -gt '100' ]] ; then echo 1 ; else echo 0 ; fi ),1)
    AS_EXTENSION = S
else
    AS_EXTENSION = s
endif

# Optimisation
#
ifeq ($(MAKECMDGOALS),debug)
    OPTIMISATION    = -O0 -ggdb -DDEBUG
else
    OPTIMISATION    = -Os -DNDEBUG
endif


# Option for make
# . FAST with pre-compiled library
# . FULL build whole library
#
ifeq ($(wildcard $(APPLICATION_PATH)/build/mbed/$(LEVEL0)/$(TOOLCHAIN)/lib_eXmbed.a),)
    MBED_MAKE_OPTION := FULL
endif

MBED_MAKE_OPTION := FULL

ifeq ($(BOARD_TAG),K64F)
    ifeq ($(MBED_MAKE_OPTION),FAST)
        include $(MAKEFILE_PATH)/mbed_K64F_fast.mk
    else
        include $(MAKEFILE_PATH)/mbed_K64F_full.mk
    endif
else
    ifeq ($(MBED_MAKE_OPTION),FAST)
        include $(MAKEFILE_PATH)/mbed_fast.mk
    else
        include $(MAKEFILE_PATH)/mbed_full.mk
    endif
endif


# Commands
# ----------------------------------
#
#COMMAND_LINK = $(CXX) $(CPPFLAGS) $(LDFLAGS) $(LOCAL_OBJS) $(TARGET_A) $(MBED_A) $(OUT_PREPOSITION)$@ -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys

COMMAND_LINK = $(CXX) $(CPPFLAGS) $(LDFLAGS) $$(find Builds -name \*.o) $(OUT_PREPOSITION)$@ -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys


