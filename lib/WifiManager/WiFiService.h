#ifndef WIFI_SERVICE_h
#define WIFI_SERVICE_h

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <FS.h>

#include "EasyWebServer.h"

#define DEFAULT_DEVICE_NAME "Controller"
#define DEFAULT_SSID "Controller"
#define DEFALUT_PASS "controller"

namespace WiFiManagment {

    enum WifiMode { SEFL_CONFIG_MODE, AP_MODE, RUNTIME_MODE };

    class WiFiService {
        public:
        WiFiService(WebServer::EasyWebServer &webServer);
        virtual ~WiFiService();
        virtual void startWifi() = 0;
        virtual String getLocation() = 0;
        virtual WifiMode getMode() = 0;
        virtual void handleClient();

        protected:
        void startSPIFSS();
        void restartDevice();
        virtual bool startMDNS();
        virtual void handleRoot();
        virtual void initWebServer();
        virtual String getContentType(String);
        virtual bool handleFileRead(String);

        String defaultSSID = DEFAULT_SSID;
        String defaultPassword = DEFALUT_PASS;
        String defaultDeviceName = DEFAULT_DEVICE_NAME;
        WebServer::EasyWebServer &webServer;
        DNSServer dnsServer;
        const uint16_t dnsPort = 53;
    };

};

#endif