int i = 0;

void setup() {
  pinMode(10,OUTPUT);
  pinMode(13,OUTPUT);
}

void loop() {
  i = 1;
  if(i>0){
    digitalWrite(10,HIGH);
    digitalWrite(13,HIGH);
    delay(5000);
    i--;
  }
  if(i<=0){
    digitalWrite(10,LOW);
    digitalWrite(13,LOW);
    delay(5000);
    i++;
  }

}
