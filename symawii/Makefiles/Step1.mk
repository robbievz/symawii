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
# Last update: Mar 21, 2016 release 4.4.1






# Sketch unicity test and extension
# ----------------------------------
#
ifndef SKETCH_EXTENSION
    ifeq ($(words $(wildcard *.pde) $(wildcard *.ino)), 0)
        $(error No pde or ino sketch)
    endif

    ifneq ($(words $(wildcard *.pde) $(wildcard *.ino)), 1)
        $(error More than 1 pde or ino sketch)
    endif

    ifneq ($(wildcard *.pde),)
        SKETCH_EXTENSION := pde
    else ifneq ($(wildcard *.ino),)
        SKETCH_EXTENSION := ino
    else
        $(error Extension error)
    endif
endif

ifneq ($(MULTI_INO),1)
ifneq ($(SKETCH_EXTENSION),__main_cpp_only__)
    ifneq ($(SKETCH_EXTENSION),_main_cpp_only_)
        ifneq ($(SKETCH_EXTENSION),cpp)
            ifeq ($(words $(wildcard *.$(SKETCH_EXTENSION))), 0)
                $(error No $(SKETCH_EXTENSION) sketch)
            endif

            ifneq ($(words $(wildcard *.$(SKETCH_EXTENSION))), 1)
                $(error More than one $(SKETCH_EXTENSION) sketch)
            endif
        endif
    endif
endif
endif


# Board selection
# ----------------------------------
# Board specifics defined in .xconfig file
# BOARD_TAG and AVRDUDE_PORT 
#
ifneq ($(MAKECMDGOALS),boards)
    ifneq ($(MAKECMDGOALS),clean)
        ifndef BOARD_TAG
            $(error BOARD_TAG not defined)
        endif
    endif
endif

ifndef BOARD_PORT
    BOARD_PORT = /dev/tty.usb*
endif


# Path to applications folder
#
# $(HOME) same as $(wildcard ~)
# $(USER_PATH)/Library same as $(USER_LIBRARY_DIR)
#
USER_PATH      := $(HOME)
EMBEDXCODE_APP  = $(USER_LIBRARY_DIR)/embedXcode
PARAMETERS_TXT  = $(EMBEDXCODE_APP)/parameters.txt

# ~
ifndef APPLICATIONS_PATH
    ifneq ($(wildcard $(PARAMETERS_TXT)),)
        ap1 = $(shell grep ^applications.path '$(PARAMETERS_TXT)' | cut -d = -f 2-;)
        ifneq ($(ap1),)
            APPLICATIONS_PATH = $(ap1)
        endif
    endif
endif
# ~~

ifndef APPLICATIONS_PATH
    APPLICATIONS_PATH = /Applications
endif


# APPlications full paths
# ----------------------------------
#
# Welcome dual releases 1.6.5 and 1.7 with new and fresh nightmares!
# Arduino, ArduinoCC and ArduinoORG
#
ifneq ($(wildcard $(APPLICATIONS_PATH)/Arduino.app),)
    ARDUINO_APP   := $(APPLICATIONS_PATH)/Arduino.app
else ifneq ($(wildcard $(APPLICATIONS_PATH)/ArduinoCC.app),)
    ARDUINO_APP   := $(APPLICATIONS_PATH)/ArduinoCC.app
else ifneq ($(wildcard $(APPLICATIONS_PATH)/ArduinoORG.app),)
    ARDUINO_APP   := $(APPLICATIONS_PATH)/ArduinoORG.app
endif

# Arduino.app or ArduinoCC.app or Genuino.app by Arduino.CC or Genuino
#
ifneq ($(wildcard $(APPLICATIONS_PATH)/Arduino.app),)
    ifneq ($(shell grep -e '$(ARDUINO_CC_RELEASE)' $(APPLICATIONS_PATH)/Arduino.app/Contents/Java/lib/version.txt),)
        ARDUINO_CC_APP = $(APPLICATIONS_PATH)/Arduino.app
    endif
endif

ifeq ($(ARDUINO_CC_APP),)
    ifneq ($(wildcard $(APPLICATIONS_PATH)/ArduinoCC.app),)
        ifneq ($(shell grep -e '$(ARDUINO_CC_RELEASE)' $(APPLICATIONS_PATH)/ArduinoCC.app/Contents/Java/lib/version.txt),)
            ARDUINO_CC_APP = $(APPLICATIONS_PATH)/ArduinoCC.app
        endif
    endif
endif

ifeq ($(ARDUINO_CC_APP),)
    ifneq ($(wildcard $(APPLICATIONS_PATH)/Genuino.app),)
        ifneq ($(shell grep -e '$(ARDUINO_CC_RELEASE)' $(APPLICATIONS_PATH)/Genuino.app/Contents/Java/lib/version.txt),)
            ARDUINO_CC_APP = $(APPLICATIONS_PATH)/Genuino.app
        endif
    endif
endif

# Arduino.app by Arduino.ORG
#
ifneq ($(wildcard $(APPLICATIONS_PATH)/Arduino.app),)
    ifneq ($(shell grep -e '$(ARDUINO_ORG_RELEASE)' $(APPLICATIONS_PATH)/Arduino.app/Contents/Java/lib/version.txt),)
        ARDUINO_ORG_APP = $(APPLICATIONS_PATH)/Arduino.app
    endif
