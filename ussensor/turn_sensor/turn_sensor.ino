// const int trigPin = 13;
// const int echoPin = 3;
// float distance, duration;
// void setup() {
//  pinMode(trigPin, OUTPUT);
//   pinMode(echoPin, INPUT);
//  Serial.begin(9600);
// }

// void loop() {
//    digitalWrite(trigPin, LOW);
//   delayMicroseconds(2);
//   digitalWrite(trigPin, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(trigPin, LOW);
//   duration = pulseIn(echoPin, HIGH);
//   distance = (duration*.0343)/2;
//  Serial.print("Distance: ");
//   Serial.println(distance);
//   delay(100);



// }



#include <math.h>                         // Mathematical functions

const int trigPin = 11;                   // Front US trigger pin
const int echoPin = 2;                    // Front US echo pin

void setup()
{
    pinMode(trigPin, OUTPUT);             // Configure trigger as output
    pinMode(echoPin, INPUT);              // Configure echo as input

    digitalWrite(trigPin, LOW);           // Initially disable trigger

    Serial.begin(9600);                   // Initialize serial communication
}

// ======================================================
// VARIABLES FOR ULTRASONIC DISTANCE MEASUREMENT
// ======================================================

unsigned long duration = 0;               // Echo pulse duration in us

float distance = 0;                       // Front-wall distance in cm

bool validMeasurement = false;            // Indicates reliable US reading

int minimumDistance = 2;                  // Minimum reliable US distance

int maximumDistance = 400;                // Maximum reliable US distance

// ======================================================
// VARIABLES FOR FRONT-WALL DETECTION
// ======================================================

bool wallDetected = false;                // Confirms presence of front wall

bool turningEnabled = true;               // Enables front-wall detection

bool potentialWall = false;               // Indicates possible front wall

bool turnInProgress = false;              // Prevents repeated turn initiation

bool turnCompleted = false;               // Confirms completion of turn

bool emergencyStop = false;               // Indicates dangerous proximity

int wallCounter = 0;                      // Counts consecutive wall readings

int requiredReadings = 3;                 // Readings required to confirm wall

int samplingInterval = 60;                // US sampling interval in ms

unsigned long lastReadingTime = 0;        // Previous measurement time

// ======================================================
// DISTANCE THRESHOLDS
// ======================================================

int slowingDistance = 40;                 // Begin reducing thrust at 40 cm

int turningDistance = 25;                 // Initiate turning at 25 cm

int criticalDistance = 10;                // Emergency stopping threshold

int wallClearanceDistance = 45;           // Distance required to rearm detection

// ======================================================
// VARIABLES FOR IMU AND TURNING
// ======================================================

float currentYaw = 0.0;                   // Current yaw received from IMU

float initialYaw = 0.0;                   // Heading before starting turn

float targetYaw = 0.0;                    // Desired heading after turning

float headingError = 0.0;                 // Difference from target heading

float turningAngle = 90.0;                // Required chassis rotation

int angularTolerance = 5;                 // Acceptable turning error in degrees

bool imuValid = false;                    // True when IMU data is available

int turnDirection = -1;                   // -1 = left, +1 = right

int completedTurns = 0;                   // Number of completed course turns

unsigned long turnStartTime = 0;          // Time when turning begins

unsigned long maximumTurnTime = 4000;     // Maximum turning duration in ms

// ======================================================
// VARIABLES FOR SERVO AND FAN CONTROL
// ======================================================

int normalThrust = 130;                   // Normal propulsion PWM

int reducedThrust = 80;                   // Reduced propulsion PWM

int turningThrust = 100;                  // Propulsion PWM during turning

int liftSpeed = 255;                      // Lift fan PWM

int servoCenter = 90;                     // Neutral steering position

int servoLeft = 60;                       // Left steering position

int servoRight = 120;                     // Right steering position

// ======================================================
// ULTRASONIC DISTANCE MEASUREMENT
// ======================================================

float readFrontDistance()
{
    digitalWrite(trigPin, LOW);           // Ensure trigger starts LOW

    delayMicroseconds(2);                 // Wait for signal stabilization

    digitalWrite(trigPin, HIGH);          // Start ultrasonic transmission

    delayMicroseconds(10);                // Generate a 10 us trigger pulse

    digitalWrite(trigPin, LOW);           // End trigger pulse

    duration = pulseIn(
        echoPin, HIGH, 25000UL
    );                                    // Measure echo with timeout

    if (duration == 0)
    {
        return -1.0;                      // No echo or measurement timeout
    }

    float measuredDistance =
        (duration * 0.0343) / 2.0;        // Convert travel time to cm

    if (measuredDistance < minimumDistance ||
        measuredDistance > maximumDistance)
    {
        return -1.0;                      // Reject unreliable measurements
    }

    return measuredDistance;              // Return valid front-wall distance
}

// ======================================================
// ANGLE NORMALIZATION
// ======================================================

float normalizeAngle(float angle)
{
    while (angle > 180.0)
    {
        angle -= 360.0;                   // Keep angle below 180 degrees
    }

    while (angle < -180.0)
    {
        angle += 360.0;                   // Keep angle above -180 degrees
    }

    return angle;                         // Return normalized heading
}

// ======================================================
// TURN INITIATION
// ======================================================

void initiateTurn()
{
    if (!imuValid)
    {
        emergencyStop = true;             // Cannot turn safely without IMU

        // Stop thrust fan

        return;
    }

    initialYaw = currentYaw;              // Record heading before turn

    targetYaw = normalizeAngle(
        initialYaw + turnDirection * turningAngle
    );                                    // Calculate desired heading

    turnInProgress = true;                // Enable turning procedure

    turnCompleted = false;                // Turn has not finished yet

    turningEnabled = false;               // Disable new wall-triggered turns

    turnStartTime = millis();             // Record turning start time

    wallCounter = 0;                      // Reset wall confirmation counter

    Serial.println("WALL CONFIRMED");

    Serial.print("Target yaw: ");
    Serial.println(targetYaw);

    // Keep lift fan operating

    // Reduce forward thrust

    // Set servo toward the required turning direction

    // Apply turning thrust
}

