Symawii for Syma X5C
=======

This is a port of bradwii to the Syma X5c quad.  It runs another ARM architecture, Nuvoton Mini51 which is represented
by commercial microhelis WL-Toys V202, V252, and JXD JD-385. For hardware description
details see https://github.com/hackocopter/JD385_Documentation .

Briefly, the Syma X5C  uses Nuvoton MINI54ZAN ARM MCU with 16KB ROM and 2KB RAM, combined gyro/accelerometer
InvenSense MPU-6050, and nRF24L01+ clone Beken BK2423.



#### The Syma X5C uses the following hardware
 * Nuvoton MINI54ZAN ARM Cortex-M0
 * Beken BK2423 2.4GHz transceiver
 * InvenSense MPU-6050 6-Axis MEMS Gyroscope/accelerometer


extra notes from TheLastMutt:
======

Current status
####
The control parameters habe been optimized in a way that flying feels similar to the original firmware.
It is possible to lift off very slowly and fly close to the ground.
The control parameters have not been tested in acro or semi-acro mode.

Accelerometer calibration can be separated from gyro calibration, like the original firmware does. After executing the manual accelerometer calibration,
the parameters are stored in data flash. At subsequent power-on only the gyro calibration is executed. This way there is no need to find a level surface
for every power-on.

Testing a new flight mode: "yaw hold". In this mode the yaw controller controls the yaw angle instead of the yaw rate.
If the quadcopter gets rotated away from its desired heading (due to wind or high throttle or whatever), it will try to rotate back to the desired heading.
This is similar to compass mode, but a compass is not needed. This mode can be activated using the checkboxes like all the other flight modes.
It is currently activated using AUX2 ("flip"/"no flip" channel of X4).

The X4 board does not have a serial port, so a PC configuration software cannot be connected.

How to use:
 * Switch quadcopter on
 * (If all LEDs slowly blink 4 times, no accelerometer calibration was found in data flash. Accelerometer will be calibrated after binding with transmitter,
so a level surface is needed. Use manual accelerometer calibration to remedy this)
 * All LEDs blink alternating
 * Put quadcopter on steady surface
 * Switch transmitter on
 * LEDs blink in circular pattern. Don't move the quadcopter during this time because the calibration is ongoing.
 * When calibration is done, all LEDs blink short pulses. Quadcopter is not armed yet and will not respond to throttle.
 * Press the lower throttle trim button for one second ("LEDs off" for Hubsan firmware)
 * All LEDs are on and you are ready to fly.
 * When the LEDs start to blink during flight it's time to land because the battery is nearly empty.
With this firmware the LEDs only blink *while* the battery voltage is low, so blinking might be temporary at high throttle.

Disarm by pressing the lower throttle trim button again for one second.

Manual accelerometer calibration:
 * Quadcopter must be on level surface
 * Quadcopter must be in "not armed" state
 * Throttle stick at minimum
 * Move roll stick 3 times left and right
 * LEDs blink in circular pattern to indicate calibration process. When finished, results are stored in data flash.

#### Development issues:

When burning a firmware with new PID control parameters, checkboxconfig or anything else from the usersettings struct make sure to erase the data flash.
Otherwise the firmware will continue to use the old data. 


Credits
======

 * Bradwii was originally coded by Brad Quick for AVR: https://github.com/bradquick/bradwii
 * Trollcop forked and ported to ARM STM32, untested: https://github.com/trollcop/bradwii
 * The Mini54ZAN ARM port to V202/JD385 was done by Victor: https://github.com/victzh/bradwii
 * The Hubsan X4 H107L port was done by Goebish: https://github.com/goebish/bradwii-X4
 * Extra work on the H107L port by TheLastMutt: https://github.com/TheLastMutt/bradwii-x4-gcc
 * Robbievz for the stripped-down version customized for the Syma X5C https://github.com/robbievz/symawii.git
