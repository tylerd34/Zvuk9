#include "Pads.h"

int16_t Pads::getPressure()  {

    static int16_t  tempPressureValueZ1 = 0,
                    tempPressureValueZ2 = 0;

    static bool firstPin = true;

    static bool firstPressurePinFinished = false;
    static bool secondPressurePinFinished = false;

    bool readInitiated = analogReadInProgress();
    int16_t returnValue = -1;

    //conversion isn't started yet, setup pressure and request first value
    if (!readInitiated && (!firstPressurePinFinished && !secondPressurePinFinished) && firstPin)   {

        setupPressure();
        setAnalogInput(readAftertouch0);
        analogReadStart();
        firstPin = false;

    }   else

    //conversion finished, save first value and request second
    if (!readInitiated && !firstPressurePinFinished && !secondPressurePinFinished && !firstPin)   {

        firstPressurePinFinished = true;
        tempPressureValueZ1 = getAnalogValue();
        setAnalogInput(readAftertouch1);
        analogReadStart();
        firstPin = true;

    }   else

    //second conversion finished, save value
    if (!readInitiated && firstPressurePinFinished && !secondPressurePinFinished)    {

        secondPressurePinFinished = true;
        tempPressureValueZ2 = getAnalogValue();

        firstPressurePinFinished = false;
        secondPressurePinFinished = false;
        returnValue = (1023 - (tempPressureValueZ2 - tempPressureValueZ1));

        tempPressureValueZ1 = 0;
        tempPressureValueZ2 = 0;

    }

    return returnValue;

}

int16_t Pads::getX()  {

    bool readInitiated = analogReadInProgress();
    static bool xSwitched = false;

    if (!readInitiated && !xSwitched)   {

        analogReadStart();
        xSwitched = true;
        return -1;

    }   else if (readInitiated && xSwitched)   return -1;
    else {

        xSwitched = false;

        return getAnalogValue();

    }

}

int16_t Pads::getY()  {

    bool readInitiated = analogReadInProgress();
    static bool ySwitched = false;

    if (!readInitiated && !ySwitched)   {

        analogReadStart();
        ySwitched = true;
        return -1;

    }   else if (readInitiated && ySwitched)   return -1;

    else {

        ySwitched = false;

        return getAnalogValue();

    }

}