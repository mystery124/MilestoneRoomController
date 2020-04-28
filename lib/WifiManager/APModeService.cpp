#include "APModeService.h"

using namespace WiFiManagment;

APModeService::APModeService(WebServer::EasyWebServer &wServer):WiFiService(wServer), acceessPointIP(192,168,1,1), mask(255,255,255,0){};

APModeService::~APModeService(){};

void APModeService::startWifi(){
    startAPwifi();
    delay(10);
    startDNSServer();
    startMDNS();
    startSPIFSS();
    initWebServer();
};

void APModeService::startAPwifi(){
    WiFi.hostname(defaultDeviceName);
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(acceessPointIP, acceessPointIP, mask);
    WiFi.softAP(defaultDeviceName, defaultPassword, 1, 0, 1);
};

void APModeService::startDNSServer(){
    dnsServer.start(dnsPort, "*", acceessPointIP);
};

String APModeService::getLocation(){
    return "/d";
};

void APModeService::initWebServer(){
    webServer.attachCallHandler("/getSSIDs", HTTP_GET, [this](){scanSSIDs();});
    webServer.attachCallHandler("/saveConfig", HTTP_POST, [this](){saveConfig();});
    webServer.attachCallHandler("/restart", HTTP_POST, [this](){restartDevice();});

    WiFiService::initWebServer();
};

void APModeService::scanSSIDs(){
    byte countSSIDs = WiFi.scanNetworks();
    StaticJsonDocument<2000> doc;
    doc.createNestedArray("ssids");
    for(int i = 0; i < countSSIDs; i++){
        doc["ssids"][i]["name"] =  WiFi.SSID(i);
        doc["ssids"][i]["isPublic"]= WiFi.encryptionType(i) == ENC_TYPE_NONE ? 0 : 1;
        doc["ssids"][i]["signalStrength"] =  calculateWiFiSignalStrength(WiFi.RSSI(i));
        doc["ssids"][i]["channel"] = WiFi.channel(i);
    }
    String response = doc.as<String>();
    webServer.sendServerResponse(200, "application/json", response); 
}

int APModeService::calculateWiFiSignalStrength(int32_t strength ){
    int value = 0;
    if(strength <= -90 ){
        value = 0;
    } else if(strength <= -80 ){
        value = 1;
    } else if(strength <= -70 ){
        value = 2;
    } else if(strength <= -67 ){
        value = 3;
    } else if(strength <= -30 ){
        value = 4;
    }
    return value;
}

void APModeService::saveConfig(){
    String ssidParamName = "SSID";
    String passParamName = "password";
    String deviceNameParamName = "nodeName";
    if(webServer.args() > 0 ) {
        String requestBody = webServer.arg("plain");
        cFileService.saveConfigFile(requestBody);
        webServer.sendServerResponse(200, "text/plain", "OK");
    } else {
        webServer.sendServerResponse(400, "text/plain", "400: Invalid Request");
    }
};

WifiMode APModeService::getMode(){
    return AP_MODE;
};