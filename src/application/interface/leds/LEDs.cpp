/*
    Zvuk9 MIDI controller
    Copyright (C) 2014-2017 Ad Bit LLC
    Author: Igor Petrović
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    You may ONLY use this file:
    1) if you have a valid commercial Ad Bit LLC license and then in accordance with
    the terms contained in the written license agreement between you and Ad Bit LLC,
    or alternatively
    2) if you follow the terms found in GNU General Public License version 3 as
    published by the Free Software Foundation here
    <https://www.gnu.org/licenses/gpl-3.0.txt>
*/

#include "LEDs.h"
#include "../pads/Pads.h"
#include "../../database/Database.h"
#include "../pads/Pads.h"
#include "Variables.h"

const uint8_t ledNoteArray[] =
{
    LED_NOTE_C,
    LED_NOTE_C_SHARP,
    LED_NOTE_D,
    LED_NOTE_D_SHARP,
    LED_NOTE_E,
    LED_NOTE_F,
    LED_NOTE_F_SHARP,
    LED_NOTE_G,
    LED_NOTE_G_SHARP,
    LED_NOTE_A,
    LED_NOTE_A_SHARP,
    LED_NOTE_B
};

LEDs::LEDs()
{
    //default constructor
}

void LEDs::init()
{
    setFadeSpeed(DEFAULT_FADE_SPEED);
    setBlinkSpeed(DEFAULT_BLINK_SPEED);
}

void LEDs::setAllOff()
{
    for (int i=0; i<MAX_NUMBER_OF_LEDS; i++)
        setLEDstate(i, ledStateOff);
}

void LEDs::setAllOn()
{
    for (int i=0; i<MAX_NUMBER_OF_LEDS; i++)
        setLEDstate(i, ledStateFull);
}

void LEDs::setLEDstate(uint8_t ledNumber, ledState_t state)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        switch(state)
        {
            case ledStateOff:
            ledState[ledNumber] = 0;
            break;

            case ledStateDim:
            //clear intensity bit
            BIT_CLEAR(ledState[ledNumber], LED_INTENSITY_BIT);
            //set active and constant on bit
            BIT_SET(ledState[ledNumber], LED_ACTIVE_BIT);
            BIT_SET(ledState[ledNumber], LED_CONSTANT_ON_BIT);
            break;

            case ledStateFull:
            //set full intensity bit
            BIT_SET(ledState[ledNumber], LED_INTENSITY_BIT);
            //set active and constant on bit
            BIT_SET(ledState[ledNumber], LED_ACTIVE_BIT);
            BIT_SET(ledState[ledNumber], LED_CONSTANT_ON_BIT);
            break;

            default:
            return;
        }
    }

    checkBlinkLEDs();
}

void LEDs::setBlinkState(uint8_t ledID, bool state, bool forceOn)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        switch(state)
        {
            case true:
            BIT_SET(ledState[ledID], LED_BLINK_STATE_BIT);
            BIT_SET(ledState[ledID], LED_BLINK_ON_BIT);
            break;

            case false:
            BIT_CLEAR(ledState[ledID], LED_BLINK_STATE_BIT);
            BIT_CLEAR(ledState[ledID], LED_BLINK_ON_BIT);
            break;
        }

        if (forceOn)
        {
            BIT_WRITE(ledState[ledID], LED_ACTIVE_BIT, forceOn);
        }
    }

    checkBlinkLEDs();
}

bool LEDs::getBlinkState(uint8_t ledID)
{
    bool value;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        value = BIT_READ(ledState[ledID], LED_BLINK_ON_BIT);
    }

    return value;
}