endif

ifeq ($(ARDUINO_ORG_APP),)
    ifneq ($(wildcard $(APPLICATIONS_PATH)/ArduinoORG.app),)
        ifneq ($(shell grep -e '$(ARDUINO_ORG_RELEASE)' $(APPLICATIONS_PATH)/ArduinoORG.app/Contents/Java/lib/version.txt),)
            ARDUINO_ORG_APP   := $(APPLICATIONS_PATH)/ArduinoORG.app
        endif
    endif
endif

# Arduino.CC and Arduino.ORG
#
ARDUINO_PATH        := $(ARDUINO_APP)/Contents/Java
ARDUINO_CC_PATH     := $(ARDUINO_CC_APP)/Contents/Java
ARDUINO_ORG_PATH    := $(ARDUINO_ORG_APP)/Contents/Java
ARDUINO_ORG_AVR_BOARDS  = $(ARDUINO_ORG_PATH)/hardware/arduino/avr/boards.txt
ARDUINO_ORG_SAM_BOARDS  = $(ARDUINO_ORG_PATH)/hardware/arduino/sam/boards.txt
ARDUINO_ORG_SAMD_BOARDS = $(ARDUINO_ORG_PATH)/hardware/arduino/samd/boards.txt

## Only ArduinoORG IDE supports Arduino M0 Pro
##
#ifneq ($(wildcard $(ARDUINO_APP)/Contents/Java/hardware/arduino/samd),)
#	ARDUINO_ORG_APP := $(ARDUINO_APP)
#else
#    ARDUINO_ORG_APP := $(APPLICATIONS_PATH)/ArduinoORG.app
#endif

# Other IDEs
#
WIRING_APP    = $(APPLICATIONS_PATH)/Wiring.app
ENERGIA_APP   = $(APPLICATIONS_PATH)/Energia.app
MAPLE_APP     = $(APPLICATIONS_PATH)/MapleIDE.app
MBED_APP      = $(EMBEDXCODE_APP)/mbed-$(MBED_SDK_RELEASE)
# ~
EDISON_YOCTO_APP     = $(EMBEDXCODE_APP)/EdisonYocto
BEAGKE_DEBIAN_APP    = $(EMBEDXCODE_APP)/BeagleBone
ROBOTIS_APP          = $(APPLICATIONS_PATH)/ROBOTIS_OpenCM.app
# ~~

include $(MAKEFILE_PATH)/About.mk
RELEASE_NOW = $(shell echo $(EMBEDXCODE_RELEASE) | sed 's/\.//g')


# Additional boards for ArduinoCC 1.6.5 Boards Manager
# ----------------------------------
# Arduino.app or ArduinoCC.app or Genuino.app by Arduino.CC or Genuino
# Only if ARDUINO_CC_APP exists
#

ifneq ($(ARDUINO_CC_APP),)

PACKAGES_PATH = $(HOME)/Library/Arduino15/packages

# find $(PACKAGES_PATH) -name arm-none-eabi-gcc -type d
# find $(PACKAGES_PATH) -name avr-gcc -type d


# Arduino.app path for ArduinoCC 1.6.5
#
ARDUINO_AVR_1 = $(PACKAGES_PATH)/arduino

ifneq ($(wildcard $(ARDUINO_AVR_1)/hardware/avr),)
    ARDUINO_AVR_APP     = $(ARDUINO_AVR_1)
    ARDUINO_AVR_PATH    = $(ARDUINO_AVR_APP)
    ARDUINO_CC_AVR_BOARDS  = $(ARDUINO_AVR_APP)/hardware/avr/$(ARDUINO_AVR_RELEASE)/boards.txt
endif

ARDUINO_SAM_1 = $(PACKAGES_PATH)/arduino

ifneq ($(wildcard $(ARDUINO_SAM_1)/hardware/sam),)
    ARDUINO_SAM_APP     = $(ARDUINO_SAM_1)
    ARDUINO_SAM_PATH    = $(ARDUINO_SAM_APP)
    ARDUINO_CC_SAM_BOARDS  = $(ARDUINO_SAM_APP)/hardware/sam/$(ARDUINO_SAM_RELEASE)/boards.txt
endif

ARDUINO_SAMD_1 = $(PACKAGES_PATH)/arduino

ifneq ($(wildcard $(ARDUINO_SAMD_1)/hardware/samd),)
    ARDUINO_SAMD_APP     = $(ARDUINO_SAMD_1)
    ARDUINO_SAMD_PATH    = $(ARDUINO_SAMD_APP)
    ARDUINO_CC_SAMD_BOARDS  = $(ARDUINO_SAMD_APP)/hardware/samd/$(ARDUINO_SAMD_RELEASE)/boards.txt
endif

# Adafruit.app path for ArduinoCC 1.6.5
#
ADAFRUIT_AVR_1  = $(PACKAGES_PATH)/adafruit

