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

#include "config.h"
#include "lib_digitalio.h"
#include "output.h"

// This file takes the settings from config.h and creates all of the definitions needed for the rest of the code.

// set control board dependant defines here
// ======================================================= HUBSAN Q4 / Estes Proto-X / Estes Syncro =====================================================
#if CONTROL_BOARD_TYPE == CONTROL_BOARD_HUBSAN_Q4

#define GYRO_TYPE MPU3050       // gyro

#define GYRO_ORIENTATION(VALUES,X, Y, Z) {VALUES[ROLLINDEX] =  -Y; VALUES[PITCHINDEX] = X; VALUES[YAWINDEX] = -Z;}

#define ACCELEROMETER_TYPE MC3210      // accelerometer
// MC3210 in Hubsan Q4:
// Positive Z = level position
// Positive Y = left side down
// Positive X = front side down
// In this firmware: Z=down, X=west
#define ACC_ORIENTATION(VALUES,X, Y, Z)  {VALUES[XINDEX]  = -Y; VALUES[YINDEX]  = X; VALUES[ZINDEX]  =  Z;}

#ifndef COMPASS_TYPE
#define COMPASS_TYPE NO_COMPASS
#endif

#ifndef BAROMETER_TYPE
#define BAROMETER_TYPE NO_BAROMETER
#endif

#ifndef MULTIWII_CONFIG_SERIAL_PORTS
#define MULTIWII_CONFIG_SERIAL_PORTS NOSERIALPORT
#endif

#ifndef GPS_TYPE
#define GPS_TYPE NO_GPS
#endif

#define RXNUMCHANNELS 6 

#ifndef ARMED_MIN_MOTOR_OUTPUT
#define ARMED_MIN_MOTOR_OUTPUT 1020     // motors spin slowly when armed
#endif

#ifndef THROTTLE_TO_MOTOR_OFFSET
#define THROTTLE_TO_MOTOR_OFFSET 0      // motors spin slowly when armed
#endif
// by default don't allow the motors to stop when armed if not in acro or semi acro mode
#ifndef MOTORS_STOP
#define MOTORS_STOP NO
#endif

// LED Outputs (4)
// LEDs 1 & 3 are tied together
// LEDs 2 & 4 are tied together
#ifdef LED1
	#define LED1_OUTPUT (DIGITALPORT5 | 1)
	#define LED1_ON DIGITALON
#endif

#ifdef LED2
	#define LED2_OUTPUT	(DIGITALPORT0 | 4)
	#define LED2_ON DIGITALON
#endif

#ifdef LED3
	#define LED3_OUTPUT LED1_OUTPUT
	#define LED3_ON LED1_ON
#endif

#ifdef LED4
	#define LED4_OUTPUT	LED2_OUTPUT
	#define LED4_ON LED2_ON
#endif

#ifdef LED5
	#define LED5_OUTPUT	(DIGITALPORT5 | 0)
	#define LED5_ON DIGITALON
#endif

#ifdef LED6
	#define LED6_OUTPUT (DIGITALPORT2 | 6)
	#define LED6_ON DIGITALON
#endif

#define DEBUGPORT 6
// end of Hubsan Q4 defs

// ======================================================= HUBSAN H107L ===============================================================
#elif CONTROL_BOARD_TYPE == CONTROL_BOARD_HUBSAN_H107L

#define GYRO_TYPE MPU3050       // gyro

#define GYRO_ORIENTATION(VALUES,X, Y, Z) {VALUES[ROLLINDEX] =  -Y; VALUES[PITCHINDEX] = X; VALUES[YAWINDEX] = -Z;}

#define ACCELEROMETER_TYPE MC3210      // accelerometer
// MC3210 in Hubsan X4:
// Positive Z = level position
// Positive Y = left side down
// Positive X = front side down
// In this firmware: Z=down, X=west
#define ACC_ORIENTATION(VALUES,X, Y, Z)  {VALUES[XINDEX]  = -Y; VALUES[YINDEX]  = X; VALUES[ZINDEX]  =  Z;}

#ifndef COMPASS_TYPE
#define COMPASS_TYPE NO_COMPASS
#endif

#ifndef BAROMETER_TYPE
#define BAROMETER_TYPE NO_BAROMETER
#endif

#ifndef MULTIWII_CONFIG_SERIAL_PORTS
#define MULTIWII_CONFIG_SERIAL_PORTS NOSERIALPORT
#endif

