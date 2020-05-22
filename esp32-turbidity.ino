#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include "mainpage.h"
#include "jscript.h"

const char* ssid = "<your WiFI SSID>";
const char* password = "<your WiFi Password>";

WebServer server(80);

String text= "";
double turbidity;

void setup(void) {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/data", [](){
    delay(100);
    int val = analogRead(36);
    turbidity = map(val, 0, 2800, 5, 1); 
    text = (String)turbidity;
    Serial.println(val);
    server.send(200, "text/plain", text);
  });

  server.on("/", []() {
   server.send(200, "text/html", page);
  });

  server.on("/jscript.js", []() {
   server.send(200, "text/javascript", javascript);
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
