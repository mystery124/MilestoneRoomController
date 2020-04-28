#include "WifiManager.h"

using namespace WiFiManagment;

WifiManager::WifiManager(WebServer::EasyWebServer &wServer):webServer(wServer){};

void WifiManager::startWifi(){
    if(cFileService.hasConfigFile() && !cFileService.readConfigFile().startMode == AP_MODE){
        wifiService = new StationModeService(
            webServer,
            [this](){handleNoConnection();},
            [this](){handleConnection();}
        );
    } else {
        wifiService = new APModeService(webServer);
    }
    wifiService->startWifi();
};

void WifiManager::handleClient(){
    wifiService->handleClient();
};

WifiMode WifiManager::getMode(){
    return wifiService->getMode();
}

void WifiManager::handleNoConnection(){
    connectionAttempts++;
    if(connectionAttempts > connectionAttemptsAllowed ){
        resetConfigAndResartWifi();
    }
}

void WifiManager::resetConfigAndResartWifi(){
    cFileService.deleteConfigFile();
    delete wifiService;
    wifiService = nullptr;
    startWifi();
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