#include <Arduino.h>
#include <Ticker.h>

#define LOGGING_SERIAL
#include "Logger.h"

#include "WifiManager.h"
#include "BME280Sensor.h"




/*

#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

//local
#include <OTAHandler.h>


#define SEALEVELPRESSURE_HPA (1013.25)

OTAHandler otaHandler;

ESP8266WebServer server(80);

Adafruit_BME280 bme;


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
void goToSleep();

WiFiManagment::WifiManager wifiService;
Sensors::BME280Sensor bmeSensor;
bool sendFlag = false;
Ticker tick;

void setup(){
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    LOGGER
    bmeSensor.init();
    LOG("Start WIFI");
    wifiService.attachConnectionHandler(pinOn);
    wifiService.attachConnectionHandler(readSensorAndPrint);
    wifiService.startWifi();
    LOG("config WIFI done");
    tick.attach(30, goToSleep);
}

void loop(){
    wifiService.handleClient();
    //otaHandler.handleOTA();
    //server.handleClient();
    LOGGER;
    if(sendFlag){
        goToSleep();
    }
}

void goToSleep(){
    int val = 60;
    LOG("Going Sleep");
    ESP.deepSleep(val*1000*1000);
}

void readSensorAndPrint(){
    LOGGER;
    bmeSensor.setMode(Adafruit_BME280::MODE_NORMAL);
    Sensors::BME20SensorData data = bmeSensor.read();
    bmeSensor.setMode(Adafruit_BME280::MODE_SLEEP);
    LOG("Temp: ");
    LOG(data.temperature);
    LOG("Humidity: ");
    LOG(data.humidity);
    LOG("Pressure: ");
    LOG(data.pressure);
    sendFlag = true;
};

void pinOn(){
    digitalWrite(LED_BUILTIN, LOW);
};