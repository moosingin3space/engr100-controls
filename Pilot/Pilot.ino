// Used to write PWM signals (from Paul Stoffregen)
#include "TimerOne.h"
// Used for pin change interrupts
#include <PinChangeInt.h>
// Contains blimp channel configuration
#include "localdefs.h"

// These macros define the range of the
// trimmed controls with the trim at 
// the center
#define TRIMMED_MIN 69
#define TRIMMED_MAX 95

// These macros define the range of the 
// trimless controls.
#define TRIMLESS_MIN 45
#define TRIMLESS_MAX 108

// This macro defines the center of the 
// servo range 
#define CENTER 77 

// These macros define the duty cycle 
// range for the throttle
#define THROTTLE_MIN 0
#define THROTTLE_MAX 255

// Holds the most recent rising-edge times
volatile int throttlePrevTime = 0;
volatile int rudderPrevTime = 0;
volatile int elevatorPrevTime = 0;

// Holds the current duty cycles
volatile int throttleDuty = 0;
volatile int rudderDuty = 0;
volatile int elevatorDuty = 0;

// Converts from pulse-width to duty cycle.
inline int pulse2duty(int pulse) {
    return (pulse*1023)/20000;
}

// Writes the microsecond value to the throttle
inline void writeThrottle(int duty) {
    analogWrite(FRONT_PROPELLER, duty);
    analogWrite(REAR_PROPELLER, duty);
}

// Writes the microsecond value to the rudder servo
inline void writeRudder(int duty) {
    Timer1.setPwmDuty(RUDDER_SERVO, duty);
}

// Writes the microsecond value to the elevator servo
inline void writeElevator(int duty) {
    Timer1.setPwmDuty(ELEVATOR_SERVO, duty);
}

// Converts a trimmed value to a trimless value (for servos)
int trimlessServo(int duty) {
    // TODO test this
    // Start with upscaling
    int upscaledDuty = map(duty, TRIMMED_MIN, TRIMMED_MAX, TRIMLESS_MIN, TRIMLESS_MAX);
    // Now constrain the output to trimless range
    return constrain(upscaledDuty, TRIMLESS_MIN, TRIMLESS_MAX);
}

// Converts a trimmed value to a trimless value (for throttle)
int trimlessThrottle(int duty) {
    // TODO test this
    // Start by upscaling
    int upscaledDuty = map(duty, TRIMLESS_MIN, TRIMLESS_MAX, THROTTLE_MIN, THROTTLE_MAX);
    // Now constrain the output to trimless range
    return constrain(upscaledDuty, THROTTLE_MIN, THROTTLE_MAX);
}

// Rising-edge interrupt handler
// Just store the times
void rising() {
    uint8_t pin = PCintPort::arduinoPin;
    PCintPort::attachInterrupt(pin, &falling, FALLING);
    switch(pin) {
        case IN_THROTTLE: throttlePrevTime = micros();
                          break;
        case IN_RUDDER:   rudderPrevTime = micros();
                          break;
        case IN_ELEVATOR: elevatorPrevTime = micros();
                          break;
    }
}

// Falling-edge interrupt handler
void falling() {
    uint8_t pin = PCintPort::arduinoPin;
    PCintPort::attachInterrupt(pin, &rising, RISING);
    switch(pin) {
        case IN_THROTTLE: throttleDuty = trimlessThrottle(micros() - throttlePrevTime); // Calculate time
                          // Amplify the signal
                          throttleDuty = trimlessThrottle(throttleDuty);
                          writeThrottle(throttleDuty);
                          break;
        case IN_RUDDER:   rudderDuty = pulse2duty(micros() - rudderPrevTime); // Calculate time
                          // Convert to a trimless signal
                          rudderDuty = trimlessServo(rudderDuty);
                          writeRudder(rudderDuty);
                          break;
        case IN_ELEVATOR: elevatorDuty = pulse2duty(micros() - elevatorPrevTime); // Calculate time
                          // Convert to a trimless signal
                          elevatorDuty = trimlessServo(elevatorDuty);
                          writeElevator(elevatorDuty);
                          break;
    }
}

// Initialize the program
void setup() {
  // initialize H-bridge
  pinMode(FRONT_PROPELLER_DIR, OUTPUT);
  pinMode(REAR_PROPELLER_DIR, OUTPUT);
  digitalWrite(FRONT_PROPELLER_DIR, LOW);
  digitalWrite(REAR_PROPELLER_DIR, LOW);

  // Initialize timer
  Timer1.initialize(20000);
  Timer1.pwm(RUDDER_SERVO, CENTER);
  Timer1.pwm(ELEVATOR_SERVO, CENTER);

  // configure input channels
  pinMode(IN_THROTTLE, INPUT);
  pinMode(IN_RUDDER, INPUT);
  pinMode(IN_ELEVATOR, INPUT);
  pinMode(IN_BRAKE, INPUT);
  // Activate 20kohm pull-up resistors
  digitalWrite(IN_THROTTLE, HIGH);
  digitalWrite(IN_RUDDER, HIGH);
  digitalWrite(IN_ELEVATOR, HIGH);
  digitalWrite(IN_BRAKE, HIGH);
  // Attach rising-edge interrupts
  PCintPort::attachInterrupt(IN_THROTTLE, &rising, RISING);
  PCintPort::attachInterrupt(IN_RUDDER, &rising, RISING);
  PCintPort::attachInterrupt(IN_ELEVATOR, &rising, RISING);
}

// Do nothing here to avoid synchronization issues
void loop() {}
