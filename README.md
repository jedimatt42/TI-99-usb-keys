# TI-99-usb-keys
Teensy 3.1 sketch to translate usb keyboard to TI-99/4a 15 pin interface.

This project interfaces a Teensy 3.1 and a Circuits@Home USB Host shield, to take input from a USB keyboard, and simulate the
matrix keyboard built into the TI. 

The USB keyboard can be used with the TI keyboard still in place. The user can switch between them at will.

At this time, it is on a breadboard. 

[Pictures](https://goo.gl/photos/xhLKCTwCPuAeUbP86)

The keyboard mapping is documented in: keymap.xlsx (needs an update)

## Issues

Holding modifiers is not recognized.

Pressing both modifiers ( left & right shift for example, + a key like h does not produce H. )

Parsec - using arrow keys DOWN (FCTN-x) while pressing Q for fire causes ship to go UP. Turning on scroll lock, so DOWN is just X and not FCTN X fixes the problem but requires user intervention.

SHIFT-LOCK occurs when typing punctuation. Usually '?' ' ' (question-space) quickly will cause it.  I need to implement reference counting for modifiers. 

Micro Pinball II - 1 key doens't work for left paddle. This happened after fixing 0 that used to stick. Actually this is intermittent.. sometimes it works just fine 1, 0 and =. 

Ms. Pacman - the keyboard seemed to reboot... ? just using arrow keys... didn't do it after it settled down. No problem a day later..

XB - 10 GOTO 10, run, F4-break doesn't work... 10 call key(1,k,s):: goto 10, works fine.
  pressing c while still holding backspace acts like fctn c.


## Design

TIUSBKeys.pcb - A pcboard layout suitable for exposing the USB host port and programming port from the rear or left rear side of the TI console. Drops one gpio port, and rearranges numerous pin assignments to better layout the board.

TIUSBKeys.sch - An expresspcb.com schematic file.

## Resources

[Teensy 3.1](https://www.pjrc.com/teensy/teensy31.html)

[USB Host Shield](https://www.circuitsathome.com/products-page/arduino-shields/usb-host-shield-for-arduino-pro-mini) 

[Mod to teensy arduino libraries for OUTPUT_OPENDRAIN](https://forum.pjrc.com/threads/7531-Internal-pull-up-and-pull-down-resistors-on-teensy-3?p=63944&viewfull=1#post63944)

[USB Host shield libary 2.0](https://github.com/felis/USB_Host_Shield_2.0)

