#include "BME280Sensor.h"

using namespace Sensors;

bool BME280Sensor::init(void){
    LOGGER
    LOG("BME280Sensor::init");
    isInitialized = bme.begin(0x76);
    return isInitialized;
};

BME20SensorData& BME280Sensor::read(void){
    LOGGER
    LOG("BME280Sensor::read");
    if(isInitialized){
        sensorData.humidity = bme.readHumidity();
        sensorData.temperature = bme.readTemperature();
        sensorData.pressure = bme.readPressure() / 100.0F;
    }
    return sensorData;
};

void BME280Sensor::setMode(Adafruit_BME280::sensor_mode mode){
    bme.setMode(mode);
};

/*
void startBME280(){
    unsigned status;
    // default settings
    status = bme.begin(0x76);
    // You can also pass in a Wire library object like &Wire2
    // status = bme.begin(0x76, &Wire2)
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1) delay(10);
    }
}
*/