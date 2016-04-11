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
# Last update: Nov 06, 2015 release 4.0.2






# Debug rules
# ----------------------------------
#
# ~
debug: 		make message_debug reset raw_upload serial launch_debug end_debug
# ~~

# ~
launch_debug:

# 1. GDB
ifneq ($(GDB),)
	@if [ -f $(UTILITIES_PATH)/embedXcode_debug ]; then export STECK_EXTENSION=$(STECK_EXTENSION); $(UTILITIES_PATH)/embedXcode_debug; fi;

# 1.1 mspdebug
  ifeq ($(UPLOADER),mspdebug)
    ifeq ($(UPLOADER_PROTOCOL),tilib)
# Debug 1: Launch the server
		@echo "---- Launch server ----"
		$(call SHOW,"8.1-DEBUG",$(UPLOADER))
		osascript -e 'tell application "Terminal" to do script "cd $(UPLOADER_PATH); ./mspdebug $(UPLOADER_PROTOCOL) gdb"'

    else
		@echo "---- Launch server ----"
		$(call SHOW,"8.2-DEBUG",$(UPLOADER))
		@osascript -e 'tell application "Terminal" to do script "$(UPLOADER_EXEC) $(UPLOADER_PROTOCOL) gdb"'
    endif

# Debug 2: Launch the client
		@echo "---- Launch client ----"
		$(call SHOW,"8.3-DEBUG",$(UPLOADER))
		@sleep 5
		@osascript -e 'tell application "Terminal" to do script "cd $(CURRENT_DIR_SPACE); $(GDB) -x \"$(UTILITIES_PATH_SPACE)/gdb.txt\""'

# Debug 3: Garbage collector
		@if [ -f libmsp430.so ]; then rm libmsp430.so; fi
		@if [ -f comm.log ]; then rm comm.log; fi

# 1.2 lm4flash
  else ifeq ($(UPLOADER),lm4flash)
# Debug 1: Launch the server
		@echo "---- Launch server ----"
		$(call SHOW,"8.4-DEBUG",$(UPLOADER))
		-killall openocd
		@osascript -e 'tell application "Terminal" to do script "openocd --file \"$(UTILITIES_PATH_SPACE)/debug_LM4F120XL.cfg\""'

# Debug 2: Launch the client
		@echo "---- Launch client ----"
		$(call SHOW,"8.5-DEBUG",$(UPLOADER))
		@sleep 5
		@osascript -e 'tell application "Terminal" to do script "cd $(CURRENT_DIR_SPACE); $(GDB) -x \"$(UTILITIES_PATH_SPACE)/gdb.txt\""'

# 1.3 cc3200serial
  else ifeq ($(UPLOADER),cc3200serial)
# Debug 1: Launch the server
		@echo "---- Launch server ----"
		$(call SHOW,"8.6-DEBUG",$(UPLOADER))
		-killall openocd
		@osascript -e 'tell application "Terminal" to do script "openocd --file \"$(UTILITIES_PATH_SPACE)/debug_CC3200.cfg\""'

# Debug 2: Launch the client
		@echo "---- Launch client ----"
		$(call SHOW,"8.7-DEBUG",$(UPLOADER))
		@sleep 5
		@osascript -e 'tell application "Terminal" to do script "cd $(CURRENT_DIR_SPACE); $(GDB) -x \"$(UTILITIES_PATH_SPACE)/gdb.txt\""'

# 1.4 DSLite
  else ifeq ($(UPLOADER),DSLite)
# Debug 1: Launch the server
		@echo "---- Launch server ----"
		$(call SHOW,"8.20-DEBUG",$(UPLOADER))
		-killall openocd
		@osascript -e 'tell application "Terminal" to do script "openocd --file \"$(UTILITIES_PATH_SPACE)/debug_MSP432P4.cfg\""'

# Debug 2: Launch the client
		@echo "---- Launch client ----"
		$(call SHOW,"8.21-DEBUG",$(UPLOADER))
		@sleep 5
		@osascript -e 'tell application "Terminal" to do script "cd $(CURRENT_DIR_SPACE); $(GDB) -x \"$(UTILITIES_PATH_SPACE)/gdb.txt\""'

# 1.5 mbed
  else ifeq ($(PLATFORM),mbed)
# Debug 1: Launch the server
    ifeq ($(DEBUG_SERVER),stlink)
		@echo "---- Launch server ----"
		$(call SHOW,"8.8-DEBUG",$(UPLOADER))
		-killall st-util
		@osascript -e 'tell application "Terminal" to do script "st-util -p 3333"'
    else
		@echo "---- Launch server ----"
		$(call SHOW,"8.9-DEBUG",$(UPLOADER))
		-killall openocd
		@osascript -e 'tell application "Terminal" to do script "openocd --file \"$(UTILITIES_PATH_SPACE)/debug_$(BOARD_TAG).cfg\""'
    endif

