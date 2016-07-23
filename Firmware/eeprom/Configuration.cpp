#ifndef EEPROMSETTINGS_H_
#define EEPROMSETTINGS_H_

#include "Configuration.h"
#include <util/delay.h>
#include "../midi/MIDI_parameters.h"
#include "../version/Firmware.h"
#include "../BitManipulation.h"

void Configuration::factoryReset(factoryResetType_t type)   {

    #ifdef MODULE_LCD
        strcpy_P(stringBuffer, restoringDefaults_string);
        lcd_clrscr();
        lcd_gotoxy(0, 0);
        lcd_puts(stringBuffer);
        lcd_gotoxy(0, 1);
        _delay_ms(2000);
        strcpy_P(stringBuffer, pleaseWait_string);
        lcd_puts(stringBuffer);
        _delay_ms(2000);
    #endif

    if (type == factoryReset_wipeRestore)
        clearEEPROM();

    initSettings(type == factoryReset_partial);

    writeSignature();

    #ifdef MODULE_LCD
        lcd_gotoxy(0,2);
        strcpy_P(stringBuffer, complete_string);
        lcd_puts(stringBuffer);
        _delay_ms(2000);
    #endif

}

void Configuration::writeSignature()    {

    uint8_t unique_id_invert = invertByte(EEPROM_UNIQUE_ID);

    for (int i=0; i<START_OFFSET; i++)
        (i%2) ? eeprom_update_byte((uint8_t*)i, unique_id_invert) : eeprom_update_byte((uint8_t*)i, EEPROM_UNIQUE_ID);

}

Configuration::Configuration()    {

    //def const

}

void Configuration::init() {

    createMemoryLayout();
    createSectionAddresses();

    checkReset();

}

void Configuration::checkReset()    {

    //if ID bytes haven't been written to EEPROM on specified address,
    //write default configuration to EEPROM

    bool factory_reset = false;
    uint8_t unique_id_invert = invertByte(EEPROM_UNIQUE_ID);
    uint8_t checkByte;

    //first check if all bytes up to START_OFFSET address match unique id
    for (int i=0; i<START_OFFSET; i++)  {

        (i%2) ? checkByte = unique_id_invert : checkByte = EEPROM_UNIQUE_ID;

        if (eeprom_read_byte((uint8_t*)i) != checkByte)  {

            factory_reset = true;
            break;

        }

    }

    //id bytes don't match
    //check if all bytes up to START_OFFSET are same
    //if they're the same, eeprom has been cleared before, only restore configuration
    //else clear eeprom and apply config
    if (factory_reset)  {

        for (int i=0; i<START_OFFSET-1; i++)  {

            if (eeprom_read_byte((uint8_t*)i) != invertByte(eeprom_read_byte((uint8_t*)i+1)))    {

                factoryReset(factoryReset_wipeRestore);
                return;

            }

        }

        factoryReset(factoryReset_restore);

    }

}

void Configuration::clearEEPROM()  {

    for (int i=0; i<EEPROM_SIZE; i++) eeprom_update_byte((uint8_t*)i, 0xFF);

}

void Configuration::createSectionAddresses()   {

    for (int i=0; i<CONF_BLOCKS; i++)  {

        uint16_t memory_usage = 0;

        for (int j=0; j<blocks[i].sections; j++)    {

            if (!j) {

                //first section address is always 0
                blocks[i].sectionAddress[0] = 0;

            }   else {

                switch(blocks[i].sectionParameterType[j-1])   {

                    case BIT_PARAMETER:
                    blocks[i].sectionAddress[j] = ((blocks[i].sectionParameters[j]%8 != 0) + blocks[i].sectionParameters[j-1]/8) + blocks[i].sectionAddress[j-1];
                    break;

                    case BYTE_PARAMETER:
                    blocks[i].sectionAddress[j] = blocks[i].sectionParameters[j-1] + blocks[i].sectionAddress[j-1];
                    break;

                    case WORD_PARAMETER:
                    blocks[i].sectionAddress[j] = 2*blocks[i].sectionParameters[j-1] + blocks[i].sectionAddress[j-1];
                    break;

                }

            }

        }

        uint8_t lastSection = blocks[i].sections-1;

        switch(blocks[i].sectionParameterType[lastSection]) {

            case BIT_PARAMETER:
            memory_usage = blocks[i].sectionAddress[lastSection]+((blocks[i].sectionParameters[lastSection]%8 != 0)+blocks[i].sectionParameters[lastSection]/8);
            break;

            case BYTE_PARAMETER:
            memory_usage = blocks[i].sectionAddress[lastSection]+blocks[i].sectionParameters[lastSection];
            break;

            case WORD_PARAMETER:
            memory_usage = blocks[i].sectionAddress[lastSection]+2*blocks[i].sectionParameters[lastSection];
            break;

        }

        if (i < CONF_BLOCKS-1) {

            blocks[i+1].blockStartAddress = blocks[i].blockStartAddress + memory_usage;

        }

    }

}

void Configuration::initSettings(bool partialReset) {

    //we need to init each block and section with data
    //program area
    initProgramSettings(partialReset);
    initUserScales(partialReset);
    initPadCalibration(partialReset);
    initMIDIsettings(partialReset);

}

void Configuration::initProgramSettings(bool partialReset) {

    if (partialReset && blocks[CONF_BLOCK_PROGRAM].preserveOnPartialReset) return;

    uint16_t blockStartAddress = getBlockAddress(CONF_BLOCK_PROGRAM);

    uint16_t parameterAddress = blocks[CONF_BLOCK_PROGRAM].sectionAddress[programLastActiveProgramSection] + blockStartAddress;

    //init default active program
    eeprom_update_byte((uint8_t*)parameterAddress, DEFAULT_ACTIVE_PROGRAM);

    parameterAddress = blocks[CONF_BLOCK_PROGRAM].sectionAddress[programLastActiveScaleSection] + blockStartAddress;

    //init default active scales for programs
    for (int i=0; i<NUMBER_OF_PROGRAMS; i++)
        eeprom_update_byte((uint8_t*)parameterAddress+i, DEFAULT_ACTIVE_SCALE);

    //init global settings for all programs
    for (int i=0; i<NUMBER_OF_PROGRAMS; i++)    {

        parameterAddress = blockStartAddress + blocks[CONF_BLOCK_PROGRAM].sectionAddress[programGlobalSettingsSection] + (i*GLOBAL_PROGRAM_SETTINGS);

        for (int j=0; j<GLOBAL_PROGRAM_SETTINGS; j++)
            eeprom_update_byte((uint8_t*)parameterAddress+j, defaultGlobalProgramSettingArray[j]);

    }

    //init local settings for all programs
    for (int i=0; i<NUMBER_OF_PROGRAMS; i++)    {

        parameterAddress = blockStartAddress + blocks[CONF_BLOCK_PROGRAM].sectionAddress[programLocalSettingsSection] + ((LOCAL_PROGRAM_SETTINGS*MAX_PADS)*i);

        for (int j=0; j<MAX_PADS; j++)    {

            for (int k=0; k<LOCAL_PROGRAM_SETTINGS; k++)    {

                eeprom_update_byte((uint8_t*)parameterAddress+k+(LOCAL_PROGRAM_SETTINGS*j), defaultLocalProgramSettingArray[k]);

            }

        }

    }

    //init predefined scale settings
    for (int i=0; i<NUMBER_OF_PROGRAMS; i++) {

        parameterAddress = blocks[CONF_BLOCK_PROGRAM].sectionAddress[programScalePredefinedSection] + ((NUMBER_OF_PREDEFINED_SCALES*PREDEFINED_SCALE_PARAMETERS)*i) + blockStartAddress;

        for (int j=0; j<NUMBER_OF_PREDEFINED_SCALES; j++)  {

            for (int k=0; k<PREDEFINED_SCALE_PARAMETERS; k++)   {

                eeprom_update_byte((uint8_t*)parameterAddress+k+(PREDEFINED_SCALE_PARAMETERS*j), defaultPredefinedScaleParametersArray[k]);

            }

        }

    }

}

void Configuration::initUserScales(bool partialReset)   {

    if (partialReset && blocks[CONF_BLOCK_USER_SCALE].preserveOnPartialReset) return;

    uint16_t blockStartAddress = getBlockAddress(CONF_BLOCK_USER_SCALE);
    uint16_t parameterAddress;

    for (int i=0; i<NUMBER_OF_USER_SCALES; i++) {

        parameterAddress = blockStartAddress + blocks[CONF_BLOCK_USER_SCALE].sectionAddress[padNotesSection] + ((MAX_PADS*NOTES_PER_PAD)*i);

        for (int j=0; j<MAX_PADS; j++)    {

            for (int k=0; k<NOTES_PER_PAD; k++) {

                //set first note to default note
                //set all other notes to blank

                if (!k) eeprom_update_byte((uint8_t*)parameterAddress+k+(NOTES_PER_PAD*j), DEFAULT_NOTE);
                else    eeprom_update_byte((uint8_t*)parameterAddress+k+(NOTES_PER_PAD*j), BLANK_NOTE);

            }

        }

    }

}

