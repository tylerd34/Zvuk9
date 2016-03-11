#ifndef PADS_H
#define PADS_H

#include "Arduino.h"
#include <avr/eeprom.h>
#include "../../Debug.h"
#include "../../hardware/pins/Pins.h"
#include "PadXYscales.h"
#include "../../midi/MIDI_parameters.h"
#include "PadsCalibration.h"
#include "../../Types.h"
#include "../../hardware/timer/TimerObject.h"
#include "../../midi/MIDI.h"
#include "../../eeprom/EEPROMsettings.h"

#define NUMBER_OF_PADS                      9

//DO NOT CHANGE
#define NUMBER_OF_SAMPLES                   3
#define NUMBER_OF_MEDIAN_RUNS               5

//multiplexer pins
const uint8_t muxCommonPinsAnalogRead[] = { MUX_COMMON_PIN_0_INDEX, MUX_COMMON_PIN_1_INDEX, MUX_COMMON_PIN_2_INDEX, MUX_COMMON_PIN_3_INDEX };
const uint8_t padID[] = { PAD_0, PAD_1, PAD_2, PAD_3, PAD_4, PAD_5, PAD_6, PAD_7, PAD_8 };

class Pads  {

    public:

    Pads();
    void init();
    void update();
    changeOutput_t assignPadNote(uint8_t tonic);

    //program
    bool setActiveProgram(int8_t program);

    void notesOnOff();
    void aftertouchOnOff();
    void xOnOff();
    void yOnOff();

    void setPadEditMode(bool state);
    bool getPadEditMode();
    changeOutput_t changePadNote(uint8_t tonic);

    //calibration
    bool setLowerPressureLimit(uint8_t pad, uint16_t limit);
    bool setUpperPressureLimit(uint8_t pad, uint16_t limit);
    bool setLowerXLimit(uint8_t pad, uint16_t limit);
    bool setUpperXLimit(uint8_t pad, uint16_t limit);
    bool setLowerYLimit(uint8_t pad, uint16_t limit);
    bool setUpperYLimit(uint8_t pad, uint16_t limit);

    void setSplit();

    bool noteActive(tonic_t _tonic);

    //note control
    changeOutput_t shiftOctave(bool direction);
    changeOutput_t shiftNote(bool direction, bool internalChange = false);
    changeOutput_t setTonic(tonic_t _tonic, bool internalChange = false);
    void changeActiveOctave(bool direction);

    //callbacks
    void setHandlePadEditModeCallback(void (*fptr)(uint8_t pad));
    void setHandleLEDstateCallback(void (*fptr)(uint8_t ledNumber, ledIntensity_t state));
    void setHandlePadPressCallback(void (*fptr)(uint8_t pad, uint8_t *notes, uint8_t numberOfNotes, uint8_t velocity, uint8_t ccX, uint8_t ccY));
    void setHandlePadReleaseCallback(void (*fptr)(uint8_t pad, uint8_t *notes, uint8_t numberOfNotes));
    void setHandleLCDAfterTouchCallback(void (*fptr)(uint8_t pressure));
    void setHandleLCDxyCallback(void (*fptr)(uint8_t pad, uint8_t x, uint8_t y, bool xAvailable, bool yAvailable));
    void setHandleClearPadDataCallback(void(*fptr)(uint8_t pad));

    //setters
    changeOutput_t changeCC(bool direction, ccType_t type, int8_t steps);
    changeOutput_t changeXYlimits(bool direction, ccLimitType_t ccType, int8_t steps);
    changeOutput_t changeCurve(bool direction, curveCoordinate_t coordinate, int8_t steps=1);
    changeOutput_t setMIDIchannel(uint8_t channel);

    //getters
    //features - single
    bool getNoteSendEnabled(uint8_t padNumber);
    bool getAfterTouchSendEnabled(uint8_t padNumber);
    bool getCCXsendEnabled(uint8_t padNumber);
    bool getCCYsendEnabled(uint8_t padNumber);

    tonic_t getActiveTonic();
    uint8_t getActiveOctave();
    uint8_t getMIDIchannel();
    uint8_t *getPadNotes(uint8_t padNumber);

    //split
    splitState_t getSplitState();
    ledIntensity_t getSplitStateLEDvalue();

    //CC parameters
    uint8_t getPadCCvalue(ccType_t type, uint8_t padNumber);
    uint8_t getPadCClimitValue(ccType_t type, ccLimitType_t limitType, uint8_t padNumber);
    curveType_t getPadCurve(curveCoordinate_t curve, uint8_t padNumber);

