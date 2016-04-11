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
# Last update: Jan 30, 2016 release 4.2.4

INFO_MESSAGE = Using pre-compiled mbed distribution

# mbed specifics with pre-compiled library
# ----------------------------------
#
# libmbed.a available under mbed/build folder
#
#
CORE_LIB_PATH   = $(APPLICATION_PATH)/build/mbed

# Special files
#
MBED_A          = $(CORE_LIB_PATH)/$(LEVEL0)/$(TOOLCHAIN)/lib_eXmbed.a
STARTUP_O       = $(OBJDIR)/$(STARTUP).o
SYSTEM_O        = $(CORE_LIB_PATH)/$(LEVEL0)/$(TOOLCHAIN)/$(SYSTEM).o


# More options
#
MORE_CORTEX_INCLUDE    = TARGET_CORTEX_M
MORE_TARGET_EXCLUDE    = $(filter-out $(MORE_TARGET_INCLUDE),TARGET_M0 TARGET_M0P TARGET_M3 TARGET_M4)
MORE_TOOLCHAIN_EXCLUDE = $(filter-out $(MORE_TOOLCHAIN_INCLUDE),TOOLCHAIN_ARM TOOLCHAIN_GCC TOOLCHAIN_IAR)
MORE_CORTEX_EXCLUDE    = $(filter-out $(MORE_CORTEX_INCLUDE),TARGET_CORTEX_M TARGET_CORTEX_A)

# CORE files
#
#CORE_AS_SRCS      = $(CORE_LIB_PATH)/$(LEVEL0)/$(TOOLCHAIN)/$(STARTUP)
#CORE_AS1_SRCS_OBJ = $(CORE_AS_SRCS:.$(AS_EXTENSION)=.$(AS_EXTENSION).o)
#CORE_AS2_SRCS_OBJ = $(CORE_AS1_SRCS:.$(AS_EXTENSION)=.$(AS_EXTENSION).o)
#
#CORE_OBJ_FILES  = $(CORE_C_SRCS:.c=.c.o) $(CORE_CPP_SRCS:.cpp=.cpp.o) $(CORE_AS2_SRCS_OBJ)
#CORE_OBJS       = $(patsubst $(CORE_LIB_PATH)/%,$(OBJDIR)/%,$(CORE_OBJ_FILES))


# APPlication files
#
APP_LIB_PATH        = $(APPLICATION_PATH)/libraries
APP_LIBS_LOCK       = 1

ifeq ($(APP_LIBS_LIST),)
    $(error The option APP_LIBS_LIST = is not allowed for mbed. Please list the libraries.)
endif

