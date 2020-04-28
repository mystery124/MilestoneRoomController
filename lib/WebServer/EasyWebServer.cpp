#include "EasyWebServer.h"

using namespace WebServer;

EasyWebServer::EasyWebServer():EasyWebServer(webServerDefaultPort){}

EasyWebServer::EasyWebServer(int port){
    webServer = new ESP8266WebServer(80);
}

EasyWebServer::~EasyWebServer(){
    delete webServer;
}

void EasyWebServer::startServer(){
    webServer->begin();
}

void EasyWebServer::stopServer(){
    webServer->close();
}

void EasyWebServer::handleClient(){
    webServer->handleClient();
}

void EasyWebServer::sendServerResponse(int code, const String& contentType, const String& content){
    webServer->send(code, contentType, content);
}

void EasyWebServer::streamFile(File &file, const String& contentType){
    webServer->streamFile(file, contentType);
}

void EasyWebServer::attachCallHandler(const String &uri, HTTPMethod method, std::function<void(void)> fn){
    webServer->on(uri, method, fn);
}

void EasyWebServer::attachCallStaticHandler(const char *uri, fs::FS &fs, const char *path){
    webServer->serveStatic(uri, fs, path);
}

void EasyWebServer::attachNotFoundHandler(std::function<void(void)> fn){
    webServer->onNotFound(fn);
}

int EasyWebServer::args(){
    return webServer->args();
}

const String& EasyWebServer::arg(int i){
    return webServer->arg(i);
}

const String& EasyWebServer::arg(const String& name){
    return webServer->arg(name);
}

const String& EasyWebServer::uri(){
    return webServer->uri();
}