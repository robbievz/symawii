/* 
Copyright 2013-2014 TheSmartGerman

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

#define LED_BLINK_FAST 100
#define LED_BLINK_MEDIUM 250
#define LED_BLINK_SLOW 500

#define LED1_STATE 0x01
#define LED2_STATE 0x02
#define LED3_STATE 0x04
#define LED4_STATE 0x08
#define LED5_STATE 0x10
#define LED6_STATE 0x20
#define LED7_STATE 0x40
#define LED8_STATE 0x80

#define LED_ALL 0xFF
#define LED_NONE 0x00


void leds_init(void);
void leds_set(unsigned char state);
void leds_blink_cycles(unsigned char state, unsigned int on_time, unsigned int off_time, unsigned char cycles);
void leds_blink_continuous(unsigned char state, unsigned int on_time, unsigned int off_time);
