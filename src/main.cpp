#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <Wire.h>


#include <OTAHandler.h>
#include <LCD_I2CEasy.h>



#ifndef STASSID
#define STASSID "wariatkowo"
#define STAPSK "istnydomwariatow!@"
#endif

#define LED_OFF 1
#define LED_ON 0

const char *ssid = STASSID;
const char *password = STAPSK;


LCDEasy easyLCD;
OTAHandler otaHandler;


WiFiEventHandler disconnectedEventHandler;

WiFiEventHandler gotIpEventHandler = WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP& event)
{
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Station connected, IP: ");
    Serial.println(WiFi.localIP());


    easyLCD.getLCD()->setCursor(0, 1);
    easyLCD.getLCD()->print(WiFi.localIP());

    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
});

void setup()
{
    unsigned long startTime = millis();
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    easyLCD.init();
    easyLCD.getLCD()->begin(16, 2);
    easyLCD.getLCD()->setBacklightPin(BACKLIGHT_PIN, POSITIVE);
    easyLCD.getLCD()->setBacklight(LED_ON);
    easyLCD.getLCD()->backlight();

    easyLCD.getLCD()->clear();
    delay(100);
    easyLCD.getLCD()->home();

    Serial.begin(115200);
    Serial.println("Booting");
    easyLCD.getLCD()->print("Booting");

    WiFi.hostname("ESP Server123");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    otaHandler.otaInit();
    easyLCD.getLCD()->clear();
    delay(100);
    easyLCD.getLCD()->home();
    unsigned long currentTime = millis();
    unsigned long clapsedTime = currentTime - startTime;
    easyLCD.getLCD()->print("Ready: ");
    easyLCD.getLCD()->print(clapsedTime);
    easyLCD.getLCD()->print("ms");
}

void loop()
{
    otaHandler.handleOTA();
}