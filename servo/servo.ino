#include<Servo.h>
Servo servo1;
void setup() {
  servo1.attach(5);
 Serial.begin(9600);
}
void loop() {
//for (int angle = 0; angle <= 90; angle += 1) {
  //  servo1.write(angle);         
   // delay(15);                   
  //}
  
  servo1.write(0);
  delay(3000);
 servo1.write();
  delay(3000);
}