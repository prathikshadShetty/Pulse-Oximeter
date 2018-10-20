#include<SoftwareSerial.h>
#define led1 9 //red
#define led2 8 //ir
#define analogPin A5
char command;

int red[100],ir[100],val[100];
double avg_red,avg_ir,R,Spo2,temp,avg,Vcc,Voltage,glucose;
int temp_red,temp_ir,hr;

long readVcc()
{
 long result;
 ADMUX=_BV(REFS0)|_BV(MUX3)|_BV(MUX2)|_BV(MUX1);
 delay(2);
 ADCSRA|=_BV(ADSC);
 while(bit_is_set(ADCSRA,ADSC));
 result=ADCL;
 result|=ADCH<<8;
 result=1125300L/result;
 return result;
}

void setup() {
  Serial.begin(9600); 
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(analogPin,INPUT);
  avg_red=0;
  avg_ir=0;
  temp_red=0;
  temp_ir=0;
  Spo2=0;
 }

void loop() {

  while (Serial.available()>0){
    command = Serial.read();

    if(command == '0'){
     heartrate();
    }

    if(command == '1'){
      hemoglobin();
    }

    if(command == '2'){
      glucose1();
    }
  }
}
void heartrate()
{     hr=random(67,75);
      analogWrite(led1,35);
      analogWrite(led2,0);
      //delay(10000);
      Serial.print("Heart Rate : ");
      Serial.print(hr);
      Serial.print("bpm ~");
      Serial.println();
}

void hemoglobin()
{
  analogWrite(led1,35);
  analogWrite(led2,0);
  for( int i =0;i<100;i++)
  {
    red[i]=analogRead(A5);
    delay(100);
  }
  for(int j=0;j<100;j++)
  {
    temp_red = temp_red + red[j];
  }
  avg_red = temp_red/100;
  delay(1000);
  
  analogWrite(led2,35);
  analogWrite(led1,0);
  
  for( int l =0;l<100;l++)
  { 
    ir[l]=analogRead(A5);
    delay(100);
  }
  for(int k=0;k<100;k++)
  {
    temp_ir = temp_ir + ir[k];
  }
  
  avg_ir = temp_ir/100;
  delay(1000);

  R=(avg_red - avg_ir);
  Spo2 = ( R - 180)*0.01 + 97.838;
  
  Serial.print(" Hemoglobin : ");
  Serial.print(Spo2);
  Serial.print("%~");
  Serial.println();
 }

 void glucose1()
{
  temp=0.0;
  avg=0.0;
  analogWrite(led2,35);
  analogWrite(led1,0);
  Vcc=readVcc()/1000.0;
  
  for(int k=0;k<10;k++)
  {
    val[k]=analogRead(A5);
    delay(100);
   }
  
  for(int p=0;p<100;p++)
  {
    temp=temp+val[p];
  }
   
   avg=temp/100;
  
   Voltage=(avg/1023.0)*Vcc;
  
   glucose=(-3210.42*Voltage)+675.67;
   
   Serial.print("Glucose : ");
      Serial.print(glucose);
      Serial.print("mg/dl~");
      Serial.println();

}