#ifndef GPS_TYPE
#define GPS_TYPE NO_GPS
#endif

#define RXNUMCHANNELS 6 

#ifndef ARMED_MIN_MOTOR_OUTPUT
#define ARMED_MIN_MOTOR_OUTPUT 1020     // motors spin slowly when armed
#endif

#ifndef THROTTLE_TO_MOTOR_OFFSET
#define THROTTLE_TO_MOTOR_OFFSET 0      // motors spin slowly when armed
#endif
// by default don't allow the motors to stop when armed if not in acro or semi acro mode
#ifndef MOTORS_STOP
#define MOTORS_STOP NO
#endif

// LED Outputs (4)
// LEDs 1 & 3 are tied together
// LEDs 2 & 4 are tied together
#ifdef LED1
	#define LED1_OUTPUT (DIGITALPORT3 | 0)
	#define LED1_ON DIGITALOFF
#endif

#ifdef LED2
	#define LED2_OUTPUT	(DIGITALPORT0 | 4)
	#define LED2_ON DIGITALOFF
#endif

#ifdef LED3
	#define LED3_OUTPUT LED1_OUTPUT
	#define LED3_ON LED1_ON
#endif

#ifdef LED4
	#define LED4_OUTPUT	LED2_OUTPUT
	#define LED4_ON LED2_ON
#endif

#ifdef LED5
	#define LED5_OUTPUT	(DIGITALPORT5 | 2)
	#define LED5_ON DIGITALON
#endif

#ifdef LED6
	#define LED6_OUTPUT (DIGITALPORT2 | 6)
	#define LED6_ON DIGITALON
#endif
// end of Hubsan X4 defs

// ======================================================= WLT V202 ===============================================================
#elif CONTROL_BOARD_TYPE == CONTROL_BOARD_WLT_V202

#define GYRO_TYPE MPU6050       // gyro
#define GYRO_ORIENTATION(VALUES,X, Y, Z) {VALUES[ROLLINDEX] =  -X; VALUES[PITCHINDEX] = Y; VALUES[YAWINDEX] = Z;}

#define ACCELEROMETER_TYPE MPU6050      // accelerometer
#define ACC_ORIENTATION(VALUES,X, Y, Z)  {VALUES[ROLLINDEX]  = Y; VALUES[PITCHINDEX]  = X; VALUES[YAWINDEX]  =  -Z;}

#ifndef COMPASS_TYPE
#define COMPASS_TYPE NO_COMPASS
#endif

#ifndef BAROMETER_TYPE
#define BAROMETER_TYPE NO_BAROMETER
#endif

#ifndef MULTIWII_CONFIG_SERIAL_PORTS
#define MULTIWII_CONFIG_SERIAL_PORTS SERIALPORT0
#endif

#ifndef GPS_TYPE
#define GPS_TYPE NO_GPS
#endif

#define RXNUMCHANNELS 8

#ifndef ARMED_MIN_MOTOR_OUTPUT
#define ARMED_MIN_MOTOR_OUTPUT 1020     // motors spin slowly when armed
#endif

#ifndef THROTTLE_TO_MOTOR_OFFSET
#define THROTTLE_TO_MOTOR_OFFSET 0      // motors spin slowly when armed
#endif
// by default don't allow the motors to stop when armed if not in acro or semi acro mode
#ifndef MOTORS_STOP
#define MOTORS_STOP NO
#endif

// LED Outputs
#ifdef LED1
	#define LED1_OUTPUT (DIGITALPORT0 | 0)
	#ifndef LED1_ON
	#define LED1_ON DIGITALON
	#endif
#endif

#define USERSETTINGS_PID_PGAIN_PITCHINDEX PID_TO_CONFIGURATORVALUE_P(3.3)
#define USERSETTINGS_PID_IGAIN_PITCHINDEX PID_TO_CONFIGURATORVALUE_I(0.03)
#define USERSETTINGS_PID_DGAIN_PITCHINDEX PID_TO_CONFIGURATORVALUE_D(23)

#define USERSETTINGS_PID_PGAIN_ROLLINDEX  PID_TO_CONFIGURATORVALUE_P(3.3)
#define USERSETTINGS_PID_IGAIN_ROLLINDEX  PID_TO_CONFIGURATORVALUE_I(0.03)
#define USERSETTINGS_PID_DGAIN_ROLLINDEX  PID_TO_CONFIGURATORVALUE_D(23)

