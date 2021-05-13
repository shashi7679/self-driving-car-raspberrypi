const int EnableR=5;
const int HighR =6;  //Right Side
const int LowR = 7;

const int EnableL = 11;
const int HighL = 10; // Left Side
const int LowL = 9;

void setup() {
  // put your setup code here, to run once:
  pinMode(EnableR,OUTPUT);
  pinMode(HighR,OUTPUT);
  pinMode(LowR,OUTPUT);
  pinMode(EnableL,OUTPUT);
  pinMode(HighL,OUTPUT);
  pinMode(LowL,OUTPUT);

}
void Forward(){
  digitalWrite(HighL,LOW);
  digitalWrite(LowL,HIGH);
  analogWrite(EnableL,255);

  digitalWrite(HighR,LOW);
  digitalWrite(LowR,HIGH);
  analogWrite(EnableR,255);
}

void Backward(){
  digitalWrite(HighL,LOW);
  digitalWrite(LowL,HIGH);
  analogWrite(EnableL,255);

  digitalWrite(HighR,LOW);
  digitalWrite(LowR,HIGH);
  analogWrite(EnableR,255);
}
void loop() {
  // put your main code here, to run repeatedly:
  Forward();
  

}
