#include "WifiManager.h"

using namespace WiFiManagment;

void WifiManager::startWifi(){
    if(cFileService.hasConfigFile() && !cFileService.readConfigFile().startAPMode){
        wifiService = new StationModeService(
            [this](){handleNoConnection();},
            [this](){handleConnection();}
        );
        mode = NORMAL_MODE;
    } else {
        wifiService = new APModeService();
        mode = AP_MODE;
    }
    wifiService->startWifi();
};

void WifiManager::handleClient(){
    wifiService->handleClient();
};

void WifiManager::handleNoConnection(){
    connectionAttempts++;
    if(connectionAttempts > 10 ){
        cFileService.deleteConfigFile();
        startWifi();
    }
}

void WifiManager::handleConnection(){
    std::list<std::function<void(void)>>::iterator f;
    for (f = onConnectionHandlers.begin(); f != onConnectionHandlers.end(); f++){
        (*f)();
    }
}

void WifiManager::attachConnectionHandler(std::function<void(void)> f){
    onConnectionHandlers.push_back(f);
}