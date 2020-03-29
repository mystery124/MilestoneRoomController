#include "LCD_I2CEasy.h"


void LCDEasy::init(){
    lcd = new LiquidCrystal_I2C(i2Caddr, enPin, rwPin, rsPin, d4Pin, d5Pin, d6Pin, d7Pin);
}

LiquidCrystal_I2C* LCDEasy::getLCD(){
    return lcd;
}