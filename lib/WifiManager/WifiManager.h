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
        WifiManager(WebServer::EasyWebServer&);
        void startWifi();
        void handleClient();
        void attachConnectionHandler(std::function<void(void)>);
        WifiMode getMode();
    private:
        void handleNoConnection();
        void handleConnection();
        void resetConfigAndResartWifi();
        int connectionAttempts = 0;
        int connectionAttemptsAllowed = 10;
        WiFiService *wifiService;
        ConfigFileService cFileService;
        WifiMode mode;
        std::list<std::function<void(void)>> onConnectionHandlers;
        WebServer::EasyWebServer webServer;
    };

};
#endif