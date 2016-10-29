#pragma once

#include "../interface/pads/DataTypes.h"
#include "../interface/pads/curves/Curves.h"

//parameters
#define NUMBER_OF_PROGRAMS                          10

#define DEFAULT_OCTAVE                              4
#define MIDI_OCTAVE_RANGE                           11

#define MIN_MIDI_VALUE                              0
#define MAX_MIDI_VALUE                              127

#define DEFAULT_NOTE                                (DEFAULT_OCTAVE*MIDI_NOTES)
#define BLANK_NOTE                                  128

#define NUMBER_OF_USER_SCALES                       10

typedef enum
{
    //list of IDs for global program settings for access
    GLOBAL_PROGRAM_SETTING_SPLIT_STATE_ID,
    GLOBAL_PROGRAM_SETTING_X_ENABLE_ID,
    GLOBAL_PROGRAM_SETTING_Y_ENABLE_ID,
    GLOBAL_PROGRAM_SETTING_NOTE_ENABLE_ID,
    GLOBAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE_ID,
    GLOBAL_PROGRAM_SETTING_MIDI_CHANNEL_ID,
    GLOBAL_PROGRAM_SETTING_CC_X_ID,
    GLOBAL_PROGRAM_SETTING_CC_Y_ID,
    GLOBAL_PROGRAM_SETTING_X_MIN_ID,
    GLOBAL_PROGRAM_SETTING_X_MAX_ID,
    GLOBAL_PROGRAM_SETTING_Y_MIN_ID,
    GLOBAL_PROGRAM_SETTING_Y_MAX_ID,
    GLOBAL_PROGRAM_SETTING_X_CURVE_GAIN_ID,
    GLOBAL_PROGRAM_SETTING_Y_CURVE_GAIN_ID,
    GLOBAL_PROGRAM_SETTINGS
} globalProgramSettings;

//define default global program settings
#define GLOBAL_PROGRAM_SETTING_SPLIT_STATE          0x00
#define GLOBAL_PROGRAM_SETTING_X_ENABLE             0x01
#define GLOBAL_PROGRAM_SETTING_Y_ENABLE             0x01
#define GLOBAL_PROGRAM_SETTING_NOTE_ENABLE          0x01
#define GLOBAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE    0x01
#define GLOBAL_PROGRAM_SETTING_MIDI_CHANNEL         0x01
#define GLOBAL_PROGRAM_SETTING_CC_X                 0x14
#define GLOBAL_PROGRAM_SETTING_CC_Y                 0x15
#define GLOBAL_PROGRAM_SETTING_X_MIN                0
#define GLOBAL_PROGRAM_SETTING_X_MAX                127
#define GLOBAL_PROGRAM_SETTING_Y_MIN                0
#define GLOBAL_PROGRAM_SETTING_Y_MAX                127
#define GLOBAL_PROGRAM_SETTING_X_CURVE_GAIN         curveTypeLinear
#define GLOBAL_PROGRAM_SETTING_Y_CURVE_GAIN         curveTypeLinear

//put global settings in array for easier access
const uint8_t defaultGlobalProgramSettingArray[GLOBAL_PROGRAM_SETTINGS] =
{
    GLOBAL_PROGRAM_SETTING_SPLIT_STATE,
    GLOBAL_PROGRAM_SETTING_X_ENABLE,
    GLOBAL_PROGRAM_SETTING_Y_ENABLE,
    GLOBAL_PROGRAM_SETTING_NOTE_ENABLE,
    GLOBAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE,
    GLOBAL_PROGRAM_SETTING_MIDI_CHANNEL,
    GLOBAL_PROGRAM_SETTING_CC_X,
    GLOBAL_PROGRAM_SETTING_CC_Y,
    GLOBAL_PROGRAM_SETTING_X_MIN,
    GLOBAL_PROGRAM_SETTING_X_MAX,
    GLOBAL_PROGRAM_SETTING_Y_MIN,
    GLOBAL_PROGRAM_SETTING_Y_MAX,
    GLOBAL_PROGRAM_SETTING_X_CURVE_GAIN,
    GLOBAL_PROGRAM_SETTING_Y_CURVE_GAIN
};

