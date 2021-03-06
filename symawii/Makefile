#GCC-arm Makefile for symawii
#Copyright 2015 pokey9000
#
#This program is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#any later version.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

# Note: lifted from Eclipse generated makefiles


COPTER="X4"

GCC=/Users/gerrie/Library/embedXcode/gcc-arm-none-eabi-4_8-2014q3/bin/arm-none-eabi-gcc
GCCOBJCOPY=/Users/gerrie/Library/embedXcode/gcc-arm-none-eabi-4_8-2014q3/bin/arm-none-eabi-objcopy
GCCSIZE=/Users/gerrie/Library/embedXcode/gcc-arm-none-eabi-4_8-2014q3/bin/arm-none-eabi-size

ifeq ($(COPTER), "X4")
MICRO="MINI51"
endif

OBJS += src/a7105.o src/accelerometer.o src/autotune.o src/symawii.o src/checkboxes.o src/eeprom.o src/gyro.o src/imu.o src/leds.o src/output.o src/pilotcontrol.o src/serial.o src/rx_x4.o src/vectors.o 

MINI51_OBJS += lib-Mini51/hal/drv_gpio.o lib-Mini51/hal/drv_hal.o lib-Mini51/hal/drv_pwm.o lib-Mini51/hal/drv_serial.o lib-Mini51/hal/lib_adc.o lib-Mini51/hal/lib_digitalio.o lib-Mini51/hal/lib_fp.o lib-Mini51/hal/lib_i2c.o lib-Mini51/hal/lib_serial.o lib-Mini51/hal/lib_soft_3_wire_spi.o lib-Mini51/hal/lib_timers.o 

MINI51_OBJS += lib-Mini51/StdDriver/src/adc.o lib-Mini51/StdDriver/src/clk.o lib-Mini51/StdDriver/src/fmc.o lib-Mini51/StdDriver/src/gpio.o lib-Mini51/StdDriver/src/i2c.o lib-Mini51/StdDriver/src/pwm.o lib-Mini51/StdDriver/src/sys.o lib-Mini51/StdDriver/src/timer.o lib-Mini51/StdDriver/src/wdt.o lib-Mini51/StdDriver/src/uart.o

MINI51_OBJS += lib-Mini51/Device/Nuvoton/Mini51Series/Source/system_Mini51Series.o 

MINI51_OBJS += lib-Mini51/startup_Mini51Series.o 

ifeq ($(MICRO), "MINI51")
OBJS += $(MINI51_OBJS)
endif

# Add inputs and outputs from these tool invocations to the build variables 

SECONDARY_FLASH += symawii-x4-gcc.hex
SECONDARY_SIZE += symawii-x4-gcc.siz

# All Target
all: symawii-x4-gcc.siz

# Tool invocations

%.o: %.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	$(GCC) -mcpu=cortex-m0 -mthumb -Os -ffunction-sections -fno-common -fno-builtin -Wall  -g -DX4_BUILD -DDEBUG_ENABLE_SEMIHOST -I"lib-Mini51/hal" -I"src" -I"lib-Mini51/Device/Nuvoton/Mini51Series/Include" -I"lib-Mini51/CMSIS/Include" -I"lib-Mini51/StdDriver/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"

symawii-x4-gcc: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C Linker'
	$(GCC) -mcpu=cortex-m0 -mthumb -Os -ffunction-sections -fno-common -fno-builtin -Wall  -g -T "lib-Mini51/linker/memory.ld" -T "lib-Mini51/linker/link.ld" -nostartfiles -Xlinker --gc-sections -Wl,-Map,"symawii-x4-gcc.map" -o "symawii-x4-gcc" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

symawii-x4-gcc.hex: symawii-x4-gcc
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	$(GCCOBJCOPY) -O ihex --strip-unneeded "symawii-x4-gcc"  "symawii-x4-gcc.hex"
	@echo 'Finished building: $@'
	@echo ' '

symawii-x4-gcc.siz: symawii-x4-gcc
	@echo 'Invoking: Cross ARM GNU Print Size'
	$(GCCSIZE) --format=berkeley "symawii-x4-gcc"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-rm -f $(SECONDARY_SIZE) $(OBJS) $(OBJS:.o=.d) $(SECONDARY_FLASH) symawii-x4-gcc symawii-x4-gcc.map
	-@echo ' '

# TODO: package up openocd changes
flash: symawii-x4-gcc
	-openocd -f target/mini51_stlinkv2.cfg -c "init; halt; flash write_image erase symawii-x4-gcc 0; reset; shutdown"

debug:
	-openocd -f target/mini51_stlinkv2.cfg -c "init; halt; arm semihosting enable; reset run"

.PHONY: all clean dependents
.SECONDARY:
