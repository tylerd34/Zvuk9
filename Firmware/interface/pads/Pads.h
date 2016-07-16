#ifndef PADS_H
#define PADS_H

#include "../../Modules.h"
#include <avr/eeprom.h>
#include <stdlib.h>
#include "../../Debug.h"
#include "../../hardware/pins/Pins.h"
#include "XYscales.h"
#include "../../midi/MIDI_parameters.h"
#include "Calibration.h"
#include "../../Types.h"
#include "../../hardware/adc/ADC.h"
#include "../../eeprom/Configuration.h"
#include "../../midi/MIDI.h"
#include "../lcd/Macros.h"
#include "../../hardware/timer/TimerObject.h"

#ifdef MODULE_LEDS
#include "../leds/LEDs.h"
#endif

#ifdef MODULE_LCD
#include "../lcd/LCD.h"
#include "../lcd/menu/Menu.h"
#endif

#define CONNECTED_PADS          9
#define MAX_PADS                9

#define NUMBER_OF_SAMPLES       3

#define DEFAULT_XY_AT_VALUE     255
#define PAD_NOTE_BUFFER_SIZE    32

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

//multiplexer pins
const uint8_t muxCommonPinsAnalogRead[] = { MUX_COMMON_PIN_0_INDEX, MUX_COMMON_PIN_1_INDEX, MUX_COMMON_PIN_2_INDEX, MUX_COMMON_PIN_3_INDEX };
const uint8_t padID[] = { PAD_0, PAD_1, PAD_2, PAD_3, PAD_4, PAD_5, PAD_6, PAD_7, PAD_8 };

class Pads  {

    public:

    //init
    Pads();
    void init();
    void update();

    //program/preset
    //getters
    uint8_t getActiveProgram();
    uint8_t getActivePreset();
    //setters
    bool setActiveProgram(int8_t& program);
    bool setActivePreset(int8_t& preset);

    //pad edit mode
    void setEditMode(bool state);
    bool editModeActive();
    void setupPadEditMode(uint8_t& pad);
    void exitPadEditMode();
    void displayActivePadNotes(uint8_t& pad);
    changeOutput_t assignPadNote(uint8_t& pad, note_t& note);
    void changeActiveOctave(bool& direction);

    //functions on/off
    //getters
    bool getNoteSendEnabled(uint8_t& padNumber);
    bool getAfterTouchSendEnabled(uint8_t& padNumber);
    bool getCCXsendEnabled(uint8_t& padNumber);
    bool getCCYsendEnabled(uint8_t& padNumber);
    aftertouchType_t getAftertouchType();
    bool getSplitState();
    //setters
    void notesOnOff();
    void aftertouchOnOff();
    void xOnOff();
    void yOnOff();
    void splitOnOff();
    void setAftertouchType(aftertouchType_t type);

    //notes
    //getters
    uint8_t getActiveOctave();
    uint8_t getPadNote(uint8_t& pad, uint8_t& note);
    note_t getActiveTonic();
    bool noteActive(note_t note);
    note_t getTonicFromNote(uint8_t note);
    uint8_t getOctaveFromNote(uint8_t note);
    shiftMode_t getShiftMode();
    //setters
    void setNoteShiftMode(shiftMode_t mode);
    changeOutput_t shiftOctave(bool& direction);
    changeOutput_t shiftNote(bool direction, bool internalChange = false);
    changeOutput_t setTonic(note_t note, bool internalChange = false);

    //CC
    //getters
    uint8_t getCCvalue(ccType_t type, uint8_t padNumber);
    uint8_t getCClimitValue(ccType_t type, ccLimitType_t limitType, uint8_t padNumber);
    curveType_t getCCcurve(curveCoordinate_t curve, uint8_t padNumber);
    //setters
    changeOutput_t changeCC(bool direction, ccType_t type, int8_t steps);
    changeOutput_t changeCClimits(bool direction, ccLimitType_t ccType, int8_t steps);
    changeOutput_t changeCCcurve(bool direction, curveCoordinate_t coordinate, int8_t steps=1);

