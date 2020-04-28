#ifndef ConfigFile_h
#define ConfigFile_h

#include <Arduino.h>

namespace WiFiManagment {

    struct ConfigFile {
        String SSID;
        String password;
        String nodeName;
        int startMode; // 0 - selfconfig; 1 - ap mode; 2 - station mode
    };

};

#endif