void Configuration::initPadCalibration(bool partialReset)   {

    if (partialReset && blocks[CONF_BLOCK_PAD_CALIBRATION].preserveOnPartialReset) return;

    uint16_t blockStartAddress = getBlockAddress(CONF_BLOCK_PAD_CALIBRATION);
    uint16_t parameterAddress;

    //set padCalibrationStatus to false by default
    parameterAddress = blockStartAddress+blocks[CONF_BLOCK_PAD_CALIBRATION].sectionAddress[padCalibrationStatus];
    eeprom_update_byte((uint8_t*)parameterAddress, 0);

    //init lower pressure limits
    for (int i=0; i<MAX_PADS; i++)    {

        parameterAddress = blockStartAddress+blocks[CONF_BLOCK_PAD_CALIBRATION].sectionAddress[padCalibrationPressureLowerSection] + i*2;
        eeprom_update_word((uint16_t*)parameterAddress, DEFAULT_PAD_PRESSURE_LIMIT_LOWER);

    }

    //init upper pressure limits
    for (int i=0; i<MAX_PADS; i++)    {

        parameterAddress = blockStartAddress+blocks[CONF_BLOCK_PAD_CALIBRATION].sectionAddress[padCalibrationPressureUpperSection] + i*2;
        eeprom_update_word((uint16_t*)parameterAddress, DEFAULT_PAD_PRESSURE_LIMIT_UPPER);

    }

    //init lower x limits
    for (int i=0; i<MAX_PADS; i++)    {

        parameterAddress = blockStartAddress+blocks[CONF_BLOCK_PAD_CALIBRATION].sectionAddress[padCalibrationXlowerSection] + i*2;
        eeprom_update_word((uint16_t*)parameterAddress, DEFAULT_PAD_X_LIMIT_LOWER);

    }

    //init upper x limits
    for (int i=0; i<MAX_PADS; i++)    {

        parameterAddress = blockStartAddress+blocks[CONF_BLOCK_PAD_CALIBRATION].sectionAddress[padCalibrationXupperSection] + i*2;
        eeprom_update_word((uint16_t*)parameterAddress, DEFAULT_PAD_X_LIMIT_UPPER);

    }

    //init lower y limits
    for (int i=0; i<MAX_PADS; i++)    {

        parameterAddress = blockStartAddress+blocks[CONF_BLOCK_PAD_CALIBRATION].sectionAddress[padCalibrationYlowerSection] + i*2;
        eeprom_update_word((uint16_t*)parameterAddress, DEFAULT_PAD_Y_LIMIT_LOWER);

    }

    //init upper y limits
    for (int i=0; i<MAX_PADS; i++)    {

        parameterAddress = blockStartAddress+blocks[CONF_BLOCK_PAD_CALIBRATION].sectionAddress[padCalibrationYupperSection] + i*2;
        eeprom_update_word((uint16_t*)parameterAddress, DEFAULT_PAD_Y_LIMIT_UPPER);

    }

}

void Configuration::initMIDIsettings(bool partialReset) {

    if (partialReset && blocks[CONF_BLOCK_MIDI].preserveOnPartialReset) return;

    uint16_t blockStartAddress = getBlockAddress(CONF_BLOCK_MIDI);
    uint16_t parameterAddress = blockStartAddress;

    for (int i=0; i<MIDI_SETTINGS; i++)
        eeprom_update_byte((uint8_t*)parameterAddress+i, defaultMIDIsettingArray[i]);

}

bool Configuration::writeParameter(uint8_t blockID, uint8_t sectionID, int16_t parameterID, int16_t newValue)    {

    uint16_t startAddress = getSectionAddress(blockID, sectionID);

    if (startAddress > EEPROM_SIZE) {

        #if MODE_SERIAL > 0
            printf("Requested address out of EEPROM memory range\n");
        #endif
        return 0;

    }

    uint8_t parameterType = getParameterType(blockID, sectionID);

    uint8_t arrayIndex;
    uint8_t arrayValue;
    uint8_t parameterIndex;

    switch(parameterType)   {

        case BIT_PARAMETER:
        arrayIndex = parameterID/8;
        parameterIndex = parameterID - 8*arrayIndex;
        arrayValue = eeprom_read_byte((uint8_t*)startAddress+arrayIndex);
        bitWrite(arrayValue, parameterIndex, newValue);
        eeprom_update_byte((uint8_t*)startAddress+arrayIndex, arrayValue);
        return (arrayValue == eeprom_read_byte((uint8_t*)startAddress+arrayIndex));
        break;

        case BYTE_PARAMETER:
        eeprom_update_byte((uint8_t*)startAddress+parameterID, newValue);
        return (newValue == eeprom_read_byte((uint8_t*)startAddress+parameterID));
        break;

        case WORD_PARAMETER:
        eeprom_update_word((uint16_t*)startAddress+parameterID, newValue);
        return (newValue == (int16_t)eeprom_read_word((uint16_t*)startAddress+parameterID));
        break;

    }   return 0;

}

Configuration configuration;

#endif