    //midi channel
    uint8_t getMIDIchannel(uint8_t& pad);
    bool setMIDIchannel(uint8_t& pad, uint8_t& channel);

    //pad info
    uint8_t getLastTouchedPad();
    bool isPadPressed(uint8_t padNumber);
    bool allPadsReleased();

    //scale info
    bool isUserScale(uint8_t scale);
    bool isPredefinedScale(uint8_t scale);

    //calibration
    bool calibrate(coordinateType_t type, calibrationDirection direction, uint8_t pad, uint16_t limit);
    void setCalibrationMode(bool state, coordinateType_t type = coordinateX);
    void setCalibration(coordinateType_t type, calibrationDirection direction, bool state);
    void resetCalibration();

    private:

    //init
    void initPadPins();
    void initHardware();

    //EEPROM config read
    void getConfiguration();
    void getProgramParameters();
    void getPresetParameters();
    void getPadLimits();
    void getPressureLimits();
    void getXLimits();
    void getYLimits();
    void getAftertouchLimits();
    void getPadParameters();

    //hardware control
    void setMuxInput(uint8_t muxInput);
    void setNextPad();
    void setupPressure();
    int16_t getPressure();
    void setupX();
    int16_t getX();
    void setupY();
    int16_t getY();

    //midi scaling
    uint8_t scalePressure(uint8_t pad, int16_t pressure, pressureType_t type);
    uint8_t scaleXY(uint8_t& pad, int16_t xyValue, ccType_t type);
    uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max);

    //data sampling/debouncing
    void addPressureSamples(int16_t& pressure);
    void addXYSamples(int16_t& xValue, int16_t& yValue);
    bool pressureSampled();
    bool xySampled();
    bool pressureStable(uint8_t& padNumber, bool& pressDetected);
    int16_t getMedianValueXYZ(coordinateType_t coordinate);

    //data processing
    bool pressureUpdated();
    bool xyUpdated(uint8_t& pad);

    //data availability checks
    bool checkAftertouch(uint8_t& pad);
    bool checkX(uint8_t& pad);
    bool checkY(uint8_t& pad);
    bool checkVelocity(uint8_t& pad);

    //pad press updating/info
    void setPadPressed(uint8_t& padNumber, bool padState);
    void updateLastPressedPad(uint8_t& pad, bool state);

    //lcd/led handling on midi event
    void handleNoteLEDs(uint8_t pad, bool state);
    void handleNoteLCD(uint8_t pad, uint8_t velocity, bool state);
    void handleXYlcd(uint8_t pad, uint8_t xPosition, uint8_t yPosition, bool xEnabled, bool yEnabled);

    //scale
    scaleType_t getScaleType(int8_t& scale);
    void generateScale(scale_t scale);

    //features on/off
    void setNoteSendEnabled(uint8_t padNumber, bool state);
    void setCCXsendEnabled(uint8_t padNumber, bool state);
    void setCCYsendEnabled(uint8_t padNumber, bool state);
    void setAfterTouchSendEnabled(uint8_t padNumber, bool state);
    void setFunctionLEDs(uint8_t& padNumber);

    //MIDI send
    void checkMIDIdata(uint8_t& pad, bool& velocityAvailable, bool& aftertouchAvailable, bool& xAvailable, bool& yAvailable);
    void checkNoteBuffer();
    void checkLCDdata(uint8_t& pad, bool velocityAvailable, bool aftertouchAvailable, bool xAvailable, bool yAvailable);
    void sendAftertouch(uint8_t& pad);
    void sendX(uint8_t& pad);
    void sendY(uint8_t& pad);
    void sendNotes(uint8_t pad, uint8_t velocity, bool state);

    //note buffer
    void storeNotes(uint8_t& pad);

    //pad press history buffer
    void updatePressHistory(uint8_t& pad);
    void clearTouchHistoryPad(uint8_t& pad);

    //internal checking functions
    void checkRemainingOctaveShift();
    void checkRemainingNoteShift();

    //last midi values
    uint8_t     lastXMIDIvalue[MAX_PADS],
                lastYMIDIvalue[MAX_PADS];

    uint8_t     lastVelocityValue[MAX_PADS],
                lastAftertouchValue[MAX_PADS],
                lastMIDInoteState[MAX_PADS];

    //last raw pressure value
    //needed to get correct aftertouch value
    int16_t     lastPressureValue[MAX_PADS];

    //median value samples get stored here (3 samples)
    int16_t     xValueSamples[NUMBER_OF_SAMPLES],
                yValueSamples[NUMBER_OF_SAMPLES],
                pressureValueSamples[NUMBER_OF_SAMPLES];

    //store press states for all pads inside this variable
    uint16_t    padPressed;

    //parameters from eeprom
    int8_t      ccXPad[MAX_PADS],
                ccYPad[MAX_PADS],
                ccXminPad[MAX_PADS],
                ccXmaxPad[MAX_PADS],
                ccYminPad[MAX_PADS],
                ccYmaxPad[MAX_PADS];

    bool        xSendEnabled[MAX_PADS],
                ySendEnabled[MAX_PADS],
                noteSendEnabled[MAX_PADS],
                aftertouchSendEnabled[MAX_PADS];

    int8_t      padCurveX[MAX_PADS],
                padCurveY[MAX_PADS];

    int16_t     padPressureLimitLower[MAX_PADS],
                padPressureLimitUpper[MAX_PADS],
                padXLimitLower[MAX_PADS],
                padXLimitUpper[MAX_PADS],
                padYLimitLower[MAX_PADS],
                padYLimitUpper[MAX_PADS],
                padAftertouchLimitLower[MAX_PADS],
                padAftertouchLimitUpper[MAX_PADS];

    uint8_t     padNote[MAX_PADS][NOTES_PER_PAD];

    uint8_t     midiChannel[MAX_PADS];
    uint8_t     aftertouchType;
    uint8_t     maxAftertouchValue;

    bool        splitState;

    int8_t      activeOctave;

    int8_t      activePreset,
                activeProgram;

    shiftMode_t activeShiftMode;

    //debouncing
    bool        padDebounceTimerStarted[MAX_PADS],
                firstPressureValueDelayTimerStarted[MAX_PADS],
                aftertouchActivated[MAX_PADS];

    uint32_t    padDebounceTimer[MAX_PADS],
                firstPressureValueDelayTimer[MAX_PADS],
                xSendTimer[MAX_PADS],
                ySendTimer[MAX_PADS],
                afterTouchSendTimer[MAX_PADS],
                aftertouchActivationDelay[MAX_PADS];

    uint8_t     sampleCounterPressure,
                sampleCounterXY;

    //pad read control
    uint8_t     activePad;
    bool        switchToNextPad,
                switchToXYread;

    //used to shift octave once all pads are released
    int8_t      octaveShiftAmount[MAX_PADS];
    uint16_t    octaveShiftPadBuffer;

    //used to shift scale once all pads are released
    int8_t      noteShiftAmount[MAX_PADS];
    uint16_t    scaleShiftPadBuffer;

    //pad press history buffer
    uint8_t     padPressHistory_buffer[MAX_PADS];
    uint8_t     padPressHistory_counter;

    //note buffer
    uint8_t     pad_buffer[PAD_NOTE_BUFFER_SIZE];
    uint8_t     velocity_buffer[PAD_NOTE_BUFFER_SIZE];
    uint32_t    pad_note_timer_buffer[PAD_NOTE_BUFFER_SIZE];
    uint8_t     note_buffer_head;
    uint8_t     note_buffer_tail;

    //calibration
    bool        calibrationEnabled;
    int16_t     minCalibrationValue,
                maxCalibrationValue;
    coordinateType_t activeCalibration;

};

extern Pads pads;
#endif
