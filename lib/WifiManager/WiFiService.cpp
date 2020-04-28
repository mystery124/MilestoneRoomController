#include "WifiService.h"

using namespace WiFiManagment;

WiFiService::WiFiService(WebServer::EasyWebServer &server):webServer(server), dnsServer() {}

WiFiService::~WiFiService(){};

bool WiFiService::startMDNS(){
    bool result = MDNS.begin(defaultDeviceName, WiFi.localIP());
    MDNS.addService("http", "tcp", 80);
    return result;
};

void WiFiService::startSPIFSS(){
    SPIFFS.begin();
}

void WiFiService::handleRoot() {
    handleFileRead("/index.html");
};

void WiFiService::initWebServer(){
    webServer.attachCallHandler("/", HTTP_GET, [this](){handleRoot();});
    webServer.attachNotFoundHandler( [this](){
        if(!handleFileRead(webServer.uri())){
            handleRoot();
        }
    });
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
    MDNS.update();
}

void WiFiService::restartDevice(){
    webServer.sendServerResponse(200, "text/plain", "OK");
    delay(2000);
    ESP.restart();
};