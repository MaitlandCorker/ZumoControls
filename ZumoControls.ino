#include <Wire.h>
#include <Zumo32U4.h>

// This might need to be tuned for different lighting conditions,
// surfaces, etc.
#define QTR_THRESHOLD     600  // microseconds

// These might need to be tuned for different motor types.
#define REVERSE_SPEED     100  // 0 is stopped, 400 is full speed
#define TURN_SPEED        100
#define FORWARD_SPEED     100
#define REVERSE_DURATION  100  // ms
#define TURN_DURATION     250  // ms

Zumo32U4Buzzer buzzer;
Zumo32U4Motors motors;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4LineSensors lineSensors;

const uint8_t sensorThreshold = 1;
uint16_t lastTimeObjectSeen = 0;

#define NUM_SENSORS 5
unsigned int lineSensorValues[NUM_SENSORS];

void countDown()
{
  ledYellow(1);

  ledYellow(0);
  

  // Play audible countdown.
  for (int i = 0; i < 3; i++)
  {
    delay(1000);
    buzzer.playNote(NOTE_G(3), 200, 15);
  }
  delay(1000);
  buzzer.playNote(NOTE_G(4), 500, 15);
  delay(1000);
}

void waitForCommand()
{
  motors.setSpeeds(0,0);

  if(Serial1.available() > 0){
  String cmd=Serial1.readString();
    if (cmd == "stop")
  {
    // If button is pressed, stop and wait for another press to
    // go again.
    motors.setSpeeds(0, 0);
    delay(10000);
  }else if (cmd == "turn_right")
  {
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }else if (cmd == "turn_left")
  {
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }else if (cmd == "auto")
  {
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    
  }
  Serial1.println(cmd);
  }
}

void calibrateSensors()
{


  // Wait 1 second and then begin automatic sensor calibration
  // by rotating in place to sweep the sensors over the line
  delay(1000);
  for(uint16_t i = 0; i < 120; i++)
  {
    if (i > 30 && i <= 90)
    {
      motors.setSpeeds(-200, 200);
    }
    else
    {
      motors.setSpeeds(200, -200);
    }

    lineSensors.calibrate();
  }
  motors.setSpeeds(0, 0);
}

void setup()
{
  // Uncomment if necessary to correct motor directions:
  //motors.flipLeftMotor(true);
  //motors.flipRightMotor(true);
   Serial1.begin(9600);

  lineSensors.initFiveSensors();
  proxSensors.initThreeSensors();
  calibrateSensors();
  waitForCommand();
}

void loop()
{
  lineSensors.read(lineSensorValues);
  proxSensors.read();
   uint8_t leftValue = proxSensors.countsFrontWithLeftLeds();
  uint8_t rightValue = proxSensors.countsFrontWithRightLeds();
  bool objectSeen = leftValue >= sensorThreshold || rightValue >= sensorThreshold;

  if (objectSeen)
  {
    // An object is visible, so we will start decelerating in
    // order to help the robot find the object without
    // overshooting or oscillating.
    motors.setSpeeds(0,0);
        ledYellow(1);
    buzzer.playNote(NOTE_G(3), 500, 15);
  }
  else
  {
    // An object is not visible, so we will accelerate in order
    // to help find the object sooner.
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
if (objectSeen)
  {
    // An object seen.
    ledYellow(1);
    buzzer.playNote(NOTE_G(3), 500, 15);
  

    lastTimeObjectSeen = millis();

  }
  if (lineSensorValues[0] > QTR_THRESHOLD)
  {
    waitForCommand();
    // If leftmost sensor detects line, reverse and turn to the
    // right.
    //motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    //delay(TURN_DURATION);
    //motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
  else if (lineSensorValues[NUM_SENSORS - 1] > QTR_THRESHOLD)
  {
    waitForCommand();
    // If rightmost sensor detects line, reverse and turn to the
    // left.
    //motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    //delay(TURN_DURATION);
    //motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
  else if(lineSensorValues[NUM_SENSORS - 2] > QTR_THRESHOLD)
  {
    waitForCommand();
  }
  else
  {
    // Otherwise, go straight.
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    
    
  }
}
