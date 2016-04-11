#
# embedXcode
# ----------------------------------
# Embedded Computing on Xcode
#
# Copyright © Rei VILO, 2010-2016
# http://embedxcode.weebly.com
# All rights reserved
#
#
# Last update: Dec 17, 2015 release 4.1.2

INFO_MESSAGE = Building mbed framework for K64F

# mbed # mbed Freescale K64F specifics
# ----------------------------------
#
# Download mbed SDK from https://github.com/mbedmicro/mbed
#
#
CORE_LIB_PATH   = $(APPLICATION_PATH)/libraries/mbed


# Special files
#
STARTUP_O       = $(OBJDIR)/$(STARTUP).o

# More options
#
MORE_CORTEX_INCLUDE    = TARGET_CORTEX_M
MORE_TARGET_EXCLUDE    = $(filter-out $(MORE_TARGET_INCLUDE),TARGET_M0 TARGET_M0P TARGET_M3 TARGET_M4)
MORE_TOOLCHAIN_EXCLUDE = $(filter-out $(MORE_TOOLCHAIN_INCLUDE),TOOLCHAIN_ARM TOOLCHAIN_GCC TOOLCHAIN_IAR)
MORE_CORTEX_EXCLUDE    = $(filter-out $(MORE_CORTEX_INCLUDE),TARGET_CORTEX_M TARGET_CORTEX_A)

ifneq ($(EXCLUDE_NAMES),)
    EXCLUDE_LIST   += $(addprefix %,$(EXCLUDE_NAMES).h)
    EXCLUDE_LIST   += $(addprefix %,$(EXCLUDE_NAMES).c)
    EXCLUDE_LIST   += $(addprefix %,$(EXCLUDE_NAMES).cpp)
#    EXCLUDE_LIST   += $(addprefix %,$(EXCLUDE_NAMES).S)
endif


# CORE files
#
BUILD_CORE_LIB_PATH   = $(CORE_LIB_PATH)/targets/hal/$(LEVEL1)/$(LEVEL2)

# Look for files inside folders
#
u101     = $(BUILD_CORE_LIB_PATH)
u101    += $(CORE_LIB_PATH)/common
u101    += $(CORE_LIB_PATH)/targets/hal/$(LEVEL1)/$(LEVEL2)
u101    += $(CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL2)/$(LEVEL4)
u101    += $(CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL4)

#ifneq ($(LEVEL3),)
#    u101   += $(CORE_LIB_PATH)/targets/hal/$(LEVEL1)/$(LEVEL2)/$(LEVEL3)
#    u101   += $(CORE_LIB_PATH)/targets/hal/$(LEVEL1)/$(LEVEL3)
#endif

# Look for all files inside folders and sub-folders
#
u101    += $(shell find $(CORE_LIB_PATH)/targets/hal/$(LEVEL1)/$(LEVEL2)/$(LEVEL3) -type d)
u101    += $(shell find $(CORE_LIB_PATH)/targets/hal/$(LEVEL1)/$(LEVEL2)/$(LEVEL4)/MK64F12 -type d)
u101    += $(shell find $(CORE_LIB_PATH)/targets/hal/$(LEVEL1)/$(LEVEL2)/$(LEVEL4)/TARGET_FRDM -type d)
u101    += $(shell find $(CORE_LIB_PATH)/targets/hal/$(LEVEL1)/$(LEVEL2)/$(LEVEL4)/device -type d)

