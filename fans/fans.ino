const int fanPin = 5; // Must be a PWM-capable pin (Pin 3, 5, 6, 9, 10, or 11 on Nano)[cite: 1]
const int fanPin1 = 6;
void setup() {
  pinMode(fanPin, OUTPUT);
  pinMode(fanPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // 1. RAMP UP (Dim to Full Speed)
  //Serial.println("Ramping up fan speed...");
    //analogWrite(fanPin, 255); // 0 = OFF, 255 = 100% Speed
    digitalWrite(fanPin, HIGH);
    digitalWrite(fanPin1, HIGH);
  }
  

