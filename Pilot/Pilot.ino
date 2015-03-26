// Used to write PWM signals
#include <Servo.h>
// Contains blimp channel configuration
#include "localdefs.h"

// These macros define the range of the 
// PWM signals that the radio receives and
// that the servos are sent.
#define PWM_MIN 900
#define PWM_MAX 2100

// Define all outputs
Servo frontPropeller;
Servo rearPropeller;
Servo rudderServo;
Servo elevatorServo;

// Amplifies the PWM signal stored in 'time'
// by the 100-based 'gain' parameter.
// The offset parameter is applied after
// the amplification.
int amplify(int time, int gain, int offset) {
  // start by mapping the time to a percentage power scale
  int pct = map(time, PWM_MIN, PWM_MAX, -100, 100);
  // Now, multiply by the gain and map it back to a percentage power scale
  int outPct = map(constrain(pct * gain, -10000, 10000), -10000, 10000, -100, 100);
  // Now, add the offset
  outPct = constrain(outPct + offset, -100, 100);
  // Now, map it back to a PWM scale
  int outTime = map(outPct, -100, 100, PWM_MIN, PWM_MAX);
  return outTime;
}

// Reads a PWM signal synchronously
void readPWM(int pin) {
    return constrain(pulseIn(pin, HIGH, 50000), PWM_MIN, PWM_MAX);
}

// Initialize the program
void setup() {
  // initialize output servos
  frontPropeller.attach(FRONT_PROPELLER);
  rearPropeller.attach(REAR_PROPELLER);
  rudderServo.attach(RUDDER_SERVO);
  elevatorServo.attach(ELEVATOR_SERVO);

  // configure input channels
  pinMode(IN_THROTTLE, INPUT);
  pinMode(IN_RUDDER, INPUT);
  pinMode(IN_ELEVATOR, INPUT);
  pinMode(IN_BRAKE, INPUT);
}

// Run repeatedly
void loop() {
    // Throttle
    int throttleTime = readPWM(IN_THROTTLE);
    throttleTime = amplify(throttleTime, THROTTLE_GAIN, THROTTLE_OFFSET);
    frontPropeller.writeMicroseconds(throttleTime);
    rearPropeller.writeMicroseconds(throttleTime);

    // Rudder
    int rudderTime = readPWM(IN_RUDDER);
    rudderTime = amplify(rudderTime, RUDDER_GAIN, RUDDER_OFFSET);
    rudderServo.writeMicroseconds(rudderTime);

    // Elevator
    int elevatorTime = readPWM(IN_ELEVATOR);
    elevatorTime = amplify(elevatorTime, ELEVATOR_GAIN, ELEVATOR_OFFSET);
    elevatorServo.writeMicroseconds(elevatorTime);
} 
