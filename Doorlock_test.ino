#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid = "changyup";  //와이파이 ssid
const char* pass = "dkssud12gkgk";  //와이파이 password

int doorlock = 4;
int value;

void setup() {
  Serial.begin(115200);
  pinMode(doorlock, OUTPUT);
  WiFi.begin(ssid, pass); //와이파이 연결 시작
  Serial.print("[WIFI] ");
  while(WiFi.status() != WL_CONNECTED){ //와이파이가 연결될 때까지
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("[Wifi] Connected");
  Serial.print("IP address : \t");
  Serial.println(WiFi.localIP());
}

void loop() {
  if ((WiFi.status() != WL_CONNECTED)){
    Serial.println("NO WIFI");
    WiFi.begin(ssid, pass);
    Serial.print("[WIFI]");
    while(WiFi.status() != WL_CONNECTED){ //와이파이가 연결될 때까지
      delay(1000);
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("[WIFI] Connected IP address : \t");
    Serial.println(WiFi.localIP());
  }
  
  if ((WiFi.status() == WL_CONNECTED)) {  //와이파이가 연결되어있는 경우
    WiFiClient client;
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, "http://52.79.87.246:8080/arduino_check?id=5")) {  // HTTP
      Serial.print("[HTTP] GET...\n");
      int httpCode = http.GET();
      if (httpCode > 0) { //서버로부터 데이터를 받아온 경우
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);       
        String payload = http.getString();
        Serial.println(payload);
        const char* json = payload.c_str();
          
        StaticJsonDocument<200> doc;

        auto error = deserializeJson(doc, json);          
        if (error) {
          Serial.print(F("deserializeJson() failed with code "));
          Serial.println(error.c_str());
          return;
        }
        value = doc["condValue"];
        Serial.println(value);

      } 
      else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
  }

  if (value > 0){
    Serial.println("Doorlock system on");
    digitalWrite(doorlock, HIGH);
  }

  if (value <= 0 ){
    Serial.println("Doorlock system off");
    digitalWrite(doorlock, LOW);
  }
  
  delay(1000*10);
}