    //notes
    tonic_t getTonicFromNote(uint8_t note);
    uint8_t getOctaveFromNote(uint8_t note);

    //last touched pad - 0 default
    uint8_t getLastTouchedPad();

    //check if selected pad is still pressed
    bool getPadPressed(uint8_t padNumber);

    uint8_t getActivePreset();

    bool setActivePreset(uint8_t preset);

    uint8_t getActiveProgram();

    private:

    void checkOctaveShift();
    bool checkPadsPressed();

    //init
    void initPadPins();
    void initVariables();
    void setUpADCtimer();

    //EEPROM config read
    void getPadConfig();
    void getProgramParameters();
    void getPresetParameters();
    void getPadLimits();
    void getPressureLimits();
    void getXLimits();
    void getYLimits();
    void getAfterTouchUpperPressureLimits();

    //hardware control
    void setMuxInput(uint8_t muxInput);
    void setupPressure();
    int16_t getPressure();
    void setupX();
    int16_t getX();
    void setupY();
    int getY();
    void resetPadReadOrder();
    void setAnalogInput(uint8_t pin);

    bool readXY();
    void setReadXY(bool state);

    bool nextPad();
    void setNextPad();

    bool processPressure();
    bool processXY();
    void checkAftertouch();

    int16_t getMedianValueZXY(coordinateType_t);

    //adc
    void setFastADC();
    void analogReadStart();
    void enableADCinterrupt();
    int16_t getAnalogValue();
    bool analogReadInProgress();
    uint8_t getADCpinCounter();
    void setADCpinCounter(uint8_t value);

    //calibration
    uint8_t calibratePressure(uint8_t pad, int16_t pressure, pressureType_t type);
    uint8_t calibrateXY(uint8_t padNumber, int16_t xyValue, ccType_t type);

    //pad data processing
    void addPressureSamples(uint16_t pressure);
    void addXYSamples(uint16_t xValue,  uint16_t yValue);
    bool pressureSampled();
    bool xySampled();

    //pad press
    void setPadPressed(uint8_t padNumber, bool padState);
    bool pressureStable(uint8_t padNumber, uint8_t pressDetected);

    //callbacks
    void (*sendPadEditModeCallback)(uint8_t pad);
    void (*sendLEDstateCallback)(uint8_t ledNumber, ledIntensity_t state);
    void (*sendPadPressCallback)(uint8_t pad, uint8_t *notes, uint8_t numberOfNotes, uint8_t velocity, uint8_t ccX, uint8_t ccY);
    void (*sendPadReleaseCallback)(uint8_t pad, uint8_t *notes, uint8_t numberOfNotes);
    void (*sendLCDAfterTouchCallback)(uint8_t pressure);
    void (*sendLCDxyCallback)(uint8_t pad, uint8_t x, uint8_t y, bool xAvailable, bool yAvailable);
    void (*sendClearPadDataCallback)(uint8_t pad);

    //preset
    scaleType_t getScaleType(uint8_t scale);

    //notes
    void setNoteSendEnabled(uint8_t padNumber, uint8_t state);
    void clearPadNote(uint8_t padNumber);
    uint8_t getNumberOfAssignedNotes(uint8_t padNumber);

    //aftertouch
    bool getAfterTouchGestureActivated(uint8_t padNumber, uint8_t calibratedPressure);
    void resetAfterTouchCounters(uint8_t padNumber);
    void setAfterTouchSendEnabled(uint8_t padNumber, uint8_t state);

    //CC
    void setCCXsendEnabled(uint8_t padNumber, uint8_t state);
    void setCCYsendEnabled(uint8_t padNumber, uint8_t state);

    //MIDI send
    void storePadNotes();
    void sendPadAftertouch();
    void sendPadXY();

    void checkXY();
    void checkVelocity();

    void getPadParameters();
    void setFunctionLEDs(uint8_t pad);

    bool pressureMIDIdataAvailable();
    bool afterTouchMIDIdataAvailable();
    bool xyMIDIdataAvailable();
    void checkNoteBuffer();
    void updateLastTouchedPad();
    void checkMIDIdata();

    void generateScale(scale_t scale);
    bool isUserScale(uint8_t scale);
    bool isPredefinedScale(uint8_t scale);

