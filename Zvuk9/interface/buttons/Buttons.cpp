#include "Buttons.h"
#include "../../hardware/i2c/i2c_master.h"
#include "../pads/Pads.h"
#include "../lcd/LCD.h"
#include "../leds/LEDs.h"
#include "../../midi/MIDI.h"

//time after which expanders are checked in ms
#define EXPANDER_CHECK_TIME         10

#define EDIT_MODE_COUNTER           2

//MCP23017 address bytes
const uint8_t expanderAddress[] = { 0x21, 0x20 };   //chip address
const uint8_t gpioAddress[]     = { 0x12, 0x13 };   //input/output
const uint8_t iodirAddress[]    = { 0x00, 0x01 };   //port A/port B
const uint8_t gppuAddress[]     = { 0x0C, 0x0D };   //interrupt/pull-up

//shift new values from button in this variable
//if it's 0xFF or buttonDebounceCompare, its reading is stable
const uint8_t buttonDebounceCompare = 0b11110000;

Buttons::Buttons()  {

    //default constructor
    lastCheckTime               = 0;
    lastButtonDataPress         = 0;
    mcpData                     = 0;

    for (int i=0; i<MAX_NUMBER_OF_BUTTONS; i++) {

        previousButtonState[i] = buttonDebounceCompare;
        buttonEnabled[i] = true;

    }

}

uint8_t read_I2C_reg(uint8_t address, uint8_t reg)   {

    uint8_t value;

    i2c_start((address << 1) + I2C_WRITE);
    i2c_write(reg);
    i2c_stop();

    i2c_start((address << 1) + I2C_READ);
    value = i2c_read_nack();
    i2c_stop();

    return value;
}

void write_I2C_reg(uint8_t address, uint8_t reg, uint8_t value)  {

    i2c_start((address << 1) + I2C_WRITE);
    i2c_write(reg);
    i2c_write(value);
    i2c_stop();

}

void Buttons::init()  {

    i2c_init();

    //ensure that we know the configuration
    write_I2C_reg(expanderAddress[0], 0x0A, 0x00);         //IOCON=0x00 if BANK=0
    write_I2C_reg(expanderAddress[0], 0x05, 0x00);         //IOCON=0x00 if BANK=1
    write_I2C_reg(expanderAddress[1], 0x0A, 0x00);         //IOCON=0x00 if BANK=0
    write_I2C_reg(expanderAddress[1], 0x05, 0x00);         //IOCON=0x00 if BANK=1

    write_I2C_reg(expanderAddress[0], iodirAddress[0], 0xFF);   //expander 1, set all pins on PORTA to input mode
    write_I2C_reg(expanderAddress[0], iodirAddress[1], 0xFF);   //expander 1, set all pins on PORTB to input mode

    write_I2C_reg(expanderAddress[1], iodirAddress[0], 0xFF);   //expander 2, set all pins on PORTA to input mode
    write_I2C_reg(expanderAddress[1], iodirAddress[1], 0xFF);   //expander 2, set all pins on PORTB to input mode

    write_I2C_reg(expanderAddress[0], gppuAddress[0], 0xFF);    //expander 1, turn on pull-ups, PORTA
    write_I2C_reg(expanderAddress[0], gppuAddress[1], 0xFF);    //expander 1, turn on pull-ups, PORTB

    write_I2C_reg(expanderAddress[1], gppuAddress[0], 0xFF);    //expander 2, turn on pull-ups, PORTA
    write_I2C_reg(expanderAddress[1], gppuAddress[1], 0xFF);    //expander 2, turn on pull-ups, PORTB

}

void Buttons::update(bool processingEnabled)    {

    if (!((newMillis() - lastCheckTime) > EXPANDER_CHECK_TIME)) return;

     if (readStates())   {

         for (int i=0; i<MAX_NUMBER_OF_BUTTONS; i++) {

             //invert button state because of pull-ups
             uint8_t state = !((mcpData >> i) & 0x01);
             bool debounced = buttonDebounced(i, state);

             if (debounced) {

                if (state == getPreviousButtonState(i)) continue;

                //update previous button state with current one
                setPreviousButtonState(i, state);
                if (processingEnabled) processButton(i, state);

             }

         }   mcpData = 0;

     }

     lastCheckTime = newMillis();

}

bool Buttons::getButtonPressed(uint8_t buttonNumber) {

    return getPreviousButtonState(buttonNumber);

}

