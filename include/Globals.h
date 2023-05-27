#pragma once
#include <Arduino.h>

#define DEBUG 0
#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#define debugStart Serial.begin(BAUD_RATE)
#else
#define debug(x)
#define debugln(x)
#define debugStart
#endif

namespace Pins {
    uint8_t RX = D1;
    uint8_t TX = D2;
    uint8_t LedSwitch = D6;
    uint8_t AM2302 = D7;
    uint8_t LED = D8;
    uint8_t CO2TouchButton = D9;
    uint8_t EnableMHZ19 = D10;
}

int MakeUINT(float inValue)
{
    return (static_cast<uint8_t>(inValue));
}

int MakeINT(float inValue)
{
    return (static_cast<int8_t>(inValue));
}
