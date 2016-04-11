/* 
Copyright 2013 Brad Quick

Some of this code is based on Multiwii code by Alexandre Dubus (www.multiwii.com)

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

/*

The code is for controlling multi-copters.  Many of the ideas in the code come from the Multi-Wii project
(see multiwii.com).  This project doesn't contain all of the features in Multi-Wii, but I believe it incorporates
a number of improvements.

In order to make the code run quickly on 8 bit processors, much of the math is done using fixed point numbers
instead of floating point.  Much pain was taken to write almost the entire code without performing any
division, which is slow. As a result, main loop cycles can take well under 2 milliseconds.

A second advantage is that I believe that this code is more logically layed out and better commented than 
some other multi-copter code.  It is designed to be easy to follow for the guy who wants to understand better how
the code works and maybe wants to modify it.

In general, I didn't include code that I haven't tested myself, therefore many aircraft configurations, control boards,
sensors, etc. aren't yet included in the code.  It should be fairly easy, however for interested coders to add the
components that they need.

If you find the code useful, I'd love to hear from you.  Email me at the address that's shown vertically below:

b         I made my
r         email address
a         vertical so
d         the spam bots
@         won't figure
j         it out.
a         - Thanks.
m
e
s
l
t
a
y
l
o
r
.
c
o
m

*/


// project file headers
#include "symawii.h"
#include "rx.h"
#include "serial.h"
#include "output.h"
#include "gyro.h"
#include "accelerometer.h"
#include "imu.h"
#include "baro.h"
#include "compass.h"
#include "eeprom.h"
#include "gps.h"
#include "navigation.h"
#include "pilotcontrol.h"
#include "autotune.h"
#include "leds.h"


//Lets add a test

// library headers
#include "hal.h"
#include "lib_timers.h"
#if (MULTIWII_CONFIG_SERIAL_PORTS != NOSERIALPORT) || (DEBUGPORT == 6)
#include "lib_serial.h"
#endif
#include "lib_i2c.h"
#include "lib_digitalio.h"
#include "lib_fp.h"
#if (BATTERY_ADC_CHANNEL != NO_ADC)
#include "lib_adc.h"
#endif

// Data type for stick movement detection to execute accelerometer calibration
typedef enum stickstate_tag {
    STICK_STATE_START,   // No stick movement detected yet
    STICK_STATE_LOW,     // Stick was low recently
    STICK_STATE_HIGH     // Stick was high recently
} stickstate_t;

globalstruct global;            // global variables
usersettingsstruct usersettings;        // user editable variables

fixedpointnum altitudeholddesiredaltitude;
fixedpointnum integratedaltitudeerror;  // for pid control

fixedpointnum integratedangleerror[3];

// limit pid windup
#define INTEGRATEDANGLEERRORLIMIT FIXEDPOINTCONSTANT(1000)

#if (BATTERY_ADC_CHANNEL != NO_ADC)
	// Factor from ADC input voltage to battery voltage
	#define FP_BATTERY_VOLTAGE_FACTOR FIXEDPOINTCONSTANT(BATTERY_VOLTAGE_FACTOR)

	// If battery voltage gets below this value the LEDs will blink
	#define FP_BATTERY_UNDERVOLTAGE_LIMIT FIXEDPOINTCONSTANT(BATTERY_UNDERVOLTAGE_LIMIT)
#endif

// Stick is moved out of middle position towards low
#define FP_RXMOVELOW FIXEDPOINTCONSTANT(-0.2)
// Stick is moved out of middle position towards high
#define FP_RXMOVEHIGH FIXEDPOINTCONSTANT(0.2)

// timesliver is a very small slice of time (.002 seconds or so).  This small value doesn't take much advantage
// of the resolution of fixedpointnum, so we shift timesliver an extra TIMESLIVEREXTRASHIFT bits.
unsigned long timeslivertimer = 0;

// Local functions
static void detectstickcommand(void);


