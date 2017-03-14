#include <SPI.h>
#include <Servo.h>
#include <MFRC522.h>
#include <Ultrasonic.h>
#include <SoftwareSerial.h>

#define TRIGGER_PIN  		4
#define ECHO_PIN     		2
#define SS_PIN 		 		53
#define RST_PIN 	 		A1
#define COVER_MOTOR_PIN			A0

#define LEFT_MOTOR_ENABLE		3
#define LEFT_FRONT_MOTOR_PIN	9
#define LEFT_BACK_MOTOR_PIN		8

#define RIGHT_MOTOR_ENABLE		5
#define RIGHT_FRONT_MOTOR_PIN	7
#define RIGHT_BACK_MOTOR_PIN	6

#define LEFT_IR_SENSOR			A2
#define MIDDLE_IR_SENSOR		A3
#define RIGHT_IR_SENSOR			A4

Servo myservo;
SoftwareSerial HC06(10, 11); // TX, RX
MFRC522 rfid(SS_PIN, RST_PIN);
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

void setup()
{
	SPI.begin();
	rfid.PCD_Init();
	HC06.begin(9600);
	Serial.begin(9600);
	myservo.attach(COVER_MOTOR_PIN);
  
	//Initialize Left Side Mortors
  
	pinMode(LEFT_MOTOR_ENABLE	,OUTPUT);
	pinMode(LEFT_BACK_MOTOR_PIN	,OUTPUT);
	pinMode(LEFT_FRONT_MOTOR_PIN,OUTPUT);
 
	//Initialize Right Side Motors
 
	pinMode(RIGHT_MOTOR_ENABLE	,OUTPUT);
	pinMode(RIGHT_BACK_MOTOR_PIN,OUTPUT);
	pinMode(RIGHT_FRONT_MOTOR_PIN,OUTPUT);
	
	//Initialize IR sensors
	
	pinMode(LEFT_IR_SENSOR, INPUT);
	pinMode(RIGHT_IR_SENSOR, INPUT);
	pinMode(MIDDLE_IR_SENSOR, INPUT);
	
}

void loop(){
	
	char control=' ';
	char user_Instruct=' ';
	
	if(HC06.available()){
		
		char data_From_Bluethooth = (char)HC06.read();
		
		Serial.println("=> "+data_From_Bluethooth+" Recived From Bluethooth\n");
    
		if(data_From_Bluethooth != 'Y')
			user_Instruct= ' ';
		
		stopAllMotor();
		control = data_From_Bluethooth;

		if(control == 'Y')
			user_Instruct = data_From_Bluethooth;
	} 	
	
	if(control=='W')
		IR_Walk_Mode();
}
void IR_Walk_Mode(){
	
	float cmMsec, inMsec;
    long microsec = ultrasonic.timing();
	int s1,s2,s3;
    
	cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
    inMsec = ultrasonic.convert(microsec, Ultrasonic::IN);
    
    Serial.println("MS: "+microsec+", CM: "+cmMsec+", IN: "+inMsec);
    
	delayMicroseconds(10);
        
	if(cmMsec > 25){
	
		s1=digitalRead(RIGHT_IR_SENSOR);
        s2=digitalRead(MIDDLE_IR_SENSOR);
        s3=digitalRead(LEFT_IR_SENSOR);
		
		if(s1==HIGH && s2==HIGH && s3==HIGH)
			forward(20);
		else if(s1==LOW && s2==LOW && s3==LOW)
			stopAllMotor();
        else if(s1==LOW && s2==HIGH && s3==HIGH)
			turnRight(20);
        else if(s1==LOW && s2==LOW && s3==HIGH)
			turnRight(20);
        else if(s1==HIGH && s2==HIGH && s3==LOW)
			turnLeft(20);
        else if(s1==HIGH && s2==LOW && s3==LOW)
			turnLeft(20);
        else if(s1==LOW && s2==HIGH && s3==LOW)
			forward(20);
    }
    else
		stopAllMotor(); 
	
}
void enableMotors()
{
	digitalWrite(LEFT_MOTOR_ENABLE,HIGH);
	digitalWrite(RIGHT_MOTOR_ENABLE,HIGH);
} 

void disableMotors()
{
	digitalWrite(LEFT_MOTOR_ENABLE,LOW);
	digitalWrite(RIGHT_MOTOR_ENABLE,LOW);
}

void forward(int time)
{
	digitalWrite(LEFT_FRONT_MOTOR_PIN	,HIGH);
	digitalWrite(LEFT_BACK_MOTOR_PIN	,LOW);
	digitalWrite(RIGHT_FRONT_MOTOR_PIN	,HIGH);
	digitalWrite(RIGHT_BACK_MOTOR_PIN	,LOW);
}

void backward(int time)
{
	digitalWrite(LEFT_FRONT_MOTOR_PIN	,LOW);
	digitalWrite(LEFT_BACK_MOTOR_PIN	,HIGH);
	digitalWrite(RIGHT_FRONT_MOTOR_PIN	,LOW);
	digitalWrite(RIGHT_BACK_MOTOR_PIN	,HIGH);
} 

void turnLeft(int time)
{
	digitalWrite(LEFT_FRONT_MOTOR_PIN	,LOW);
	digitalWrite(LEFT_BACK_MOTOR_PIN	,HIGH);
	digitalWrite(RIGHT_FRONT_MOTOR_PIN	,HIGH);
	digitalWrite(RIGHT_BACK_MOTOR_PIN	,LOW);
}

void turnRight(int time)
{
	digitalWrite(LEFT_FRONT_MOTOR_PIN	,HIGH);
	digitalWrite(LEFT_BACK_MOTOR_PIN	,LOW);
	digitalWrite(RIGHT_FRONT_MOTOR_PIN	,LOW);
	digitalWrite(RIGHT_BACK_MOTOR_PIN	,HIGH);
}

void motorABrake()
{
	digitalWrite(LEFT_BACK_MOTOR_PIN	,HIGH);
	digitalWrite(LEFT_FRONT_MOTOR_PIN	,HIGH);
}

void motorBBrake()
{
	digitalWrite(RIGHT_BACK_MOTOR_PIN	,HIGH);
	digitalWrite(RIGHT_FRONT_MOTOR_PIN	,HIGH);
}

void stopAllMotor()
{
	
	digitalWrite(LEFT_FRONT_MOTOR_PIN	,LOW);
	digitalWrite(LEFT_BACK_MOTOR_PIN	,LOW);
	digitalWrite(RIGHT_FRONT_MOTOR_PIN	,LOW);
	digitalWrite(RIGHT_BACK_MOTOR_PIN	,LOW);

}

void printHex(byte *buffer, byte bufferSize) {
  
  String temp;
  
  for (byte i = 0; i < bufferSize; i++) 
  {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
    temp = temp + String(buffer[i], HEX) + " ";
  }
	HC06.println("RFID " + temp);
	Serial.println();
}



