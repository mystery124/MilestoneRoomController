#include "ConfigFileService.h"

using namespace WiFiManagment;

ConfigFileService::ConfigFileService(){
        SPIFFS.begin();
}

ConfigFile& ConfigFileService::readConfigFile(){
    if(SPIFFS.exists(cofingFilePath)){
        File file = SPIFFS.open(cofingFilePath, "r");
        StaticJsonDocument<512> doc;
        DeserializationError error = deserializeJson(doc, file);
        cFile = {
            doc["SSID"],
            doc["password"],
            doc["nodeName"],
            doc["startAPMode"]
        };
        return cFile;
    }
    return cFile;
}

bool ConfigFileService::deleteConfigFile(){
    return SPIFFS.remove(cofingFilePath);;
}

bool ConfigFileService::saveConfigFile(String& configJSON){
    File file = SPIFFS.open(cofingFilePath, "w");
    file.println(configJSON);
    file.close();
    return true;
}

bool ConfigFileService::hasConfigFile(){
    return SPIFFS.exists(cofingFilePath);
}