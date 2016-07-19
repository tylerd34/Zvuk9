#include "Buttons.h"

#ifdef BUTTONS_H_

#include "../../midi/MIDI.h"

note_t Buttons::getTonicFromButton(uint8_t buttonNumber)   {

    switch(buttonNumber)    {

        case BUTTON_NOTE_C:
        return C;

        case BUTTON_NOTE_C_SHARP:
        return C_SHARP;

        case BUTTON_NOTE_D:
        return D;

        case BUTTON_NOTE_D_SHARP:
        return D_SHARP;

        case BUTTON_NOTE_E:
        return E;

        case BUTTON_NOTE_F:
        return F;

        case BUTTON_NOTE_F_SHARP:
        return F_SHARP;

        case BUTTON_NOTE_G:
        return G;

        case BUTTON_NOTE_G_SHARP:
        return G_SHARP;

        case BUTTON_NOTE_A:
        return A;

        case BUTTON_NOTE_A_SHARP:
        return A_SHARP;

        case BUTTON_NOTE_B:
        return B;

    }   return MIDI_NOTES;   //impossible case

}

void Buttons::handleOnOffEvent(uint8_t buttonNumber, bool state)    {

    //determine action based on pressed button

    uint8_t ledNumber = 0;
    functionsOnOff_t lcdMessageType;
    ledIntensity_t ledState = ledIntensityOff;
    uint8_t lastTouchedPad = pads.getLastTouchedPad();

    switch (buttonNumber)    {

        case BUTTON_ON_OFF_NOTES:
        if (state)  {

            activeShiftMode = shiftMode_note;
            return;

        }   else {

            //restore previous shift state
            //check split button since it's used for midi channel shift mode
            //if it's pressed, set midi channel shift mode when notes on/off is released
            //else restore shift mode to default
            if (getButtonState(BUTTON_ON_OFF_SPLIT))
                activeShiftMode = shiftMode_channel;
            else if (activeShiftMode == shiftMode_note)
                activeShiftMode = shiftMode_octave;

            pads.notesOnOff();
            lcdMessageType = featureNotes;
            ledNumber = LED_ON_OFF_NOTES;
            if (pads.getNoteSendEnabled(lastTouchedPad)) ledState = ledIntensityFull; else ledState = ledIntensityOff;


        }
        break;

        case BUTTON_ON_OFF_AFTERTOUCH:
        if (!state)  {

            pads.aftertouchOnOff();
            lcdMessageType = featureAftertouch;
            ledNumber = LED_ON_OFF_AFTERTOUCH;
            if (pads.getAfterTouchSendEnabled(lastTouchedPad)) ledState = ledIntensityFull; else ledState = ledIntensityOff;

        }   else return;
        break;

        case BUTTON_ON_OFF_X:
        if (!state)  {

            pads.xOnOff();
            lcdMessageType = featureX;
            ledNumber = LED_ON_OFF_X;
            if (pads.getCCXsendEnabled(lastTouchedPad)) ledState = ledIntensityFull; else ledState = ledIntensityOff;

        }   else return;
        break;

        case BUTTON_ON_OFF_Y:
        if (!state)  {

            pads.yOnOff();
            lcdMessageType = featureY;
            ledNumber = LED_ON_OFF_Y;
            if (pads.getCCYsendEnabled(lastTouchedPad)) ledState = ledIntensityFull; else ledState = ledIntensityOff;

        }   else return;
        break;

        case BUTTON_ON_OFF_SPLIT:
        if (state)  {

            activeShiftMode = shiftMode_channel;
            return;

        }   else {

            pads.splitOnOff();
            lcdMessageType = featureSplit;
            ledNumber = LED_ON_OFF_SPLIT;
            pads.getSplitState() ? ledState = ledIntensityFull : ledState = ledIntensityOff;
            //restore previous shift state
            //check note button since it's used for note shift mode
            //if it's pressed, set note shift mode when split is released
            //else restore shift mode to default
            if (getButtonState(BUTTON_ON_OFF_NOTES))
                activeShiftMode = shiftMode_note;
            else if (activeShiftMode == shiftMode_channel)
                activeShiftMode = shiftMode_octave;

        }
        break;

        default:
        return;

    }

    #ifdef MODULE_LEDS
        leds.setLEDstate(ledNumber, ledState);
    #endif

    #ifdef MODULE_LCD
        display.displayOnOff(lcdMessageType, pads.getSplitState(), (uint8_t)ledState, lastTouchedPad+1);
    #endif

}