# Debug 2: Launch the client
		@echo "---- Launch client ----"
		$(call SHOW,"8.10-DEBUG",$(UPLOADER))
		-killall $(GDB)
		@sleep 5
		@osascript -e 'tell application "Terminal" to do script "cd $(CURRENT_DIR_SPACE); $(GDB) -x \"$(UTILITIES_PATH_SPACE)/gdb.txt\""'

# 1.5 IntelYocto
  else ifeq ($(PLATFORM),IntelYocto)
# Debug 1: Launch the server
		@echo "---- Launch server ----"
		$(call SHOW,"8.11-DEBUG",$(UPLOADER))
		-killall $(GDB)

		osascript -e 'tell application "Terminal" to do script "cd $(CURRENT_DIR); $(UTILITIES_PATH)/uploader_ssh.sh $(SSH_ADDRESS) $(SSH_PASSWORD) $(REMOTE_FOLDER) $(TARGET) -debug"'
		@sleep 5

# Debug 2: Launch the client
		@echo "---- Launch client ----"
		$(call SHOW,"8.12-DEBUG",$(UPLOADER))
		@sleep 5
		@osascript -e 'tell application "Terminal" to do script "cd $(CURRENT_DIR_SPACE); $(GDB) -x \"$(UTILITIES_PATH_SPACE)/gdb.txt\""'

# 1.6 Edison
  else ifeq ($(PLATFORM),Edison)
# Debug 1: Launch the server
		@echo "---- Launch server ----"
		$(call SHOW,"8.13-DEBUG",$(UPLOADER))
		-killall $(GDB)

		osascript -e 'tell application "Terminal" to do script "cd $(CURRENT_DIR); $(UTILITIES_PATH)/uploader_ssh.sh $(SSH_ADDRESS) $(SSH_PASSWORD) $(REMOTE_FOLDER) $(TARGET) -debug"'
		@sleep 5

# Debug 2: Launch the client
		@echo "---- Launch client ----"
		$(call SHOW,"8.14-DEBUG",$(UPLOADER))
		@sleep 5
		@osascript -e 'tell application "Terminal" to do script "cd $(CURRENT_DIR_SPACE); $(GDB) -x \"$(UTILITIES_PATH_SPACE)/gdb.txt\""'

# 1.7 openocd
  else ifeq ($(UPLOADER),openocd)
# Debug 1: Launch the server
		@echo "---- Launch server ----"
		$(call SHOW,"8.15-DEBUG",$(UPLOADER))
		-killall $(GDB)

		osascript -e 'tell application "Terminal" to do script "cd $(CURRENT_DIR); $(UPLOADER_EXEC) $(UPLOADER_OPTS)"'
		@sleep 5

# Debug 2: Launch the client
		@echo "---- Launch client ----"
		$(call SHOW,"8.16-DEBUG",$(UPLOADER))
		@sleep 5
		@osascript -e 'tell application "Terminal" to do script "cd $(CURRENT_DIR_SPACE); $(GDB) -x \"$(UTILITIES_PATH_SPACE)/gdb.txt\""'

# 1.8 BeagleBoneDebian
  else ifeq ($(PLATFORM),BeagleBoneDebian)
# Debug 1: Launch the server
		@echo "---- Launch server ----"
		$(call SHOW,"8.17-DEBUG",$(UPLOADER))
		-killall $(GDB)

		osascript -e 'tell application "Terminal" to do script "cd $(CURRENT_DIR); $(UTILITIES_PATH)/uploader_ssh.sh $(SSH_ADDRESS) $(SSH_PASSWORD) $(REMOTE_FOLDER) $(TARGET) -debug"'
		@sleep 5

# Debug 2: Launch the client
		@echo "---- Launch client ----"
		$(call SHOW,"8.18-DEBUG",$(UPLOADER))
		@sleep 5
		@osascript -e 'tell application "Terminal" to do script "cd $(CURRENT_DIR_SPACE); $(GDB) -x \"$(UTILITIES_PATH_SPACE)/gdb.txt\""'

# 1.9 none
  else
		@echo "Board not supported" $(PLATFORM)
  endif

# 2. MDB
else ifneq ($(MDB),)
		@if [ -f $(UTILITIES_PATH)/embedXcode_debug ]; then export STECK_EXTENSION=$(STECK_EXTENSION); $(UTILITIES_PATH)/embedXcode_debug; fi;

		@echo "---- Launch programmer-debugger ----"
		$(call SHOW,"8.19-DEBUG",$(UPLOADER))
		@osascript -e 'tell application "Terminal" to do script "$(MDB) \"$(UTILITIES_PATH_SPACE)/mdb.txt\""'

endif

message_debug:
		@echo "==== Debug ===="

end_debug:
		@echo "==== Debug done ==== "
# ~~


.PHONY:	debug launch_debug message_debug end_debug

