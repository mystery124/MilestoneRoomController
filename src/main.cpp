#include <Arduino.h>

#define LOGGING_SERIAL
#include "Logger.h"

#include "WifiManager.h"





/*

#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

//local
#include <OTAHandler.h>

#ifndef STASSID
#define STASSID "wariatkowo"
#define STAPSK "istnydomwariatow!@"
#endif

#define LED_OFF 1
#define LED_ON 0

#define SEALEVELPRESSURE_HPA (1013.25)

const char *ssid = STASSID;
const char *password = STAPSK;

OTAHandler otaHandler;

ESP8266WebServer server(80);

Adafruit_BME280 bme;

void handleRoot() {
    File file = SPIFFS.open("/d/index.html","r");
    server.streamFile(file, "text/html");
}

int calculateWiFiSignalStrength(int32_t strength ){
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

void handleSSIDscan(){
    byte countSSIDs = WiFi.scanNetworks();
    StaticJsonDocument<600> doc;
    doc.createNestedArray("ssids");
    for(int i = 0; i < countSSIDs; i++){
        doc["ssids"][i]["name"] =  WiFi.SSID(i);
        doc["ssids"][i]["isPublic"]= WiFi.encryptionType(i) == ENC_TYPE_NONE ? 0 : 1;
        doc["ssids"][i]["signalStrength"] =  calculateWiFiSignalStrength(WiFi.RSSI(i));
        doc["ssids"][i]["channel"] = WiFi.channel(i);
    }
    String response = doc.as<String>();
    server.send(200, "application/json", response); 
}

// hold uploaded file
File fsUploadFile;

void handleFileUpload(){
    HTTPUpload& upload = server.upload();
    if(upload.status == UPLOAD_FILE_START)
    {
        String filename = upload.filename;
        if(!filename.startsWith("/"))
        filename = "/d/"+filename;
        Serial.print("handleFileUpload Name: "); Serial.println(filename);
        fsUploadFile = SPIFFS.open(filename, "w+");

    } else if(upload.status == UPLOAD_FILE_WRITE)
    {
        if(fsUploadFile)
        fsUploadFile.write(upload.buf, upload.currentSize);
    } else if(upload.status == UPLOAD_FILE_END)
    {
        if(fsUploadFile)
        fsUploadFile.close();
        Serial.print("handleFileUpload Size: "); Serial.println(upload.totalSize);
    }
}

String getContentType(String filename) { // convert the file extension to the MIME type
    if (filename.endsWith(".html")) return "text/html";
    else if (filename.endsWith(".css")) return "text/css";
    else if (filename.endsWith(".js")) return "application/javascript";
    else if (filename.endsWith(".ico")) return "image/x-icon";
    else if (filename.endsWith(".gz")) return "application/x-gzip";
    else if (filename.endsWith(".png")) return "image/png";
    else if (filename.endsWith(".jpg") || filename.endsWith(".jpeg")) return "image/jpeg";
    return "text/plain";
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  path = "/d"+path;
  if (path.endsWith("/")){
      path += "d/index.html";          // If a folder is requested, send the index file
  }
  String contentType = getContentType(path);             // Get the MIME type
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) { // If the file exists, either as a compressed archive, or normal
    if (SPIFFS.exists(pathWithGz)){                         // If there's a compressed version available
      path += ".gz";                                         // Use the compressed verion
    }
    File file = SPIFFS.open(path, "r");                    // Open the file
    size_t sent = server.streamFile(file, contentType);    // Send it to the client
    file.close();                                          // Close the file again
    Serial.println(String("\tSent file: ") + path);
    return true;
  }
  Serial.println(String("\tFile Not Found: ") + path);   // If the file doesn't exist, return false
  return false;
}

WiFiEventHandler disconnectedEventHandler;

WiFiEventHandler gotIpEventHandler = WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP& event)
{
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Station connected, IP: ");
    Serial.println(WiFi.localIP());

    Serial.println("Ready");
    server.on("/", handleRoot);
    server.onNotFound([]() {
        if(!handleFileRead(server.uri())){
            server.send(404, "text/plain", "404: Not Found");
        }
    });
    server.on("/getSSIDs", handleSSIDscan);
    server.on("/upload", HTTP_POST, [](){ server.send(200); }, handleFileUpload);
    server.begin();
});

void startBME280(){
    unsigned status;
    // default settings
    status = bme.begin(0x76);
    // You can also pass in a Wire library object like &Wire2
    // status = bme.begin(0x76, &Wire2)
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1) delay(10);
    }
}

void printBME280Values() {
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");

    Serial.print("Pressure = ");

    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.println();
    
}
*/

void pinOn(){
    digitalWrite(LED_BUILTIN, LOW);
}

WiFiManagment::WifiManager wifiService;

void setup(){
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    LOGGER
    LOG("Start WIFI");
    wifiService.attachConnectionHandler(pinOn);
    wifiService.startWifi();
    LOG("config WIFI done");
}

void loop(){

    wifiService.handleClient();
    //otaHandler.handleOTA();
    //server.handleClient();
    //ESP.deepSleep(10*1000*1000);
}