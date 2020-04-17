#include "WifiService.h"

using namespace WiFiManagment;

WiFiService::WiFiService():webServer(80), dnsServer() {};

WiFiService::~WiFiService(){};

bool WiFiService::startMDNS(){
    bool result = MDNS.begin(defaultDeviceName);
    MDNS.addService("http", "tcp", 80);
    return result;
};

void WiFiService::startSPIFSS(){
    SPIFFS.begin();
}

void WiFiService::handleRoot() {
    String rootPath = getLocation() + "/index.html";
    File file = SPIFFS.open(rootPath,"r");
    webServer.streamFile(file, "text/html");
    file.close();
};

void WiFiService::startServer(){
    webServer.onNotFound( [this](){
        if(!handleFileRead(webServer.uri())){
            webServer.send(404, "text/plain", "404: Not Found");
        }
    });
    webServer.on("/", HTTP_GET, [this](){handleRoot();});
    webServer.begin();
};

String WiFiService::getContentType(String filename) {
    if (filename.endsWith(".html")) return "text/html";
    else if (filename.endsWith(".css")) return "text/css";
    else if (filename.endsWith(".js")) return "application/javascript";
    else if (filename.endsWith(".ico")) return "image/x-icon";
    else if (filename.endsWith(".gz")) return "application/x-gzip";
    else if (filename.endsWith(".png")) return "image/png";
    else if (filename.endsWith(".jpg") || filename.endsWith(".jpeg")) return "image/jpeg";
    return "text/plain";
}

bool WiFiService::handleFileRead(String path) {
  path = getLocation() +path;
  if (path.endsWith("/")){
      path += getLocation() + "/index.html";
  }
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
    if (SPIFFS.exists(pathWithGz)){
      path += ".gz";
    }
    File file = SPIFFS.open(path, "r");
    webServer.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
};

void WiFiService::handleClient(){
    dnsServer.processNextRequest();
    webServer.handleClient();
}

void WiFiService::restartDevice(){
    webServer.send(200, "text/plain", "OK");
    delay(2000);
    ESP.restart();
};