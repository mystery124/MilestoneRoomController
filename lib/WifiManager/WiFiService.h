#ifndef WIFI_SERVICE_h
#define WIFI_SERVICE_h

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <FS.h>

#define DEFAULT_DEVICE_NAME "Controller"
#define DEFAULT_SSID "Controller"
#define DEFALUT_PASS "controller"

namespace WiFiManagment {

    enum WifiMode { AP_MODE, NORMAL_MODE };

    class WiFiService {
        public:
        WiFiService();
        virtual void startWifi() = 0;
        virtual String getLocation() = 0;
        virtual void handleClient();
        //virtual WifiMode getMode();

        protected:
        void startSPIFSS();
        void restartDevice();
        virtual bool startMDNS();
        virtual void handleRoot();
        virtual void startServer();
        virtual String getContentType(String);
        virtual bool handleFileRead(String);

        String defaultSSID = DEFAULT_SSID;
        String defaultPassword = DEFALUT_PASS;
        String defaultDeviceName = DEFAULT_DEVICE_NAME;
        ESP8266WebServer webServer;
        DNSServer dnsServer;
        byte dnsPort = 53;
    };

};

#endif