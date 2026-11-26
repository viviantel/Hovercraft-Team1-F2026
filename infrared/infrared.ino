#define sensor A2 
void setup() 
{
  Serial.begin(9600);
}
// Variables to initialize for exit detection
bool finalSegmentReached = false; //To make sure that we are on the last segment of the track
                          //Enables the exit detection

float headingError; //To verify the orientation of our sensor  

int angularTolerance = 10;/*degrees*/ //The maximum acceptable heading error

bool wallPreviouslyDetected = false; //variable used to verify that there is an opening
 
int noWallCounter = 0; //Counter used to keep track of the opening readings.
                      // As the hovercraft (hc) might consider a corner as an opening 
                      // and therefore wrongly initiates a turn, when encountering an opening,
                      // the hc will check the distance (opening reading) every 50 ms 
                      //(to make sure it's the valid exit)

int requiredReadings = 3; // number of opening readings required to be considered as the valid exit

int samplingInterval = 50;/*ms*/ //After how much time do the hc check the opening reading

bool exitDetected = false; //Confirms opening and prevent repeated triggers

unsigned long lastReadingTime = 0;

float distance;

bool potentialOpening = false; bool potentialOpening = false; // potentialOpening should be set to true when:
                                                              // 1. A valid distance between 60 and 80 cm is detected or
                                                              // 2. The analog voltage corresponds to the calibrated absence of a wall.
                                                              // The navigation system must maintain sufficient clearance from the left 
                                                              // wall to avoid ambiguous measurements below 10 cm.


void loop() 
{

  //Code for technical assignment

  /*float volts = analogRead(sensor)*0.0048828125;
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
*/


// Logic behind the exit detection

if (finalSegmentReached == true && exitDetected == false) {

    if (abs(headingError) <= angularTolerance) {

        if (millis() - lastReadingTime >= samplingInterval) {

            lastReadingTime = millis();

            // Read the analog IR voltage

            // Convert the voltage into estimated distance when the measurement is valid

            // Determine whether the reading indicates a potential opening


            if (distance >= 10 && distance <= 55) {

                wallPreviouslyDetected = true;

                noWallCounter = 0;

                // Continue normal navigation

            }

            else if (potentialOpening && wallPreviouslyDetected == true) {

                noWallCounter++;

                if (noWallCounter >= requiredReadings) {

                    exitDetected = true;

                    // Notify the navigation system that the opening is confirmed

                    // Reduce the forward thrust

                    // Align the hovercraft with the opening

                    // Initiate a left turn

                }

                else {

                    // Continue checking for the opening

                    // Maintain the counter value

                    // Continue normal navigation of the hc 

                }

            }

            else {

                noWallCounter = 0;

                // Uncertain or unreliable reading

                // Do not initiate a left turn

            }

        }

    }

    else {

        noWallCounter = 0;

        // Correct the heading of the hovercraft to be at correct angle 

    }

}

else {

    noWallCounter = 0;

    // Exit detection is disabled

    // Allow the navigation system to operate normally or complete the exit maneuver

}