ifneq ($(APP_LIBS_LIST),0)
    a101            = $(realpath $(sort $(foreach dir,$(APP_LIBS_LIST),$(shell find -L $(APP_LIB_PATH)/$(dir) -type d))))
    a102            = $(filter $(addprefix %/,$(MORE_CORTEX_EXCLUDE)),$(a101))
    a103            = $(filter-out $(addsuffix /%,$(a102)) $(a102),$(a101))
    a104            = $(filter $(addprefix %/,$(MORE_TARGET_EXCLUDE)),$(a101))
    a105            = $(filter-out $(addsuffix /%,$(a104)) $(a104),$(a103))
    a106            = $(filter-out $(addprefix %/,$(MORE_TOOLCHAIN_EXCLUDE)),$(a105))
    APP_LIBS_PATH   = $(filter-out $(addprefix %/,$(MORE_TARGET_EXCLUDE)),$(a106))

    APP_LIB_CPP_SRC = $(realpath $(sort $(foreach dir,$(APP_LIBS_PATH),$(wildcard $(dir)/*.cpp)))) # */
    APP_LIB_C_SRC   = $(realpath $(sort $(foreach dir,$(APP_LIBS_PATH),$(wildcard $(dir)/*.c)))) # */
    APP_LIB_AS_SRC  = $(realpath $(sort $(foreach dir,$(APP_LIBS_PATH),$(wildcard $(dir)/*.$(AS_EXTENSION))))) # */

    APP_LIB_OBJS    = $(patsubst $(APPLICATION_PATH)/%.cpp,$(OBJDIR)/%.cpp.o,$(APP_LIB_CPP_SRC))
    APP_LIB_OBJS   += $(patsubst $(APPLICATION_PATH)/%.c,$(OBJDIR)/%.c.o,$(APP_LIB_C_SRC))
    APP_LIB_OBJS   += $(patsubst $(APPLICATION_PATH)/%.$(AS_EXTENSION),$(OBJDIR)/%.$(AS_EXTENSION).o,$(APP_LIB_AS_SRC))
endif


# USER files
# Sketchbook/Libraries path
# wildcard required for ~ management
# ?ibraries required for libraries and Libraries
#
ifeq ($(MBED_PATH)/preferences.txt,)
    $(error Error: define sketchbook.path in preferences.txt first)
endif

ifeq ($(wildcard $(SKETCHBOOK_DIR)),)
    SKETCHBOOK_DIR = $(shell grep sketchbook.path $(MBED_PATH)/preferences.txt | cut -d = -f 2-)
endif

ifeq ($(wildcard $(SKETCHBOOK_DIR)),)
    $(error Error: sketchbook path not found ($(SKETCHBOOK_DIR)))
endif

USER_LIB_PATH  = $(wildcard $(SKETCHBOOK_DIR)/?ibraries)


# VARIANT files
#
#VARIANT_PATH = $(CORE_LIB_PATH)/targets/cmsis/$(FAMILY)/$(BOARD)/$(TOOLCHAIN)
ifneq ($(wildcard $(CORE_LIB_PATH)/cmsis/$(LEVEL1)/$(LEVEL2)/$(TOOLCHAIN)/*),) # */
    VARIANT_PATH = $(CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL2)/$(TOOLCHAIN)
else
    VARIANT_PATH = $(CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL2)/$(LEVEL3)/$(TOOLCHAIN)
endif


# Include paths
#
INCLUDE_PATH  = $(FAST_LIB_PATH)/api
INCLUDE_PATH += $(FAST_LIB_PATH)/common
INCLUDE_PATH += $(FAST_LIB_PATH)/hal
INCLUDE_PATH += $(FAST_LIB_PATH)/targets/cmsis
INCLUDE_PATH += $(FAST_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL2)
INCLUDE_PATH += $(FAST_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL2)/$(TOOLCHAIN)
INCLUDE_PATH += $(FAST_LIB_PATH)/targets/hal/$(LEVEL1)/$(LEVEL2)
ifneq ($(LEVEL3),)
    INCLUDE_PATH += $(FAST_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL2)/$(LEVEL3)
    INCLUDE_PATH += $(FAST_LIB_PATH)/targets/hal/$(LEVEL1)/$(LEVEL2)/$(LEVEL3)
    INCLUDE_PATH += $(FAST_LIB_PATH)/targets/hal/$(LEVEL1)/$(LEVEL3)
endif
INCLUDE_PATH += $(VARIANT_PATH)
INCLUDE_PATH += $(APP_LIBS)
INCLUDE_PATH += $(APP_LIBS_PATH)

ifneq ($(wildcard $(FAST_LIB_PATH)/cmsis/$(LEVEL1)/$(LEVEL2)/$(TOOLCHAIN)/*),) # */
    INCLUDE_PATH += $(FAST_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL2)/$(TOOLCHAIN)
else
    INCLUDE_PATH += $(FAST_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL2)/$(LEVEL3)/$(TOOLCHAIN)
endif

ifneq ($(wildcard $(FAST_LIB_PATH)/targets/hal/$(LEVEL1)/$(LEVEL2)/Lib),)
    INCLUDE_PATH += $(shell find $(FAST_LIB_PATH)/targets/hal/$(LEVEL1)/$(LEVEL2)/Lib -type d)
endif
# Local libraries paths to be added in step2.mk


# Flags for gcc, g++ and linker
# ----------------------------------
#
# Common CPPFLAGS for gcc, g++ and linker
#
CPPFLAGS     = -$(MCU_FLAG_NAME)=$(MCU) -mthumb -mlittle-endian -mthumb-interwork
CPPFLAGS    += $(OPTIMISATION) -ffunction-sections -fdata-sections -fno-builtin
CPPFLAGS    += $(FPU_OPTIONS)
CPPFLAGS    += $(addprefix -D, $(PLATFORM_TAG) $(LEVEL1) $(BUILD_OPTIONS) $(MORE_OPTIONS))
CPPFLAGS    += $(addprefix -I, $(INCLUDE_PATH))
# Local libraries paths to be added in step2.mk

# Specific CFLAGS for gcc only
# gcc uses CPPFLAGS and CFLAGS
#
CFLAGS        = -std=c99 -std=gnu99

# Specific CXXFLAGS for g++ only
# g++ uses CPPFLAGS and CXXFLAGS
#
CXXFLAGS      = -std=c++11 -U__STRICT_ANSI__ -fno-exceptions -fno-rtti

# Specific LDFLAGS for linker only
# linker uses CPPFLAGS and LDFLAGS
#
LDFLAGS       = --specs=nosys.specs -Wl,-gc-sections --specs=nano.specs -u _printf_float -u _scanf_float


OLD_CORE_LIB_PATH = $(APPLICATION_PATH)/libraries/mbed

ifneq ($(wildcard $(OLD_CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL2)/$(TOOLCHAIN)/$(LDSCRIPT)),)
    LDFLAGS      += -T$(OLD_CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL2)/$(TOOLCHAIN)/$(LDSCRIPT)
else ifneq ($(wildcard $(OLD_CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL2)/$(LEVEL3)/$(TOOLCHAIN)/$(LDSCRIPT)),)
    LDFLAGS      += -T$(OLD_CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL2)/$(LEVEL3)/$(TOOLCHAIN)/$(LDSCRIPT)
    INCLUDE_PATH += $(OLD_CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL2)/$(LEVEL3)/$(TOOLCHAIN)
else ifneq ($(wildcard $(OLD_CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL3)/$(TOOLCHAIN)/$(LDSCRIPT)),)
    LDFLAGS      += -T$(OLD_CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL3)/$(TOOLCHAIN)/$(LDSCRIPT)
    INCLUDE_PATH += $(OLD_CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL3)/$(TOOLCHAIN)
    INCLUDE_PATH += $(OLD_CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL3)
else
    $(error Not found $(LDSCRIPT))
endif

