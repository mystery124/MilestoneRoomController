#include <Arduino.h>
<<<<<<< HEAD
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
=======
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>

#ifndef STASSID
#define STASSID "your-ssid"
#define STAPSK "your-password"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

void otaSetup(){
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
    {
      type = "sketch";
    }
    else
    { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
    {
      Serial.println("Auth Failed");
    }
    else if (error == OTA_BEGIN_ERROR)
    {
      Serial.println("Begin Failed");
    }
    else if (error == OTA_CONNECT_ERROR)
    {
      Serial.println("Connect Failed");
    }
    else if (error == OTA_RECEIVE_ERROR)
    {
      Serial.println("Receive Failed");
    }
    else if (error == OTA_END_ERROR)
    {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
}

void handleOTA(){
  ArduinoOTA.handle();
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  otaSetup();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop(){
  handleOTA();
>>>>>>> master
}