void Buttons::handleTransportControlEvent(uint8_t buttonNumber, bool state)  {

    #if MODE_SERIAL < 1
    uint8_t sysExArray[] =  { 0xF0, 0x7F, 0x7F, 0x06, 0x00, 0xF7 }; //based on MIDI spec for transport control
    #endif

    transportControl_t type;

    if (!state)  {

        switch(buttonNumber)    {

            case BUTTON_TRANSPORT_PLAY:
            type = transportPlay;
            #if MODE_SERIAL < 1
            sysExArray[4] = 0x02;
            #endif
            #ifdef MODULE_LEDS
            leds.setLEDstate(LED_TRANSPORT_PLAY, ledIntensityFull);
            #endif
            break;

            case BUTTON_TRANSPORT_STOP:
            type = transportStop;
            #if MODE_SERIAL < 1
            sysExArray[4] = 0x01;
            #endif
            #ifdef MODULE_LEDS
            leds.setLEDstate(LED_TRANSPORT_PLAY, ledIntensityOff);
            leds.setLEDstate(LED_TRANSPORT_STOP, ledIntensityOff);
            #endif
            break;

            case BUTTON_TRANSPORT_RECORD:
            #ifdef MODULE_LEDS
            if (leds.getLEDstate(LED_TRANSPORT_RECORD) == ledIntensityFull) {

                leds.setLEDstate(LED_TRANSPORT_RECORD, ledIntensityOff);
                type = transportRecordOff;
                #if MODE_SERIAL < 1
                sysExArray[4] = 0x07;
                #endif

            }   else {

                leds.setLEDstate(LED_TRANSPORT_RECORD, ledIntensityFull);
                type = transportRecordOn;
                #if MODE_SERIAL < 1
                sysExArray[4] = 0x06;
                #endif

            }
            #endif
            break;

            default:
            return;

        }

    }   else return;

    #if MODE_SERIAL < 1
        midi.sendSysEx(sysExArray, SYS_EX_ARRAY_SIZE);
    #endif

    #ifdef MODULE_LCD
        display.displayTransportControl(type);
    #endif

}

void Buttons::handleTonicEvent(note_t note, bool state) {

    if (state) return;

    if (!pads.editModeActive())   {

        changeOutput_t result = pads.setTonic(note);
        note_t activeTonic = pads.getActiveTonic();

        switch(result)  {

            case outputChanged:
            #ifdef MODULE_LEDS
                leds.displayActiveNoteLEDs();
            #endif
            break;

            default:
            break;

        }

        //always do this
        #ifdef MODULE_LCD
            display.displayNoteChange(result, tonicChange, activeTonic);
        #endif

    }   else {

        //add note to pad
        uint8_t pad = pads.getLastTouchedPad();
        pads.assignPadNote(pad, note);
        pads.displayActivePadNotes(pad);
        #ifdef MODULE_LEDS
            leds.displayActiveNoteLEDs(true, pad);
        #endif

    }

}

