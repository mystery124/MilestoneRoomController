#include <Arduino.h>
#include <FS.h>
#include <Ticker.h>

#define LOGGING_SERIAL
#include "Logger.h"

#include "WifiManager.h"
#include "BME280Sensor.h"
#include "EasyWebServer.h"



/*

//local
#include <OTAHandler.h>

OTAHandler otaHandler;


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
*/

void pinOn();
void readSensorAndPrint();
void serverAttatch();
void goToSleep();
void readSensor();

WebServer::EasyWebServer webServer = WebServer::EasyWebServer(80);
WiFiManagment::WifiManager wifiService = WiFiManagment::WifiManager(webServer);
Sensors::BME280Sensor bmeSensor;
bool sendFlag = false;
Ticker tick;
Sensors::BME20SensorData sensorData;

void setup(){
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    SPIFFS.begin();
    LOGGER

    LOG("BME280 sensor read");
    bmeSensor.init();
    //bmeSensor.setMode(Adafruit_BME280::MODE_NORMAL);
    sensorData = bmeSensor.read();
    //bmeSensor.setMode(Adafruit_BME280::MODE_SLEEP);
    LOG("BME280 sensor read - done");

    LOG("Start WIFI");
    wifiService.attachConnectionHandler(pinOn);
    wifiService.attachConnectionHandler(readSensorAndPrint);
    wifiService.attachConnectionHandler(serverAttatch);
    wifiService.startWifi();
    LOG("config WIFI done");

    LOG("Web Server start");
    webServer.startServer();
    LOG("Web Server - done");

    //tick.attach(30, goToSleep);
}

void loop(){
    wifiService.handleClient();
    webServer.handleClient();
    //otaHandler.handleOTA();
    LOGGER;
    if(sendFlag){
        //goToSleep();
    }
}

void goToSleep(){
    int val = 60;
    LOG("goToSleep");
    WiFi.disconnect(true);
    delay( 1 );
    ESP.deepSleep(val*1000*1000, WAKE_RF_DISABLED);
}

void readSensorAndPrint(){
    LOGGER;
    LOG("Temp: ");
    LOG(sensorData.temperature);
    LOG("Humidity: ");
    LOG(sensorData.humidity);
    LOG("Pressure: ");
    LOG(sensorData.pressure);
    sendFlag = true;
};

void pinOn(){
    digitalWrite(LED_BUILTIN, LOW);
};

void serverAttatch(){
    webServer.attachCallStaticHandler("/", SPIFFS, "/sensor/index.html");
    webServer.attachCallStaticHandler("/sensor/main.js", SPIFFS, "/sensor/main.js");
    webServer.attachCallStaticHandler("/sensor/bootstrap.min.css", SPIFFS, "/sensor/bootstrap.min.css");
    webServer.attachCallHandler("/sensor/readSensor", HTTP_GET, readSensor);
}

void readSensor(){
   // bmeSensor.setMode(Adafruit_BME280::MODE_NORMAL);
   // delay(100);
    Sensors::BME20SensorData sData = bmeSensor.read();
   // bmeSensor.setMode(Adafruit_BME280::MODE_SLEEP);
    StaticJsonDocument<400> doc;
    doc["temp"] =  sData.temperature;
    doc["humid"] = sData.humidity;
    doc["press"] =  sData.pressure;
    String response = doc.as<String>();
    webServer.sendServerResponse(200, "application/json", response); 
}