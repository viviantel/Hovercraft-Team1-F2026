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

//if statement for bar detection. If detects something within 15cm, stops and starts again
  if(bar_distance<15cm){
    // hovercraft should stop (lift fan stays on, thurst fan stops)
    // hovercraft should slow down in the segment with the bar to give enough time for sensor to detect bar
    // will need to consider moving the placement of the US sensor to the front so by the time it detects, it can properly stop under the bar
    //in testing, we will need to measure how long it takes for US sensor to detect bar and actually stop. this can help us determine US sensor placement
    stopMotors(); //create function to stopMotor
    delay(100); //delay should be about 2-3 seconds. enough to visibly see it has stopped but not enough to cause issues starting up again
    //need to ensure when it stops, it doesnt spin out of control
    startMotor(); //create function to startMotor again 
  }

}