bool Buttons::readStates()   {

    mcpData <<= 8;
    mcpData |= read_I2C_reg(expanderAddress[0], gpioAddress[0]);     //expander A, GPIOA
    mcpData <<= 8;
    mcpData |= read_I2C_reg(expanderAddress[0], gpioAddress[1]);     //expander A, GPIOB
    mcpData <<= 8;
    mcpData |= read_I2C_reg(expanderAddress[1], gpioAddress[0]);     //expander B, GPIOA
    mcpData <<= 8;
    mcpData |= read_I2C_reg(expanderAddress[1], gpioAddress[1]);     //expander B, GPIOB

    return true;

}

bool Buttons::buttonDebounced(uint8_t buttonNumber, uint8_t state)  {

    //shift new button reading into previousButtonState
    previousButtonState[buttonNumber] = (previousButtonState[buttonNumber] << 1) | state | buttonDebounceCompare;

    //if button is debounced, return true
    return ((previousButtonState[buttonNumber] == buttonDebounceCompare) || (previousButtonState[buttonNumber] == 0xFF));

}

void Buttons::processButton(uint8_t buttonNumber, bool state)    {

    if (buttonEnabled[buttonNumber])    {

        if (state)    {

            switch(buttonNumber)    {

                case BUTTON_ON_OFF_AFTERTOUCH:
                case BUTTON_ON_OFF_NOTES:
                case BUTTON_ON_OFF_X:
                case BUTTON_ON_OFF_Y:
                case BUTTON_ON_OFF_SPLIT:
                handleOnOffEvent(buttonNumber);
                break;

                case BUTTON_NOTE_C_SHARP:
                case BUTTON_NOTE_D_SHARP:
                case BUTTON_NOTE_F_SHARP:
                case BUTTON_NOTE_G_SHARP:
                case BUTTON_NOTE_A_SHARP:
                case BUTTON_NOTE_C:
                case BUTTON_NOTE_D:
                case BUTTON_NOTE_E:
                case BUTTON_NOTE_F:
                case BUTTON_NOTE_G:
                case BUTTON_NOTE_A:
                case BUTTON_NOTE_B:
                tonic_t _tonic = getTonicFromButton(buttonNumber);
                handleTonicEvent(_tonic);
                break;

            }

        }

        switch (buttonNumber)   {

            case BUTTON_OCTAVE_DOWN:
            handleOctaveEvent(false, state);
            break;

            case BUTTON_OCTAVE_UP:
            handleOctaveEvent(true, state);
            break;

            case BUTTON_TRANSPORT_STOP:
            case BUTTON_TRANSPORT_PLAY:
            case BUTTON_TRANSPORT_RECORD:
            handleTransportControlEvent(buttonNumber, state);
            break;

        }

    }

    //resume all callbacks
    for (int i=0; i<MAX_NUMBER_OF_BUTTONS; i++) {

        if (!buttonEnabled[i] && !getButtonPressed(i)) {

            buttonEnabled[i] = true;

        }

    }

}

bool Buttons::getPreviousButtonState(uint8_t buttonNumber) {

    return bitRead(lastButtonDataPress, buttonNumber);

}

void Buttons::setPreviousButtonState(uint8_t buttonNumber, uint8_t state) {

    bitWrite(lastButtonDataPress, buttonNumber, state);

}

tonic_t Buttons::getTonicFromButton(uint8_t buttonNumber)   {

    switch(buttonNumber)    {

        case BUTTON_NOTE_C:
        return tonicC;

        case BUTTON_NOTE_C_SHARP:
        return tonicCSharp;

        case BUTTON_NOTE_D:
        return tonicD;

        case BUTTON_NOTE_D_SHARP:
        return tonicDSharp;

        case BUTTON_NOTE_E:
        return tonicE;

        case BUTTON_NOTE_F:
        return tonicF;

        case BUTTON_NOTE_F_SHARP:
        return tonicFSharp;

        case BUTTON_NOTE_G:
        return tonicG;

        case BUTTON_NOTE_G_SHARP:
        return tonicGSharp;

        case BUTTON_NOTE_A:
        return tonicA;

        case BUTTON_NOTE_A_SHARP:
        return tonicASharp;

        case BUTTON_NOTE_B:
        return tonicB;

    }   return tonicInvalid;   //impossible case

}

