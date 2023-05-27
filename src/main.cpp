#include <Arduino.h>

#include "Globals.h"
#include "LCD.h"
#include "LED.h"
#include "Sensor_DHT22.h"
#include "Sensor_MHZ19.h"

#define HEARTBEAT 1000
#define BAUD_RATE 9600
#define DHT22_UPDATE_INTERVAL_MIN 3000
#define MHZ19_UPDATE_INTERVAL_MIN 5000

unsigned long SensorUpdateTime_DHT22 = 0;
unsigned long SensorUpdateTime_MHZ19 = 0;
unsigned long PulseBeatTime = 0;

bool TickTack;

// Because GPIO "D3" uses Pull-Up mode, default value = 1 which means button is unpressed
int CO2Button = 1; 
int CO2Button_PREV = 1;
// By default, show only AM2302 readings
bool HideCO2 = true;

// Use LED by defaulr
int LedButton = 1;
int LedButtonPrev = 1;
bool ShowLed = true;

void setup()
{
    debugStart;
    pinMode(Pins::AM2302, INPUT);
    pinMode(Pins::LED, OUTPUT);
    pinMode(Pins::CO2TouchButton, INPUT_PULLUP);
    pinMode(Pins::EnableMHZ19, OUTPUT);
    pinMode(Pins::LedSwitch, INPUT_PULLUP);

    LCD::Initialize();
    LED::Initialize();
    LCD::ShowInitMessage("Starting DHT22");
    DHTSensor::Setup();
    LCD::SetCaptions();

    debugln("Setup complete.");
}

void SerialPrint_DHT22(int8_t temp, uint8_t hum)
{
    debug("[DHT22] Temperature: ");
    debug(temp);
    debug(" *C | Humidity: ");
    debug(hum);
    debugln(" %RH");
}

void SerialPrint_MHZ19(int &co2value)
{
    debug("[MHZ19] CO2: ");
    debug(co2value);
    debugln(" ppm");
}

void HeartBeat(unsigned long Uptime, bool &TickTack, unsigned long &PulseBeatTime)
{
    if (Uptime - PulseBeatTime >= HEARTBEAT) {
        LCD::ShowPulse(TickTack);
        TickTack = !TickTack;
        PulseBeatTime = Uptime;
    }
}

void StartSensor_MHZ19()
{
    // Sensor startup can take up to 90 seconds
    LCD::ShowInitMessage("Starting Sensor");
    MHZ19Sensor::Initialize();
    LCD::SetCaptions();
    LCD::SetCaptionCO2(8, 0, true);
}

void loop()
{
    unsigned long UpTime = millis();

    // Check CO2 button state
    CO2Button = digitalRead(Pins::CO2TouchButton);
    if (CO2Button != CO2Button_PREV) {
        if (CO2Button == LOW) {
            HideCO2 = !HideCO2;
            debug("HideCO2 ");
            debugln(HideCO2);
            digitalWrite(Pins::EnableMHZ19, !HideCO2);
        }
        if (HideCO2) {
            LCD::SetCaptionCO2(8, 0, false);
        }
    }

    // Check Led button state
    LedButton = digitalRead(Pins::LedSwitch);
    if (LedButton != LedButtonPrev) {
        if (LedButton == LOW) {
            ShowLed = !ShowLed;
            if (!ShowLed) {
                LED::Clear();
            }
        }
    }

    // Read and show AM2302 (DHT22) sensor data
    if (UpTime - SensorUpdateTime_DHT22 >= DHT22_UPDATE_INTERVAL_MIN) {
        int8_t temp;
        uint8_t hum;
        LCD::ShowSensorRead();
        hum = DHTSensor::GetHumidity();
        temp = DHTSensor::GetTemperature();
        LCD::ClearSensorRead();
        SerialPrint_DHT22(temp, hum);
        LCD::PrintDHTSensorValues(temp, hum);
        SensorUpdateTime_DHT22 = UpTime;
        if (HideCO2 && hum > 0 && hum < 101 && ShowLed) {
            LED::SetColorDHT(hum);
        }
    }

    // Read and show MHZ19 sensor data
    if (!HideCO2 && UpTime - SensorUpdateTime_MHZ19 >= MHZ19_UPDATE_INTERVAL_MIN) {
        int co2value = 0;
        if (CO2Button == LOW) {
            debugln("Starting MHZ19");
            StartSensor_MHZ19();
        }
        LCD::ShowSensorRead();
        co2value = MHZ19Sensor::GetCO2Value();
        LCD::ClearSensorRead();
        SerialPrint_MHZ19(co2value);
        if (co2value > 0) {
            LCD::PrintCO2(co2value);
            if (ShowLed) {
                LED::SetColorMHZ19(co2value);
            }
        }
        SensorUpdateTime_MHZ19 = UpTime;
    }

    HeartBeat(UpTime, TickTack, PulseBeatTime);
    CO2Button_PREV = CO2Button;
    LedButtonPrev =  LedButton;
    delay(3);
}
