/*
ESP32 Turbidity Sensor
by Roland Pelayo
for TeachMeMicro

Rev 1.0 - May 22, 2020

Full tutorial on https://www.teachmemicro.com/esp32-turbidity-Sensor
*/
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include "mainpage.h"
#include "jscript.h"

//provide your own WiFi SSID and password
const char* ssid = "<your WiFI SSID>";
const char* password = "<your WiFi Password>";

WebServer server(80);

//For storing data as string
String text= "";
//Sensor data
double turbidity;

void setup(void) {
  //For debugging
  Serial.begin(9600);
  //Use ESP32 as WiFi Station
  WiFi.mode(WIFI_STA);
  //Initiate WiFi Connection
  WiFi.begin(ssid, password);
  Serial.println("");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  //Print your WiFi's SSID (might be insecure)
  Serial.println(ssid);
  Serial.print("IP address: ");
  //Print your local IP address (needed for browsing the app)
  Serial.println(WiFi.localIP());
  //Page for reading data. Sensor is read in this part
  server.on("/data", [](){
    delay(100);
    int val = analogRead(36);
    turbidity = map(val, 0, 2800, 5, 1); 
    text = (String)turbidity;
    Serial.println(val);
    server.send(200, "text/plain", text);
  });
  //Home page. Contents of 'page' is in mainpage.h
  server.on("/", []() {
   server.send(200, "text/html", page);
  });
  //JavaScript! Contents of 'javascript' is in jscript.h
  server.on("/jscript.js", []() {
   server.send(200, "text/javascript", javascript);
  });
  //start web server
  server.begin();
  //Just stating things
  Serial.println("HTTP server started");
}

void loop(void) {
  //Make the ESP32 always handle web clients
  server.handleClient();
}
