#include <Servo.h>
#include <Ultrasonic.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>

#define TRIGGER_PIN  4
#define ECHO_PIN     2

#define SS_PIN 		53
#define RST_PIN 	A1
#define COVER_PIN   A0

Servo myservo;
SoftwareSerial HC06(10, 11); // TX, RX
MFRC522 rfid(SS_PIN, RST_PIN);

int pos = 90;
int enableA = 3;
int pinA1 = 9;
int pinA2 = 8;


// motor two
int enableB = 5;
int pinB1 = 7;
int pinB2 = 6;
int a = 0;
int a2 = 0;
int a3 = 0;
int a4 = 0;
int a5 = 0;
char b;
char c = 'Y';
int i1 = A2;     //sensor1
int i2 = A3;
int i3 = A4;    //sensor2
int s1,s2,s3;
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

void setup()
{
  myservo.attach(COVER_PIN);
  Serial.begin(9600);
  SPI.begin(); 
  rfid.PCD_Init();
  HC06.begin(9600);
  
  pinMode(enableA, OUTPUT);
  pinMode(pinA1, OUTPUT);
  pinMode(pinA2, OUTPUT);
 
  pinMode(enableB, OUTPUT);
  pinMode(pinB1, OUTPUT);
  pinMode(pinB2, OUTPUT);

  pinMode(i1, INPUT);
  pinMode(i2, INPUT);
  pinMode(i3, INPUT);
}
void loop()
{
  if(HC06.available()){
    char d= (char)HC06.read();
    Serial.println(d);
    switch(d){
      case 'W':
          b = ' ';
          digitalWrite(pinA1, LOW);
          digitalWrite(pinA2, LOW);
          digitalWrite(pinB1, LOW);
          digitalWrite(pinB2, LOW);
          c = 'W';
          break;
      case 'X':
          s1==LOW;
          s2==LOW;
          s3==LOW;
          b = ' ';
          digitalWrite(pinA1, LOW);
          digitalWrite(pinA2, LOW);
          digitalWrite(pinB1, LOW);
          digitalWrite(pinB2, LOW);
          c = 'X';
          break;
      case 'Y':
          s1==LOW;
          s2==LOW;
          s3==LOW;
          digitalWrite(pinA1, LOW);
          digitalWrite(pinA2, LOW);
          digitalWrite(pinB1, LOW);
          digitalWrite(pinB2, LOW);
        c = 'Y';
        break;
      case 'Z':        
        b = ' ';
        s1==LOW;
        s2==LOW;
        s3==LOW;
          digitalWrite(pinA1, LOW);
          digitalWrite(pinA2, LOW);
          digitalWrite(pinB1, LOW);
          digitalWrite(pinB2, LOW);
        c = 'Z';
        break;
                }               
if(c == 'Y')
  {
        switch(d){
          case 'U':
            b = 'U';
            break;
          case 'D':
            b = 'D';
            break;
          case 'R':
            b = 'R';
            break;
        case 'L':
            b = 'L';
            break;
        case 'B':
            b = 'B' ;     
            break;
        case 'O':
            b = 'O' ;     
            break;
        case 'C':
            b = 'C' ;     
            break;    
                 }
  }
  } 
  if(c == 'W')
  {
       float cmMsec, inMsec;
       long microsec = ultrasonic.timing();
       cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
       inMsec = ultrasonic.convert(microsec, Ultrasonic::IN);
        Serial.print("MS: ");
        Serial.print(microsec); 
        Serial.print(", CM: ");
        Serial.print(cmMsec);
        Serial.print(", IN: ");
        Serial.println(inMsec);
        delayMicroseconds(10);
        if(cmMsec > 25){
          s1=digitalRead(i1);
          s2=digitalRead(i2);
          s3=digitalRead(i3);
          if(s1==HIGH && s2==HIGH && s3==HIGH)
          {
              digitalWrite(pinA1, HIGH);
              digitalWrite(pinA2, LOW);
              digitalWrite(pinB1, HIGH);
              digitalWrite(pinB2, LOW);
          }
          else if(s1==LOW && s2==LOW && s3==LOW)
          {
              digitalWrite(pinA1, LOW);
              digitalWrite(pinA2, LOW);
              digitalWrite(pinB1, LOW);
              digitalWrite(pinB2, LOW);
          }
          else if(s1==LOW && s2==HIGH && s3==HIGH)
          {
              digitalWrite(pinA1, HIGH);
              digitalWrite(pinA2, LOW);
              digitalWrite(pinB1, LOW);
              digitalWrite(pinB2, HIGH);
          }
          else if(s1==LOW && s2==LOW && s3==HIGH)
          {
              digitalWrite(pinA1, HIGH);
              digitalWrite(pinA2, LOW);
              digitalWrite(pinB1, LOW);
              digitalWrite(pinB2, HIGH);
          }
          else if(s1==HIGH && s2==HIGH && s3==LOW)
          {
              digitalWrite(pinA1, LOW);
              digitalWrite(pinA2, HIGH);
              digitalWrite(pinB1, HIGH);
              digitalWrite(pinB2, LOW);
          }
          else if(s1==HIGH && s2==LOW && s3==LOW)
          {
              digitalWrite(pinA1, LOW);
              digitalWrite(pinA2, HIGH);
              digitalWrite(pinB1, HIGH);
              digitalWrite(pinB2, LOW);
          }
          else if(s1==LOW && s2==HIGH && s3==LOW)
          {
              digitalWrite(pinA1, HIGH);
              digitalWrite(pinA2, LOW);
              digitalWrite(pinB1, HIGH);
              digitalWrite(pinB2, LOW);
          }
            }
        else{
          digitalWrite(pinA1, LOW);
          digitalWrite(pinA2, LOW);
          digitalWrite(pinB1, LOW);
          digitalWrite(pinB2, LOW);
        }  
  }
  if(c == 'X')
  {
      if ( ! rfid.PICC_IsNewCardPresent())  // Look for new cards
        return;
      if ( ! rfid.PICC_ReadCardSerial()) // Verify if the NUID has been readed
        return;
      Serial.println("The NUID tag is:");
      printHex(rfid.uid.uidByte, rfid.uid.size);
      rfid.PICC_HaltA();  // Halt PICC
      {
      if(rfid.uid.uidByte[0] == 0xA3 && 
       rfid.uid.uidByte[1] == 0x84 &&
       rfid.uid.uidByte[2] == 0xA7 &&
       rfid.uid.uidByte[3] == 0xF7)
       {
       if(a <= 10)
       { 
       a = a + 1;
       myservo.write(0);
       pos=0;            
       Serial.print("you have use how many time: ");
       Serial.print(a);  
       delay(8000);
       }
       else
       {         
        Serial.print("You have over the limit");
       }
       }
       else if(rfid.uid.uidByte[0] == 0xE3 && 
       rfid.uid.uidByte[1] == 0xE3 &&
       rfid.uid.uidByte[2] == 0xA6 &&
       rfid.uid.uidByte[3] == 0xF7)
       {
       if(a2 <= 7)
       { 
       a2 = a2 + 1;
       myservo.write(0);
       pos=0;            
       Serial.print("you have use how many time: ");
       Serial.print(a2);  
       delay(8000);
       }
       else
       {         
        Serial.print("You have over the limit");
       }
       }
       else if(rfid.uid.uidByte[0] == 0x63 && 
       rfid.uid.uidByte[1] == 0x38 &&
       rfid.uid.uidByte[2] == 0x74 &&
       rfid.uid.uidByte[3] == 0xF7)
       {
       if(a3 <= 5)
       { 
       a3 = a3 + 1;
       myservo.write(0);
       pos=0;            
       Serial.print("you have use how many time: ");
       Serial.print(a3);  
       delay(5000);
       }
       else
       {         
        Serial.print("You have over the limit");
       }
       }
       else if(rfid.uid.uidByte[0] == 0x53 && 
       rfid.uid.uidByte[1] == 0x9D &&
       rfid.uid.uidByte[2] == 0x78 &&
       rfid.uid.uidByte[3] == 0xF7)
       {
       if(a4 <= 4)
       { 
       a4 = a4 + 1;
       myservo.write(0);
       pos=0;            
       Serial.print("you have use how many time: ");
       Serial.print(a4);  
       delay(3000);
       }
       else
       {         
        Serial.print("You have over the limit");
       }
       }
       else if(rfid.uid.uidByte[0] == 0xD3 && 
       rfid.uid.uidByte[1] == 0x6A &&
       rfid.uid.uidByte[2] == 0x7E &&
       rfid.uid.uidByte[3] == 0xF7)
       {
       a5 = a5 + 1;
       myservo.write(0);
       pos=0;            
       Serial.print("you have use how many time: ");
       Serial.print(a5);  
       delay(8000);
       }
  }   
      if(pos==0){      
       myservo.write(90);
       pos=90;
       delay(1000);   
       }               
  }
  else if(c == 'Z')
  {
       float cmMsec, inMsec;
       long microsec = ultrasonic.timing();
       cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
       inMsec = ultrasonic.convert(microsec, Ultrasonic::IN);
        Serial.print("MS: ");
        Serial.print(microsec); 
        Serial.print(", CM: ");
        Serial.print(cmMsec);
        Serial.print(", IN: ");
        Serial.println(inMsec);
        delayMicroseconds(10);
        if(cmMsec > 45){
          digitalWrite(pinA1, HIGH);
          digitalWrite(pinA2, LOW);
          digitalWrite(pinB1, HIGH);
          digitalWrite(pinB2, LOW);
        }
        else if(cmMsec > 35 && cmMsec < 46)
        {
          digitalWrite(pinA1, LOW);
          digitalWrite(pinA2, HIGH);
          digitalWrite(pinB1, LOW);
          digitalWrite(pinB2, HIGH);
        }
        else if(cmMsec < 36)
        {   
          enableMotors();
          digitalWrite(pinA1, LOW);
          digitalWrite(pinA2, HIGH);
          digitalWrite(pinB1, LOW);
          digitalWrite(pinB2, HIGH);
          delay(500);
          digitalWrite(pinA1, LOW);
          digitalWrite(pinA2, HIGH);
          digitalWrite(pinB1, HIGH);
          digitalWrite(pinB2, LOW);
          delay(1000);
        }
        else
        {
          enableMotors();
          forward(1000);
        }      
  }
  if(b == 'U')
  {
    enableMotors();
    forward(100);
    }
  else if(b == 'D')
  {
    enableMotors();
    backward(100);
    }
  else if(b == 'L')
  {
    enableMotors();
    turnLeft(100);
    }
  else if(b == 'R')
  {
    enableMotors();
    turnRight(100);
    }
  else if(b == 'B')
  {
    motorABrake();
    motorBBrake();
    }    
  else if(b == 'O')
  {
    myservo.write(0);              
    }  
  else if(b == 'C')
  {
    myservo.write(90);
    }            
}
void enableMotors()
{
  digitalWrite(enableA, HIGH);
  digitalWrite(enableB, HIGH);
} 
void disableMotors()
{
 digitalWrite(enableA, LOW);
 digitalWrite(enableB, LOW);
}
void forward(int time)
{
 digitalWrite(pinA1, HIGH);
 digitalWrite(pinA2, LOW);
 digitalWrite(pinB1, HIGH);
 digitalWrite(pinB2, LOW);
}
void backward(int time)
{
 digitalWrite(pinA1, LOW);
 digitalWrite(pinA2, HIGH);
 digitalWrite(pinB1, LOW);
 digitalWrite(pinB2, HIGH);
} 
void turnLeft(int time)
{
 digitalWrite(pinA1, LOW);
 digitalWrite(pinA2, HIGH);
 digitalWrite(pinB1, HIGH);
 digitalWrite(pinB2, LOW);
}
void turnRight(int time)
{
 digitalWrite(pinA1, HIGH);
 digitalWrite(pinA2, LOW);
 digitalWrite(pinB1, LOW);
 digitalWrite(pinB2, HIGH);
}
void motorABrake()
{
 digitalWrite(pinA1, HIGH);
 digitalWrite(pinA2, HIGH);
}
void motorBBrake()
{
 digitalWrite(pinB1, HIGH);
 digitalWrite(pinB2, HIGH);
}
void printHex(byte *buffer, byte bufferSize) {
  String temp;
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
    temp = temp + String(buffer[i], HEX) + " ";
  }
  HC06.println("RFID " + temp);
  Serial.println();
}