#define USERSETTINGS_PID_PGAIN_YAWINDEX   PID_TO_CONFIGURATORVALUE_P(6.8)
#define USERSETTINGS_PID_IGAIN_YAWINDEX   PID_TO_CONFIGURATORVALUE_I(0.045)
#define USERSETTINGS_PID_DGAIN_YAWINDEX   PID_TO_CONFIGURATORVALUE_D(0)

// ======================================================= JXD 385 ===============================================================
#elif CONTROL_BOARD_TYPE == CONTROL_BOARD_JXD_JD385

#define GYRO_TYPE MPU6050       // gyro
#define GYRO_ORIENTATION(VALUES,X, Y, Z) {VALUES[ROLLINDEX] =  -X; VALUES[PITCHINDEX] = -Y; VALUES[YAWINDEX] = -Z;}

#define ACCELEROMETER_TYPE MPU6050      // accelerometer
#define ACC_ORIENTATION(VALUES,X, Y, Z)  {VALUES[ROLLINDEX]  = -Y; VALUES[PITCHINDEX]  = X; VALUES[YAWINDEX]  =  Z;}

#ifndef COMPASS_TYPE
#define COMPASS_TYPE NO_COMPASS
#endif

#ifndef BAROMETER_TYPE
#define BAROMETER_TYPE NO_BAROMETER
#endif

#ifndef MULTIWII_CONFIG_SERIAL_PORTS
#define MULTIWII_CONFIG_SERIAL_PORTS SERIALPORT0
#endif

#ifndef GPS_TYPE
#define GPS_TYPE NO_GPS
#endif

#define RXNUMCHANNELS 8

#ifndef ARMED_MIN_MOTOR_OUTPUT
#define ARMED_MIN_MOTOR_OUTPUT 1015     // motors spin slowly when armed
#endif

#ifndef THROTTLE_TO_MOTOR_OFFSET
#define THROTTLE_TO_MOTOR_OFFSET 0      // motors spin slowly when armed
#endif
// by default don't allow the motors to stop when armed if not in acro or semi acro mode
#ifndef MOTORS_STOP
#define MOTORS_STOP NO
#endif

// LED Outputs (2)
// LEDs 1 & 3 are tied together
// LEDs 2 & 4 are tied together
#ifdef LED1
	#define LED1_OUTPUT (DIGITALPORT0 | 0)
	#define LED1_ON DIGITALOFF
#endif

// These LEDs are not populated on the PCB
#ifdef LED2
	#define LED2_OUTPUT	(DIGITALPORT5 | 2)
	#define LED2_ON DIGITALOFF
#endif

#ifdef LED3
	#define LED3_OUTPUT LED1_OUTPUT
	#define LED3_ON LED1_ON
#endif

#ifdef LED4
	#define LED4_OUTPUT	LED2_OUTPUT
	#define LED4_ON LED2_ON
#endif

#else // all other control boards

#warning "all other control board"

#define GYRO_TYPE MPU6050       // gyro
#define GYRO_ORIENTATION(VALUES,X, Y, Z) {VALUES[ROLLINDEX] =  Y; VALUES[PITCHINDEX] = -X; VALUES[YAWINDEX] = -Z;}
#define ACCELEROMETER_TYPE MPU6050      // accelerometer
#define ACC_ORIENTATION(VALUES,X, Y, Z)  {VALUES[ROLLINDEX]  = -X; VALUES[PITCHINDEX]  = -Y; VALUES[YAWINDEX]  =  Z;}
#ifndef COMPASS_TYPE
#define COMPASS_TYPE HMC5883  // compass
#endif
#define COMPASS_ORIENTATION(VALUES,X, Y, Z)  {VALUES[ROLLINDEX]  =  X; VALUES[PITCHINDEX]  = Y; VALUES[YAWINDEX]  = -Z;}
#ifndef BAROMETER_TYPE
#define BAROMETER_TYPE MS5611
#endif
#ifndef MULTIWII_CONFIG_SERIAL_PORTS
#define MULTIWII_CONFIG_SERIAL_PORTS SERIALPORT0+SERIALPORT3
#endif
#ifndef GPS_TYPE
#define GPS_TYPE SERIAL_GPS
#define GPS_SERIAL_PORT 2
#define GPS_BAUD 38400
#endif