// ======================================================
// IMU-BASED TURNING CONTROL
// ======================================================

void controlTurning()
{
    if (!imuValid)
    {
        emergencyStop = true;             // IMU measurement unavailable

        turnInProgress = false;           // Cancel turning procedure

        // Stop thrust fan
        // Center servo

        return;
    }

    headingError = normalizeAngle(
        targetYaw - currentYaw
    );                                    // Calculate remaining rotation

    if (millis() - turnStartTime >= maximumTurnTime)
    {
        emergencyStop = true;             // Turning took too long

        turnInProgress = false;           // Cancel turning procedure

        Serial.println("TURN TIMEOUT");

        // Stop thrust fan
        // Center servo

        return;
    }

    if (fabs(headingError) <= angularTolerance)
    {
        turnInProgress = false;           // Stop turning procedure

        turnCompleted = true;             // Confirm desired heading reached

        completedTurns++;                 // Record completed course turn

        Serial.println("TURN COMPLETED");

        // Stop turning thrust

        // Return servo to neutral position

        // Update straight-line heading reference

        // Resume forward navigation when stable
    }

    else
    {
        if (headingError < 0)
        {
            // Set servo toward the left

            Serial.println("TURNING LEFT");
        }

        else
        {
            // Set servo toward the right

            Serial.println("TURNING RIGHT");
        }

        if (fabs(headingError) <= 20.0)
        {
            // Reduce thrust as the target heading approaches

            // Prevent excessive rotation and overshooting
        }

        else
        {
            // Maintain normal turning thrust
        }
    }
}

// ======================================================
// MAIN LOOP
// ======================================================

void loop()
{
    // Update currentYaw using the IMU module

    // Set imuValid according to IMU measurement validity

    // Determine turnDirection from the course navigation plan

    // ==================================================
    // ULTRASONIC SENSOR SAMPLING
    // ==================================================

    if (millis() - lastReadingTime >= samplingInterval)
    {
        lastReadingTime = millis();       // Record sampling time

        distance = readFrontDistance();   // Read front US sensor

        validMeasurement =
            distance >= minimumDistance &&
            distance <= maximumDistance; // Verify measurement validity

        if (validMeasurement)
        {
            Serial.print("Front distance: ");
            Serial.print(distance);
            Serial.println(" cm");
        }

        else
        {
            Serial.println("Invalid US measurement");
        }

        // ==============================================
        // EMERGENCY WALL PROXIMITY
        // ==============================================

        if (validMeasurement &&
            distance < criticalDistance)
        {
            emergencyStop = true;         // Wall dangerously close

            turnInProgress = false;       // Cancel active turning

            wallCounter = 0;              // Reset confirmation counter

            Serial.println("EMERGENCY STOP");
        }

        // ==============================================
        // FRONT-WALL DETECTION
        // ==============================================

        if (!emergencyStop &&
            turningEnabled &&
            !turnInProgress &&
            !wallDetected)
        {
            if (!validMeasurement)
            {
                wallCounter = 0;          // Reject unreliable reading

                potentialWall = false;    // Do not confirm a wall

                // Stop thrust until valid readings return
            }

            else if (distance > slowingDistance)
            {
                wallCounter = 0;          // No approaching wall

                potentialWall = false;    // No wall candidate

                // Continue normal forward navigation
            }

            else if (distance > turningDistance &&
                     distance <= slowingDistance)
            {
                wallCounter = 0;          // Wall not close enough to turn

                potentialWall = false;    // Wait for closer measurement

                Serial.println("SLOW DOWN");

                // Reduce forward thrust

                // Maintain lift fan speed

                // Maintain straight heading using IMU
            }

            else if (distance >= criticalDistance &&
                     distance <= turningDistance)
            {
                potentialWall = true;     // Possible front wall

                wallCounter++;            // Count consecutive readings

                // Reduce forward thrust

                if (wallCounter >= requiredReadings)
                {
                    wallDetected = true;  // Confirm presence of wall

                    initiateTurn();       // Start turning procedure
                }

                else
                {
                    // Continue checking front-wall distance

                    // Maintain reduced thrust

                    // Do not initiate a turn yet
                }
            }

            else
            {
                wallCounter = 0;          // Reset uncertain readings

                potentialWall = false;    // Reject wall candidate
            }
        }

        // ==============================================
        // REARM WALL DETECTION AFTER TURNING
        // ==============================================

        if (turnCompleted &&
            !turnInProgress &&
            validMeasurement &&
            distance >= wallClearanceDistance)
        {
            wallDetected = false;         // Forget previous wall

            potentialWall = false;        // Reset wall candidate

            turnCompleted = false;        // Reset turn completion flag

            turningEnabled = true;        // Enable next wall detection

            wallCounter = 0;              // Reset reading counter

            Serial.println("READY FOR NEXT WALL");

            // Resume normal forward navigation
        }
    }

    // ==================================================
    // SAFETY CONTROL
    // ==================================================

    if (emergencyStop)
    {
        // Stop thrust fan immediately

        // Center steering servo

        // Notify main navigation controller

        return;                           // Prevent further navigation
    }

    if (!validMeasurement)
    {
        // Stop thrust until reliable US data returns

        return;                           // Do not continue turning blindly
    }

    // ==================================================
    // TURNING CONTROL
    // ==================================================

    if (turnInProgress)
    {
        controlTurning();                 // Monitor IMU heading continuously
    }
}
