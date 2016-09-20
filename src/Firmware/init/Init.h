#ifndef INIT_H_
#define INIT_H_

#include <avr/interrupt.h>

#include "../hardware/pins/Pins.h"

#include "../Modules.h"

#include "../interface/pads/Pads.h"

#ifdef MODULE_BUTTONS
#include "../interface/buttons/Buttons.h"
#endif

#ifdef MODULE_LEDS
#include "../interface/leds/LEDs.h"
#endif

#ifdef MODULE_LCD
#include "../interface/lcd/LCD.h"
#endif

#ifdef MODULE_ENCODERS
#include "../interface/encoders/Encoders.h"
#endif

#include "../hardware/timer/TimerObject.h"
#include "../eeprom/Configuration.h"
#include "../Types.h"
#include "../hardware/reset/Reset.h"
#include <util/delay.h>
#include <avr/wdt.h>
#include "../midi/MIDI.h"
#include "../version/Firmware.h"
#include "../version/Hardware.h"
#include "../sysex/SysEx.h"

#define START_UP_ANIMATION  1

void globalInit();
void initHardware();
void calibratePads();
void startUpAnimation();
void reboot();

#endif