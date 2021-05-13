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
  Serial.print(data);
}

void setup() {
  // put your setup code here, to run once:

  pinMode(D0,INPUT_PULLUP);
  pinMode(D1,INPUT_PULLUP);
  pinMode(D2,INPUT_PULLUP);
  pinMode(D3,INPUT_PULLUP);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  Data();

  if(data==0){
  Serial.print("no high");
  }
  else if(data==2){
  Serial.print("22 High"); 
  }
  else if(data==4){
    Serial.print("23 High");  
  }
  else if(data==8){
    Serial.print("24 High");   
  }


}
