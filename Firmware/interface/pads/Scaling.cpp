#include "Pads.h"

uint32_t Pads::map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max)   {

    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

}

uint8_t Pads::scalePressure(uint8_t pad, int16_t pressure, pressureType_t type) {

    switch(type)  {

        case pressureAftertouch:
        return map(constrain(pressure, padAftertouchLimitLower[pad], padAftertouchLimitUpper[pad]), padAftertouchLimitLower[pad], padAftertouchLimitUpper[pad], 0, 127);
        break;

        case pressureVelocity:
        return map(constrain(pressure, padPressureLimitLower[pad], padPressureLimitUpper[pad]), padPressureLimitLower[pad], padPressureLimitUpper[pad], 0, 127);
        break;

    }

    return 0;

}

inline uint8_t invertRange(uint8_t value, uint8_t min, uint8_t max) {

    return (max + min) - value;

}

uint8_t Pads::scaleXY(uint8_t padNumber, int16_t xyValue, ccType_t type) {

    switch (type)   {

        case ccTypeX:
        #if XY_FLIP_VALUES > 0
            return invertRange(map(constrain(xyValue, padXLimitLower[padNumber], padXLimitUpper[padNumber]), padXLimitLower[padNumber], padXLimitUpper[padNumber], ccXminPad[padNumber], ccXmaxPad[padNumber]), ccXminPad[padNumber], ccXmaxPad[padNumber]);
        #else
            return map(constrain(xyValue, padXLimitLower[padNumber], padXLimitUpper[padNumber]), padXLimitLower[padNumber], padXLimitUpper[padNumber], ccXminPad[padNumber], ccXmaxPad[padNumber]);
        #endif
        case ccTypeY:
        #if XY_FLIP_VALUES > 0
            return map(constrain(xyValue, padYLimitLower[padNumber], padYLimitUpper[padNumber]), padYLimitLower[padNumber], padYLimitUpper[padNumber], ccYminPad[padNumber], ccYmaxPad[padNumber]);
        #else
            return invertRange(map(constrain(xyValue, padYLimitLower[padNumber], padYLimitUpper[padNumber]), padYLimitLower[padNumber], padYLimitUpper[padNumber], ccYminPad[padNumber], ccYmaxPad[padNumber]), ccYminPad[padNumber], ccYmaxPad[padNumber]);
        #endif

    }   return 0;

}

uint8_t Pads::rawToMIDI(uint8_t padNumber, int16_t xyValue, ccType_t type) {

    switch (type)   {

        case ccTypeX:
        return map(constrain(xyValue, padXLimitLower[padNumber], padXLimitUpper[padNumber]), padXLimitLower[padNumber], padXLimitUpper[padNumber], ccXminPad[padNumber], ccXmaxPad[padNumber]);

        case ccTypeY:
        return map(constrain(xyValue, padYLimitLower[padNumber], padYLimitUpper[padNumber]), padYLimitLower[padNumber], padYLimitUpper[padNumber], ccYminPad[padNumber], ccYmaxPad[padNumber]);

    }   return 0;

}

uint8_t Pads::scale_raw(uint16_t rawValue, uint16_t rawMin, uint16_t rawMax) {

    return map(constrain(rawValue, rawMin, rawMax), rawMin, rawMax, 0, 127);

}