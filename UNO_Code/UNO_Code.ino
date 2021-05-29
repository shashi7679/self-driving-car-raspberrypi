const int EnableL=5;
const int HighL =6;  //Left Side
const int LowL = 7;

const int EnableR = 11;
const int HighR = 10; // Right Side
const int LowR = 9;

const int D0=0;
const int D1=1;
const int D2=2;
const int D3=3;

int a,b,c,d,data;

void Data(){
  a=digitalRead(D0);
  b=digitalRead(D1);
  c=digitalRead(D2);
  d=digitalRead(D3);

  data = a+2*b+4*c+8*d;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(EnableR,OUTPUT);
  pinMode(HighR,OUTPUT);
  pinMode(LowR,OUTPUT);
  pinMode(EnableL,OUTPUT);
  pinMode(HighL,OUTPUT);
  pinMode(LowL,OUTPUT);
  pinMode(D0,INPUT_PULLUP);
  pinMode(D1,INPUT_PULLUP);
  pinMode(D2,INPUT_PULLUP);
  pinMode(D3,INPUT_PULLUP);
  

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
  digitalWrite(HighL,HIGH);
  digitalWrite(LowL,LOW);
  analogWrite(EnableL,255);

  digitalWrite(HighR,HIGH);
  digitalWrite(LowR,LOW);
  analogWrite(EnableR,255);
}

void Left_Slight(){
  digitalWrite(HighL,LOW);
  digitalWrite(LowL,HIGH);
  analogWrite(EnableL,100);

  digitalWrite(HighR,LOW);
  digitalWrite(LowR,HIGH);
  analogWrite(EnableR,255);
}
void Left_Medium(){
  digitalWrite(HighL,LOW);
  digitalWrite(LowL,HIGH);
  analogWrite(EnableL,90);

  digitalWrite(HighR,LOW);
  digitalWrite(LowR,HIGH);
  analogWrite(EnableR,255);
}
void Left_Extreme(){
  digitalWrite(HighL,LOW);
  digitalWrite(LowL,HIGH);
  analogWrite(EnableL,50);

  digitalWrite(HighR,LOW);
  digitalWrite(LowR,HIGH);
  analogWrite(EnableR,255);
}

void Right_Slight(){
  digitalWrite(HighL,LOW);
  digitalWrite(LowL,HIGH);
  analogWrite(EnableL,255);

  digitalWrite(HighR,LOW);
  digitalWrite(LowR,HIGH);
  analogWrite(EnableR,100);
}
void Right_Medium(){
  digitalWrite(HighL,LOW);
  digitalWrite(LowL,HIGH);
  analogWrite(EnableL,255);

  digitalWrite(HighR,LOW);
  digitalWrite(LowR,HIGH);
  analogWrite(EnableR,90);
}
void Right_Extreme(){
  digitalWrite(HighL,LOW);
  digitalWrite(LowL,HIGH);
  analogWrite(EnableL,255);

  digitalWrite(HighR,LOW);
  digitalWrite(LowR,HIGH);
  analogWrite(EnableR,50);
}
void Stop()
{
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  analogWrite(EnableL,0);

  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnableR,0);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  Data();
  if(data==0){
    Forward();
  }
  else if(data==1){
    Right_Slight();
  }
  else if(data==2){
    Right_Medium();
  }
  else if(data==3){
    Right_Extreme();
  }
  else if(data==4){
    Left_Slight();
  }
  else if(data==5){
    Left_Medium();
  }
  else if(data==6){
    Left_Extreme();
  }
  else if(data==7){
    Stop();
  }
  else if(data==8){
       {
     analogWrite(EnableL, 0);
     analogWrite(EnableR, 0);
     delay(4000);                      // stop sign detection

     analogWrite(EnableL, 150);
     analogWrite(EnableR, 150);
     delay(1000);
   }
  }
  

}
