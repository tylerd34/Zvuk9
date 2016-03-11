#ifndef DEFS_H_
#define DEFS_H_

#include "../Scales.h"
#include "../Types.h"
#include "../midi/MIDI_parameters.h"

typedef enum {  //list of IDs for global program settings for access

    GLOBAL_PROGRAM_SETTING_ACTIVE_SCALE_ID,
    GLOBAL_PROGRAM_SETTING_ACTIVE_SCALE_TYPE_ID,
    GLOBAL_PROGRAM_SETTING_ACTIVE_SCALE_OCTAVE_ID,
    GLOBAL_PROGRAM_SETTING_ACTIVE_SCALE_TONIC_ID,
    GLOBAL_PROGRAM_SETTING_ACTIVE_SCALE_SHIFT_ID,
    GLOBAL_PROGRAM_SETTING_XY_SPLIT_STATE_ID,
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
    GLOBAL_PROGRAM_SETTING_X_CURVE_ID,
    GLOBAL_PROGRAM_SETTING_Y_CURVE_ID,
    GLOBAL_PROGRAM_SETTINGS

} globalProgramSettings;

//define default global program settings
#define GLOBAL_PROGRAM_SETTING_ACTIVE_SCALE         0x00
#define GLOBAL_PROGRAM_SETTING_ACTIVE_SCALE_TYPE    scaleNaturalMinor
#define GLOBAL_PROGRAM_SETTING_ACTIVE_SCALE_OCTAVE  DEFAULT_OCTAVE
#define GLOBAL_PROGRAM_SETTING_ACTIVE_SCALE_TONIC   C
#define GLOBAL_PROGRAM_SETTING_ACTIVE_SCALE_SHIFT   0x00
#define GLOBAL_PROGRAM_SETTING_XY_SPLIT_STATE       0x00
#define GLOBAL_PROGRAM_SETTING_X_ENABLE             0x01
#define GLOBAL_PROGRAM_SETTING_Y_ENABLE             0x01
#define GLOBAL_PROGRAM_SETTING_NOTE_ENABLE          0x01
#define GLOBAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE    0x01
#define GLOBAL_PROGRAM_SETTING_MIDI_CHANNEL         DEFAULT_MIDI_CHANNEL
#define GLOBAL_PROGRAM_SETTING_CC_X                 0x14
#define GLOBAL_PROGRAM_SETTING_CC_Y                 0x15
#define GLOBAL_PROGRAM_SETTING_X_MIN                MIN_MIDI_VALUE
#define GLOBAL_PROGRAM_SETTING_X_MAX                MAX_MIDI_VALUE
#define GLOBAL_PROGRAM_SETTING_Y_MIN                MIN_MIDI_VALUE
#define GLOBAL_PROGRAM_SETTING_Y_MAX                MAX_MIDI_VALUE
#define GLOBAL_PROGRAM_SETTING_X_CURVE              curveTypeLinear
#define GLOBAL_PROGRAM_SETTING_Y_CURVE              curveTypeLinear

//put global settings in array for easier access
const uint8_t defaultGlobalProgramSettingArray[GLOBAL_PROGRAM_SETTINGS] = {

    GLOBAL_PROGRAM_SETTING_ACTIVE_SCALE,
    GLOBAL_PROGRAM_SETTING_ACTIVE_SCALE_TYPE,
    GLOBAL_PROGRAM_SETTING_ACTIVE_SCALE_OCTAVE,
    GLOBAL_PROGRAM_SETTING_ACTIVE_SCALE_TONIC,
    GLOBAL_PROGRAM_SETTING_ACTIVE_SCALE_SHIFT,
    GLOBAL_PROGRAM_SETTING_XY_SPLIT_STATE,
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
    GLOBAL_PROGRAM_SETTING_X_CURVE,
    GLOBAL_PROGRAM_SETTING_Y_CURVE

};

