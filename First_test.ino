#include <ESP8266WiFi.h>
#include <WiFiClient.h>


int led = 13;
int lock = 10;
int motor = 7;

const char* ssid = "";  //와이파이 ssid
const char* pass = "";  //와이파이 password

const char* host = {52, 79, 87, 246};  //웹 서버 주소
const int port = 80;  //http port

const int inlet = A0; //데이터 수신 : A0
void setup() {
  Serial.begin(115200);  //수정해야함
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(Wifi.SSID());
  //서버랑 연결
  if(!Wificlient.connect(host, port)){
    Serial.println("connection failed");
    return;
  }

  pinMode (led, OUTPUT);
  pinMode (lock, OUTPUT);
  pinMode (motor, OUTPUT);
}



void loop() { 
  //서버로부터 value 값 가져오는 코드
  client.print(String("GET ")+"/"+" HTTP/1.1\r\n"+
    "Host: "+host+"\r\n"+"Connection: close\r\n\r\n");  //서버에 요청
  While(client.available()){
    char *line = client.readStringUntil('\r'); //서버에서 받은 모든 문자열 입력
  }
  buff[5] = {0,};
  int value = 0;
  while(line){
    if(isdigit(*line)){
      buff[i++] = *line;
    }
    line++;
    value = buff[i];
  }
  if (value > 0){
    digitalWrite (lock, HIGH);//잠금장치 해제
    digitalWrite (led, HIGH); // 13번핀을 HIGH(약 5V)로 출력 ( led on )
    digitalWrite (motor, HIGH);//선풍기 on
    
    }
  }

  if (value <= 0){
      digitalWrite (led, LOW); // 13번핀을 LOW(약 0V)로 출력 ( led off )
      digitalWrite (motor, LOW);//선풍기 off
      digitalWrite(lock, LOW);//잠금장치 작동
  }
}