ifneq ($(wildcard $(ADAFRUIT_AVR_1)/hardware/avr),)
    ADAFRUIT_AVR_APP     = $(ADAFRUIT_AVR_1)
    ADAFRUIT_AVR_PATH    = $(ADAFRUIT_AVR_APP)
    ADAFRUIT_AVR_BOARDS  = $(ADAFRUIT_AVR_APP)/hardware/avr/$(ADAFRUIT_AVR_RELEASE)/boards.txt
endif

# ~
ADAFRUIT_SAMD_1  = $(PACKAGES_PATH)/adafruit

ifneq ($(wildcard $(ADAFRUIT_SAMD_1)/hardware/samd),)
    ADAFRUIT_SAMD_APP     = $(ADAFRUIT_SAMD_1)
    ADAFRUIT_SAMD_PATH    = $(ADAFRUIT_SAMD_APP)
    ADAFRUIT_SAMD_BOARDS  = $(ADAFRUIT_SAMD_APP)/hardware/samd/$(ADAFRUIT_SAMD_RELEASE)/boards.txt
endif
# ~~

# chipKIT.app path for ArduinoCC 1.6.5
#
CHIPKIT_1     = $(PACKAGES_PATH)/chipKIT
ifneq ($(wildcard $(CHIPKIT_1)),)
    CHIPKIT_APP     = $(CHIPKIT_1)
    CHIPKIT_PATH    = $(CHIPKIT_APP)
    CHIPKIT_BOARDS  = $(CHIPKIT_APP)/hardware/pic32/$(CHIPKIT_RELEASE)/boards.txt
endif

# ~
# RFduino.app path for ArduinoCC 1.6.5
#
RFDUINO_1    = $(PACKAGES_PATH)/RFduino

ifneq ($(wildcard $(RFDUINO_1)),)
    RFDUINO_APP     = $(RFDUINO_1)
    RFDUINO_PATH    = $(RFDUINO_APP)
    RFDUINO_BOARDS  = $(RFDUINO_APP)/hardware/RFduino/$(RFDUINO_RELEASE)/boards.txt
endif
# ~~

# ~
# Simblee.app path for ArduinoCC 1.6.5
#
SIMBLEE_1    = $(PACKAGES_PATH)/Simblee

ifneq ($(wildcard $(SIMBLEE_1)),)
    SIMBLEE_APP     = $(SIMBLEE_1)
    SIMBLEE_PATH    = $(SIMBLEE_APP)
    SIMBLEE_BOARDS  = $(SIMBLEE_APP)/hardware/Simblee/$(SIMBLEE_RELEASE)/boards.txt
endif
# ~~

# ~
# Moteino.app path for ArduinoCC 1.6.5
#
MOTEINO_1    = $(PACKAGES_PATH)/Moteino

ifneq ($(wildcard $(MOTEINO_1)),)
    MOTEINO_APP     = $(MOTEINO_1)
    MOTEINO_PATH    = $(MOTEINO_APP)
    MOTEINO_BOARDS  = $(MOTEINO_APP)/hardware/avr/$(MOTEINO_AVR_RELEASE)/boards.txt
endif
# ~~

# UDOO_NEO path for ArduinoCC 1.6.5
#
UDOO_NEO_1    = $(PACKAGES_PATH)/UDOO

ifneq ($(wildcard $(UDOO_NEO_1)),)
    UDOO_NEO_APP     = $(UDOO_NEO_1)
    UDOO_NEO_PATH    = $(UDOO_NEO_APP)
    UDOO_NEO_BOARDS  = $(UDOO_NEO_APP)/hardware/solox/$(UDOO_NEO_RELEASE)/boards.txt
endif

# IntelArduino.app path for ArduinoCC 1.6.5
#
INTEL_1    = $(PACKAGES_PATH)/Intel

ifneq ($(wildcard $(INTEL_1)),)
    INTEL_APP     = $(INTEL_1)
    INTEL_PATH    = $(INTEL_APP)
    INTEL_GALILEO_BOARDS  = $(INTEL_APP)/hardware/i586/$(INTEL_GALILEO_RELEASE)/boards.txt
    INTEL_EDISON_BOARDS   = $(INTEL_APP)/hardware/i686/$(INTEL_EDISON_RELEASE)/boards.txt
    INTEL_CURIE_BOARDS    = $(INTEL_APP)/hardware/arc32/$(INTEL_CURIE_RELEASE)/boards.txt
endif

# RedBearLab.app path for ArduinoCC 1.6.5
#
REDBEARLAB_AVR_1    = $(PACKAGES_PATH)/RedBearLab
REDBEARLAB_NRF_1    = $(PACKAGES_PATH)/RedBearLab

ifneq ($(wildcard $(REDBEARLAB_AVR_1)/hardware/avr),)
    REDBEARLAB_AVR_APP     = $(REDBEARLAB_AVR_1)
    REDBEARLAB_AVR_PATH    = $(REDBEARLAB_AVR_APP)
    REDBEARLAB_AVR_BOARDS  = $(REDBEARLAB_AVR_1)/hardware/avr/$(REDBEARLAB_AVR_RELEASE)/boards.txt
endif

