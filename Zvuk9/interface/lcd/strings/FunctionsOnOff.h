#ifndef FUNCTIONSONOFF_STRINGS_H_
#define FUNCTIONSONOFF_STRINGS_H_

#include <avr/pgmspace.h>

const char PROGMEM offNotes_string[] PROGMEM = "Notes off";
const char PROGMEM offAftertouch_string[] PROGMEM = "Aftertouch off";
const char PROGMEM offX_string[] PROGMEM = "X off";
const char PROGMEM offY_string[] PROGMEM = "Y off";

const char PROGMEM onNotes_string[] PROGMEM = "Notes on";
const char PROGMEM onAftertouch_string[] PROGMEM = "Aftertouch on";
const char PROGMEM onX_string[] PROGMEM = "X on";
const char PROGMEM onY_string[] PROGMEM = "Y on";

const char PROGMEM splitXY_string[] PROGMEM = "Split X, Y";
const char PROGMEM splitXYFunctions_string[] PROGMEM = "+ functions";
const char PROGMEM splitOff_string[] PROGMEM = "Split off";

PGM_P const offArray[] PROGMEM = {

    offNotes_string,
    offAftertouch_string,
    offX_string,
    offY_string

};

PGM_P const onArray[] PROGMEM = {

    onNotes_string,
    onAftertouch_string,
    onX_string,
    onY_string

};

PGM_P const splitArray[] PROGMEM = {

    splitOff_string,
    splitXY_string,
    splitXYFunctions_string,

};

#endif