void Buttons::handleOnOffEvent(uint8_t buttonNumber)    {

    //determine action based on pressed button

    uint8_t ledNumber = 0;
    functionsOnOff_t lcdMessageType;
    ledIntensity_t ledState = ledIntensityOff;
    uint8_t lastTouchedPad = pads.getLastTouchedPad();

    switch (buttonNumber)    {

        case BUTTON_ON_OFF_NOTES:
        pads.notesOnOff();
        lcdMessageType = featureNotes;
        ledNumber = LED_ON_OFF_NOTES;
        if (pads.getNoteSendEnabled(lastTouchedPad)) ledState = ledIntensityFull; else ledState = ledIntensityOff;
        break;

        case BUTTON_ON_OFF_AFTERTOUCH:
        pads.aftertouchOnOff();
        lcdMessageType = featureAftertouch;
        ledNumber = LED_ON_OFF_AFTERTOUCH;
        if (pads.getAfterTouchSendEnabled(lastTouchedPad)) ledState = ledIntensityFull; else ledState = ledIntensityOff;
        break;

        case BUTTON_ON_OFF_X:
        pads.xOnOff();
        lcdMessageType = featureX;
        ledNumber = LED_ON_OFF_X;
        if (pads.getCCXsendEnabled(lastTouchedPad)) ledState = ledIntensityFull; else ledState = ledIntensityOff;
        break;

        case BUTTON_ON_OFF_Y:
        pads.yOnOff();
        lcdMessageType = featureY;
        ledNumber = LED_ON_OFF_Y;
        if (pads.getCCYsendEnabled(lastTouchedPad)) ledState = ledIntensityFull; else ledState = ledIntensityOff;
        break;

        case BUTTON_ON_OFF_SPLIT:
        pads.setSplit();
        lcdMessageType = featureSplit;
        ledNumber = LED_ON_OFF_SPLIT;
        ledState = pads.getSplitStateLEDvalue();
        break;

        default:
        return;

    }

    leds.setLEDstate(ledNumber, ledState);
    lcDisplay.displayOnOffMessage(lcdMessageType, pads.getSplitState(), (ledState == ledIntensityFull), lastTouchedPad+1);

}

void Buttons::handleTransportControlEvent(uint8_t buttonNumber, bool state)  {

    uint8_t sysExArray[] =  { 0xF0, 0x7F, 0x7F, 0x06, 0x00, 0xF7 }; //based on MIDI spec for transport control
    transportControl_t type = transportStop;
    bool displayState = true;

    switch(buttonNumber)    {

        case BUTTON_TRANSPORT_PLAY:
        if (state) {

            sysExArray[4] = 0x02;
            type = transportPlay;
            #if MODE_SERIAL > 0
                Serial.println(F("Transport Control Play"));
            #endif

        } else return;

        break;

        case BUTTON_TRANSPORT_STOP:
        if (!state)    {

            sysExArray[4] = 0x01;
            type = transportStop;
            #if MODE_SERIAL > 0
            Serial.println(F("Transport Control Stop"));
            #endif

        } else return;
        break;

        case BUTTON_TRANSPORT_RECORD:
        if (state) {

            ledIntensity_t recordState = leds.getLEDstate(LED_TRANSPORT_RECORD);
            if (recordState == ledIntensityFull) {

                sysExArray[4] = 0x07;
                recordState = ledIntensityOff;
                #if MODE_SERIAL > 0
                    Serial.println(F("Transport Control Record Stop"));
                #endif
                displayState = false;

                }   else if (recordState == ledIntensityOff) {

                sysExArray[4] = 0x06;
                recordState = ledIntensityFull;
                #if MODE_SERIAL > 0
                    Serial.println(F("Transport Control Record Start"));
                #endif
                displayState = true;

            }

            type = transportRecord;
            leds.setLEDstate(LED_TRANSPORT_RECORD, recordState);

        } else return;

        break;

    }

    #if MODE_SERIAL < 1
        midi.sendSysEx(sysExArray, SYS_EX_ARRAY_SIZE);
    #endif

    lcDisplay.displayTransportControlMessage(type, displayState);

}

void Buttons::handleTonicEvent(tonic_t _tonic) {

    if (!pads.editModeActive())   {

        //change tonic
        //FIXME: DO NOT ALLOW THIS WHILE PADS ARE PRESSED
        for (int i=0; i<NUMBER_OF_PADS; i++)
        if (pads.getPadPressed(i))  {

            //
            return;

        }

        changeOutput_t result = pads.setTonic(_tonic);

        if (result == outputChanged)
            leds.displayActiveNoteLEDs();

        tonic_t activeTonic = pads.getActiveTonic();
        lcDisplay.displayNoteChange(result, noteChange, activeTonic);

        }   else {

        //add note to pad
        uint8_t pad = pads.getLastTouchedPad();
        lcDisplay.displayPadEditResult(pads.assignPadNote(pad, _tonic));
        pads.displayActivePadNotes(pad);
        leds.displayActiveNoteLEDs(true, pad);

    }

}