ifneq ($(wildcard $(REDBEARLAB_NRF_1)/hardware/nRF51822),)
    REDBEARLAB_NRF_APP     = $(REDBEARLAB_NRF_1)
    REDBEARLAB_NRF_PATH    = $(REDBEARLAB_NRF_APP)
    REDBEARLAB_NRF_BOARDS  = $(REDBEARLAB_NRF_1)/hardware/nRF51822/$(REDBEARLAB_NRF_RELEASE)/boards.txt
endif

# ~
REDBEARLAB_DUO_1    = $(PACKAGES_PATH)/RedBear

ifneq ($(wildcard $(REDBEARLAB_DUO_1)/hardware/STM32F2),)
    REDBEARLAB_DUO_APP     = $(REDBEARLAB_DUO_1)
    REDBEARLAB_DUO_PATH    = $(REDBEARLAB_DUO_APP)
    REDBEARLAB_DUO_BOARDS  = $(REDBEARLAB_DUO_1)/hardware/STM32F2/$(REDBEARLAB_DUO_RELEASE)/boards.txt
endif
# ~~

# DigisparkArduino.app path for ArduinoCC 1.6.5
#
DIGISTUMP_AVR_1 = $(PACKAGES_PATH)/digistump
DIGISTUMP_SAM_1 = $(PACKAGES_PATH)/digistump
DIGISTUMP_OAK_1 = $(PACKAGES_PATH)/digistump

ifneq ($(wildcard $(DIGISTUMP_AVR_1)),)
    DIGISTUMP_AVR_APP    = $(DIGISTUMP_AVR_1)
    DIGISTUMP_AVR_PATH   = $(DIGISTUMP_AVR_APP)
    DIGISTUMP_AVR_BOARDS = $(DIGISTUMP_AVR_APP)/hardware/avr/$(DIGISTUMP_AVR_RELEASE)/boards.txt
endif

ifneq ($(wildcard $(DIGISTUMP_SAM_1)),)
    DIGISTUMP_SAM_APP    = $(DIGISTUMP_SAM_1)
    DIGISTUMP_SAM_PATH   = $(DIGISTUMP_SAM_APP)
    DIGISTUMP_SAM_BOARDS = $(DIGISTUMP_SAM_APP)/hardware/sam/$(DIGISTUMP_SAM_RELEASE)/boards.txt
endif

ifneq ($(wildcard $(DIGISTUMP_OAK_1)/hardware/oak),)
    DIGISTUMP_OAK_APP    = $(DIGISTUMP_OAK_1)
    DIGISTUMP_OAK_PATH   = $(DIGISTUMP_OAK_APP)
    DIGISTUMP_OAK_BOARDS = $(DIGISTUMP_OAK_APP)/hardware/oak/$(DIGISTUMP_OAK_RELEASE)/boards.txt
endif

# ESP8266 NodeMCU.app path for ArduinoCC 1.6.5
#
ESP8266_1 = $(PACKAGES_PATH)/esp8266

ifneq ($(wildcard $(ESP8266_1)),)
    ESP8266_APP     = $(ESP8266_1)
    ESP8266_PATH    = $(ESP8266_APP)
    ESP8266_BOARDS  = $(ESP8266_1)/hardware/esp8266/$(ESP8266_RELEASE)/boards.txt
endif

# LittleRobotFriends.app path for ArduinoCC 1.6.5
#
LITTLEROBOTFRIENDS_1 = $(PACKAGES_PATH)/littlerobotfriends

ifneq ($(wildcard $(LITTLEROBOTFRIENDS_1)),)
    LITTLEROBOTFRIENDS_APP  = $(ARDUINO_APP)
    LITTLEROBOTFRIENDS_PATH = $(ARDUINO_APP)
    LITTLEROBOTFRIENDS_BOARDS = $(LITTLEROBOTFRIENDS_1)/hardware/avr/$(LITTLEROBOTFRIENDS_AVR_RELEASE)/boards.txt
endif

# ~
# Cosa.app path for ArduinoCC 1.6.5
#
COSA_AVR_1    = $(PACKAGES_PATH)/Cosa

ifneq ($(wildcard $(COSA_AVR_1)),)
    COSA_AVR_APP     = $(COSA_AVR_1)
    COSA_AVR_PATH    = $(COSA_AVR_APP)
    COSA_AVR_BOARDS  = $(COSA_AVR_APP)/hardware/avr/$(COSA_AVR_RELEASE)/boards.txt
endif
# ~~

# ~
# LinkIt.app path for ArduinoCC 1.6.5
#
LINKIT_ARM_1    = $(PACKAGES_PATH)/LinkIt

ifneq ($(wildcard $(LINKIT_ARM_1)/hardware/arm/$(LINKIT_ONE_RELEASE)),)
    LINKIT_ARM_APP      = $(LINKIT_ARM_1)
    LINKIT_ARM_PATH     = $(LINKIT_ARM_APP)
    LINKIT_ARM_BOARDS   = $(LINKIT_ARM_PATH)/hardware/arm/$(LINKIT_ONE_RELEASE)/boards.txt
endif

LINKIT_AVR_1    = $(PACKAGES_PATH)/LinkIt

