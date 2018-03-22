
#include <Wire.h>
#include <Servo.h>
const int current_1=A0;
const int current_2=A1;
const int current_3=A2;
const int current_4=A3;
const int battery_v0=A4;
const int battery_va=A5;
const int battery_vb=A7;
const int trigPin = 12;
const int echoPin = 13;
float data[4];
//byte motor[4];
int z=0;
int d=0;
long duration;
float distance;
byte current1;
byte current2;
byte current3;
byte current4;
byte current[4];
Servo motor_1;
Servo motor_2;
Servo motor_3;
Servo motor_4;
//resistance for battery circuit

float r1 = 1000;
float r2 = 1000;
float r3 = 2000;
float r4 = 1000;
void setup() {
  Wire.begin(0x40);        // join i2c bus with address #40
  Wire.onReceive(receiveEvent);
  Wire.onRequest(sendData);               // register event
  pinMode(trigPin, OUTPUT); 
//  Serial.begin(9600);
  motor_1.attach(3);
  motor_2.attach(4);
  motor_3.attach(5);
  motor_4.attach(6); 
  getdata();
}

void loop() {
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  
  while (0 < Wire.available()) { // loop through all but the last
      if(z==0){
        double x = map(Wire.read(),0,255,700,2300);
        motor_1.writeMicroseconds(x);
        z++;
      }
      else if(z==1){
        double x = map(Wire.read(),0,255,700,2300);
        motor_2.writeMicroseconds(x);
        z++;
      }
       else if(z==2){
        double x = map(Wire.read(),0,255,700,2300);
        motor_3.writeMicroseconds(x);
        z++;
      }
      else if(z==3){
        double x = map(Wire.read(),0,255,700,2300);
        motor_4.writeMicroseconds(x);
        z=0;
      }
  }
 //z=0 ;
 //while (Wire.available()>0){
 //motor[z]=Wire.read(); //define motor[] first foo2
 //z++;  
 //} 
 //motor_1.writeMicroseconds(map(motor[0],0,255,700,2300));
 //motor_2.writeMicroseconds(map(motor[1],0,255,700,2300));
 //motor_3.writeMicroseconds(map(motor[2],0,255,700,2300));
 //motor_4.writeMicroseconds(map(motor[3],0,255,700,2300));
}
float getdistance(){
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance
return duration*0.034/2;  
  
  }
void getdata(){
   current[0]= (byte) map(analogRead(current_1),0,1023,0,255);
   current[1]= (byte) map(analogRead(current_2),0,1023,0,255);
   current[2]= (byte) map(analogRead(current_3),0,1023,0,255);
   current[3]= (byte) map(analogRead(current_4),0,1023,0,255);
   data[0]=(float)analogRead(battery_v0);
   data[1]=(float)analogRead(battery_va);
   data[2]=(float)analogRead(battery_vb);
   data[3] =getdistance();
  }
void sendData(){
//     Serial.print(current[0]);
//   Serial.print("   ");
//   Serial.print(current[1]);
//   Serial.print("   ");
//   Serial.print(current[2]);
//   Serial.print("   ");
//   Serial.println(current[3]);
   if(d==0){
      Wire.write((byte*)&current,4*sizeof(byte));
      d++;
      }
      else if(d==1){
    Wire.write((byte*) &data,4*sizeof(float));
    getdata();
    d=0;
    }
  }
