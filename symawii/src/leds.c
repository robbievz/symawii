/* 
Copyright 2013-2014 TheLastMutt, TheSmartGerman

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

#include "bradwii.h"
#include "leds.h"
#include "defs.h"
#include "lib_timers.h"


// if LED is defined set the according output from defs.s to according IO Pin

void leds_init()
{
	#ifdef LED1
			lib_digitalio_initpin(LED1_OUTPUT, DIGITALOUTPUT);
	#endif
		
	#ifdef LED2	
			lib_digitalio_initpin(LED2_OUTPUT, DIGITALOUTPUT);
	#endif

	#ifdef LED3	
			lib_digitalio_initpin(LED3_OUTPUT, DIGITALOUTPUT);
	#endif
		
	#ifdef LED4	
			lib_digitalio_initpin(LED4_OUTPUT, DIGITALOUTPUT);
	#endif	
		
	#ifdef LED5	
			lib_digitalio_initpin(LED5_OUTPUT, DIGITALOUTPUT);
	#endif
		
	#ifdef LED6
			lib_digitalio_initpin(LED6_OUTPUT, DIGITALOUTPUT);
	#endif
	
	#ifdef LED7
			lib_digitalio_initpin(LED7_OUTPUT, DIGITALOUTPUT);
	#endif

	#ifdef LED8
			lib_digitalio_initpin(LED8_OUTPUT, DIGITALOUTPUT);
	#endif

}

// according to the state, switch the led on or off

void leds_set(unsigned char state)
{
	#ifdef LED1
		lib_digitalio_setoutput( LED1_OUTPUT , (state & LED1) ? LED1_ON : !LED1_ON);
	#endif
	
	#ifdef LED2	
		lib_digitalio_setoutput( LED2_OUTPUT , (state & LED2) ? LED2_ON : !LED2_ON);
	#endif

	#ifdef LED3	
		lib_digitalio_setoutput( LED3_OUTPUT , (state & LED3) ? LED3_ON : !LED3_ON);		
	#endif
		
	#ifdef LED4	
		lib_digitalio_setoutput( LED4_OUTPUT , (state & LED4) ? LED4_ON : !LED4_ON);
	#endif
		
	#ifdef LED5	
		lib_digitalio_setoutput( LED5_OUTPUT , (state & LED5) ? LED5_ON : !LED5_ON);
	#endif
		
	#ifdef LED6
		lib_digitalio_setoutput( LED6_OUTPUT , (state & LED6) ? LED6_ON : !LED6_ON);
	#endif

	#ifdef LED7
		lib_digitalio_setoutput( LED7_OUTPUT , (state & LED7) ? LED7_ON : !LED7_ON);
	#endif	

	#ifdef LED8
		lib_digitalio_setoutput( LED8_OUTPUT , (state & LED8) ? LED8_ON : !LED8_ON);
	#endif	

}


// blink the LEDs a specified amount of cycles

void leds_blink_cycles(unsigned char state, unsigned int on_time, unsigned int off_time, unsigned char cycles)
{
	for(uint8_t i=0;i<cycles;i++) {
			leds_set(state);
			lib_timers_delaymilliseconds(on_time);
			leds_set(~state);
			lib_timers_delaymilliseconds(off_time);
	}							
}

// switch the LEDs according to the timer, the function have to be called if the programm is running in a loop
// 
void leds_blink_continuous(unsigned char state, unsigned int on_time, unsigned int off_time) {
	if(lib_timers_gettimermicroseconds(0) % ((on_time + off_time) * 1000) > (off_time * 1000) )
			leds_set(state);
	else
			leds_set(~state);
			 
}