void LEDs::checkBlinkLEDs()
{
    //this function will disable blinking
    //if none of the LEDs is in blinking state

    //else it will enable it

    bool _blinkEnabled = false;
    uint8_t ledState;

    //if any LED is blinking, set _blinkEnabled to true and exit the loop
    for (int i=0; i<MAX_NUMBER_OF_LEDS; i++)
    {
        ledState = getState(i);
        if (BIT_READ(ledState, LED_BLINK_ON_BIT) && BIT_READ(ledState, LED_ACTIVE_BIT))
        {
            _blinkEnabled = true;
            break;
        }
    }

    if (_blinkEnabled && !blinkEnabled)
    {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            blinkEnabled = true;
            blinkState = true;
            blinkTimerCounter = 0;
        }
    }
    else if (!_blinkEnabled && blinkEnabled)
    {
        //don't bother reseting variables if blinking is already disabled
        //reset blinkState to default value
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            blinkState = true;
            blinkTimerCounter = 0;
            blinkEnabled = false;
        }
    }
}

uint8_t LEDs::getLEDnumberFromTonic(note_t note)
{
    return ledNoteArray[note];
}

ledState_t LEDs::getLEDstate(uint8_t ledNumber)
{
    if (!BIT_READ(ledState[ledNumber], LED_CONSTANT_ON_BIT))
        return ledStateOff;
    else if (BIT_READ(ledState[ledNumber], LED_INTENSITY_BIT))
        return ledStateFull;
    else
        return ledStateDim;

    return ledStateOff;
}

void LEDs::setFadeSpeed(uint8_t speed)
{
    pwmSteps = speed;
}

void LEDs::setBlinkSpeed(uint16_t blinkTime)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        ledBlinkTime = blinkTime*100;
        blinkTimerCounter = 0;
    }
}

void LEDs::tonicLEDsOff()
{
    for (int i=0; i<MIDI_NOTES; i++)
        setLEDstate(ledNoteArray[i], ledStateOff);
}

void LEDs::setNoteLEDstate(note_t note, ledState_t state)
{
    uint8_t ledNumber = getLEDnumberFromTonic(note);
    setLEDstate(ledNumber, state);
}

ledState_t LEDs::getTonicLEDstate(note_t note)
{
    return getLEDstate(getLEDnumberFromTonic(note));
}

void LEDs::displayActiveNoteLEDs(bool padEditMode, uint8_t pad)
{
    uint8_t tonicArray[NOTES_PER_PAD],
            octaveArray[NOTES_PER_PAD],
            padNote,
            noteCounter = 0;

    switch(padEditMode)
    {
        case true:
        //indicate assigned notes in pad edit mode using note leds
        for (uint8_t i=0; i<NOTES_PER_PAD; i++)
        {
            padNote = pads.getPadNote(pad, i);

            if (padNote == BLANK_NOTE)
                continue;

            tonicArray[noteCounter] = pads.getTonicFromNote(padNote);
            octaveArray[noteCounter] = pads.getOctaveFromNote(padNote);
            noteCounter++;
        }

        //turn off all LEDs
        for (int i=0; i<MIDI_NOTES; i++)
            setNoteLEDstate((note_t)i, ledStateOff);

        //set dim led state for assigned notes on current pad
        for (int i=0; i<noteCounter; i++)
            setNoteLEDstate((note_t)tonicArray[i], ledStateDim);

        //set full led state for assigned notes on current pad if note matches current octave
        for (int i=0; i<noteCounter; i++)
        {
            if (octaveArray[i] == pads.getOctave(true))
                setNoteLEDstate((note_t)tonicArray[i], ledStateFull);
        }
        break;

        case false:
        //first, turn off all tonic LEDs
        tonicLEDsOff();
        for (int i=0; i<MIDI_NOTES; i++)
        {
            //turn tonic LED on only if corresponding note is active
            if (pads.isNoteAssigned((note_t)i))
                leds.setNoteLEDstate((note_t)i, ledStateDim);
        }
        break;
    }
}

//internal - get raw led state value
uint8_t LEDs::getState(uint8_t ledID)
{
    uint8_t returnValue;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        returnValue = ledState[ledID];
    }

    return returnValue;
}

LEDs leds;