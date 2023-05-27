#include <Arduino.h>
#include <Mhz19.h>
#include <SoftwareSerial.h>

#include "Globals.h"

#define BAUD_RATE 9600
#define PREHEAT_WAIT 3000

Mhz19 sensor;
SoftwareSerial SensorSerial;

namespace MHZ19Sensor {
    void Initialize()
    {
        SensorSerial.begin(BAUD_RATE, EspSoftwareSerial::SWSERIAL_8N1, Pins::RX, Pins::TX, false);
        sensor.begin(&SensorSerial);
        sensor.setMeasuringRange(Mhz19MeasuringRange::Ppm_5000);
        sensor.enableAutoBaseCalibration();
        debug("[MHZ19] Preheating...");
        while (!sensor.isReady()) {
            debug(".");
            delay(PREHEAT_WAIT);
        }
        debugln("");
    }

    int GetCO2Value()
    {
        delay(30);
        return sensor.getCarbonDioxide();
    }
}