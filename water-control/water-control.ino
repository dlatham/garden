// Add a voltage divider to bring the A0 voltage down to less than 1.0V https://learn.sparkfun.com/tutorials/voltage-dividers
// Build a transistor switch to turn the sensor on and off https://roboindia.com/tutorials/switching-transistor-nodemcu
// The board should connect to Wifi, send the moisture sensor readings to the application and wait for a response as either 'ok' or 'water' with a time interval and then go to deep sleep


#include <featherOLED.h>
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>

#define Moisture A0 //The definition of AO pin IO-A0 
#define DO 14        //The definition of DO pin IO-7  
#define valve1 3
#define valve2 4
WiFiManager wifiManager;
ESP8266WebServer server(80);

void sendMoisture() {
  // Function to get moisture readings from sensors and return as JSON using ArduinoJSON
  StaticJsonBuffer<126> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["name"] = "garden1";
  root["sensors"] = 2;
  JsonArray& data = root.createNestedArray("data");
  data.add(analogRead(Moisture));
  data.add(0);
  root.prettyPrintTo(Serial);
  String JSONresponse;
  root.prettyPrintTo(JSONresponse);
  
  //After assembling the JSON send to the client
  server.send(200, "application/json", JSONresponse);
  
}

void waterCycle() {
  //Start the water cycle based on the time provided in the call
  int duration = server.arg("t").toInt() * 1000;
  long current = millis();
  while (millis() <= (current + duration)) {
    digitalWrite(valve1, LOW);
  }
  digitalWrite(valve1, HIGH);
}

void handleRoot() {
  
}
  
void setup() {  
  pinMode(Moisture, INPUT);//Define A0 as input mode  
  pinMode(DO, INPUT);  
  Serial.begin(115200);
  wifiManager.autoConnect("833_GARDEN");
  server.on("/", handleRoot);
  server.on("/read", sendMoisture);
  server.on("/water", waterCycle);
}  
  
void loop() {  
  //Returns the serial measurement data  
  Serial.print("Moisture=");  
  Serial.print(analogRead(Moisture));//Numerical read AO
  Serial.print("|DO=");  
  Serial.println(digitalRead(DO));//Numerical read DO  
  delay(1000);  


  server.handleClient();
} 
