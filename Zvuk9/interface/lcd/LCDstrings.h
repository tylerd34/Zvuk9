#ifndef LCDSTRINGS_H_
#define LCDSTRINGS_H_

#include <avr/pgmspace.h>
#include "Arduino.h"

const char PROGMEM tonicC_string[] PROGMEM = "C";
const char PROGMEM tonicCSharp_string[] PROGMEM = "C#";
const char PROGMEM tonicD_string[] PROGMEM = "D";
const char PROGMEM tonicDSharp_string[] PROGMEM = "D#";
const char PROGMEM tonicE_string[] PROGMEM = "E";
const char PROGMEM tonicF_string[] PROGMEM = "F";
const char PROGMEM tonicFSharp_string[] PROGMEM = "F#";
const char PROGMEM tonicG_string[] PROGMEM = "G";
const char PROGMEM tonicGSharp_string[] PROGMEM = "G#";
const char PROGMEM tonicA_string[] PROGMEM = "A";
const char PROGMEM tonicASharp_string[] PROGMEM = "A#";
const char PROGMEM tonicB_strig[] PROGMEM = "B";

const char PROGMEM presetNaturalMinor_string[] PROGMEM = "Natural Minor";
const char PROGMEM presetMelodicMinor_string[] PROGMEM = "Melodic Minor";
const char PROGMEM presetHarmonicMinor_string[] PROGMEM = "Harmonic Minor";
const char PROGMEM presetMajor_string[] PROGMEM = "Major";
const char PROGMEM presetHarmonicMajor_string[] PROGMEM = "Harmonic Major";
const char PROGMEM presetMinorPentatonic_string[] PROGMEM = "Minor Pentatonic";
const char PROGMEM presetMajorPentatonic_string[] PROGMEM = "Major Pentatonic";

const char PROGMEM offGlobalNotes_string[] PROGMEM = "Notes off";
const char PROGMEM offGlobalAftertouch_string[] PROGMEM = "Aftertouch off";
const char PROGMEM offGlobalX_string[] PROGMEM = "X off";
const char PROGMEM offGlobalY_string[] PROGMEM = "Y off";

const char PROGMEM onGlobalNotes_string[] PROGMEM = "Notes on";
const char PROGMEM onGlobalAftertouch_string[] PROGMEM = "Aftertouch on";
const char PROGMEM onGlobalX_string[] PROGMEM = "X on";
const char PROGMEM onGlobalY_string[] PROGMEM = "Y on";

const char PROGMEM offLocalNotes_string[] PROGMEM = "Notes off";
const char PROGMEM offLocalAftertouch_string[] PROGMEM = "Aftertouch off";
const char PROGMEM offLocalX_string[] PROGMEM = "X off";
const char PROGMEM offLocalY_string[] PROGMEM = "Y off";

const char PROGMEM onLocalNotes_string[] PROGMEM = "Notes on";
const char PROGMEM onLocalAftertouch_string[] PROGMEM = "Aftertouch on";
const char PROGMEM onLocalX_string[] PROGMEM = "X on";
const char PROGMEM onLocalY_string[] PROGMEM = "Y on";

const char PROGMEM splitXY_string[] PROGMEM = "Split X, Y";
const char PROGMEM splitXYFunctions_string[] PROGMEM = "+ functions";
const char PROGMEM splitOff_string[] PROGMEM = "Split off";

const char PROGMEM userEditError0[] PROGMEM = "Select user preset";
const char PROGMEM userEditError1[] PROGMEM = "to edit pad notes";
const char PROGMEM userEditError2[] PROGMEM = "Release pad";
const char PROGMEM userEditError3[] PROGMEM = "to edit";

const String emptyLine = "                                        ";

const String helloMessage = "Welcome!";

const char PROGMEM curveCoordinateX_string[] PROGMEM = "X curve=";
const char PROGMEM curveCoordinateY_string[] PROGMEM = "Y curve=";

const char PROGMEM curveTypeLinear_string[] PROGMEM = "linear";
const char PROGMEM curveTypeWideMiddle_string[] PROGMEM = "wide middle";
const char PROGMEM curveTypeWideEnds_string[] PROGMEM = "wide ends";