ifneq ($(wildcard $(LINKIT_AVR_1)/hardware/avr/$(LINKIT_DUO_RELEASE)),)
    LINKIT_AVR_APP      = $(LINKIT_AVR_1)
    LINKIT_AVR_PATH     = $(LINKIT_AVR_APP)
    LINKIT_AVR_BOARDS   = $(LINKIT_AVR_PATH)/hardware/avr/$(LINKIT_DUO_RELEASE)/boards.txt
endif
# ~~

# panStamp.app path for ArduinoCC 1.6.5
#
PANSTAMP_AVR_1    = $(PACKAGES_PATH)/panstamp_avr

ifneq ($(wildcard $(PANSTAMP_AVR_1)),)
    PANSTAMP_AVR_APP    = $(PANSTAMP_AVR_1)
    PANSTAMP_AVR_PATH   = $(PANSTAMP_AVR_APP)
    PANSTAMP_AVR_BOARDS = $(PANSTAMP_AVR_APP)/hardware/avr/$(PANSTAMP_AVR_RELEASE)/boards.txt
endif

PANSTAMP_NRG_1    = $(PACKAGES_PATH)/panstamp_nrg

ifneq ($(wildcard $(PANSTAMP_NRG_1)),)
    PANSTAMP_NRG_APP    = $(PANSTAMP_NRG_1)
    PANSTAMP_NRG_PATH   = $(PANSTAMP_NRG_APP)
    PANSTAMP_NRG_BOARDS = $(PANSTAMP_NRG_APP)/hardware/msp430/$(PANSTAMP_MSP_RELEASE)/boards.txt
endif

endif # end  ArduinoCC 1.6.5


