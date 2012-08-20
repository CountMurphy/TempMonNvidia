const int TxPin = 4;
const int PwrPin=8;
bool Daemon=false;
int Counter=0;
bool isFraked=false;
const String noData="!Data to Report";


#include <SoftwareSerial.h>
SoftwareSerial mySerial = SoftwareSerial(255, TxPin);

void setup() {

  pinMode(TxPin, OUTPUT);
  digitalWrite(TxPin, HIGH);
 

  mySerial.begin(9600);
 Serial.begin(9600);
  delay(300);
  mySerial.write(12);     // Clear    
  mySerial.write(22);
  mySerial.write(17);           // Turn backlight on
  delay(100);                           // Required delay
  OldSpice();                       
  mySerial.print("Temperature");
  mySerial.write(13);
  mySerial.print("Monitor");  
  delay(2000);
  mySerial.write(12);
  delay(5);
  mySerial.write(18);
  mySerial.print(noData);
}

void loop() {
  FUUUUUUUUU();
  Sensor();
  if(Serial.available() >0)
  {
    Daemon=true;
    Counter=0;
    char Letter;
    while(Serial.available()>0)
    {
      Letter=Serial.read();
      Serial.print(Letter);
      switch(Letter)
      {
        case '$':
          mySerial.print("Stable");
          break;
        case 'L':
          mySerial.write(12);
          delay(10);
          break;
        case 'N':
           mySerial.write(148);
           break;
        case '#':
           break;
        case 'F':
          isFraked=true;
          break;
        default:
           mySerial.print(Letter);
      }
    }
    //Serial.print("Data Recieved");
  }else{
    Counter++;
    if(Counter>=3 && Daemon)
    {
     mySerial.write(12);
     delay(5);
     mySerial.write(18);
     mySerial.print(noData);
     Counter=0;
     Daemon=false;
     isFraked=false;
    } 
  }
   delay(1000);
}


void OldSpice()
{
    mySerial.write(210);
    mySerial.write(228); //f
    mySerial.write(228); //f
    mySerial.write(227); //e
    mySerial.write(232);//wait
    mySerial.write(230);//g
    mySerial.write(232);//wait
    mySerial.write(228);//f
    delay(5);
    mySerial.write(232);//wait
    mySerial.write(230);//g
    //mySerial.write(232);//wait
    mySerial.write(220);//a
    mySerial.write(232);
    mySerial.write(228);//f
}

void FUUUUUUUUU()
{
  if(isFraked)
  {
    mySerial.write(212);          // Quarter note
    mySerial.write(220);
    delay(5);
  }
}

void Sensor()
{
  int light=analogRead(3);
  if(light<80 || light>= 700)
  {
    mySerial.write(17);
  }else{
    mySerial.write(18);
  }
}
  





