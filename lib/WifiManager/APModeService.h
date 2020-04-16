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

namespace WiFiManagment {

    class APModeService : public WiFiService {
    public:
        virtual void startWifi();
    private:
        virtual String getLocation();
        virtual void startServer();
        void startAPwifi();
        void scanSSIDs();
        void saveConfig();
        int calculateWiFiSignalStrength(int32_t);
        ConfigFileService cFileService;
    };

};

#endif