void Buttons::handleOctaveEvent(bool direction, bool state)   {

    if (buttons.getButtonPressed(BUTTON_OCTAVE_DOWN) && buttons.getButtonPressed(BUTTON_OCTAVE_UP))   {

        //try to enter pad edit mode

        if (pads.getActivePreset() >= NUMBER_OF_PREDEFINED_SCALES)    {

            pads.setEditMode(!pads.editModeActive());

            if (pads.editModeActive())  {

                //check if last touched pad is pressed
                if (pads.getPadPressed(pads.getLastTouchedPad()))   {

                    lcDisplay.displayEditModeNotAllowed(padNotReleased);
                    checkOctaveUpDownEnabled();
                    pads.setEditMode(false);

                }   else {

                    //normally, this is called in automatically in Pads.cpp
                    //but on first occasion call it manually
                    #if MODE_SERIAL > 0
                        Serial.println(F("----------------------------------------"));
                        Serial.println(F("Pad edit mode"));
                        Serial.println(F("----------------------------------------"));
                    #endif
                    pads.setupPadEditMode(pads.getLastTouchedPad());

                }

            }   else pads.exitPadEditMode();

        }   else {

            //used to "cheat" checkOctaveUpDownEnabled function
            //this will cause reset of function counters so that buttons are disabled
            //after LCD shows message that pad editing isn't allowed
            //pads.setEditMode(!pads.editModeActive());
            //checkOctaveUpDownEnabled();
            //pads.setEditMode(!pads.editModeActive());
            lcDisplay.displayEditModeNotAllowed(noUserPreset);

        }

    }   else if (checkOctaveUpDownEnabled())    {

        bool editMode = pads.editModeActive();

        switch(editMode)    {

            case true:
            //do not shift octaves while pad is pressed
            if (pads.getPadPressed(pads.getLastTouchedPad()))   {

                lcDisplay.displayEditModeNotAllowed(padNotReleased);

            }   else if (!state)   {

                pads.changeActiveOctave(direction);
                lcDisplay.displayActiveOctave(normalizeOctave(pads.getActiveOctave()));
                leds.displayActiveNoteLEDs(true, pads.getLastTouchedPad());

            }
            break;

            case false:
            if (!buttons.getButtonPressed(BUTTON_TRANSPORT_STOP))   {

                //shift entire octave

                //shift all notes up or down
                if (!state)    {

                    changeOutput_t shiftResult = pads.shiftOctave(direction);
                    int8_t activeOctave = pads.getActiveOctave();
                    lcDisplay.displayNoteChange(shiftResult, octaveChange, activeOctave);
                    direction ? leds.setLEDstate(LED_OCTAVE_UP, ledIntensityOff) : leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityOff);

                }   else {

                    //direction ? leds.setLEDstate(LED_OCTAVE_UP, ledIntensityFull) : leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityFull);

                }

            }   else {

                //shift single note
                if (state)    {

                    if (pads.getPadPressed(pads.getLastTouchedPad()))   {

                        lcDisplay.displayEditModeNotAllowed(padNotReleased);
                        return;

                }

                    //stop button is modifier
                    //disable it on release
                    buttonEnabled[BUTTON_TRANSPORT_STOP] = false;
                    changeOutput_t shiftResult = pads.shiftNote(direction);
                    lcDisplay.displayNoteChange(shiftResult, noteUpOrDown, direction);

                }

            }
            break;

        }

    }

}

bool Buttons::checkOctaveUpDownEnabled() {

    //HORRIBLE HACK!!!!!
    //please fix me someday

    //this function will check whether octave up/down is enabled
    //there are two scenarios when those two buttons should be disabled:
    //1) Pad edit mode is activated
    //2) Pad edit mode is disabled

    //in both cases octave/up down functions should be disabled until both buttons
    //have been released

    //check should start when pad edit mode is activated or deactivated and stopped when
    //both buttons are released

    static bool lastPadEditMode = false;
    bool currentPadEditMode = pads.editModeActive();
    static bool checkEnabled = false;
    bool returnValue = true;

    if (lastPadEditMode != currentPadEditMode) {

        lastPadEditMode = currentPadEditMode;
        checkEnabled = true;

    }

    if (checkEnabled) {

        //checking has been started
        if (!getButtonPressed(BUTTON_OCTAVE_DOWN) && !getButtonPressed(BUTTON_OCTAVE_UP))   {

            //both buttons have been released, stop with the checking
            //use returnValue as return variable since we don't want
            //to enable octave up/down immediately after release,
            //only when one of octave buttons has been pushed again
            if (!checkEnabled) returnValue = true; else returnValue = false;
            checkEnabled = false;

        }

    }

    //if checking is in progress, return false
    if (checkEnabled) return false;

    //if checking has been finished, return returnValue
    return returnValue;

}

void Buttons::pauseButton(uint8_t buttonNumber) {

    buttonEnabled[buttonNumber] = false;

}

Buttons buttons;