#define RXNUMCHANNELS 8

// LED Outputs

#ifdef LED1
	#define LED1_OUTPUT (DIGITALPORTB | 3)
		#ifndef LED1_ON
		#define LED1_ON DIGITALON
	#endif
#endif

#ifdef LED2
	#define LED2_OUTPUT (DIGITALPORTB | 4)
		#ifndef LED2_ON
		#define LED2_ON DIGITALON
	#endif
#endif

#endif // CONTROL_BOARD_TYPE

// default to QUADX if no configuration was chosen
#ifndef AIRCRAFT_CONFIGURATION
#define AIRCRAFT_CONFIGURATION QUADX
#endif
// set aircraft type dependant defines here
#if (AIRCRAFT_CONFIGURATION==QUADX)
#define NUMMOTORS 4
#endif
// set configuration port baud rates to defaults if none have been set
#if (MULTIWII_CONFIG_SERIAL_PORTS & SERIALPORT0)
#ifndef SERIAL_0_BAUD
#define SERIAL_0_BAUD 115200
#endif
#endif
#if (MULTIWII_CONFIG_SERIAL_PORTS & SERIALPORT1)
#ifndef SERIAL_1_BAUD
#define SERIAL_1_BAUD 115200
#endif
#endif
#if (MULTIWII_CONFIG_SERIAL_PORTS & SERIALPORT2)
#ifndef SERIAL_2_BAUD
#define SERIAL_2_BAUD 115200
#endif
#endif
#if (MULTIWII_CONFIG_SERIAL_PORTS & SERIALPORT3)
#ifndef SERIAL_3_BAUD
#define SERIAL_3_BAUD 115200
#endif
#endif
#if (GPS_TYPE==SERIAL_GPS)
#ifndef GPS_SERIAL_PORT
#define GPS_SERIAL_PORT 2
#endif
#ifndef GPS_BAUD
#define GPS_BAUD 115200
#endif
#endif
// use default values if not set anywhere else
#ifndef ARMED_MIN_MOTOR_OUTPUT
#define ARMED_MIN_MOTOR_OUTPUT 1067     // motors spin slowly when armed
#endif
#ifndef THROTTLE_TO_MOTOR_OFFSET
#define THROTTLE_TO_MOTOR_OFFSET 0      // motors spin slowly when armed
#endif
// by default don't allow the motors to stop when armed if not in acro or semi acro mode
#ifndef MOTORS_STOP
#define MOTORS_STOP NO
#endif
// default low pass filter
#ifndef GYRO_LOW_PASS_FILTER
#define GYRO_LOW_PASS_FILTER 0
#endif
// default gain scheduling
#ifndef GAIN_SCHEDULING_FACTOR
#define GAIN_SCHEDULING_FACTOR 1.0
#endif

//conversion from fp to internal representation:
// NOTE: the values shown in config gui and the real floatingpoint
//       numbers do NOT MATCH ?! use this conversion here:
#define PID_TO_CONFIGURATORVALUE_P(a) ((fixedpointnum)((((a)*(10<<3)))))
#define PID_TO_CONFIGURATORVALUE_I(a) ((fixedpointnum)((a)*1000))
#define PID_TO_CONFIGURATORVALUE_D(a) ((fixedpointnum)((a)*(1<<2)))
//
#define PID_TO_CONFIGURATORVALUE_ALT_P(a) ((fixedpointnum)(((a)*(10<<7))))
#define PID_TO_CONFIGURATORVALUE_ALT_I(a) ((fixedpointnum)((a)*1000))
#define PID_TO_CONFIGURATORVALUE_ALT_D(a) ((fixedpointnum)((a)*(1<<9)))
//
#define PID_TO_CONFIGURATORVALUE_NAV_P(a) ((fixedpointnum)((a)*(10<<7)))
#define PID_TO_CONFIGURATORVALUE_NAV_I(a) ((fixedpointnum)((a)* 1000))
#define PID_TO_CONFIGURATORVALUE_NAV_D(a) ((fixedpointnum)((a)* (1<<9)))

//switch to use eeprom or not
#ifndef EEPROM_SIZE
#define EEPROM_SIZE NO_EEPROM
#endif

// time in ms the stick command have to persist to take action
#ifndef STICK_ARM_TIME
#define STICK_ARM_TIME 500
#endif
