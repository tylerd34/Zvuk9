#pragma once

#define NUMBER_OF_LED_TRANSITIONS           120
#define LED_ACTIVE_BIT                      0x00
#define LED_CONSTANT_ON_BIT                 0x01
#define LED_BLINK_ON_BIT                    0x02
#define LED_BLINK_STATE_BIT                 0x03
#define LED_INTENSITY_BIT                   0x04

#define DEFAULT_FADE_SPEED                  6
#define DEFAULT_BLINK_SPEED                 3

#define LED_NO_INTENSITY                    0
#define LED_FULL_INTENSITY                  NUMBER_OF_LED_TRANSITIONS-1
#define LED_HALF_INTENSITY                  110

const uint8_t ledTransitionScale[NUMBER_OF_LED_TRANSITIONS] =
{
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    2,
    2,
    2,
    2,
    2,
    2,
    3,
    3,
    3,
    3,
    3,
    4,
    4,
    4,
    4,
    5,
    5,
    5,
    6,
    6,
    6,
    7,
    7,
    7,
    8,
    8,
    9,
    9,
    10,
    10,
    11,
    11,
    12,
    13,
    13,
    14,
    15,
    16,
    16,
    17,
    18,
    19,
    20,
    21,
    22,
    23,
    25,
    26,
    27,
    29,
    30,
    31,
    33,
    35,
    36,
    38,
    40,
    42,
    44,
    46,
    49,
    51,
    54,
    56,
    59,
    62,
    65,
    68,
    71,
    75,
    78,
    82,
    86,
    90,
    95,
    99,
    104,
    109,
    114,
    120,
    126,
    132,
    138,
    145,
    152,
    159,
    167,
    175,
    183,
    192,
    201,
    211,
    221,
    232,
    243,
    255
};
