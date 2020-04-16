#ifndef ConfigFile_h
#define ConfigFile_h

#include <Arduino.h>

namespace WiFiManagment {

    struct ConfigFile {
        String SSID;
        String password;
        String nodeName;
        bool startAPMode;
    };

};

#endif