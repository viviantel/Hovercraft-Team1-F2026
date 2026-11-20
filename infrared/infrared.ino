#define sensor A2 
void setup() 
{
  Serial.begin(9600);
}
void loop() 
{
  float volts = analogRead(sensor)*0.0048828125;
  int distance = 30*pow(volts, -1.173);
  delay(1000); 
  
  if ((distance >=10)&&(distance <=80))
  {
    Serial.print("Distance:");
    Serial.println(distance);
    //Serial.print("Volts:");
    //Serial.println(volts);   
  }
  else
  Serial.println("Out of range");
}