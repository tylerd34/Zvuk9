#include "Board.h"

volatile int8_t      encoderMoving[NUMBER_OF_ENCODERS];
uint16_t             encoderData[NUMBER_OF_ENCODERS];

volatile uint8_t *encoderPort1Array[] = {

    &ENCODER_PAIR_00_PIN_0_PORT,
    &ENCODER_PAIR_01_PIN_0_PORT,
    &ENCODER_PAIR_02_PIN_0_PORT,
    &ENCODER_PAIR_03_PIN_0_PORT,
    &ENCODER_PAIR_04_PIN_0_PORT,
    &ENCODER_PAIR_05_PIN_0_PORT,
    &ENCODER_PAIR_06_PIN_0_PORT,
    &ENCODER_PAIR_07_PIN_0_PORT,
    &ENCODER_PAIR_08_PIN_0_PORT,
    &ENCODER_PAIR_09_PIN_0_PORT

};

volatile uint8_t *encoderPort2Array[] = {

    &ENCODER_PAIR_00_PIN_1_PORT,
    &ENCODER_PAIR_01_PIN_1_PORT,
    &ENCODER_PAIR_02_PIN_1_PORT,
    &ENCODER_PAIR_03_PIN_1_PORT,
    &ENCODER_PAIR_04_PIN_1_PORT,
    &ENCODER_PAIR_05_PIN_1_PORT,
    &ENCODER_PAIR_06_PIN_1_PORT,
    &ENCODER_PAIR_07_PIN_1_PORT,
    &ENCODER_PAIR_08_PIN_1_PORT,
    &ENCODER_PAIR_09_PIN_1_PORT

};

int8_t Board::getEncoderState(uint8_t encoderNumber)  {

    int8_t returnValue;
    returnValue = encoderMoving[encoderNumber];
    encoderMoving[encoderNumber] = 0;
    return returnValue;

}