#include "StationModeService.h"

using namespace WiFiManagment;

StationModeService::StationModeService(
        WebServer::EasyWebServer &webServer,
        std::function<void(void)> onDisconected,
        std::function<void(void)> onConnected)
        :WiFiService(webServer), onStationModeDisconnected(onDisconected), onStationModeConnected(onConnected){
};

StationModeService::~StationModeService(){};

void StationModeService::startWifi(){
    startStationMode();
    initWebServer();
};

void StationModeService::startStationMode(){
    ConfigFile &cFile = cFileService.readConfigFile();
    WiFi.hostname(cFile.nodeName);
    WiFi.mode(WIFI_STA);
    stationModeDisconnectedEvt = WiFi.onStationModeDisconnected([this](const WiFiEventStationModeDisconnected& evt){
        LOGGER
        LOG("onStationModeDisconnected");
        onStationModeDisconnected();
    });
   onStationModeGotIP = WiFi.onStationModeGotIP([this](const WiFiEventStationModeGotIP& evt){
        LOGGER
        LOG("onStationModeGotIP");
        startMDNS();
        onStationModeConnected();
    });
    WiFi.begin(cFile.SSID, cFile.password);
};

bool StationModeService::startMDNS(){
    ConfigFile &cFile = cFileService.readConfigFile();
    LOGGER
    LOG("startMDNS");
    LOG(cFile.nodeName);
    bool result = MDNS.begin(cFile.nodeName);
    MDNS.addService("http", "tcp", 80);
    return result;
};

void StationModeService::initWebServer(){
    webServer.attachCallHandler("/config/restart", HTTP_POST, [this](){restartDevice();});
    webServer.attachCallHandler("/config/reset", HTTP_POST, [this](){resetDevice();});
    webServer.attachCallHandler("/config", HTTP_GET, [this](){handleRoot();});
    webServer.attachCallHandler("/config/main.js", HTTP_GET, [this](){handleFileRead("/main.js");});
    webServer.attachCallHandler("/config/style.css", HTTP_GET, [this](){handleFileRead("/style.css");});
};

String StationModeService::getLocation(){
    return "/s";
};

void StationModeService::resetDevice(){
    cFileService.deleteConfigFile();
    webServer.sendServerResponse(200, "text/plain", "OK");
}

WifiMode StationModeService::getMode(){
    return RUNTIME_MODE;
};