typedef enum
{
    LOCAL_PROGRAM_SETTING_X_ENABLE_ID,
    LOCAL_PROGRAM_SETTING_Y_ENABLE_ID,
    LOCAL_PROGRAM_SETTING_NOTE_ENABLE_ID,
    LOCAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE_ID,
    LOCAL_PROGRAM_SETTING_MIDI_CHANNEL_ID,
    LOCAL_PROGRAM_SETTING_CC_X_ID,
    LOCAL_PROGRAM_SETTING_CC_Y_ID,
    LOCAL_PROGRAM_SETTING_X_MIN_ID,
    LOCAL_PROGRAM_SETTING_X_MAX_ID,
    LOCAL_PROGRAM_SETTING_Y_MIN_ID,
    LOCAL_PROGRAM_SETTING_Y_MAX_ID,
    LOCAL_PROGRAM_SETTING_X_CURVE_GAIN_ID,
    LOCAL_PROGRAM_SETTING_Y_CURVE_GAIN_ID,
    LOCAL_PROGRAM_SETTINGS
} localProgramSettings;

#define LOCAL_PROGRAM_SETTING_X_ENABLE              0x01
#define LOCAL_PROGRAM_SETTING_Y_ENABLE              0x01
#define LOCAL_PROGRAM_SETTING_NOTE_ENABLE           0x01
#define LOCAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE     0x01
#define LOCAL_PROGRAM_SETTING_MIDI_CHANNEL          0x01
#define LOCAL_PROGRAM_SETTING_CC_X                  0x14
#define LOCAL_PROGRAM_SETTING_CC_Y                  0x15
#define LOCAL_PROGRAM_SETTING_X_MIN                 0
#define LOCAL_PROGRAM_SETTING_X_MAX                 127
#define LOCAL_PROGRAM_SETTING_Y_MIN                 0
#define LOCAL_PROGRAM_SETTING_Y_MAX                 127
#define LOCAL_PROGRAM_SETTING_X_CURVE_GAIN          curveTypeLinear
#define LOCAL_PROGRAM_SETTING_Y_CURVE_GAIN          curveTypeLinear

const uint8_t defaultLocalProgramSettingArray[] =
{
    LOCAL_PROGRAM_SETTING_X_ENABLE,
    LOCAL_PROGRAM_SETTING_Y_ENABLE,
    LOCAL_PROGRAM_SETTING_NOTE_ENABLE,
    LOCAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE,
    LOCAL_PROGRAM_SETTING_MIDI_CHANNEL,
    LOCAL_PROGRAM_SETTING_CC_X,
    LOCAL_PROGRAM_SETTING_CC_Y,
    LOCAL_PROGRAM_SETTING_X_MIN,
    LOCAL_PROGRAM_SETTING_X_MAX,
    LOCAL_PROGRAM_SETTING_Y_MIN,
    LOCAL_PROGRAM_SETTING_Y_MAX,
    LOCAL_PROGRAM_SETTING_X_CURVE_GAIN,
    LOCAL_PROGRAM_SETTING_Y_CURVE_GAIN
};

typedef enum
{
    MIDI_SETTING_AFTERTOUCH_TYPE_ID,
    MIDI_SETTING_RUNNING_STATUS_ID,
    MIDI_SETTING_NOTE_OFF_TYPE_ID,
    MIDI_SETTINGS
} midiSettings;

#define MIDI_SETTING_AFTERTOUCH_TYPE                0x00 //aftertouchChannel
#define MIDI_SETTING_RUNNING_STATUS                 0x00
#define MIDI_SETTING_NOTE_OFF_TYPE                  0x00 //noteOffType_standardNoteOff

const uint8_t defaultMIDIsettingArray[] =
{
    MIDI_SETTING_AFTERTOUCH_TYPE,
    MIDI_SETTING_RUNNING_STATUS,
    MIDI_SETTING_NOTE_OFF_TYPE
};

typedef enum
{
    PREDEFINED_SCALE_OCTAVE_ID,
    PREDEFINED_SCALE_TONIC_ID,
    PREDEFINED_SCALE_SHIFT_ID,
    PREDEFINED_SCALE_PARAMETERS
} predefinedScaleParameters;

#define PREDEFINED_SCALE_OCTAVE                     DEFAULT_OCTAVE
#define PREDEFINED_SCALE_TONIC                      C
#define PREDEFINED_SCALE_SHIFT                      0

const uint8_t defaultPredefinedScaleParametersArray[] =
{
    PREDEFINED_SCALE_OCTAVE,
    PREDEFINED_SCALE_TONIC,
    PREDEFINED_SCALE_SHIFT
};

#define DEFAULT_ACTIVE_PROGRAM                      0
#define DEFAULT_ACTIVE_SCALE                        0

#define DEFAULT_PAD_X_LIMIT_LOWER                   440
#define DEFAULT_PAD_X_LIMIT_UPPER                   600

#define DEFAULT_PAD_Y_LIMIT_LOWER                   450
#define DEFAULT_PAD_Y_LIMIT_UPPER                   620

#define DEFAULT_PAD_PRESSURE_LIMIT_UPPER            380
