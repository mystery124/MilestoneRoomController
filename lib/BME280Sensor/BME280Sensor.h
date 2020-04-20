#ifndef MBE280SENSOR_h
#define MBE280SENSOR_h

#include <Adafruit_BME280.h>

#define LOGGING_SERIAL
#include "Logger.h"
#include "BME20SensorData.h"

namespace Sensors{

    class BME280Sensor{
    public:
        bool init(void);
        BME20SensorData& read(void);
        void setMode(Adafruit_BME280::sensor_mode mode);
    private:
        bool isInitialized;
        Adafruit_BME280 bme;
        BME20SensorData sensorData = {};
    };
};

#endif