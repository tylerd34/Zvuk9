#pragma once

#define ENCODER_SPEED_1             1
#define ENCODER_SPEED_2             5

#define SPEED_TIMEOUT               100
#define ENCODER_DEBOUNCE_TIME       500

#define SCALE_ENC_DISABLE_MENU_EXIT 2000

#ifdef BOARD_R2
const uint8_t encoderPairs[MAX_NUMBER_OF_ENCODERS] =
{
    
};
#endif