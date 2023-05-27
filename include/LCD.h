#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include "Globals.h"

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C screen(0x27, 16, 2);

byte Heart[8] = {
    0b00000,
    0b01010,
    0b11111,
    0b11111,
    0b01110,
    0b00100,
    0b00000,
    0b00000
};

byte Celsius[8] = {
    0b00110,
    0b01001,
    0b01001,
    0b00110,
    0b00000,
    0b00000,
    0b00000,
    0b00000
};

byte Two[8] = {
    0b00000,
    0b00000,
    0b00000,
    0b01100,
    0b10010,
    0b00100,
    0b01000,
    0b11110
};

byte HeartRight[8] = {
    0b01100,
    0b11110,
    0b11111,
    0b11111,
    0b11110,
    0b11100,
    0b11000,
    0b10000
};

byte HeartLeft[8] = {
    0b00110,
    0b01111,
    0b11111,
    0b11111,
    0b01111,
    0b00111,
    0b00011,
    0b00001
};

namespace LCD {

    void Initialize()
    {
        // Initialize LCD
        screen.init();
        screen.backlight();
        screen.clear();

        screen.createChar(0, Heart);
        screen.createChar(1, Celsius);
        screen.createChar(2, Two);
        screen.createChar(3, HeartLeft);
        screen.createChar(4, HeartRight);
    }

    void SetCaptionCO2(int StartPos, int Line, bool ShowCO2)
    {
        // Trigger CO2 related captions
        if (ShowCO2) {
            screen.setCursor(StartPos, Line);
            screen.print("CO");
            screen.setCursor(StartPos + 2, Line);
            screen.write(byte(2));
            screen.setCursor(StartPos + 5, Line + 1);
            screen.print("ppm");
        }
        else {
            screen.setCursor(StartPos, Line);
            screen.print("   ");
            screen.setCursor(StartPos, Line + 1);
            screen.print("        ");
        }
    }

    void SetCaptions()
    {
        // Set captions that do not change over time, to reduce screen draw
        screen.clear();

        screen.home();
        screen.print("   ");
        screen.setCursor(3, 0);
        screen.write(byte(1));
        screen.setCursor(4, 0);
        screen.print("C");

        screen.setCursor(0, 1);
        screen.print("  ");
        screen.setCursor(3, 1);
        screen.print("%RH");
    }

    void PrintDHTSensorValues(int temp, int hum)
    {
        // Clear previous and show new temp and hum values
        screen.setCursor(0, 0);
        screen.print("   ");
        screen.setCursor(0, 0);
        screen.print(temp);

        screen.setCursor(0, 1);
        screen.print("  ");
        screen.setCursor(0, 1);
        screen.print(hum);
    }

    void PrintCO2(int co2value)
    {
        // Show CO2 readings
        screen.setCursor(8, 1);
        screen.print("    ");
        screen.setCursor(8, 1);
        screen.print(co2value);
    }

    void ShowPulse(bool Tick)
    {
        // Show or hide the heart
        screen.setCursor(14, 0);
        if (Tick) {
            screen.write(byte(3));
            screen.write(byte(4));
        }
        else {
            screen.print("  ");
        }
    }

    void ShowInitMessage(String Message)
    {
        screen.clear();
        screen.home();
        screen.print(Message);
    }

    void ShowSensorRead()
    {
        // Show "R" on LCD
        screen.setCursor(13, 0);
        screen.print("R");
    }

    void ClearSensorRead()
    {
        // Remove "R" from LCD
        screen.setCursor(13, 0);
        screen.print(" ");
    }
}