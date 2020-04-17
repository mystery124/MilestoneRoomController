#ifndef STMODESERVICE_h
#define STMODESERVICE_h

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include <FS.h>
#define LOGGING_SERIAL
#include "Logger.h"
#include "WiFiService.h"
#include "ConfigFile.h"
#include "ConfigFileService.h"

namespace WiFiManagment {

    class StationModeService : public WiFiService {
    public:
        StationModeService(std::function<void(void)> onDisconected, std::function<void(void)> onConnected);
        ~StationModeService();
        virtual void startWifi();
    private:
        virtual void startServer();
        virtual bool startMDNS();
        virtual String getLocation();

        void startStationMode(ConfigFile&);
        void resetDevice();
        std::function<void(void)> onStationModeDisconnected;
        std::function<void(void)> onStationModeConnected;
        ConfigFileService cFileService;
        WiFiEventHandler stationModeDisconnectedEvt;
        WiFiEventHandler onStationModeGotIP;
    };

};

#endif