    uint8_t midiVelocity;
    bool midiNoteOnOff;
    uint8_t midiX;
    uint8_t midiY;
    bool velocityAvailable;
    bool xyAvailable;
    bool xAvailable;
    bool yAvailable;
    bool afterTouchAvailable;
    uint8_t midiAfterTouch;
    int8_t shiftedNote;

    uint8_t     selectedMuxChannel;

    bool        padEditMode;

    uint8_t     lastPadState[NUMBER_OF_PADS],
                sampleCounter;

    uint16_t    padPressed;

    int8_t      midiChannel;

    int16_t     lastPressureValue[NUMBER_OF_PADS],
                lastAfterTouchValue[NUMBER_OF_PADS],
                initialPressure[NUMBER_OF_PADS],
                initialXvalue[NUMBER_OF_PADS],
                initialYvalue[NUMBER_OF_PADS],
                lastXValue[NUMBER_OF_PADS],
                lastYValue[NUMBER_OF_PADS];

    uint8_t     lastXMIDIvalue[NUMBER_OF_PADS],
                lastYMIDIvalue[NUMBER_OF_PADS];

    int8_t      ccXPad[NUMBER_OF_PADS],
                ccYPad[NUMBER_OF_PADS],
                ccXminPad[NUMBER_OF_PADS],
                ccXmaxPad[NUMBER_OF_PADS],
                ccYminPad[NUMBER_OF_PADS],
                ccYmaxPad[NUMBER_OF_PADS];

    uint8_t     splitCounter;
    uint8_t     padSplitState;
    int8_t      localOctaveValue;

    uint8_t     padNote[NUMBER_OF_PADS][NOTES_PER_PAD];

    uint8_t     lastTouchedPad;
    int8_t      activePreset,
                activeProgram;
    uint32_t    lastSwitchTime;

    int16_t     padPressureLimitLower[NUMBER_OF_PADS],
                padPressureLimitUpper[NUMBER_OF_PADS],
                padXLimitLower[NUMBER_OF_PADS],
                padXLimitUpper[NUMBER_OF_PADS],
                padYLimitLower[NUMBER_OF_PADS],
                padYLimitUpper[NUMBER_OF_PADS],
                padPressureLimitUpperAfterTouch[NUMBER_OF_PADS];

    bool        afterTouchActivated[NUMBER_OF_PADS];

    uint32_t    afterTouchGestureTimer[NUMBER_OF_PADS];
    uint8_t     afterTouchGestureCounter[NUMBER_OF_PADS],
                lastAfterTouchGestureDirection[NUMBER_OF_PADS];

    uint8_t     activePad;
    bool        switchToNextPad;
    uint32_t    afterTouchSendTimer[NUMBER_OF_PADS];
    uint32_t    xSendTimer[NUMBER_OF_PADS],
                ySendTimer[NUMBER_OF_PADS];

    bool        initialXYignored[NUMBER_OF_PADS];
    uint8_t     sampleCounterPressure,
                sampleCounterXY;

    int16_t     xValueSamples[NUMBER_OF_SAMPLES],
                yValueSamples[NUMBER_OF_SAMPLES],
                pressureValueSamples[NUMBER_OF_SAMPLES];

    int8_t      padCurveX[NUMBER_OF_PADS],
                padCurveY[NUMBER_OF_PADS];

    bool switchToXYread;

    uint8_t     medianRunCounterXY,
                medianRunCounterPressure;

    int16_t     xAvg,
                yAvg,
                pressureAvg;

    uint32_t    padDebounceTimer[NUMBER_OF_PADS];
    bool padDebounceTimerStarted[NUMBER_OF_PADS];
    uint32_t lastPadCheckTime;

    bool        firstXYValueDelayTimerStarted[NUMBER_OF_PADS],
                firstPressureValueDelayTimerStarted[NUMBER_OF_PADS];

    uint32_t    firstXYValueDelayTimer[NUMBER_OF_PADS],
                firstPressureValueDelayTimer[NUMBER_OF_PADS];

    int8_t      previousPad;
    bool padMovementDetected;
    uint8_t lastVelocityValue[NUMBER_OF_PADS];

    bool        octaveShiftScheduled;
    int8_t      shiftAmount;

    bool        xSendEnabled[NUMBER_OF_PADS],
                ySendEnabled[NUMBER_OF_PADS],
                noteSendEnabled[NUMBER_OF_PADS],
                aftertouchSendEnabled[NUMBER_OF_PADS];

};

extern Pads pads;
#endif