// It all starts here:
int main(void)
{

#if (BATTERY_ADC_CHANNEL != NO_ADC)
    // Static to keep it off the stack
    static bool isbatterylow;         // Set to true while voltage is below limit
    static bool isadcchannelref;      // Set to true if the next ADC result is reference channel
    // Current unfiltered battery voltage [V]. Filtered value is in global.batteryvoltage
    static fixedpointnum batteryvoltage;
    // Current raw battery voltage.
    static fixedpointnum batteryvoltageraw;
    // Current raw bandgap reference voltage.
    static fixedpointnum bandgapvoltageraw;
    // Initial bandgap voltage [V]. We measure this once when there is no load on the battery
    // because the specified tolerance for this is pretty high.
    static fixedpointnum initialbandgapvoltage;
		// Enables battery low indicator after the voltage droped under the specified limit for a amount of time
		unsigned long batterylowtimer;
		
		// not working why?
	  // static lib_adc_channel_t adc_bat_channel = (lib_adc_channel_t) BATTERY_ADC_CHANNEL;
		// static lib_adc_channel_t adc_ref_channel = (lib_adc_channel_t) 8;
#endif
	
    static bool isfailsafeactive;     // true while we don't get new data from transmitter
		
    // initialize hardware
		lib_hal_init();

    //initialize the libraries that require initialization
    lib_timers_init();
    lib_i2c_init();
	
		//initialize the leds
		leds_init();		
		
		// start with default user settings in case there's nothing in eeprom
		defaultusersettings();		
    
    // try to load usersettings from eeprom
    readusersettingsfromeeprom();

    if(!global.usersettingsfromeeprom) {
			// If nothing found in EEPROM (= data flash on Mini51)
			// use default settings.
			// start with default user settings in case there's nothing in eeprom
			defaultusersettings();
		
			// Indicate that default settings are used
			leds_blink_cycles(LED1, 100, 100, 10);
    }

	
    // pause a moment before initializing everything. To make sure everything is powered up
    lib_timers_delaymilliseconds(100); 
		
    // initialize all other modules
    initrx();
		
#if (BATTERY_ADC_CHANNEL != NO_ADC)
    // Give the battery voltage lowpass filter a reasonable starting point.
    global.batteryvoltage = FP_BATTERY_UNDERVOLTAGE_LIMIT;
    lib_adc_init();  // For battery voltage
#endif
   
		initoutputs();

#if (MULTIWII_CONFIG_SERIAL_PORTS != NOSERIALPORT)
    serialinit();
#endif

    initgyro();
    initacc();
    initimu();
		
#if (BATTERY_ADC_CHANNEL != NO_ADC)
		// Measure internal bandgap voltage now.
    // Battery is probably full and there is no load,
    // so we can expect to have a good external ADC reference
    // voltage now.
    lib_adc_select_channel(LIB_ADC_CHANREF);
    initialbandgapvoltage = 0;
		batterylowtimer  = 0;		
    isbatterylow = false;	
		
    // Take average of 8 measurements
    for(int i=0;i<8;i++) {
        lib_adc_startconv();
        while(lib_adc_is_busy()){
				}
        initialbandgapvoltage += lib_adc_read_volt();
    }
    initialbandgapvoltage >>= 3; //divide by 8
    bandgapvoltageraw = lib_adc_read_raw();
    // Start first battery voltage measurement
    isadcchannelref = false;
    //lib_adc_select_channel(adc_bat_channel);
		lib_adc_select_channel(BATTERY_ADC_CHANNEL);
    lib_adc_startconv();
		#if (BATTERY_ADC_DEBUG)
		while(lib_adc_is_busy()){
		}
		batteryvoltageraw = lib_adc_read_raw();
		
		lib_serial_sendstring(DEBUGPORT, "POWER ON ADC MEASURMENTS:================\r\n");
		lib_serial_sendstring(DEBUGPORT, "BANDGAP=");
		serialprintfixedpoint_no_linebreak(0, initialbandgapvoltage);
		lib_serial_sendstring(DEBUGPORT, "\r\nBGAPVOLTAGERAW=");
		serialprintfixedpoint_no_linebreak(0, bandgapvoltageraw);
		lib_serial_sendstring(DEBUGPORT, "\r\nBATTERY RAW=");
		serialprintfixedpoint_no_linebreak(0, batteryvoltageraw);
		lib_serial_sendstring(DEBUGPORT, "=========================================\r\n");
		lib_timers_delaymilliseconds(2000);
	#endif
#endif
    // set the default i2c speed to 400 kHz.  If a device needs to slow it down, it can, but it should set it back.
    lib_i2c_setclockspeed(I2C_400_KHZ);

    global.armed = 0;
    global.navigationmode = NAVIGATIONMODEOFF;
    global.failsafetimer = lib_timers_starttimer();
		

		/*
		//testcode to see if uart works
		unsigned int led_state = 0;
		for (;;) {
			//lib_serial_sendchar(0, 'H');
			char x = lib_serial_numcharsavailable(0);
			//lib_serial_sendchar(0, '0'+x);
			if (x != 0){
				lib_serial_sendstring(DEBUGPORT, "POWER ON ADC MEASURMENTS:================\r\n");
			  unsigned char c = lib_serial_getchar(0);
				lib_serial_sendchar(0, c);
				lib_serial_sendchar(0, '\r');
				lib_serial_sendchar(0, '\n');
			}
			leds_set(led_state);
			led_state = 0xFF-led_state;
			lib_timers_delaymilliseconds(1);
		}
		*/
		
    for (;;) {

        // check to see what switches are activated
        checkcheckboxitems();

#if (MULTIWII_CONFIG_SERIAL_PORTS != NOSERIALPORT)
        // check for config program activity
        serialcheckforaction();
#endif
        calculatetimesliver();

        // run the imu to estimate the current attitude of the aircraft
        imucalculateestimatedattitude();

        // arm and disarm via rx aux switches
        if (global.rxvalues[THROTTLEINDEX] < FPSTICKLOW) {      // see if we want to change armed modes
            if (!global.armed) {
                if (global.activecheckboxitems & CHECKBOXMASKARM) {
                    global.armed = 1;
                    global.heading_when_armed = global.currentestimatedeulerattitude[YAWINDEX];
                    global.altitude_when_armed = global.barorawaltitude;
                }
            } else if (!(global.activecheckboxitems & CHECKBOXMASKARM)) {
                global.armed = 0;
			}
        } // if throttle low

        if(!global.armed) {
            // Not armed: check if there is a stick command to execute.
            detectstickcommand();
        }

        // read the receiver
        readrx();
		
       // get the angle error.  Angle error is the difference between our current attitude and our desired attitude.
        // It can be set by navigation, or by the pilot, etc.
        fixedpointnum angleerror[3];

        // let the pilot control the aircraft.
        getangleerrorfrompilotinput(angleerror);

        if (global.rxvalues[THROTTLEINDEX] < FPSTICKLOW) {
            // We are probably on the ground. Don't accumnulate error when we can't correct it
            resetpilotcontrol();

            // bleed off integrated error by averaging in a value of zero
            lib_fp_lowpassfilter(&integratedangleerror[ROLLINDEX], 0L, global.timesliver >> TIMESLIVEREXTRASHIFT, FIXEDPOINTONEOVERONEFOURTH, 0);
            lib_fp_lowpassfilter(&integratedangleerror[PITCHINDEX], 0L, global.timesliver >> TIMESLIVEREXTRASHIFT, FIXEDPOINTONEOVERONEFOURTH, 0);
            lib_fp_lowpassfilter(&integratedangleerror[YAWINDEX], 0L, global.timesliver >> TIMESLIVEREXTRASHIFT, FIXEDPOINTONEOVERONEFOURTH, 0);
        }
        
        // get the pilot's throttle component
        // convert from fixedpoint -1 to 1 to fixedpoint 0 to 1
        fixedpointnum throttleoutput = (global.rxvalues[THROTTLEINDEX] >> 1) + FIXEDPOINTONEOVERTWO + FPTHROTTLETOMOTOROFFSET



        if ((global.activecheckboxitems & CHECKBOXMASKAUTOTHROTTLE) ||altitudeholdactive) {
            // Auto Throttle Adjust - Increases the throttle when the aircraft is tilted so that the vertical
            // component of thrust remains constant.
            // The AUTOTHROTTLEDEADAREA adjusts the value at which the throttle starts taking effect.  If this
            // value is too low, the aircraft will gain altitude when banked, if it's too low, it will lose
            // altitude when banked. Adjust to suit.
#define AUTOTHROTTLEDEADAREA FIXEDPOINTCONSTANT(.25)

            if (global.estimateddownvector[ZINDEX] > FIXEDPOINTCONSTANT(.3)) {
                // Divide the throttle by the throttleoutput by the z component of the down vector
                // This is probaly the slow way, but it's a way to do fixed point division
                fixedpointnum recriprocal = lib_fp_invsqrt(global.estimateddownvector[ZINDEX]);
                recriprocal = lib_fp_multiply(recriprocal, recriprocal);

                throttleoutput = lib_fp_multiply(throttleoutput - AUTOTHROTTLEDEADAREA, recriprocal) + AUTOTHROTTLEDEADAREA;
            }
        }
        // if we don't hear from the receiver for over a second, try to land safely
        if (lib_timers_gettimermicroseconds(global.failsafetimer) > 1000000L) {
            throttleoutput = FPFAILSAFEMOTOROUTPUT;
            isfailsafeactive = true;

            // make sure we are level!
            angleerror[ROLLINDEX] = -global.currentestimatedeulerattitude[ROLLINDEX];
            angleerror[PITCHINDEX] = -global.currentestimatedeulerattitude[PITCHINDEX];
        }
        else
            isfailsafeactive = false;

        // calculate output values.  Output values will range from 0 to 1.0

        // calculate pid outputs based on our angleerrors as inputs
        fixedpointnum pidoutput[3];

        // Gain Scheduling essentialy modifies the gains depending on
        // throttle level. If GAIN_SCHEDULING_FACTOR is 1.0, it multiplies PID outputs by 1.5 when at full throttle,
        // 1.0 when at mid throttle, and .5 when at zero throttle.  This helps
        // eliminate the wobbles when decending at low throttle.
        fixedpointnum gainschedulingmultiplier = lib_fp_multiply(throttleoutput - FIXEDPOINTCONSTANT(.5), FIXEDPOINTCONSTANT(GAIN_SCHEDULING_FACTOR)) + FIXEDPOINTONE;

        for (int x = 0; x < 3; ++x) {
            integratedangleerror[x] += lib_fp_multiply(angleerror[x], global.timesliver);

            // don't let the integrated error get too high (windup)
            lib_fp_constrain(&integratedangleerror[x], -INTEGRATEDANGLEERRORLIMIT, INTEGRATEDANGLEERRORLIMIT);

            // do the attitude pid
            pidoutput[x] = lib_fp_multiply(angleerror[x], usersettings.pid_pgain[x])
                - lib_fp_multiply(global.gyrorate[x], usersettings.pid_dgain[x])
            + (lib_fp_multiply(integratedangleerror[x], usersettings.pid_igain[x]) >> 4);

            // add gain scheduling.  
            pidoutput[x] = lib_fp_multiply(gainschedulingmultiplier, pidoutput[x]);
        }

#if (CONTROL_BOARD_TYPE == CONTROL_BOARD_HUBSAN_H107L) || (CONTROL_BOARD_TYPE == CONTROL_BOARD_HUBSAN_Q4)

		// On Hubsan X4 H107L the front right motor
		// rotates clockwise (viewed from top).
		// On the J385 the motors spin in the opposite direction.
		// PID output for yaw has to be reversed
        pidoutput[YAWINDEX] = -pidoutput[YAWINDEX];
#endif

        lib_fp_constrain(&throttleoutput, 0, FIXEDPOINTONE);

        // set the final motor outputs
        // if we aren't armed, or if we desire to have the motors stop, 
        if (!global.armed
#if (MOTORS_STOP==YES)
            || (global.rxvalues[THROTTLEINDEX] < FPSTICKLOW && !(global.activecheckboxitems & (CHECKBOXMASKFULLACRO | CHECKBOXMASKSEMIACRO)))
#endif
            )
            setallmotoroutputs(MIN_MOTOR_OUTPUT);
        else {
            // mix the outputs to create motor values
#if (AIRCRAFT_CONFIGURATION==QUADX)
            setmotoroutput(0, 0, throttleoutput - pidoutput[ROLLINDEX] + pidoutput[PITCHINDEX] - pidoutput[YAWINDEX]);
            setmotoroutput(1, 1, throttleoutput - pidoutput[ROLLINDEX] - pidoutput[PITCHINDEX] + pidoutput[YAWINDEX]);
            setmotoroutput(2, 2, throttleoutput + pidoutput[ROLLINDEX] + pidoutput[PITCHINDEX] + pidoutput[YAWINDEX]);
            setmotoroutput(3, 3, throttleoutput + pidoutput[ROLLINDEX] - pidoutput[PITCHINDEX] - pidoutput[YAWINDEX]);
#endif // QUADX config
        }

#if (BATTERY_ADC_CHANNEL != NO_ADC)
        // Measure battery voltage
        if(!lib_adc_is_busy())
        {
            // What did we just measure?
            // Always alternate between reference channel
            // and battery voltage
            if(isadcchannelref) {
                bandgapvoltageraw = lib_adc_read_raw();
                isadcchannelref = false;
                //lib_adc_select_channel(adc_bat_channel);
								lib_adc_select_channel(BATTERY_ADC_CHANNEL);
            } else {
								//raw voltage is 0.0-1.0 (min to max adc )
                batteryvoltageraw = lib_adc_read_raw();
                isadcchannelref = true;
								lib_adc_select_channel(LIB_ADC_CHANREF);

                // Unfortunately we have to use fixed point division now
                batteryvoltage = (batteryvoltageraw << 12) / (bandgapvoltageraw >> (FIXEDPOINTSHIFT-12));
                // Now we have battery voltage relative to bandgap reference voltage.
                // Multiply by initially measured bandgap voltage to get the voltage at the ADC pin.
                batteryvoltage = lib_fp_multiply(batteryvoltage, initialbandgapvoltage);
                // Now take the voltage divider into account to get battery voltage.
                batteryvoltage = lib_fp_multiply(batteryvoltage, FP_BATTERY_VOLTAGE_FACTOR);

                // Since we measure under load, the voltage is not stable.
                // Apply 0.5 second lowpass filter.
                // Use constant FIXEDPOINTONEOVERONEFOURTH instead of FIXEDPOINTONEOVERONEHALF
                // Because we call this only every other iteration.
                // (...alternatively multiply global.timesliver by two).      
								lib_fp_lowpassfilter(&(global.batteryvoltage), batteryvoltage, global.timesliver, FIXEDPOINTONEOVERONEFOURTH, TIMESLIVEREXTRASHIFT);	

#if (BATTERY_ADC_DEBUG)
	lib_serial_sendstring(DEBUGPORT, "\r\nBANDGAP=");
	serialprintfixedpoint_no_linebreak(0, bandgapvoltageraw);
	lib_serial_sendstring(DEBUGPORT, " BATTERY RAW=");
	serialprintfixedpoint_no_linebreak(0, batteryvoltageraw);
	lib_serial_sendstring(DEBUGPORT, " BATTERY=");
	serialprintfixedpoint_no_linebreak(0, batteryvoltage);
	lib_serial_sendstring(DEBUGPORT, " FILTERED BAT=");
	serialprintfixedpoint_no_linebreak(0, global.batteryvoltage);
#endif							
								
							  // Start timer if battery is below limit
                if(global.batteryvoltage < FP_BATTERY_UNDERVOLTAGE_LIMIT) {
									// if(batterylowtimer == 0) batterylowtimer = lib_timers_starttimer();
									batterylowtimer++;
									
									if(batterylowtimer > BATTERY_LOW_TIMER)
										isbatterylow = true;
								}
								else // if bettery is above limit reset batterylowtimer
								{
									batterylowtimer = 0;
								}
								
						}
            // Start next conversion
            lib_adc_startconv();
        } // IF ADC result available			
				
				// it's not working, why? used upper "construction"
				//if (lib_timers_gettimermicroseconds(batterylowtimer) > BATTERY_LOW_TIMER * 1000L) isbatterylow = true;
				

        // Decide what LEDs have to show
        if(isbatterylow) {
            // Highest priority: Battery voltage
            // Blink all LEDs slow
						global.armed ? leds_blink_continuous(LED_ALL, 1000, 500) : leds_blink_continuous(LED_ALL, 500, 1000);
        }
#endif 
				
#if (BATTERY_ADC_CHANNEL != NO_ADC)				
				else if(isfailsafeactive) {
#else
				if(isfailsafeactive) {
#endif					
            // Lost contact with TX
            // Blink LEDs fast alternating
						leds_blink_continuous(LED_ALL, 125, 125);
						//lib_serial_sendstring(DEBUGPORT, "isfailsafeactive true\r\n");
        }
        else if(!global.armed) {
					  //lib_serial_sendstring(DEBUGPORT, "isfailsafeactive false\r\n");

            // Not armed
            // Short blinks
						leds_blink_continuous(LED_ALL, 50, 450);
						}
        else {
            // LEDs stay on
						leds_set(LED_ALL);
        }
				
    } // Endless loop
} // main()

void calculatetimesliver(void)
{
    // load global.timesliver with the amount of time that has passed since we last went through this loop
    // convert from microseconds to fixedpointnum seconds shifted by TIMESLIVEREXTRASHIFT
    // 4295L is (FIXEDPOINTONE<<FIXEDPOINTSHIFT)*.000001
    global.timesliver = (lib_timers_gettimermicrosecondsandreset(&timeslivertimer) * 4295L) >> (FIXEDPOINTSHIFT - TIMESLIVEREXTRASHIFT);

    // don't allow big jumps in time because of something slowing the update loop down (should never happen anyway)
    if (global.timesliver > (FIXEDPOINTONEFIFTIETH << TIMESLIVEREXTRASHIFT))
        global.timesliver = FIXEDPOINTONEFIFTIETH << TIMESLIVEREXTRASHIFT;
}

void defaultusersettings(void)
{
    global.usersettingsfromeeprom = 0;  // this should get set to one if we read from eeprom

    // set default acro mode rotation rates
#ifdef USERSETTINGS_MAXYAWRATE
    usersettings.maxyawrate = USERSETTINGS_MAXYAWRATE;  // degrees per second    
#else
		usersettings.maxyawrate = 600L << FIXEDPOINTSHIFT;  // degrees per second   
#endif
#ifdef USERSETTINGS_MAXPITCHANDROLLRATE	
    usersettings.maxpitchandrollrate = USERSETTINGS_MAXPITCHANDROLLRATE; // degrees per second
#else
		usersettings.maxpitchandrollrate = 400L << FIXEDPOINTSHIFT; // degrees per second
#endif

    // set default PID settings
		for (int x = 0; x < 3; ++x) {
        usersettings.pid_pgain[x] = PID_TO_CONFIGURATORVALUE_P(1.5); 		//1.5 on configurator
        usersettings.pid_igain[x] = PID_TO_CONFIGURATORVALUE_I(0.008); 	//.008 on configurator
        usersettings.pid_dgain[x] = PID_TO_CONFIGURATORVALUE_D(8.0); 		//8 on configurator        
    }

		// pitch PIDs
#ifdef USERSETTINGS_PID_PGAIN_PITCHINDEX
    usersettings.pid_pgain[PITCHINDEX] = USERSETTINGS_PID_PGAIN_PITCHINDEX;
#endif
#ifdef USERSETTINGS_PID_IGAIN_PITCHINDEX
    usersettings.pid_igain[PITCHINDEX] = USERSETTINGS_PID_IGAIN_PITCHINDEX;
#endif
#ifdef USERSETTINGS_PID_DGAIN_PITCHINDEX
    usersettings.pid_dgain[PITCHINDEX] = USERSETTINGS_PID_DGAIN_PITCHINDEX;
#endif
		
 // roll PIDs
#ifdef USERSETTINGS_PID_PGAIN_ROLLINDEX		
    usersettings.pid_pgain[ROLLINDEX] = USERSETTINGS_PID_PGAIN_ROLLINDEX;
#endif
#ifdef USERSETTINGS_PID_IGAIN_ROLLINDEX
    usersettings.pid_igain[ROLLINDEX] = USERSETTINGS_PID_IGAIN_ROLLINDEX;
#endif
#ifdef USERSETTINGS_PID_DGAIN_ROLLINDEX
    usersettings.pid_dgain[ROLLINDEX] = USERSETTINGS_PID_DGAIN_ROLLINDEX;	
#endif
	
	
    // yaw PIDs
#ifdef USERSETTINGS_PID_PGAIN_YAWINDEX
    usersettings.pid_pgain[YAWINDEX] = USERSETTINGS_PID_PGAIN_YAWINDEX;
#endif
#ifdef USERSETTINGS_PID_IGAIN_YAWINDEX 
    usersettings.pid_igain[YAWINDEX] = USERSETTINGS_PID_IGAIN_YAWINDEX;
#endif
#ifdef USERSETTINGS_PID_DGAIN_YAWINDEX
    usersettings.pid_dgain[YAWINDEX] = USERSETTINGS_PID_DGAIN_YAWINDEX;	
#endif

		for (int x = 3; x < NUMPIDITEMS; ++x) {
        usersettings.pid_pgain[x] = 0;
        usersettings.pid_igain[x] = 0;
        usersettings.pid_dgain[x] = 0;
    }


#ifdef USERSETTINGS_PID_PGAIN_ALTITUDEINDEX		
    usersettings.pid_pgain[ALTITUDEINDEX] = USERSETTINGS_PID_PGAIN_ALTITUDEINDEX;
#else
		usersettings.pid_pgain[ALTITUDEINDEX] = PID_TO_CONFIGURATORVALUE_ALT_P(2.7); // 2.7 on configurator
#endif
		
#ifdef USERSETTINGS_PID_DGAIN_ALTITUDEINDEX		
    usersettings.pid_dgain[ALTITUDEINDEX] = USERSETTINGS_PID_DGAIN_ALTITUDEINDEX;    		
#else
		usersettings.pid_dgain[ALTITUDEINDEX] = PID_TO_CONFIGURATORVALUE_ALT_D(6.0); // 6 on configurator
#endif

#ifdef USERSETTINGS_PID_PGAIN_NAVIGATIONINDEX
    usersettings.pid_pgain[NAVIGATIONINDEX] = USERSETTINGS_PID_PGAIN_NAVIGATIONINDEX;   
#else
		usersettings.pid_pgain[NAVIGATIONINDEX] = PID_TO_CONFIGURATORVALUE_NAV_P(2.5); // 2.5 on configurator
#endif
		
#ifdef USERSETTINGS_PID_DGAIN_NAVIGATIONINDEX	
    usersettings.pid_dgain[NAVIGATIONINDEX] = USERSETTINGS_PID_DGAIN_NAVIGATIONINDEX;   
#else
	usersettings.pid_dgain[NAVIGATIONINDEX] = PID_TO_CONFIGURATORVALUE_NAV_D(0.188); // .188 on configurator
#endif


    // set default configuration checkbox settings.
    for (int x = 0; x < NUMPOSSIBLECHECKBOXES; ++x) {
        usersettings.checkboxconfiguration[x] = 0;
    }

#ifdef USERSETTINGS_CHECKBOXARM
  usersettings.checkboxconfiguration[CHECKBOXARM] = USERSETTINGS_CHECKBOXARM;
#endif
#ifdef USERSETTINGS_CHECKBOXAUTOTHROTTLE
  usersettings.checkboxconfiguration[CHECKBOXAUTOTHROTTLE] = USERSETTINGS_CHECKBOXAUTOTHROTTLE;		
#endif
#ifdef USERSETTINGS_CHECKBOXALTHOLD
  usersettings.checkboxconfiguration[CHECKBOXALTHOLD] = USERSETTINGS_CHECKBOXALTHOLD;		
#endif
#ifdef USERSETTINGS_CHECKBOXCOMPASS
  usersettings.checkboxconfiguration[CHECKBOXCOMPASS] = USERSETTINGS_CHECKBOXCOMPASS;		
#endif		
#ifdef USERSETTINGS_CHECKBOXPOSITIONHOLD
  usersettings.checkboxconfiguration[CHECKBOXPOSITIONHOLD] = USERSETTINGS_CHECKBOXPOSITIONHOLD;		
#endif
#ifdef USERSETTINGS_CHECKBOXRETURNTOHOME
	  usersettings.checkboxconfiguration[CHECKBOXRETURNTOHOME] = USERSETTINGS_CHECKBOXRETURNTOHOME;	
#endif
#ifdef USERSETTINGS_CHECKBOXSEMIACRO
  usersettings.checkboxconfiguration[CHECKBOXSEMIACRO] = USERSETTINGS_CHECKBOXSEMIACRO;		
#endif
#ifdef USERSETTINGS_CHECKBOXFULLACRO
  usersettings.checkboxconfiguration[CHECKBOXFULLACRO] = USERSETTINGS_CHECKBOXFULLACRO;		
#endif
#ifdef USERSETTINGS_CHECKBOXHIGHRATES
	  usersettings.checkboxconfiguration[CHECKBOXHIGHRATES] = USERSETTINGS_CHECKBOXHIGHRATES;	
#endif
#ifdef USERSETTINGS_CHECKBOXHIGHANGLE
		usersettings.checkboxconfiguration[CHECKBOXHIGHANGLE] = USERSETTINGS_CHECKBOXHIGHANGLE;		
#endif
#ifdef USERSETTINGS_CHECKBOXAUTOTUNE
	  usersettings.checkboxconfiguration[CHECKBOXAUTOTUNE] = USERSETTINGS_CHECKBOXAUTOTUNE;	
#endif
#ifdef USERSETTINGS_CHECKBOXUNCRASHABLE
	  usersettings.checkboxconfiguration[CHECKBOXUNCRASHABLE] = USERSETTINGS_CHECKBOXUNCRASHABLE;	
#endif
#ifdef USERSETTINGS_CHECKBOXHEADFREE
  usersettings.checkboxconfiguration[CHECKBOXHEADFREE] = USERSETTINGS_CHECKBOXHEADFREE;		
#endif
#ifdef USERSETTINGS_CHECKBOXYAWHOLD
	  usersettings.checkboxconfiguration[CHECKBOXYAWHOLD] = USERSETTINGS_CHECKBOXYAWHOLD;	
#endif

	// reset the calibration settings
    for (int x = 0; x < 3; ++x) {
        usersettings.compasszerooffset[x] = 0;
        usersettings.compasscalibrationmultiplier[x] = 1L << FIXEDPOINTSHIFT;
        usersettings.gyrocalibration[x] = 0;
        usersettings.acccalibration[x] = 0;
    }
#if CONTROL_BOARD_TYPE == CONTROL_BOARD_WLT_V202
    usersettings.boundprotocol = 0; // PROTO_NONE
    usersettings.txidsize = 0;
    usersettings.fhsize = 0;
#endif
}

// Executes command based on stick movements.
// Call this only when not armed.
// Currently implemented: accelerometer calibration
static void detectstickcommand(void) {
    // Timeout for stick movements for accelerometer calibration
    static uint32_t stickcommandtimer;
    // Keeps track of roll stick movements while not armed to execute accelerometer calibration.
    static stickstate_t lastrollstickstate = STICK_STATE_START;
    // Counts roll stick movements
    static uint8_t rollmovecounter;

    // Accelerometer calibration (3x back and forth movement of roll stick while
    // throttle is in lowest position)
    if (global.rxvalues[THROTTLEINDEX] < FPSTICKLOW) {
        if (global.rxvalues[ROLLINDEX] < FP_RXMOVELOW) {
            // Stick is now low. What has happened before?
            if(lastrollstickstate == STICK_STATE_START) {
                // We just come from start position, so this is our first movement
                rollmovecounter=1;
                lastrollstickstate = STICK_STATE_LOW;
                // Detected stick movement, so restart timeout.
                stickcommandtimer = lib_timers_starttimer();
            } else if (lastrollstickstate == STICK_STATE_HIGH) {
                // Stick had been high recently, so increment counter
                rollmovecounter++;
                lastrollstickstate = STICK_STATE_LOW;
                // Detected stick movement, so restart timeout.
                stickcommandtimer = lib_timers_starttimer();
            } // else: nothing happened, nothing to do
        } else if (global.rxvalues[ROLLINDEX] > FP_RXMOVEHIGH) {
            // And now the same in opposite direction...
            if(lastrollstickstate == STICK_STATE_START) {
                // We just come from start position
                rollmovecounter=1;
                lastrollstickstate = STICK_STATE_HIGH;
                // Detected stick movement, so restart timeout.
                stickcommandtimer = lib_timers_starttimer();
            } else if (lastrollstickstate == STICK_STATE_LOW) {
                // Stick had been low recently, so increment counter
                rollmovecounter++;
                lastrollstickstate = STICK_STATE_HIGH;
                // Detected stick movement, so restart timeout.
                stickcommandtimer = lib_timers_starttimer();
            } // else: nothing happened, nothing to do
        }

        if(lib_timers_gettimermicroseconds(stickcommandtimer) > 1000000L) {
            // Timeout: last detected stick movement was more than 1 second ago.
            lastrollstickstate = STICK_STATE_START;
        }

        if(rollmovecounter == 6) {
            // Now we had enough movements. Execute calibration.
            calibrategyroandaccelerometer(true);
            // Save in EEPROM
            writeusersettingstoeeprom();
            lastrollstickstate = STICK_STATE_START;
        }
    } // if throttle low
} // checkforstickcommand()
