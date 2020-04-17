#include "APModeService.h"

using namespace WiFiManagment;

APModeService::~APModeService(){};

void APModeService::startWifi(){
    startAPwifi();
    startMDNS();
    startSPIFSS();
    startServer();
};

void APModeService::startAPwifi(){
    IPAddress apIP(192, 168, 1, 1);
    IPAddress mask(255, 255, 255, 0);
    WiFi.hostname(defaultDeviceName);
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, mask);
    WiFi.softAP(defaultDeviceName, defaultPassword, 1, 0, 1);
    dnsServer.start(dnsPort, "*", apIP);
};

String APModeService::getLocation(){
    return "/d";
};

void APModeService::startServer(){
    webServer.on("/getSSIDs", HTTP_GET, [this](){scanSSIDs();});
    webServer.on("/saveConfig", HTTP_POST, [this](){saveConfig();});
    webServer.on("/restart", HTTP_POST, [this](){restartDevice();});
    webServer.on("/generate_204", HTTP_POST, [this](){handleRoot();});
    webServer.on("/fwlink", HTTP_POST, [this](){handleRoot();});

    WiFiService::startServer();
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
    webServer.send(200, "application/json", response); 
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
    //TODO - parse message to verify format
    if(webServer.args() > 0 ) {
        String requestBody = webServer.arg("plain");
        cFileService.saveConfigFile(requestBody);
        webServer.send(200, "text/plain", "OK");
    } else {
        webServer.send(400, "text/plain", "400: Invalid Request");
    }
};