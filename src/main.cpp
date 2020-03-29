#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <Wire.h>

#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

//local
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

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
    <head>
        <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">
        <link rel=\"icon\" href=\"data:,\">
        <style>
            body {
                font-family: "Trebuchet MS", Arial, Helvetica, sans-serif;
            }
            div {
                border-radius: 5px;
                background-color: #f2f2f2;
                padding: 20px;
            }
            .wifilisttable{
                border-collapse: collapse;
                width: 100%;
            }
            .wifilisttable .col {
                border: 1px solid #ddd;
                padding: 8px;
            }
            .wifilisttable tr:nth-child(odd){
                background-color: white;
            }
            .wifilisttable tr:hover {
                background-color: #ddd;
            }
            .wifilisttable th {
                padding-top: 12px;
                padding-bottom: 12px;
                text-align: left;
                background-color: #4CAF50;
                color: white;
              }

              .submitForm input[type=text], .submitForm input[type=password] {
                width: 100%;
                padding: 12px 20px;
                margin: 8px 0;
                display: inline-block;
                border: 1px solid #ccc;
                border-radius: 4px;
                box-sizing: border-box;
              }
              
              input[type=button]{
                width: 100%;
                background-color: #4CAF50;
                color: white;
                padding: 14px 20px;
                margin: 8px 0;
                border: none;
                border-radius: 4px;
                cursor: pointer;
              }
              
              input[type=button]:hover{
                background-color: #45a049;
              }
        </style>
    </head>
    <body>
        <div class"networkList">
            <h2>WiFi Networks:</h2>
            <input type="button" onclick="loadSSIDs()" value="Scan" >
            <table class="wifilisttable">
                <thead>
                    <tr>
                      <th class="col">SSID</th>
                      <th class="col">Public</th>
                      <th class="col">Signal</th>
                    </tr>
                </thead>
                <tbody id="wifilist">
                    <tr>
                        <td  class="col" colspan="3">No items</td>
                    </tr>
                </tbody>
            </table>
        </div>
        <div class="submitForm">
            <label for="ssid">SSID:</label><br>
            <input class="ssid" type="text" id="ssid" placeholder="SSID">
            <label for="pass">Password:</label><br>
            <input class="pass" type="password" id="pass" placeholder="Password">
            <input type="button" value="Submit">
        </div>
    </body>
    <script>
        function loadSSIDs() {
            document.getElementById("wifilist").innerHTML = "<tr><td  class=\"col\" colspan=\"3\">Searching...</td></tr>";
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                    updateSSIDlist(this.responseText);
                }
            };
            xhttp.open("GET", "/getSSIDs", true);
            xhttp.send();
        }

        function updateSSIDlist(response){
            let tdVal = "<td class=\"col\">"
            let result = JSON.parse(response);
            let tableSSIDhtml = "";
            if(result.length == 0 ) {
                tableSSIDhtml = "<tr><td colspan=\"3\">No items</td></tr>";
            } else {
                for(ssid of result.ssids){
                    tableSSIDhtml += "<tr onclick=\"selectNetwork(\""+ssid.name+"\")\">"+tdVal+ssid.name+"</td>"+tdVal+ssid.isPublic+"</td>"+tdVal+ssid.signalStrength+"</td></tr>"
                }
            }
            document.getElementById("wifilist").innerHTML = tableSSIDhtml;
        }
        function selectNetwork(ssid){
            document.getElementById("ssid").value = ssid;
        }
        </script>
</html>
)rawliteral";

LCDEasy easyLCD;
OTAHandler otaHandler;

ESP8266WebServer server(80);

void handleRoot() {
    server.send(200, "text/html", index_html); //Send web page
}

String calculateWiFiSignalStrength(int32_t strength ){
    String value = "";
    if(strength <= -90 ){
        value = "Useless";
    } else if(strength <= -80 ){
        value = "Bad";
    } else if(strength <= -70 ){
        value = "Good";
    } else if(strength <= -67 ){
        value = "Very Good";
    } else if(strength <= -30 ){
        value = "Perfect";
    }
    return value;
}

void handleSSIDscan(){
    byte countSSIDs = WiFi.scanNetworks();
    StaticJsonDocument<600> doc;
    doc.createNestedArray("ssids");
    for(int i = 0; i < countSSIDs; i++){
        doc["ssids"][i]["name"] =  WiFi.SSID(i);
        doc["ssids"][i]["isPublic"]= WiFi.encryptionType(i) == ENC_TYPE_NONE ? "Public" : "Private";
        doc["ssids"][i]["signalStrength"] =  calculateWiFiSignalStrength(WiFi.RSSI(i));
    }
    String response = doc.as<String>();
    server.send(200, "application/json", response); 
}

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

    server.on("/", handleRoot);
    server.on("/getSSIDs", handleSSIDscan);
    server.begin();
});

void startLCD(){
    easyLCD.init();
    easyLCD.getLCD()->begin(16, 2);
    easyLCD.getLCD()->setBacklightPin(BACKLIGHT_PIN, POSITIVE);
    easyLCD.getLCD()->setBacklight(LED_ON);
    easyLCD.getLCD()->backlight();
    
    easyLCD.getLCD()->clear();
    easyLCD.getLCD()->home();
    easyLCD.getLCD()->print("Booting");
}

void setup()
{
    unsigned long startTime = millis();
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.begin(115200);
    Serial.println("Booting");

    startLCD();

    WiFi.hostname("ESP Server123");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    otaHandler.otaInit();
    easyLCD.getLCD()->clear();
    easyLCD.getLCD()->home();
    unsigned long currentTime = millis();
    unsigned long clapsedTime = currentTime - startTime;
    easyLCD.getLCD()->print("Ready: ");
    easyLCD.getLCD()->print(clapsedTime);
    easyLCD.getLCD()->print("ms");
}

void loop()
{
    //otaHandler.handleOTA();
    server.handleClient();
}