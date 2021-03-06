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

#pragma once

#include "../analog/pads/Pads.h"
#include "Config.h"
#include "DataTypes.h"
#include "Layout.h"
#include "Macros.h"
#include "strings/Strings.h"
#include "U8X8/U8X8.h"

///
/// \brief Display control.
/// \defgroup interfaceDisplay Display
/// \ingroup interface
/// @{

class Display
{
    public:
    Display();
    void init();
    bool update();

    void displayWelcomeMessage();

    void setDirectWriteState(bool state);

    void setupHomeScreen();
    void setupPadEditScreen(uint8_t pad, uint8_t octave, bool forceRefresh = false);
    void setupCalibrationScreen(padCoordinate_t coordinate, bool scrollCalibration);

    void displayProgramInfo(uint8_t program, uint8_t scale, note_t tonic, int8_t scaleShift);
    void displayPad(uint8_t pad = 255);
    void displayActivePadNotes(bool showNotes = true);
    void displayVelocity(uint8_t velocity = 255, int16_t rawPressure = 10000);
    void displayAftertouch(uint8_t aftertouch = 255);
    void displayXYvalue(padCoordinate_t type, midiMessageType_t messageType = midiMessageInvalidType, int16_t value1 = 10000, int16_t value2 = 10000);
    void displayMIDIchannel(uint8_t channel = 255);
    void clearPadPressData();
    void displayDeviceInfo();

    void displayChangeResult(function_t function, int16_t value, settingType_t type);
    void displayError(function_t function, changeResult_t result);

    void displayFirmwareUpdated();
    void displayFactoryResetConfirm();
    void displayFactoryResetStart();
    void displayFactoryResetEnd();

    void displayCalibrationStatus(padCoordinate_t coordinate, bool status);
    void displayPressureCalibrationTime(uint8_t seconds, uint8_t zone, bool done);

    void clearAll();
    void clearRow(uint8_t row);

    void updateText(uint8_t row, displayTextType_t textType, uint8_t startIndex);
    uint8_t getTextCenter(uint8_t textSize);

    displayTextType_t getActiveTextType();

    private:
    void updateScrollStatus(uint8_t row);
    void updateTempTextStatus();

    ///
    /// \brief Holds time index display message was shown.
    ///
    uint32_t        messageDisplayTime;

    ///
    /// \brief Holds last time display was updated.
    /// Display isn't updated in real-time but after defined amount of time (see DISPLAY_REFRESH_TIME).
    ///
    uint32_t        lastDisplayUpdateTime;

    ///
    /// \brief Holds active text type on display.
    /// Enumerated type (see displayTextType_t enumeration).
    ///
    displayTextType_t   activeTextType;

    ///
    /// \brief Array holding still display text for each display row.
    ///
    char            displayRowStillText[DISPLAY_HEIGHT][STRING_BUFFER_SIZE];

    ///
    /// \brief Array holding temp display text for each display row.
    ///
    char            displayRowTempText[DISPLAY_HEIGHT][STRING_BUFFER_SIZE];

    ///
    /// \brief Array holding true of false value representing the change of character at specific location on display row.
    /// \warning This variables assume there can be no more than 32 characters per display row.
    ///
    uint32_t        charChange[DISPLAY_HEIGHT];

    ///
    /// \brief Structure array holding scrolling information for all display rows.
    ///
    scrollEvent_t   scrollEvent[DISPLAY_HEIGHT];

    ///
    /// \brief Holds last time text has been scrolled on display.
    ///
    uint32_t        lastScrollTime;

    ///
    /// \brief Holds state of direct display writing (true/enabled, false/disabled).
    ///
    bool            directWriteState;
};

///
/// \brief External definition of Display class instance.
///
extern Display display;

/// @}