typedef enum {

    LOCAL_PROGRAM_SETTING_X_ENABLE_ID,
    LOCAL_PROGRAM_SETTING_Y_ENABLE_ID,
    LOCAL_PROGRAM_SETTING_NOTE_ENABLE_ID,
    LOCAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE_ID,
    LOCAL_PROGRAM_SETTING_CC_X_ID,
    LOCAL_PROGRAM_SETTING_CC_Y_ID,
    LOCAL_PROGRAM_SETTING_X_MIN_ID,
    LOCAL_PROGRAM_SETTING_X_MAX_ID,
    LOCAL_PROGRAM_SETTING_Y_MIN_ID,
    LOCAL_PROGRAM_SETTING_Y_MAX_ID,
    LOCAL_PROGRAM_SETTING_X_CURVE_ID,
    LOCAL_PROGRAM_SETTING_Y_CURVE_ID,
    LOCAL_PROGRAM_SETTINGS

} localProgramSettings;

#define LOCAL_PROGRAM_SETTING_X_ENABLE             0x01
#define LOCAL_PROGRAM_SETTING_Y_ENABLE             0x01
#define LOCAL_PROGRAM_SETTING_NOTE_ENABLE          0x01
#define LOCAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE    0x01
#define LOCAL_PROGRAM_SETTING_CC_X                 0x14
#define LOCAL_PROGRAM_SETTING_CC_Y                 0x15
#define LOCAL_PROGRAM_SETTING_X_MIN                MIN_MIDI_VALUE
#define LOCAL_PROGRAM_SETTING_X_MAX                MAX_MIDI_VALUE
#define LOCAL_PROGRAM_SETTING_Y_MIN                MIN_MIDI_VALUE
#define LOCAL_PROGRAM_SETTING_Y_MAX                MAX_MIDI_VALUE
#define LOCAL_PROGRAM_SETTING_X_CURVE              curveTypeLinear
#define LOCAL_PROGRAM_SETTING_Y_CURVE              curveTypeLinear

const uint8_t defaultLocalProgramSettingArray[] = {

    LOCAL_PROGRAM_SETTING_X_ENABLE,
    LOCAL_PROGRAM_SETTING_Y_ENABLE,
    LOCAL_PROGRAM_SETTING_NOTE_ENABLE,
    LOCAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE,
    LOCAL_PROGRAM_SETTING_CC_X,
    LOCAL_PROGRAM_SETTING_CC_Y,
    LOCAL_PROGRAM_SETTING_X_MIN,
    LOCAL_PROGRAM_SETTING_X_MAX,
    LOCAL_PROGRAM_SETTING_Y_MIN,
    LOCAL_PROGRAM_SETTING_Y_MAX,
    LOCAL_PROGRAM_SETTING_X_CURVE,
    LOCAL_PROGRAM_SETTING_Y_CURVE

};

typedef enum {

    PREDEFINED_SCALE_OCTAVE_ID,
    PREDEFINED_SCALE_TONIC_ID,
    PREDEFINED_SCALE_SHIFT_ID,
    PREDEFINED_SCALE_PARAMETERS

} predefinedScaleParameters;

#define PREDEFINED_SCALE_OCTAVE                     DEFAULT_OCTAVE
#define PREDEFINED_SCALE_TONIC                      C
#define PREDEFINED_SCALE_SHIFT                      0

const uint8_t defaultPredefinedScaleParametersArray[] = {

    PREDEFINED_SCALE_OCTAVE,
    PREDEFINED_SCALE_TONIC,
    PREDEFINED_SCALE_SHIFT

};

#define DEFAULT_ACTIVE_PROGRAM                      0
#define DEFAULT_ACTIVE_SCALE                        0

#define DEFAULT_PAD_PRESSURE_LIMIT_LOWER            20
#define DEFAULT_PAD_PRESSURE_LIMIT_UPPER            350

#define DEFAULT_PAD_X_LIMIT_LOWER                   350
#define DEFAULT_PAD_X_LIMIT_UPPER                   700

#define DEFAULT_PAD_Y_LIMIT_LOWER                   350
#define DEFAULT_PAD_Y_LIMIT_UPPER                   700

#endif