void Buttons::handleOctaveEvent(bool direction, bool state)   {

    uint8_t lastTouchedPad = pads.getLastTouchedPad();

    if (buttons.getButtonState(BUTTON_OCTAVE_DOWN) && buttons.getButtonState(BUTTON_OCTAVE_UP))   {

        //try to enter pad edit mode
        if (pads.isUserScale(pads.getActivePreset()))    {

            pads.setEditMode(!pads.editModeActive());

            if (pads.editModeActive())  {

                //check if last touched pad is pressed
                if (pads.isPadPressed(pads.getLastTouchedPad()))   {

                    #ifdef MODULE_LCD
                        display.displayEditModeNotAllowed(padNotReleased);
                    #endif
                    pads.setEditMode(false);

                }   else {

                    //normally, this is called in automatically in Pads.cpp
                    //but on first occasion call it manually
                    #if MODE_SERIAL > 0
                        printf("Pad edit mode\n");
                    #endif
                    pads.setupPadEditMode(pads.getLastTouchedPad());

                    leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityFull);
                    leds.setLEDstate(LED_OCTAVE_UP, ledIntensityFull);

                }

            }   else {

                leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityOff);
                leds.setLEDstate(LED_OCTAVE_UP, ledIntensityOff);
                pads.exitPadEditMode();

            }

        }   else {

                display.displayEditModeNotAllowed(notUserPreset);
                leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityOff);
                leds.setLEDstate(LED_OCTAVE_UP, ledIntensityOff);

        }

        //temporarily disable buttons
        buttonEnabled[BUTTON_OCTAVE_UP] = false;
        buttonEnabled[BUTTON_OCTAVE_DOWN] = false;

    } else {

        bool editMode = pads.editModeActive();

        switch(editMode)    {

            case true:
            switch(state)   {

                case false:
                pads.changeActiveOctave(direction);
                #ifdef MODULE_LCD
                    display.displayActiveOctave(normalizeOctave(pads.getActiveOctave()));
                #endif
                #ifdef MODULE_LEDS
                    leds.displayActiveNoteLEDs(true, lastTouchedPad);
                    direction ? leds.setLEDstate(LED_OCTAVE_UP, ledIntensityFull) : leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityFull);
                #endif
                break;

                case true:
                #ifdef MODULE_LEDS
                    direction ? leds.setLEDstate(LED_OCTAVE_UP, ledIntensityOff) : leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityOff);
                #endif
                break;

            }
            break;

            case false:
            if (pads.isUserScale(pads.getActivePreset()) || (pads.isPredefinedScale(pads.getActivePreset()) && activeShiftMode == shiftMode_octave))   {

                //shift entire octave up or down
                if (!state)    {

                    changeOutput_t shiftResult = pads.shiftOctave(direction);
                    uint8_t activeOctave = pads.getActiveOctave();
                    #ifdef MODULE_LCD
                        display.displayNoteChange(shiftResult, octaveChange, normalizeOctave(activeOctave));
                    #endif
                    #ifdef MODULE_LEDS
                        direction ? leds.setLEDstate(LED_OCTAVE_UP, ledIntensityOff) : leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityOff);
                    #endif

                }   else {

                    #ifdef MODULE_LEDS
                        direction ? leds.setLEDstate(LED_OCTAVE_UP, ledIntensityDim) : leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityDim);
                    #endif

                }

            }   else if (activeShiftMode == shiftMode_note) {

                    //shift single note, but only in predefined presets
                    if (!state)    {

                        if (pads.isPadPressed(lastTouchedPad))   {

                            #ifdef MODULE_LCD
                                display.displayEditModeNotAllowed(padNotReleased);
                            #endif

                            return;

                        }

                        #ifdef MODULE_LEDS
                            direction ? leds.setLEDstate(LED_OCTAVE_UP, ledIntensityOff) : leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityOff);
                        #endif

                        buttonEnabled[BUTTON_ON_OFF_NOTES] = false;

                        changeOutput_t shiftResult = pads.shiftNote(direction);
                        #ifdef MODULE_LCD
                            display.displayNoteChange(shiftResult, noteShift, direction);
                        #endif

                    }   else {

                        #ifdef MODULE_LEDS
                            direction ? leds.setLEDstate(LED_OCTAVE_UP, ledIntensityFull) : leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityFull);
                        #endif

                    }

                }
                break;

            }

        }

}

shiftMode_t Buttons::getShiftMode() {

    return activeShiftMode;

}

void Buttons::setShiftMode(shiftMode_t mode)   {

    activeShiftMode = mode;

}

Buttons buttons;
#endif