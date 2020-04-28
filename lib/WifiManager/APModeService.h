#ifndef APMODESERVICE_h
#define APMODESERVICE_h

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ArduinoJson.h>
#include <FS.h>

#include "ConfigFile.h"
#include "ConfigFileService.h"
#include "WiFiService.h"
#include "EasyWebServer.h"

namespace WiFiManagment {

    class APModeService : public WiFiService {
    public:
        APModeService(WebServer::EasyWebServer &webServer);
        ~APModeService();
        virtual void startWifi();
        virtual WifiMode getMode();

    private:
        virtual String getLocation();
        virtual void initWebServer();
        void startAPwifi();
        void startDNSServer();
        void scanSSIDs();
        void saveConfig();
        int calculateWiFiSignalStrength(int32_t);
        ConfigFileService cFileService;
        IPAddress acceessPointIP;
        IPAddress mask;
    };

};

#endif