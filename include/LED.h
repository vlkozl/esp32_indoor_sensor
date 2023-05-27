#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#include "Globals.h"

#define NUMPIXELS 1
#define DELAYVAL 10

// Define ranges for estimating
#define PPM_VALUE_MIN 420
#define PPM_VALUE_MED 1500
#define PPM_VALUE_HIGH 2000
#define PPM_VALUE_MAX 5000

Adafruit_NeoPixel Pixels = Adafruit_NeoPixel(NUMPIXELS, Pins::LED, NEO_GRB + NEO_KHZ800);

namespace LED {
    void Initialize()
    {
        Pixels.begin();
        Pixels.setBrightness(30);
        // Pixels.setPixelColor(0, 0);
        Pixels.clear();
        Pixels.show();
    }

    void Clear()
    {
        Pixels.clear();
        Pixels.show();
    }

    void SetColorRGB(uint8_t r, uint8_t g, uint8_t b)
    {
        Pixels.setPixelColor(0, Pixels.Color(g, r, b));
        Pixels.show();
    }

    void SetColorHEX(uint32_t color)
    {
        Pixels.setPixelColor(0, color);
        Pixels.show();
    }

    void SetColorGreen()
    {
        SetColorRGB(0, 255, 0);
    }

    void SetColorYellow()
    {
        SetColorRGB(255, 150, 0);
    }

    void SetColorOrange()
    {
        SetColorRGB(255, 85, 0);
    }

    void SetColorRed()
    {
        SetColorRGB(255, 0, 0);
    }

    void SetColorWrite()
    {
        SetColorRGB(255, 255, 180);
    }

    void SetColorBlue()
    {
        SetColorRGB(0, 0, 150);
    }

    unsigned long int GetColorMHZ(int ppm)
    {
        int r, g, b;
        if (ppm <= PPM_VALUE_MIN) {
            r = 0;
            // g = map(ppm, PPM_VALUE_MIN, PPM_VALUE_MED, 255, 150);
            g = 0;
            b = 0;
        }
        else if (ppm >= PPM_VALUE_MIN && ppm < PPM_VALUE_MED) {
            r = map(ppm, PPM_VALUE_MIN, PPM_VALUE_MED, 0, 255);
            g = map(ppm, PPM_VALUE_MIN, PPM_VALUE_MED, 255, 150);
            b = 0;
        }
        else if (ppm >= PPM_VALUE_MED && ppm < PPM_VALUE_HIGH) {
            r = 255;
            g = map(ppm, PPM_VALUE_MED, PPM_VALUE_HIGH, 150, 0);
            b = 0;
        }
        else if (ppm >= PPM_VALUE_HIGH && ppm <= PPM_VALUE_MAX) {
            r = 255;
        }
        return Pixels.Color(g, r, b);
    }

    void SetColorMHZ19(int ppm)
    {
        SetColorHEX(GetColorMHZ(ppm));
    }

    void SetColorDHT(int hum)
    {
        int b = 0;
        if (hum > 50) {
            b = map(hum, 50, 100, 1, 255);
        }
        SetColorRGB(0, 0, b);
        Pixels.show();
    }
}