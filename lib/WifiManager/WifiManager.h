#ifndef WifiManager_h
#define WifiManager_h

#include <list>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <FS.h>

#include "APModeService.h"
#include "StationModeService.h"

namespace WiFiManagment {

    class WifiManager {
    public:

        void startWifi();
        void handleClient();
        void attachConnectionHandler(std::function<void(void)>);
    private:
        void handleNoConnection();
        void handleConnection();
        int connectionAttempts = 0;
        int connectionAttemptsAllowed = 10;
        WiFiService *wifiService;
        ConfigFileService cFileService;
        WifiMode mode;
        std::list<std::function<void(void)>> onConnectionHandlers;
    };

};
#endif