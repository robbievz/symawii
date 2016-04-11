/* 
Copyright 2013 Brad Quick

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// this is the file that is used to configure the software.  Uncomment the appropriate lines by removing the // in front of them.
// Configuration works with a lot of defaults.  The only thing you really need to choose is the control board.  After that,
// all defaults will be chosen for that board.  If you want to use other than defaults, then uncomment the things you want
// to change.  To see what the defaults are, look in defs.h
// The options are defined in options.h

#include "options.h"

// Choose your control board:
//#define CONTROL_BOARD_TYPE CONTROL_BOARD_HK_MULTIWII_PRO_2
//#define CONTROL_BOARD_TYPE CONTROL_BOARD_HK_MULTIWII_328P
//#define CONTROL_BOARD_TYPE CONTROL_BOARD_HK_NANOWII
//#define CONTROL_BOARD_TYPE CONTROL_BOARD_HK_POCKET_QUAD
//#define CONTROL_BOARD_TYPE CONTROL_BOARD_SIRIUS_AIR
//#define CONTROL_BOARD_TYPE CONTROL_BOARD_SIRIUS_AIR_GPS
//#define CONTROL_BOARD_TYPE CONTROL_BOARD_SIRIUS_PARIS_V4
//#define CONTROL_BOARD_TYPE CONTROL_BOARD_WITESPY_FLIP
//#define CONTROL_BOARD_TYPE CONTROL_BOARD_WITESPY_MULTIWII_PRO_2
//#define CONTROL_BOARD_TYPE CONTROL_BOARD_WITESPY_MULTIWII_PRO_2_GPS
//#define CONTROL_BOARD_TYPE CONTROL_BOARD_WLT_V202
#define CONTROL_BOARD_TYPE CONTROL_BOARD_JXD_JD385

// Choose the type of r/c reciever that will be used
//#define RX_TYPE RX_NORMAL
//#define RX_TYPE RX_CPPM
//#define RX_TYPE RX_DSM2_1024
//#define RX_TYPE RX_DSM2_2048
//#define RX_DSM2_SERIAL_PORT 1
#define RX_TYPE RX_SPI_PROTOCOL

// Choose a channel order if you don't like the default for your receiver type selected above
//#define RX_CHANNEL_ORDER         THROTTLEINDEX,ROLLINDEX,PITCHINDEX,YAWINDEX,AUX1INDEX,AUX2INDEX,AUX3INDEX,AUX4INDEX,8,9,10,11 //For Graupner/Spektrum
//#define RX_CHANNEL_ORDER         ROLLINDEX,PITCHINDEX,THROTTLEINDEX,YAWINDEX,AUX1INDEX,AUX2INDEX,AUX3INDEX,AUX4INDEX,8,9,10,11 //For Robe/Hitec/Futaba
//#define RX_CHANNEL_ORDER         ROLLINDEX,PITCHINDEX,YAWINDEX,THROTTLEINDEX,AUX1INDEX,AUX2INDEX,AUX3INDEX,AUX4INDEX,8,9,10,11 //For Multiplex
//#define RX_CHANNEL_ORDER         PITCHINDEX,ROLLINDEX,THROTTLEINDEX,YAWINDEX,AUX1INDEX,AUX2INDEX,AUX3INDEX,AUX4INDEX,8,9,10,11 //For some Hitec/Sanwa/Others

// uncomment to set the number of RX channels, otherwise it will default to what the control board/receiver can handle
#define RXNUMCHANNELS 8

// uncomment to allow arming and disarming with the sticks:
// Arming and disarming only happen at low throttle
// Uncomment the following two lines to allow arming using yaw
#define STICK_ARM STICK_COMMAND_YAW_HIGH
#define STICK_DISARM STICK_COMMAND_YAW_LOW

// uncomment the following two lines to allow arming using yaw, roll, and pitch all at once
//#define STICK_ARM STICK_COMMAND_YAW_HIGH+STICK_COMMAND_ROLL_HIGH+STICK_COMMAND_PITCH_LOW
//#define STICK_DISARM STICK_COMMAND_YAW_LOW+STICK_COMMAND_ROLL_LOW+STICK_COMMAND_PITCH_LOW

// time in ms the stick command have to persist to take action
#define STICK_ARM_TIME 1000 //ms

// Choose an aircraft configuration (defaults to QUADX)
//#define AIRCRAFT_CONFIGURATION QUADX

// Choose which serial ports will be used to transfer data to a configuration device.
// Multiple serial channels can be configured. (i.e. one for computer, one for bluetooth).
// Be sure to uncomment and set the baud rate for any enabled serial ports.
// note: two examples are given below, but any combination of ports can be added together.

//#define MULTIWII_CONFIG_SERIAL_PORTS NOSERIALPORT
//see libserial, SERIALPORT0 will in fact open serial port 1 
#define MULTIWII_CONFIG_SERIAL_PORTS SERIALPORT0
//#define MULTIWII_CONFIG_SERIAL_PORTS SERIALPORT1
//#define MULTIWII_CONFIG_SERIAL_PORTS SERIALPORT1+SERIALPORT3

//strange on my copter 115200 does not work becaus rx pin has a pullup or pulldown resistor soldered in -> lower baudrates work
#define SERIAL_0_BAUD 38400
//#define SERIAL_0_BAUD 115200

//#define SERIAL_1_BAUD 115200
//#define SERIAL_2_BAUD 9600
//#define SERIAL_3_BAUD 115200

// Choose whether to include code for a GPS and set parameters for the GPS, otherwise it will default o what the control board come with
#define GPS_TYPE NO_GPS // select if no GPS is going to be used
//#define GPS_TYPE I2C_GPS // select if an i2c gps is going to be used
//#define GPS_TYPE SERIAL_GPS   // select if a serial GPS (NMEA) is going to be used
//#define GPS_SERIAL_PORT 2
//#define GPS_BAUD 115200

// Choose a multiplier for high rotation rates when in acro mode
#define HIGH_RATES_MULTILIER 2.0

// Choose maximum tilt angles when in level mode
#define LEVEL_MODE_MAX_TILT 55  // 55 degrees
#define LEVEL_MODE_MAX_TILT_HIGH_ANGLE 80       // 80 degrees when high angle checkbox active

// Choose maximum tilt angles owhile navigating. This will determine how fast it moves from point to point.
#define NAVIGATION_MAX_TILT 8   //15 degrees

// Choose output ranges (in microseconds)
#define MIN_MOTOR_OUTPUT 1000
#define MAX_MOTOR_OUTPUT 2000
#define FAILSAFE_MOTOR_OUTPUT 1200      // throttle setting for bringing the aircraft down at a safe speed

// Un-comment and set to YES or NO to override the default value.
// When YES, motors will stop when throttle stick is below STICK_RANGE_LOW (see below) and not in acro or semi acro mode.
#define MOTORS_STOP YES

// set the minimum motor output when armed. If not set, 1067 will be used as a default
#define ARMED_MIN_MOTOR_OUTPUT 1040     // motors spin slowly when armed
//#define ARMED_MIN_MOTOR_OUTPUT 1170 // motors spin slowly when armed (for blheli flashed q-brain)

// Optionally set an offset from RX Input to ESC output.  Usually used to make sure
// the throttle can go to zero.
//#define THROTTLE_TO_MOTOR_OFFSET 0

// Divide the Aux inputs into low, medium, and high using the following divisions
#define AUX_MID_RANGE_LOW 1300
#define AUX_MID_RANGE_HIGH 1700

// Define low and high values for stick commands
#define STICK_RANGE_LOW 1170
#define STICK_RANGE_HIGH 1830

// un-comment if you don't want to include code for a compass, otherwise it will default to what the control board has on it
#define COMPASS_TYPE NO_COMPASS
//#define COMPASS_TYPE HMC5883
//#define COMPASS_TYPE HMC5843
//#define COMPASS_TYPE MAG3110
//#define COMPASS_ORIENTATION(VALUES,X, Y, Z) {VALUES[ROLLINDEX]  =  X; VALUES[PITCHINDEX]  = Y; VALUES[YAWINDEX]  = -Z;}

// un-comment if you don't want to include code for a barometer, otherwise it will default to what the control board has on it
#define BAROMETER_TYPE NO_BAROMETER

// Get your magnetic decliniation from here : http://magnetic-declination.com/
#define MAG_DECLINATION_DEGREES  -13.4  // for Hyde Park, NY

// ESCs calibration
// To calibrate all ESCs connected to the aircraft at the same time (useful to avoid unplugging/re-plugging each ESC)
// Warning: this creates a special version of code
//       You cannot fly with this special version. It is only to be used for calibrating ESCs
//#define ESC_CALIB_CANNOT_FLY  // uncomment to activate
#define ESC_CALIB_LOW  MIN_MOTOR_OUTPUT
#define ESC_CALIB_HIGH MAX_MOTOR_OUTPUT

// un-comment if you don't want to include autotune code
//#define NO_AUTOTUNE

// To adjust how agressive the tuning is, adjust the AUTOTUNEMAXOSCILLATION value.  A larger
// value will result in more agressive tuning. A lower value will result in softer tuning.
// It will rock back and forth between -AUTOTUNE_TARGET_ANGLE and AUTOTUNE_TARGET_ANGLE degrees
// AUTOTUNE_D_MULTIPLIER is a multiplier that puts in a little extra D when autotuning is done. This helps damp
// the wobbles after a quick angle change.
// Always autotune on a full battery.
#define AUTOTUNE_MAX_OSCILLATION 2.0
#define AUTOTUNE_TARGET_ANGLE 20.0
#define AUTOTUNE_D_MULTIPLIER 1.2

// Gyro low pass filter.
// If your aircraft jumps around randomly, or it drifts from level over time when in level mode,
// then you are probably getting vibration feeding from your motors to your gyros.
// The first step is to isolate the control board from the frame of the aircraft (google Sorbothane).  If all else fails,
// increase the GYRO_LOW_PASS_FILTER from 0 through 10.  The lowest value that works correctly is the one you should use.
// Leave comment to use the default value.
#define GYRO_LOW_PASS_FILTER 4

#define UNCRAHSABLE_MAX_ALTITUDE_OFFSET 30.0    // 30 meters above where uncrashability was enabled
#define UNCRAHSABLE_RADIUS 50.0 // 50 meter radius

// Uncomment the following line if you want to modify gain scheduling.  Gain scheduling adjusts the PID gains
// depending on the level of throttle.  It attempts to eliminate the wobbles while decending under low throttle.
// A value of zero is no gain scheduling.  A value of 1.0 (the default) results in 50% gains at zero throttle, 100% gains at mid
// throttle, and 150% gains at full throttle.
// Range is 0 to 2.0
//#define GAIN_SCHEDULING_FACTOR 1.0

// Uncomment if using DC motors
#define DC_MOTORS

// Uncomment the following line iy you want to use the ADC to monitor the battery voltage
// #define BATTERY_ADC_CHANNEL NO_ADC
#define BATTERY_ADC_CHANNEL (1<<2)
#define BATTERY_ADC_DEBUG 0
// ADC external reference voltage.
// In the MINI54 the ADC reference voltage is internally tied to
// supply voltage. Supply voltage is generated by LM6206 3.0V
// low dropout linear voltage regulator with +/- 2% output tolerance.
// Problem: even this low dropout type has max. 680mV dropout
// voltage. So even at a battery voltage of 3.6V the reference
// voltage might be lower than this and then we can't measure
// battery voltage anymore. So we have to use the internal
// bandgap reference as well.
// Unit: Volt
#define ADC_REF_VOLTAGE 3.0

// Internal bandgap reference voltage which can be measured
// on channel 7. Unfortunately this has high tolerance:
// Min 1.27V, max 1.44V. At least it should be independent of
// supply voltage.
// We don't use this constant and measure the real value instead
// when we know that external reference voltage is OK.
// Unit: Volt
//#define ADC_BG_VOLTAGE 1.35


// This factor converts ADC voltage to battery voltage.
// Inverse of voltage divider for battery voltage measurement.
// On this board the voltage divider consists of two ???kOhm resistors.
// NOTE: i did not want to desolder it to measure (a parallel capacitor makes measuring impossible)
//       so i just compared battery voltage to the voltage on this divider (4.1V -> 1.59V on divider)
//       -> 4.1V is read perfectly, 3.635V iss read as 3.62V so its good enough but could be fine tuned
//          (in order to do so someone will have to measure the resistors while disconnected to the circuit)
#define BATTERY_VOLTAGE_FACTOR 2.5786

// If battery voltage is below this value,
// the pilot will be warned by blinking the LEDs.
// Battery voltage is measured under load and low pass filtered.
// A test showed that using a limit value of 3.2V the battery
// had a no-load voltage of 3.7V after landing.
// If your setup has less voltage drop or you want to be notified earlier,
// set this value higher.
// Unit: Volt
#define BATTERY_UNDERVOLTAGE_LIMIT 3.2 // 3.2

// If battery voltage is below BATTTERY_UNDERVOLTAGE_LIMIT for a defined amount of time
// enables the battery low indicator all the time until battery is replaced
// 
#define BATTERY_LOW_TIMER 1000 

// Parameters for leds
// define the number of LEDs use on the copter otherwise define NO_LEDS
// #define LEDS NO_LEDS
#define LEDS 1

// LED Outputs (1)
// LEDs FL & FR are tied together
// LEDS RL & RR are not populatet on PCB
// To use the additional LEDs following parts have to be soldern on the PCB:
// On the free pads on the PCB a transistor ?PNP? + series resistor
// and 2x LEDs plus 2x 10Ohms series resistor have to be added.
#define LED1 LED1_STATE			//0x01
// #define LED2 LED2_STATE 	//0x02
// #define LED3 LED3_STATE 	//0x04
// #define LED4 LED4_STATE	//0x08
// #define LED5 LED5_STATE	//0x10
// #define LED6 LED6_STATE	//0x20
// #define LED7 LED7_STATE  //0x40
// #define LED8 LED8_STATE	//0x80

// If a LED shall indicate GPS link status define GPS LED over here
#define GPS_LED NONE
// #define GPS_LED LED1
// #define GPS_LED LED2

// Default Config MultiWii
// set default acro mode rotation rates
#define USERSETTINGS_MAXYAWRATE  600L << FIXEDPOINTSHIFT  					// degrees per second
#define USERSETTINGS_MAXPITCHANDROLLRATE  400L << FIXEDPOINTSHIFT 	// degrees per second

// set default PID settings

// pitch PIDs
#define USERSETTINGS_PID_PGAIN_PITCHINDEX PID_TO_CONFIGURATORVALUE_P(1.5)
#define USERSETTINGS_PID_IGAIN_PITCHINDEX PID_TO_CONFIGURATORVALUE_I(0.015)
#define USERSETTINGS_PID_DGAIN_PITCHINDEX PID_TO_CONFIGURATORVALUE_D(20.0)
 
// roll PIDs
#define USERSETTINGS_PID_PGAIN_ROLLINDEX PID_TO_CONFIGURATORVALUE_P(1.5)
#define USERSETTINGS_PID_IGAIN_ROLLINDEX PID_TO_CONFIGURATORVALUE_I(0.015)
#define USERSETTINGS_PID_DGAIN_ROLLINDEX PID_TO_CONFIGURATORVALUE_D(20.0)

// yaw PIDs
#define USERSETTINGS_PID_PGAIN_YAWINDEX PID_TO_CONFIGURATORVALUE_P(1.5)
#define USERSETTINGS_PID_IGAIN_YAWINDEX PID_TO_CONFIGURATORVALUE_I(0.015)
#define USERSETTINGS_PID_DGAIN_YAWINDEX PID_TO_CONFIGURATORVALUE_D(20.0)

//
#define USERSETTINGS_PID_PGAIN_ALTITUDEINDEX PID_TO_CONFIGURATORVALUE_ALT_P(2.7)
#define USERSETTINGS_PID_IGAIN_ALTITUDEINDEX PID_TO_CONFIGURATORVALUE_ALT_I(0.0)
#define USERSETTINGS_PID_DGAIN_ALTITUDEINDEX PID_TO_CONFIGURATORVALUE_ALT_D(6.0)

#define USERSETTINGS_PID_PGAIN_NAVIGATIONINDEX PID_TO_CONFIGURATORVALUE_NAV_P(2.5)
#define USERSETTINGS_PID_IGAIN_NAVIGATIONINDEX PID_TO_CONFIGURATORVALUE_NAV_I(0.0)
#define USERSETTINGS_PID_DGAIN_NAVIGATIONINDEX PID_TO_CONFIGURATORVALUE_NAV_D(0.188)

// Checkbox settings...
// #define USERSETTINGS_CHECKBOXARM CHECKBOXMASKAUX1HIGH
// #define USERSETTINGS_CHECKBOXAUTOTHROTTLE
// #define USERSETTINGS_CHECKBOXALTHOLD
// #define USERSETTINGS_CHECKBOXCOMPASS
// #define USERSETTINGS_CHECKBOXPOSITIONHOLD
// #define USERSETTINGS_CHECKBOXRETURNTOHOME
#define USERSETTINGS_CHECKBOXSEMIACRO CHECKBOXMASKAUX1HIGH
// #define USERSETTINGS_CHECKBOXFULLACRO
#define USERSETTINGS_CHECKBOXHIGHRATES CHECKBOXMASKAUX1HIGH
// #define USERSETTINGS_CHECKBOXHIGHANGLE CHECKBOXMASKAUX1LOW
#define USERSETTINGS_CHECKBOXAUTOTUNE CHECKBOXMASKAUX2HIGH
// #define USERSETTINGS_CHECKBOXUNCRASHABLE
// #define USERSETTINGS_CHECKBOXHEADFREE
// #define USERSETTINGS_CHECKBOXYAWHOLD

// Switch to use eeprom
// EEPROM is not tested or available on all model
// #define EEPROM NO_EEPROM
// Warning just define any size
#define EEPROM_SIZE 1024
