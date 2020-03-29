#ifndef LCD_I2CEasy_h
#define LCD_I2CEasy_h

#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define BACKLIGHT_PIN 3

class LCDEasy {
public:

    void init();
    LiquidCrystal_I2C* getLCD();

private:
    LiquidCrystal_I2C *lcd;

    uint8_t i2Caddr = 0x27;
    uint8_t enPin = 2;
    uint8_t rwPin = 1;
    uint8_t rsPin = 0;
    uint8_t d4Pin = 4;
    uint8_t d5Pin = 5;
    uint8_t d6Pin = 6;
    uint8_t d7Pin = 7;
};

#endif