u102                  = $(foreach dir,$(u101),$(wildcard $(dir)/*.c)) # */
BUILD_CORE_C_SRCS     = $(filter-out %/$(EXCLUDE_LIST),$(u102))
u103                  = $(foreach dir,$(u101),$(wildcard $(dir)/*.cpp)) # */
BUILD_CORE_CPP_SRCS   = $(filter-out %/$(EXCLUDE_LIST),$(u103))

BUILD_CORE_OBJ_FILES  = $(BUILD_CORE_C_SRCS:.c=.c.o) $(BUILD_CORE_CPP_SRCS:.cpp=.cpp.o)
BUILD_CORE_OBJS       = $(patsubst $(APPLICATION_PATH)/%,$(OBJDIR)/%,$(BUILD_CORE_OBJ_FILES))

#$(info >>> BUILD_CORE_C_SRCS $(BUILD_CORE_C_SRCS))
#$(info >>> BUILD_CORE_CPP_SRCS $(BUILD_CORE_CPP_SRCS))
#$(info >>> BUILD_CORE_OBJ_FILES $(BUILD_CORE_OBJ_FILES))


ifneq ($(wildcard $(CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL2)/$(TOOLCHAIN)/*.S),) # */
    CORE_AS_SRCS      = $(CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL2)/$(TOOLCHAIN)/$(STARTUP)
else ifneq ($(wildcard $(CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL2)/$(LEVEL3)/$(TOOLCHAIN)//*.S),) # */
    CORE_AS_SRCS      = $(CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL2)/$(LEVEL3)/$(TOOLCHAIN)/$(STARTUP)
else
    CORE_AS_SRCS      = $(CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL4)/$(TOOLCHAIN)/startup_MK64F12.S
endif

CORE_OBJ_FILES        = $(CORE_C_SRCS:.c=.c.o) $(CORE_CPP_SRCS:.cpp=.cpp.o) $(CORE_AS_SRCS:.S=.S.o)
CORE_OBJS             = $(patsubst $(APPLICATION_PATH)/%,$(OBJDIR)/%,$(CORE_OBJ_FILES))

$(info >>> CORE_AS_SRCS $(CORE_AS_SRCS))

# APPlication files
#
BUILD_APP_LIB_PATH    = $(CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL2)
u201                  = $(BUILD_APP_LIB_PATH)
ifneq ($(LEVEL3),)
    u201   += $(CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL2)/$(LEVEL3)
endif

u202                  = $(foreach dir,$(u201),$(wildcard $(dir)/*.c)) # */
BUILD_APP_C_SRCS      = $(filter-out %/$(EXCLUDE_LIST),$(u202))
u203                  = $(foreach dir,$(u201),$(wildcard $(dir)/*.cpp)) # */
BUILD_APP_CPP_SRCS    = $(filter-out %/$(EXCLUDE_LIST),$(u203))
BUILD_APP_LIBS_LIST   = $(subst $(BUILD_APP_LIB_PATH)/,,$(subst .c,,$(BUILD_APP_C_SRCS)) $(subst .cpp,,$(BUILD_APP_CPP_SRCS)))
BUILD_APP_LIBS        = $(BUILD_APP_LIB_PATH)

BUILD_APP_LIB_CPP_SRC = $(wildcard $(patsubst %,%/*.cpp,$(BUILD_APP_LIBS))) # */
BUILD_APP_LIB_C_SRC   = $(wildcard $(patsubst %,%/*.c,$(BUILD_APP_LIBS))) # */

BUILD_APP_LIB_OBJS    = $(patsubst $(APPLICATION_PATH)/%.cpp,$(OBJDIR)/%.cpp.o,$(BUILD_APP_CPP_SRCS))
BUILD_APP_LIB_OBJS   += $(patsubst $(APPLICATION_PATH)/%.c,$(OBJDIR)/%.c.o,$(BUILD_APP_C_SRCS))


# APPlication files
#
APP_LIB_PATH        = $(APPLICATION_PATH)/libraries
APP_LIBS_LOCK       = 1

ifeq ($(APP_LIBS_LIST),)
    $(error The option APP_LIBS_LIST = is not allowed for mbed. Please list the libraries.)
endif

ifneq ($(APP_LIBS_LIST),0)
    u301            = $(realpath $(sort $(foreach dir,$(APP_LIBS_LIST),$(shell find -L $(APP_LIB_PATH)/$(dir) -type d))))
    u302            = $(filter $(addprefix %/,$(MORE_CORTEX_EXCLUDE)),$(u301))
    u303            = $(filter-out $(addsuffix /%,$(u302)) $(u302),$(u301))
    u304            = $(filter $(addprefix %/,$(MORE_TARGET_EXCLUDE)),$(u301))
    u305            = $(filter-out $(addsuffix /%,$(u304)) $(u304),$(u303))
    u306            = $(filter-out $(addprefix %/,$(MORE_TOOLCHAIN_EXCLUDE)),$(u305))
    APP_LIBS_PATH   = $(filter-out $(addprefix %/,$(MORE_TARGET_EXCLUDE)),$(u306))

    APP_LIB_CPP_SRC = $(realpath $(sort $(foreach dir,$(APP_LIBS_PATH),$(wildcard $(dir)/*.cpp)))) # */
    APP_LIB_C_SRC   = $(realpath $(sort $(foreach dir,$(APP_LIBS_PATH),$(wildcard $(dir)/*.c)))) # */
    APP_LIB_AS_SRC  = $(realpath $(sort $(foreach dir,$(APP_LIBS_PATH),$(wildcard $(dir)/*.S)))) # */

    APP_LIB_OBJS    = $(patsubst $(APPLICATION_PATH)/%.cpp,$(OBJDIR)/%.cpp.o,$(APP_LIB_CPP_SRC))
    APP_LIB_OBJS   += $(patsubst $(APPLICATION_PATH)/%.c,$(OBJDIR)/%.c.o,$(APP_LIB_C_SRC))
    APP_LIB_OBJS   += $(patsubst $(APPLICATION_PATH)/%.S,$(OBJDIR)/%.S.o,$(APP_LIB_AS_SRC))
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
ifneq ($(wildcard $(CORE_LIB_PATH)/cmsis/$(LEVEL1)/$(LEVEL2)/$(TOOLCHAIN)/*),) # */
    VARIANT_PATH = $(CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL2)/$(TOOLCHAIN)
else
    VARIANT_PATH = $(CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL2)/$(LEVEL3)/$(TOOLCHAIN)
endif

VARIANT_AS_SRC  = $(wildcard $(VARIANT_PATH)/*.S) # */
VARIANT_OBJS    = $(patsubst $(APPLICATION_PATH)/%.S,$(OBJDIR)/%.S.o,$(VARIANT_AS_SRC)) # */


# SYSTEM files
#
SYSTEM_PATH = $(APPLICATION_PATH)/libraries/mbed/targets/hal/$(LEVEL1)/$(LEVEL2)
#SYSTEM_LIB  = $(call PARSE_BOARD,$(BOARD_TAG),build.variant_system_lib)
#SYSTEM_OBJS = $(SYSTEM_PATH)/$(SYSTEM_LIB)


# Include paths
#
INCLUDE_PATH  = $(u101)
INCLUDE_PATH += $(CORE_LIB_PATH)/api
INCLUDE_PATH += $(CORE_LIB_PATH)/common
INCLUDE_PATH += $(CORE_LIB_PATH)/hal
INCLUDE_PATH += $(CORE_LIB_PATH)/targets/cmsis
INCLUDE_PATH += $(CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL2)
INCLUDE_PATH += $(CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL2)/$(TOOLCHAIN)
INCLUDE_PATH += $(CORE_LIB_PATH)/targets/hal/$(LEVEL1)/$(LEVEL2)
ifneq ($(LEVEL3),)
    INCLUDE_PATH += $(CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL2)/$(LEVEL3)
    INCLUDE_PATH += $(CORE_LIB_PATH)/targets/hal/$(LEVEL1)/$(LEVEL2)/$(LEVEL3)
    INCLUDE_PATH += $(CORE_LIB_PATH)/targets/hal/$(LEVEL1)/$(LEVEL3)

    ifneq ($(LEVEL4),)
        INCLUDE_PATH += $(CORE_LIB_PATH)/targets/hal/$(LEVEL1)/$(LEVEL2)/$(LEVEL4)
        INCLUDE_PATH += $(CORE_LIB_PATH)/targets/hal/$(LEVEL1)/$(LEVEL2)/$(LEVEL3)/$(LEVEL4)
    endif
endif
INCLUDE_PATH += $(VARIANT_PATH)
INCLUDE_PATH += $(APP_LIBS)
INCLUDE_PATH += $(APP_LIBS_PATH)

ifneq ($(wildcard $(CORE_LIB_PATH)/targets/hal/$(LEVEL1)/$(LEVEL2)/Lib),)
    INCLUDE_PATH += $(shell find $(CORE_LIB_PATH)/targets/hal/$(LEVEL1)/$(LEVEL2)/Lib -type d)
endif
# Local libraries paths to be added in step2.mk

D_FLAGS      = MBED_BUILD_TIMESTAMP=$(shell date +%s) __MBED__=1


# Flags for gcc, g++ and linker
# ----------------------------------
#
# Common CPPFLAGS for gcc, g++ and linker
#

CPPFLAGS     = -$(MCU_FLAG_NAME)=$(MCU) -mthumb -mlittle-endian -mthumb-interwork
CPPFLAGS    += $(OPTIMISATION) -ffunction-sections -fdata-sections -fno-builtin
CPPFLAGS    += $(FPU_OPTIONS)
CPPFLAGS    += $(addprefix -D, $(PLATFORM_TAG) $(LEVEL1) $(BUILD_OPTIONS) $(MORE_OPTIONS) $(D_FLAGS))
CPPFLAGS    += $(addprefix -I, $(INCLUDE_PATH))
# Local libraries paths to be added in step2.mk

# Specific CFLAGS for gcc only
# gcc uses CPPFLAGS and CFLAGS
#
CFLAGS       = -std=c99 -std=gnu99

# Specific CXXFLAGS for g++ only
# g++ uses CPPFLAGS and CXXFLAGS
#
CXXFLAGS      = -std=c++11 -U__STRICT_ANSI__ -fno-exceptions -fno-rtti

# Specific LDFLAGS for linker only
# linker uses CPPFLAGS and LDFLAGS
#
LDFLAGS       = --specs=nosys.specs -Wl,-gc-sections -Wl,--wrap,main --specs=nano.specs -u _printf_float -u _scanf_float 

#$(info >>> CORE_LIB_PATH $(CORE_LIB_PATH))
#$(info >>> LEVEL1 $(LEVEL1))
#$(info >>> LEVEL2 $(LEVEL2))
#$(info >>> LEVEL3 $(LEVEL3))
#$(info >>> LEVEL4 $(LEVEL4))
#$(info >>> TOOLCHAIN $(TOOLCHAIN))
#$(info >>> LDSCRIPT $(LDSCRIPT))
#$(info >>> )
#$(info >>> INCLUDE_PATH $(INCLUDE_PATH))


ifneq ($(wildcard $(CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL2)/$(TOOLCHAIN)/$(LDSCRIPT)),)
    LDFLAGS      += -T$(CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL2)/$(TOOLCHAIN)/$(LDSCRIPT)
else ifneq ($(wildcard $(CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL2)/$(LEVEL3)/$(TOOLCHAIN)/$(LDSCRIPT)),)
    LDFLAGS      += -T$(CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL2)/$(LEVEL3)/$(TOOLCHAIN)/$(LDSCRIPT)
    INCLUDE_PATH += $(CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL2)/$(LEVEL3)/$(TOOLCHAIN)
else ifneq ($(wildcard $(CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL3)/$(TOOLCHAIN)/$(LDSCRIPT)),)
    LDFLAGS      += -T$(CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL3)/$(TOOLCHAIN)/$(LDSCRIPT)
    INCLUDE_PATH += $(CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL3)/$(TOOLCHAIN)
    INCLUDE_PATH += $(CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL3)
else ifneq ($(wildcard $(CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL0)/$(TOOLCHAIN)/$(LDSCRIPT)),)
    LDFLAGS      += -T$(CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL0)/$(TOOLCHAIN)/$(LDSCRIPT)
    INCLUDE_PATH += $(CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL0)/$(TOOLCHAIN)
    INCLUDE_PATH += $(CORE_LIB_PATH)/targets/cmsis/$(LEVEL1)/$(LEVEL0)
else
    $(error Not found $(LDSCRIPT))
endif


FIRST_O_IN_A     = $$(find . -name $(STARTUP).o)


# Commands
# ----------------------------------
#
EXTRA_COMMAND = @mkdir -p $(APPLICATION_PATH)/build/mbed/$(LEVEL0)/$(TOOLCHAIN) ; cp $(TARGET_A) $(APPLICATION_PATH)/build/mbed/$(LEVEL0)/$(TOOLCHAIN)/lib_eXmbed.a ; echo "--- lib_eXmbed.a generated ---"


