#include "StationModeService.h"

using namespace WiFiManagment;

StationModeService::StationModeService(std::function<void(void)> onDisconected, std::function<void(void)> onConnected){
    onStationModeDisconnected = onDisconected;
    onStationModeConnected = onConnected;
};

StationModeService::~StationModeService(){};

void StationModeService::startWifi(){
    ConfigFile cFile = cFileService.readConfigFile();
    startMDNS();
    startStationMode(cFile);
    startServer();
};

void StationModeService::startStationMode(ConfigFile& cFile){
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
        onStationModeConnected();
    });
    WiFi.begin(cFile.SSID, cFile.password);
};

bool StationModeService::startMDNS(){
    // TODO
    bool result = MDNS.begin("controller");
    MDNS.addService("http", "tcp", 80);
    return result;
};

void StationModeService::startServer(){
    webServer.on("/restart", HTTP_POST, [this](){restartDevice();});
    webServer.on("/reset", HTTP_POST, [this](){resetDevice();});

    WiFiService::startServer();
};

String StationModeService::getLocation(){
    return "/s";
};

void StationModeService::resetDevice(){
    cFileService.deleteConfigFile();
    webServer.send(200, "text/plain", "OK");
}