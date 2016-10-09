#include "../../LCD.h"

#ifdef LCD_H_

#ifndef MENU_FUNCTIONS_H_
#define MENU_FUNCTIONS_H_

#include "../../../pads/Pads.h"
#include "../../../encoders/Encoders.h"
#include "../../../../midi/MIDI.h"
#include "../Menu.h"
#include <avr/boot.h>

//uint16_t dev_signature[20];
//
//static inline void USB_Device_GetSerialString_test()    //testing only
//{
//
    //uint8_t curr_int = SREG;
    //cli();
//
    //uint8_t SigReadAddress = 0x0E;
//
    //printf("Device serial number: ");
//
    //for (uint8_t SerialCharNum = 0; SerialCharNum < 20; SerialCharNum++)
    //{
        //uint8_t SerialByte = boot_signature_byte_get(SigReadAddress);
//
        //if (SerialCharNum & 0x01)
        //{
            //SerialByte >>= 4;
            //SigReadAddress++;
        //}
//
        //SerialByte &= 0x0F;
//
        //dev_signature[SerialCharNum] = /*cpu_to_le16*/((SerialByte >= 10) ?
        //(('A' - 10) + SerialByte) : ('0' + SerialByte));
//
        //printf("%d", dev_signature[SerialCharNum]);
    //}   printf("\n");
//
    //SREG = curr_int;
//}

//action functions

bool factoryReset(functionArgument argument);

bool deviceInfo(functionArgument argument);

bool padEditMode(functionArgument argument);

bool enableCalibration(functionArgument argument);

//check functions
bool checkCalibration(functionArgument argument);

//checkable items functions
bool checkRunningStatus(functionArgument argument);

bool checkPressureLevel(functionArgument argument);

bool checkAftertouchType(functionArgument argument);

bool checkPressureCurve(functionArgument argument);

bool checkNoteOffStatus(functionArgument argument);

#endif
#endif