# Other boards
# ----------------------------------
#
# ~
# Particle is the new name for Spark
#
SPARK_APP     = $(EMBEDXCODE_APP)/Particle
ifeq ($(wildcard $(SPARK_APP)/*),) # */
    SPARK_APP = $(EMBEDXCODE_APP)/Spark
endif
# ~~

# Teensyduino.app path
#
TEENSY_0    = $(APPLICATIONS_PATH)/Teensyduino.app
ifneq ($(wildcard $(TEENSY_0)),)
    TEENSY_APP    = $(TEENSY_0)
else
    TEENSY_APP    = $(ARDUINO_APP)
endif

# Microduino.app path
#
MICRODUINO_0 = $(APPLICATIONS_PATH)/Microduino.app

ifneq ($(wildcard $(MICRODUINO_0)),)
    MICRODUINO_APP = $(MICRODUINO_0)
else
    MICRODUINO_APP = $(ARDUINO_APP)
endif

# ~
# LightBlueIDE.app path
#
LIGHTBLUE_0 = $(APPLICATIONS_PATH)/LightBlueIDE.app

ifneq ($(wildcard $(LIGHTBLUE_0)),)
    LIGHTBLUE_APP = $(LIGHTBLUE_0)
else
    LIGHTBLUE_APP = $(ARDUINO_APP)
endif
# ~~


# Check at least one IDE installed
#
ifeq ($(wildcard $(ARDUINO_APP)),)
ifeq ($(wildcard $(ARDUINO_ORG_APP)),)
ifeq ($(wildcard $(ARDUINO_CC_APP)),)
ifeq ($(wildcard $(ESP8266_APP)),)
    ifeq ($(wildcard $(LINKIT_ARM_APP)),)
    ifeq ($(wildcard $(WIRING_APP)),)
    ifeq ($(wildcard $(ENERGIA_APP)),)
    ifeq ($(wildcard $(MAPLE_APP)),)
        ifeq ($(wildcard $(TEENSY_APP)),)
        ifeq ($(wildcard $(DIGISTUMP_APP)),)
        ifeq ($(wildcard $(MICRODUINO_APP)),)
        ifeq ($(wildcard $(LIGHTBLUE_APP)),)
            ifeq ($(wildcard $(INTEL_APP)),)
            ifeq ($(wildcard $(ROBOTIS_APP)),)
            ifeq ($(wildcard $(RFDUINO_APP)),)
            ifeq ($(wildcard $(REDBEARLAB_APP)),)
                ifeq ($(wildcard $(LITTLEROBOTFRIENDS_APP)),)
                ifeq ($(wildcard $(PANSTAMP_AVR_APP)),)
                ifeq ($(wildcard $(MBED_APP)/*),) # */
                ifeq ($(wildcard $(EDISON_YOCTO_APP)/*),) # */
                    ifeq ($(wildcard $(SPARK_APP)/*),) # */
                    ifeq ($(wildcard $(ADAFRUIT_AVR_APP)),)
                        $(error Error: no application found)
                    endif
                    endif
                endif
                endif
                endif
                endif
            endif
            endif
            endif
            endif
        endif
        endif
        endif
        endif
    endif
    endif
    endif
    endif
endif
endif
endif
endif


# Arduino-related nightmares
# ----------------------------------
#
# Get Arduino release
# Gone Arduino 1.0, 1.5 Java 6 and 1.5 Java 7 triple release nightmare
#
ifneq ($(wildcard $(ARDUINO_APP)),) # */
#    s102 = $(ARDUINO_APP)/Contents/Resources/Java/lib/version.txt
    s103 = $(ARDUINO_APP)/Contents/Java/lib/version.txt
#    ifneq ($(wildcard $(s102)),)
#        ARDUINO_RELEASE := $(shell cat $(s102) | sed -e "s/\.//g")
#    else
        ARDUINO_RELEASE := $(shell cat $(s103) | sed -e "s/\.//g")
#    endif
    ARDUINO_MAJOR := $(shell echo $(ARDUINO_RELEASE) | cut -d. -f 1-2)
else
    ARDUINO_RELEASE := 0
    ARDUINO_MAJOR   := 0
endif


# Paths list for other genuine IDEs
#
MICRODUINO_PATH = $(MICRODUINO_APP)/Contents/Java
MICRODUINO_AVR_BOARDS       = $(MICRODUINO_PATH)/hardware/Microduino/avr/boards.txt

TEENSY_PATH     = $(TEENSY_APP)/Contents/Java
TEENSY_BOARDS   = $(TEENSY_PATH)/hardware/teensy/avr/boards.txt

ENERGIA_PATH    = $(ENERGIA_APP)/Contents/Resources/Java
ENERGIA_MSP430_BOARDS       = $(ENERGIA_PATH)/hardware/msp430/boards.txt
ENERGIA_C2000_BOARDS        = $(ENERGIA_PATH)/hardware/c2000/boards.txt
ENERGIA_LM4F_BOARDS         = $(ENERGIA_PATH)/hardware/lm4f/boards.txt
ENERGIA_CC3200_BOARDS       = $(ENERGIA_PATH)/hardware/cc3200/boards.txt
ENERGIA_CC2600_EMT_BOARDS   = $(ENERGIA_PATH)/hardware/cc2600emt/boards.txt
ENERGIA_MSP432_EMT_BOARDS   = $(ENERGIA_PATH)/hardware/msp432/boards.txt
ENERGIA_CC3200_EMT_BOARDS   = $(ENERGIA_PATH)/hardware/cc3200emt/boards.txt

MAPLE_PATH      = $(MAPLE_APP)/Contents/Resources/Java
MAPLE_BOARDS    = $(MAPLE_PATH)/hardware/leaflabs/boards.txt
WIRING_PATH     = $(WIRING_APP)/Contents/Java
WIRING_BOARDS   = $(WIRING_PATH)/hardware/Wiring/boards.txt

# Paths list for IDE-less platforms
#
MBED_PATH       = $(MBED_APP)

# ~
# Paths list for other genuine IDEs
#
ROBOTIS_PATH    = $(ROBOTIS_APP)/Contents/Resources/Java
ROBOTIS_BOARDS  = $(ROBOTIS_PATH)/hardware/robotis/boards.txt
# ~~

# ~
# Paths list for other plug-ins
#
LIGHTBLUE_PATH   = $(LIGHTBLUE_APP)/Contents/Java
LIGHTBLUE_BOARDS = $(LIGHTBLUE_PATH)/hardware/LightBlue-Bean/avr/boards.txt

# ~~

# ~
# Paths list for IDE-less platforms
#
SPARK_PATH          = $(SPARK_APP)
EDISON_YOCTO_PATH   = $(EMBEDXCODE_APP)/EdisonYocto
EDISON_YOCTO_BOARDS = $(EDISON_YOCTO_PATH)/boards.txt
EDISON_MCU_PATH     = $(EMBEDXCODE_APP)/EdisonMCU
EDISON_MCU_BOARDS   = $(EDISON_MCU_PATH)/boards.txt
BEAGLE_DEBIAN_PATH  = $(BEAGKE_DEBIAN_APP)
# ~~


# Miscellaneous
# ----------------------------------
# Variables
#
TARGET      := embeddedcomputing
USER_FLAG   := false

# Builds directory
#
OBJDIR  = Builds

# Function PARSE_BOARD data retrieval from boards.txt
# result = $(call PARSE_BOARD 'boardname','parameter')
#
PARSE_BOARD = $(shell if [ -f $(BOARDS_TXT) ]; then grep ^$(1).$(2)= $(BOARDS_TXT) | cut -d = -f 2-; fi; )

# Function PARSE_FILE data retrieval from specified file
# result = $(call PARSE_FILE 'boardname','parameter','filename')
#
PARSE_FILE = $(shell if [ -f $(3) ]; then grep ^$(1).$(2) $(3) | cut -d = -f 2-; fi; )

# ~
# Warnings flags
#
ifeq ($(WARNING_OPTIONS),)
    WARNING_FLAGS = -Wall
else
    ifeq ($(WARNING_OPTIONS),0)
        WARNING_FLAGS = -w
    else
        WARNING_FLAGS = $(addprefix -W, $(WARNING_OPTIONS))
    endif
endif
# ~~


# Clean if new BOARD_TAG
# ----------------------------------
#
NEW_TAG := $(strip $(OBJDIR)/$(BOARD_TAG)-TAG) #
OLD_TAG := $(strip $(wildcard $(OBJDIR)/*-TAG)) # */

ifneq ($(OLD_TAG),$(NEW_TAG))
    CHANGE_FLAG := 1
else
    CHANGE_FLAG := 0
endif


# Identification and switch
# ----------------------------------
# Look if BOARD_TAG is listed as a Arduino/Arduino board
# Look if BOARD_TAG is listed as a Arduino/arduino/avr board *1.5
# Look if BOARD_TAG is listed as a Arduino/arduino/sam board *1.5
# Look if BOARD_TAG is listed as a chipKIT/PIC32 board
# Look if BOARD_TAG is listed as a Wiring/Wiring board
# Look if BOARD_TAG is listed as a Energia/MPS430 board
# Look if BOARD_TAG is listed as a MapleIDE/LeafLabs board
# Look if BOARD_TAG is listed as a Teensy/Teensy board
# Look if BOARD_TAG is listed as a Microduino/Microduino board
# Look if BOARD_TAG is listed as a Digistump/Digistump board
# Look if BOARD_TAG is listed as a IntelGalileo/arduino/x86 board
# Look if BOARD_TAG is listed as a Adafruit/Arduino board
# Look if BOARD_TAG is listed as a LittleRobotFriends board
# Look if BOARD_TAG is listed as a mbed board
# Look if BOARD_TAG is listed as a RedBearLab/arduino/RBL_nRF51822 board
# Look if BOARD_TAG is listed as a Spark board
# ~
# Look if BOARD_TAG is listed as a LightBlueIDE/LightBlue-Bean board
# Look if BOARD_TAG is listed as a Robotis/robotis board
# Look if BOARD_TAG is listed as a RFduino/RFduino board
# ~~
#
# Order matters!
#
ifneq ($(MAKECMDGOALS),boards)
    ifneq ($(MAKECMDGOALS),clean)
# ~
        ifneq ($(findstring COSA,$(GCC_PREPROCESSOR_DEFINITIONS)),)

            ifeq ($(COSA_BOARD_TAG),)
                COSA_BOARD_TAG = $(BOARD_TAG)
            endif
            ifneq ($(call PARSE_FILE,$(COSA_BOARD_TAG),name,$(COSA_AVR_BOARDS)),)
                include $(MAKEFILE_PATH)/Cosa_165.mk
            else
                $(error Cosa board $(BOARD_TAG) is unknown)
            endif

    else
# ~~
        # Arduino.CC or Genuino
        ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(ARDUINO_CC_AVR_BOARDS)),)
            include $(MAKEFILE_PATH)/ArduinoAVR_166.mk

        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(ARDUINO_CC_PATH)/hardware/arduino/avr/boards.txt),)
            include $(MAKEFILE_PATH)/ArduinoAVR_165.mk
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG1),name,$(ARDUINO_CC_PATH)/hardware/arduino/avr/boards.txt),)
            include $(MAKEFILE_PATH)/ArduinoAVR_165.mk

        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(ARDUINO_CC_SAM_BOARDS)),)
            include $(MAKEFILE_PATH)/ArduinoSAM_165.mk
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(ARDUINO_CC_SAMD_BOARDS)),)
            include $(MAKEFILE_PATH)/ArduinoSAMD_165.mk

        # Arduino.ORG
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(ARDUINO_ORG_AVR_BOARDS)),)
            include $(MAKEFILE_PATH)/ArduinoAVR_177.mk
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG1),name,$(ARDUINO_ORG_AVR_BOARDS)),)
            include $(MAKEFILE_PATH)/ArduinoAVR_177
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(ARDUINO_ORG_SAM_BOARDS)),)
            include $(MAKEFILE_PATH)/ArduinoSAM_177.mk
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(ARDUINO_ORG_SAMD_BOARDS)),)
            include $(MAKEFILE_PATH)/ArduinoSAMD_177.mk

        # Additional boards for Arduino.CC or Genuino
        # Intel
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(INTEL_GALILEO_BOARDS)),)
            include $(MAKEFILE_PATH)/IntelGalileo_165.mk
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(INTEL_EDISON_BOARDS)),)
            include $(MAKEFILE_PATH)/IntelEdison_165.mk
		else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(INTEL_CURIE_BOARDS)),)
			include $(MAKEFILE_PATH)/IntelCurie_165.mk

        # panStamp
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(PANSTAMP_AVR_BOARDS)),)
            include $(MAKEFILE_PATH)/panStampAVR_165.mk
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(PANSTAMP_NRG_BOARDS)),)
            include $(MAKEFILE_PATH)/panStampNRG_165.mk

        # chipKIT
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(CHIPKIT_BOARDS)),)
            include $(MAKEFILE_PATH)/chipKIT_165.mk

        # Energia
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(ENERGIA_MSP430_BOARDS)),)
            include $(MAKEFILE_PATH)/EnergiaMSP430.mk$
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(ENERGIA_C2000_BOARDS)),)
            include $(MAKEFILE_PATH)/EnergiaC2000.mk
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(ENERGIA_LM4F_BOARDS)),)
            include $(MAKEFILE_PATH)/EnergiaLM4F.mk
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(ENERGIA_CC3200_BOARDS)),)
            include $(MAKEFILE_PATH)/EnergiaCC3200.mk
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(ENERGIA_MSP432_EMT_BOARDS)),)
            include $(MAKEFILE_PATH)/EnergiaMSP432EMT.mk
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(ENERGIA_CC3200_EMT_BOARDS)),)
            include $(MAKEFILE_PATH)/EnergiaCC3200EMT.mk
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(ENERGIA_CC2600_EMT_BOARDS)),)
            include $(MAKEFILE_PATH)/EnergiaCC2600EMT.mk

        # Others boards for Arduino.CC 1.6.5
        # Adafruit
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(ADAFRUIT_AVR_BOARDS)),)
            include $(MAKEFILE_PATH)/AdafruitAVR_165.mk
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(ADAFRUIT_SAMD_BOARDS)),)
            include $(MAKEFILE_PATH)/AdafruitSAMD_165.mk

        # ESP8266
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(ESP8266_BOARDS)),)
            include $(MAKEFILE_PATH)/ESP8266_165.mk

        # LittleRobotFriends
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(LITTLEROBOTFRIENDS_BOARDS)),)
            include $(MAKEFILE_PATH)/LittleRobotFriends_165.mk

        # Digistump
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(DIGISTUMP_AVR_BOARDS)),)
            include $(MAKEFILE_PATH)/DigistumpAVR_165.mk
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(DIGISTUMP_SAM_BOARDS)),)
            include $(MAKEFILE_PATH)/DigistumpSAM_165.mk
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(DIGISTUMP_OAK_BOARDS)),)
            include $(MAKEFILE_PATH)/DigistumpOAK_165.mk

        # RedBearLab
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(REDBEARLAB_AVR_BOARDS)),)
            include $(MAKEFILE_PATH)/RedBearLabAVR_165.mk
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(REDBEARLAB_NRF_BOARDS)),)
            include $(MAKEFILE_PATH)/RedBearLabNRF_165.mk

        # UDOO Neo
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(UDOO_NEO_BOARDS)),)
            include $(MAKEFILE_PATH)/UdooNeo_165.mk

        # Other boards
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(MAPLE_BOARDS)),)
            include $(MAKEFILE_PATH)/MapleIDE.mk
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(WIRING_BOARDS)),)
            include $(MAKEFILE_PATH)/Wiring.mk
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(TEENSY_BOARDS)),)
            include $(MAKEFILE_PATH)/Teensy.mk
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG1),name,$(MICRODUINO_AVR_BOARDS)),)
            include $(MAKEFILE_PATH)/Microduino_16.mk
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(MICRODUINO_AVR_BOARDS)),)
            include $(MAKEFILE_PATH)/Microduino_16.mk

        # Other frameworks
        else ifeq ($(filter MBED,$(GCC_PREPROCESSOR_DEFINITIONS)),MBED)
            include $(MAKEFILE_PATH)/mbed.mk

        else ifeq ($(filter SPARK,$(GCC_PREPROCESSOR_DEFINITIONS)),SPARK)
            include $(MAKEFILE_PATH)/Particle.mk

