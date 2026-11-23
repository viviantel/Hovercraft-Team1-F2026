const int trigPin = 13;
const int echoPin = 3;
float distance, duration;
void setup() {
 pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
 Serial.begin(9600);
}
// US for bar detection
void loop() {
   digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
 Serial.print("Distance: ");
  Serial.println(distance);
  delay(100);

  while(true){
  
  }

}