const char PROGMEM xMax_string[] PROGMEM = "X max=";
const char PROGMEM xMin_string[] PROGMEM = "X min=";
const char PROGMEM yMax_string[] PROGMEM = "Y max=";
const char PROGMEM yMin_string[] PROGMEM = "Y min=";

const char PROGMEM xCC_string[] PROGMEM = "X CC=";
const char PROGMEM yCC_string[] PROGMEM = "Y CC=";

const char PROGMEM xCCid_string[] PROGMEM = "ccx";
const char PROGMEM yCCid_string[] PROGMEM = "ccy";

const char PROGMEM changeType0[] PROGMEM = "Tonic ";
const char PROGMEM changeType1[] PROGMEM = "Octave ";
const char PROGMEM changeType2[] PROGMEM = "CC ";
const char PROGMEM changeType3[] PROGMEM = "CC limit ";
const char PROGMEM changeType4[] PROGMEM = "Curve ";
const char PROGMEM changeType5[] PROGMEM = "MIDI channel ";

const char PROGMEM transportControlPlay_string[] PROGMEM = "Play";
const char PROGMEM transportControlStop_string[] PROGMEM = "Stop";
const char PROGMEM transportControlRecordOn_string[] PROGMEM = "Record on";
const char PROGMEM transportControlRecordOff_string[] PROGMEM = "Record off";

const char PROGMEM padAmountSingle_string[] = "pad ";
const char PROGMEM padAmountAll_string[] = "all pads";

PGM_P const transportControlChangeArray[] PROGMEM = {

    transportControlPlay_string,
    transportControlStop_string,
    transportControlRecordOn_string,
    transportControlRecordOff_string

};

PGM_P const noteNameArray[] PROGMEM = {

    tonicC_string,
    tonicCSharp_string,
    tonicD_string,
    tonicDSharp_string,
    tonicE_string,
    tonicF_string,
    tonicFSharp_string,
    tonicG_string,
    tonicGSharp_string,
    tonicA_string,
    tonicASharp_string,
    tonicB_strig

};

PGM_P const padEditErrorArray[] PROGMEM = {

    userEditError0,
    userEditError1,
    userEditError2,
    userEditError3

};

PGM_P const presetNameArray[] PROGMEM = {

    presetNaturalMinor_string,
    presetMelodicMinor_string,
    presetHarmonicMinor_string,
    presetMajor_string,
    presetHarmonicMajor_string,
    presetMinorPentatonic_string,
    presetMajorPentatonic_string

};

PGM_P const offGlobalArray[] PROGMEM = {

    offGlobalNotes_string,
    offGlobalAftertouch_string,
    offGlobalX_string,
    offGlobalY_string

};

PGM_P const onGlobalArray[] PROGMEM = {

    onGlobalNotes_string,
    onGlobalAftertouch_string,
    onGlobalX_string,
    onGlobalY_string

};

PGM_P const offLocalArray[] PROGMEM = {

    offLocalNotes_string,
    offLocalAftertouch_string,
    offLocalX_string,
    offLocalY_string

};

PGM_P const onLocalArray[] PROGMEM = {

    onLocalNotes_string,
    onLocalAftertouch_string,
    onLocalX_string,
    onLocalY_string

};

PGM_P const splitArray[] PROGMEM = {

    splitXY_string,
    splitXYFunctions_string,
    splitOff_string

};

PGM_P const curveCoordinateArray[] PROGMEM = {

    curveCoordinateX_string,
    curveCoordinateY_string

};

PGM_P const curveNameArray[] PROGMEM = {

    curveTypeLinear_string,
    curveTypeWideMiddle_string,
    curveTypeWideEnds_string

};

PGM_P const ccLimitArray[] PROGMEM = {

    xMin_string,
    xMax_string,
    yMin_string,
    yMax_string

};

PGM_P const ccArray[] PROGMEM = {

    xCC_string,
    yCC_string

};

PGM_P const ccIDarray[] PROGMEM = {

    xCCid_string,
    yCCid_string

};

PGM_P const changeTypeArray[] PROGMEM = {

    changeType0,
    changeType1,
    changeType2,
    changeType3,
    changeType4,
    changeType5

};

PGM_P const padAmountArray[] PROGMEM = {

    padAmountAll_string,
    padAmountSingle_string,
    padAmountSingle_string,

};

#endif