# ~
        # More boards
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(REDBEARLAB_DUO_BOARDS)),)
            include $(MAKEFILE_PATH)/RedBearLabDUO_165.mk

        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(MOTEINO_BOARDS)),)
            include $(MAKEFILE_PATH)/Moteino_165.mk

        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(LINKIT_ARM_BOARDS)),)
            include $(MAKEFILE_PATH)/LinkItOne_165.mk

        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(LINKIT_AVR_BOARDS)),)
            include $(MAKEFILE_PATH)/LinkItDuo_165.mk

        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(LIGHTBLUE_BOARDS)),)
            include $(MAKEFILE_PATH)/LightBlue.mk

        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(ROBOTIS_BOARDS)),)
            include $(MAKEFILE_PATH)/Robotis.mk

        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(RFDUINO_BOARDS)),)
            include $(MAKEFILE_PATH)/RFduino_165.mk
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(SIMBLEE_BOARDS)),)
            include $(MAKEFILE_PATH)/Simblee_165.mk

        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(EDISON_YOCTO_BOARDS)),)
            include $(MAKEFILE_PATH)/IntelEdisonYocto.mk
        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(EDISON_MCU_BOARDS)),)
            include $(MAKEFILE_PATH)/IntelEdisonMCU.mk

        else ifneq ($(call PARSE_FILE,$(BOARD_TAG),name,$(BEAGLE_DEBIAN_PATH)/boards.txt),)
            include $(MAKEFILE_PATH)/BeagleBoneDebianL.mk
# Alternatives for tool-chain
#            include $(MAKEFILE_PATH)/BeagleBoneDebianGCC.mk
#            include $(MAKEFILE_PATH)/BeagleBoneDebianCM.mk
# ~~

        else
            $(error $(BOARD_TAG) board is unknown)
        endif
# ~
      endif
# ~~
    endif
endif


# List of sub-paths to be excluded
#
EXCLUDE_NAMES  = Example example Examples examples Archive archive Archives archives Documentation documentation Reference reference
EXCLUDE_NAMES += ArduinoTestSuite
EXCLUDE_NAMES += $(EXCLUDE_LIBS)
EXCLUDE_LIST   = $(addprefix %,$(EXCLUDE_NAMES))

# Step 2
#
include $(MAKEFILE_PATH)/Step2.mk

