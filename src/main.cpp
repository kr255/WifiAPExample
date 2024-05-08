#include <Arduino.h>
#include <WiFi.h>
#include "WiFiClient.h"
#include "WiFiAP.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

const char *SSID = "testAP";
const char *PASSWORD = "kamranraza";


WiFiServer server(80);



void setup() {
  
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");
  
  if (!WiFi.softAP(SSID, PASSWORD)){
    log_e("FAILED");
    while(1);
  }
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
}

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available();

  if(client){
    Serial.println("New Client");
    String currentLine = "";
    log_i("CLIENT CONNECTED", client.connected());
    while(client.connected()){
      if(client.available()){
          char c = client.read();
          Serial.write(c);

          if(c == '\n'){

            if (currentLine.length() == 0 ){
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println();
              client.print("Click <a href=\"/H\">here</a> to turn ON the LED.<br>");
              client.print("Click <a href=\"/L\">here</a> to turn OFF the LED.<br>");

              // The HTTP response ends with another blank line:
              client.println();
              // break out of the while loop:
              break;
            } 
            else {
              currentLine = "";
            }
          }
          else if(c != '\r'){
            currentLine += c;
          }

          if (currentLine.endsWith("GET /H")) {
          digitalWrite(LED_BUILTIN, HIGH);  // GET /H turns the LED on
          }
          
          if (currentLine.endsWith("GET /L")) {
            digitalWrite(LED_BUILTIN, LOW);  // GET /L turns the LED off
          }
      }
    }
    client.stop();
      Serial.println("Client Disconnected");
  }
}

// put function definitions here:
