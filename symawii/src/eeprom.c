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

#include "eeprom.h"
#include "bradwii.h"

extern usersettingsstruct usersettings;
extern globalstruct global;

#define MAGICNUMBER 12345

void writeusersettingstoeeprom(void)
{
#if (EEPROM_SIZE != NO_EEPROM)
	
    uint16_t magicnumber = MAGICNUMBER;
    int16_t size = sizeof(usersettingsstruct);
    uint16_t index = 0;
	
		// if usersettings larger den defined eeprom size, leave funktion.	
		if(size > EEPROM_SIZE) 
				return;

    index += eeprom_write_block((const void *)&magicnumber, index, sizeof(magicnumber));
    index += eeprom_write_block((const void *)&size, index, sizeof(size));
    index += eeprom_write_block((const void *)&usersettings, index, size);
    eeprom_commit(); //Why? It's a emtpy function in drv_hal.c!
#endif
}

void readusersettingsfromeeprom(void)
{
#if (EEPROM_SIZE != NO_EEPROM)

    uint16_t magicnumber = 0;
    int16_t size = 0;
    uint16_t index = 0;
    index += eeprom_read_block((void *) &magicnumber, index, sizeof(magicnumber));

    if (magicnumber != MAGICNUMBER)
        return;

    index += eeprom_read_block((void *) &size, index, sizeof(size));
    if (size > sizeof(usersettingsstruct))
        size = sizeof(usersettingsstruct);

		if(size > EEPROM_SIZE) 
				return;
		
    eeprom_read_block((void *) &usersettings, index, size);
    global.usersettingsfromeeprom = 1;  // set a flag so the rest of the program know it's working with calibtated settings
#endif
}
