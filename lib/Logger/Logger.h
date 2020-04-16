#ifndef LOGGER_H
#define LOGGER_H

    #ifdef LOGGING_SERIAL
        #include <Wire.h>
        #define LOG(msg) Serial.println( (msg) );
        #define LOGGER if(!Serial) Serial.begin(115200);
    #else
        #define LOG(...)
        #define LOGGER
    #endif
#endif