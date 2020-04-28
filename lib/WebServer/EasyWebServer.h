#ifndef EasyWebServer_h
#define EasyWebServer_h

#include <ESP8266WebServer.h>

namespace WebServer{

    class EasyWebServer{
    public:
        EasyWebServer();
        EasyWebServer(int port);
        virtual ~EasyWebServer();
        void startServer();
        void stopServer();
        void handleClient();
        void attachCallHandler(const String &uri, HTTPMethod method, std::function<void(void)> fn);
        void attachCallStaticHandler(const char *uri, fs::FS &fs, const char *path);
        void attachNotFoundHandler(std::function<void(void)> fn);
        void sendServerResponse(int code, const String& content_type, const String& content);
        void streamFile(File &file, const String& contentType);
        const String& uri();
        int args();
        const String& arg(int i);
        const String& arg(const String& name);
    private:
        ESP8266WebServer* webServer;
        int webServerDefaultPort = 80;
    };

};

#endif