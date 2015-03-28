// Used to write PWM signals (from Paul Stoffregen)
#include "TimerOne.h"
// Used for pin change interrupts
#include <PinChangeInt.h>
// Contains blimp channel configuration
#include "localdefs.h"

// These macros define the range of the 
// PWM signals that the radio receives and
// that the servos are sent.
#define PWM_MIN 900
#define PWM_MAX 2100

// Holds the most recent rising-edge times
volatile int throttlePrevTime = 0;
volatile int rudderPrevTime = 0;
volatile int elevatorPrevTime = 0;

// Holds the most recent times
volatile int throttleTime = 0;
volatile int rudderTime = 0;
volatile int elevatorTime = 0;

inline void writeThrottle(int mus) {
    analogWrite(FRONT_PROPELLER, map(mus, PWM_MIN, PWM_MAX, 0, 255));
    analogWrite(REAR_PROPELLER, map(mus, PWM_MIN, PWM_MAX, 0, 255));
}

inline void writeRudder(int mus) {
    Timer1.setPwmDuty(RUDDER_SERVO, map(mus, PWM_MIN, PWM_MAX, 51, 102));
}

inline void writeElevator(int mus) {
    Timer1.setPwmDuty(ELEVATOR_SERVO, map(mus, PWM_MIN, PWM_MAX, 51, 102));
}

// Amplifies the PWM signal stored in 'time'
// by the 100-based 'gain' parameter.
// The offset parameter is applied after
// the amplification.
int amplify(int time, int gain, int offset) {
  // start by mapping the time to a percentage power scale
  int pct = map(time, PWM_MIN, PWM_MAX, 0, 100);
  // Now, multiply by the gain and map it back to a percentage power scale
  int outPct = map(constrain(pct * gain, 0, 10000), 0, 10000, 0, 100);
  // Now, add the offset
  outPct = constrain(outPct + offset, 0, 100);
  // Now, map it back to a PWM scale
  int outTime = map(outPct, 0, 100, PWM_MIN, PWM_MAX);
  return outTime;
}

// Rising-edge interrupt handler
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
        case IN_THROTTLE: throttleTime = micros() - throttlePrevTime;
                          throttleTime = amplify(throttleTime, THROTTLE_GAIN, THROTTLE_OFFSET);
                          writeThrottle(throttleTime);
                          break;
        case IN_RUDDER:   rudderTime = micros() - rudderPrevTime;
                          rudderTime = amplify(rudderTime, RUDDER_GAIN, RUDDER_OFFSET);
                          writeRudder(rudderTime);
                          break;
        case IN_ELEVATOR: elevatorTime = micros() - elevatorPrevTime;
                          elevatorTime = amplify(elevatorTime, ELEVATOR_GAIN, ELEVATOR_OFFSET);
                          writeElevator(elevatorTime);
                          break;
    }
}

// Initialize the program
void setup() {
  // initialize output servos
  pinMode(FRONT_PROPELLER_DIR, OUTPUT);
  pinMode(REAR_PROPELLER_DIR, OUTPUT);
  digitalWrite(FRONT_PROPELLER_DIR, LOW);
  digitalWrite(REAR_PROPELLER_DIR, LOW);
  Timer1.initialize(20000);
  Timer1.pwm(RUDDER_SERVO, 77);
  Timer1.pwm(ELEVATOR_SERVO, 77);

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
