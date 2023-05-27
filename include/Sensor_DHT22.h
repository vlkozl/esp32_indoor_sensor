#include <Arduino.h>
#include "Globals.h"
#include "DHTesp.h"

DHTesp AM2302;

namespace DHTSensor {
    void Setup()
    {
        AM2302.setup(Pins::AM2302, DHTesp::AM2302);
    }

    int8_t GetTemperature()
    {
        delay(10);
        return MakeINT(AM2302.getTemperature());
    }

    uint8_t GetHumidity()
    {
        delay(10);
        return MakeUINT(AM2302.getHumidity());
    }

}