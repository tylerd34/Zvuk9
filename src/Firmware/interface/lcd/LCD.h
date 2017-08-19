#pragma once

#include "../../board/Board.h"
#include "../buttons/Buttons.h"
#include "../pads/Pads.h"
#include "Config.h"
#include "DataTypes.h"
#include "Layout.h"
#include "Macros.h"
#include "strings/Strings.h"

///
/// \brief LCD control.
/// \defgroup interfaceLCD LCD
/// \ingroup interface
/// @{

class LCD
{
    public:
    LCD();
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

    void updateText(uint8_t row, lcdTextType_t textType, uint8_t startIndex);
    uint8_t getTextCenter(uint8_t textSize);

    private:

    ///
    /// \brief Holds time index LCD message was shown.
    ///
    uint32_t        messageDisplayTime;

    ///
    /// \brief Holds last time LCD was updated.
    /// LCD isn't updated in real-time but after defined amount of time (see LCD_REFRESH_TIME).
    ///
    uint32_t        lastLCDupdateTime;

    ///
    /// \brief Holds active text type on display.
    /// Enumerated type (see lcdTextType_t enumeration).
    ///
    lcdTextType_t   activeTextType;

    ///
    /// \brief Array holding still LCD text for each LCD row.
    ///
    char            lcdRowStillText[LCD_HEIGHT][MAX_TEXT_SIZE+1];

    ///
    /// \brief Array holding temp LCD text for each LCD row.
    ///
    char            lcdRowTempText[LCD_HEIGHT][MAX_TEXT_SIZE+1];

    ///
    /// \brief Array holding true of false value representing the change of character at specific location on LCD row.
    /// \warning This variables assume there can be no more than 32 characters per LCD row.
    ///
    uint32_t        charChange[LCD_HEIGHT];

    ///
    /// \brief Variable holding scrolling state for all LCD rows.
    /// \warning This variable assumes there can be no more than 8 rows on LCD.
    ///
    uint8_t         scrollEnabled;

    ///
    /// \brief Variable holding scrolling direction for all LCD rows.
    /// \warning This variable assumes there can be no more than 8 rows on LCD.
    ///
    uint8_t         scrollDirection;

    ///
    /// \brief Array holding starting scroll index for each LCD row.
    ///
    uint8_t         scrollStartIndex[LCD_HEIGHT];

    ///
    /// \brief Array holding current scroll index for each LCD row.
    ///
    uint8_t         currentScrollIndex[LCD_HEIGHT];

    ///
    /// \brief Holds last time text has been scrolled on display.
    ///
    uint32_t        lastScrollTime;

    ///
    /// \brief Holds state of direct LCD writing (true/enabled, false/disabled).
    ///
    bool            directWriteState;
};

extern LCD display;
extern U8X8_SSD1322_NHD_256X64_4W_HW_SPI u8x8;

/// @}