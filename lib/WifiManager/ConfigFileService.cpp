#include "ConfigFileService.h"

using namespace WiFiManagment;

ConfigFileService::ConfigFileService(){
        SPIFFS.begin();
}

ConfigFile& ConfigFileService::readConfigFile(){
    if(SPIFFS.exists(cofingFilePath) && cFilieInitialized == false){
        File file = SPIFFS.open(cofingFilePath, "r");
        StaticJsonDocument<512> doc;
        deserializeJson(doc, file);
        cFile = {
            doc["SSID"],
            doc["password"],
            doc["nodeName"],
            doc["startMode"]
        };
        cFilieInitialized = true;
        return cFile;
    }
    return cFile;
}

bool ConfigFileService::deleteConfigFile(){
    return SPIFFS.remove(cofingFilePath);;
}

bool ConfigFileService::saveConfigFile(String& configJSON){
    const int capacity = 512;
    DynamicJsonDocument doc(capacity);
    DeserializationError err = deserializeJson(doc, configJSON);
    if(err){
        LOG("Config Deserialization error");
        return false;
    } else {
        cFile = {
            doc["SSID"],
            doc["password"],
            doc["nodeName"],
            doc["startMode"]
        };
    }

    saveConfigFile(cFile);
    return true;
}

bool ConfigFileService::saveConfigFile(ConfigFile &configF){
    StaticJsonDocument<512> doc;
    doc["SSID"] = configF.SSID;
    doc["password"] = configF.password;
    doc["nodeName"] = configF.nodeName;
    doc["startMode"] = configF.startMode;
    String configJSON;
    serializeJson(doc, configJSON);
    File file = SPIFFS.open(cofingFilePath, "w");
    file.println(configJSON);
    file.close();
    return true;
}

bool ConfigFileService::hasConfigFile(){
    return SPIFFS.exists(cofingFilePath);
}