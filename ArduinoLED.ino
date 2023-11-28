/* An object of class Auto football gym.  
 *  Author : 00gym team
 *  Date of last modification : 22 November 2023 */
 
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid = "changyup";
const char* pass = "dkssud12gkgk";
String address = "https://00gym.shop/arduino_check?id=1";
const int port = 443;   //https port num
int value;

const int kLED = 12;
LiquidCrystal_I2C lcd(0x27, 16, 2);

/*method for connecting WIFI
 * parameter : WIFI ssid, WIFI pass
 */
void WIFIConnection(const char* ssid, const char* pass)
{
  WiFi.begin(ssid, pass);
  Serial.print("[WIFI] ");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("[Wifi] Connected");
  Serial.print("IP address : \t");
  Serial.println(WiFi.localIP());
}

/*method for get condition variable from server
 * parameter : server address, port num(443)
 */
int GetValue(String address, const int port)
{
  WiFiClientSecure client;
  client.setInsecure();
  client.connect(address, port);
  HTTPClient http;
  int num;
  Serial.print("[HTTP] begin...\n");
  if (http.begin(client, address))
  {
    Serial.print("[HTTP] GET...\n");
    int httpcode = http.GET();
    if (httpcode > 0) 
    {
      Serial.printf("[HTTP] GET... code: %d\n", httpcode);       
      String payload = http.getString();
      Serial.println(payload);
      const char* json = payload.c_str();
        
      StaticJsonDocument<200> doc;
      auto error1 = deserializeJson(doc, json);          
      if (error1)
      {
        Serial.print(F("deserializeJson() failed with code "));
        Serial.println(error1.c_str());
        return -1;
      }
      String result = doc["result"];
      Serial.println(result);
      auto error2 = deserializeJson(doc, result);          
      if (error2)
      {
        Serial.print(F("deserializeJson() failed with code "));
        Serial.println(error2.c_str());
        return -1;
      }
      num = doc["condValue"];

      Serial.println(num);  
    } 
    else 
    {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpcode).c_str());
      lcd.setCursor(1,1);
      lcd.print("CCC");
    }

    http.end();
  }
  else
  {
    Serial.printf("[HTTP} Unable to connect\n");
  }
  return num;
}

/*method for system control by condition variable.
 * parameter : value 
 */
void SystemControl(int value)
{
  if (value > 0)
  {
    Serial.println("Led system on");
    digitalWrite(kLED, HIGH);
    lcd.setCursor(1,1);
    lcd.print("led on");
  }
  else if (value <= 0 )
  {
    Serial.println("Led system off");
    digitalWrite(kLED, LOW);
    lcd.setCursor(1,1);
    lcd.print("led off");
  }
}

void setup() 
{
  Serial.begin(115200);
  pinMode(kLED, OUTPUT);
  lcd.init();
  lcd.backlight();
  WIFIConnection(ssid, pass);
}

void loop() 
{
  if ((WiFi.status() != WL_CONNECTED))
  {
    Serial.println("NO WIFI");
    lcd.setCursor(0,0);
    lcd.print("No Wifi");
    WIFIConnection(ssid, pass);
    lcd.clear();
  }
  else if ((WiFi.status() == WL_CONNECTED)) 
  {
    lcd.setCursor(0,0);
    lcd.print("Wifi connected");
    value = GetValue(address, port);
  }
  
  SystemControl(value);

  delay(1000*60*3);
  lcd.clear();  
}
