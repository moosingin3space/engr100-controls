// Used to write PWM signals
#include <Servo.h>
// Used to listen for pin-change interrupts
#include <PinChangeInt.h>
// Contains blimp-specific configuration
#include "localdefs.h"

// This macro defines the conversion between
// a pin number and the channel number
#define PIN2CHAN(x) x - INPUT_CHANNEL_OFFSET

// These macros define the range of the 
// PWM signals that the radio receives and
// that the servos are sent.
#define PWM_MIN 900
#define PWM_MAX 2100

// Each output channel is stored in this array
Servo output[NUM_OUTPUT_CHANNELS];

// The input PWM signals
volatile int input[NUM_INPUT_CHANNELS];

// The time of the rising-edge signal.
// Temporary.
volatile int prev_time[NUM_INPUT_CHANNELS];

// Amplifies the PWM signal stored in 'time'
// by the 100-based 'gain' parameter.
// The offset parameter is applied after
// the amplification.
int amplify(int gain, int offset, int time) {
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

// Writes a PWM signal to any number of output channels
void write_servo(int time, int numChans, int chans[]) {
  for (int i = 0; i < numChans; i++) {
      // get the output channel
      int chan = chans[i];
      // Write the signal using the Servo library
      output[chan].writeMicroseconds(time);
  }
}

// Called on the rising edge of a PWM signal
void rising() {
  uint8_t pin = PCintPort::arduinoPin;
  // Listen for the falling edge on this pin
  PCintPort::attachInterrupt(pin, &falling, FALLING);
  // Save the time of this event with the channel.
  prev_time[PIN2CHAN(pin)] = micros();
}

// Called on the falling edge of a PWM signal
void falling() {
  uint8_t pin = PCintPort::arduinoPin;
  // Re-register listening for the rising edge
  PCintPort::attachInterrupt(pin, &rising, RISING);
  // Using the rising-edge time, calculate the width
  input[PIN2CHAN(pin)] = micros() - prev_time[PIN2CHAN(pin)];
  // Amplify the signal using the associated gain, offset, and input
  int servo_time = amplify(gains[PIN2CHAN(pin)], offsets[PIN2CHAN(pin)], input[PIN2CHAN(pin)]);
  // Write the amplified signal to the associated output channels
  write_servo(servo_time, numOutputChans[PIN2CHAN(pin)], outputChans[PIN2CHAN(pin)]);
}

// Initialize the program
void setup() {
  // initialize output servos
  for (int i = 0; i < NUM_OUTPUT_CHANNELS; i++) {
    // Associate output pins with channels
    output[i].attach(OUTPUT_CHANNEL_OFFSET + i);
  }
  // initialize receiver inputs
  for (int i = 0; i < NUM_INPUT_CHANNELS; i++) {
    // Zero all receiver state
    input[i] = 0;
    prev_time[i] = 0;
    // Set the inputs to input mode and activate 20kohm pull-up resistors.
    // This is done because otherwise it is difficult to discern pin changes
    pinMode(INPUT_CHANNEL_OFFSET + i, INPUT);
    digitalWrite(INPUT_CHANNEL_OFFSET + i, HIGH);
    // Attach rising-edge interrupts
    PCintPort::attachInterrupt(INPUT_CHANNEL_OFFSET + i, &rising, RISING);
  }
}

// This is purposely left empty to avoid synchronization
// problems. The Interrupt Service Routines (ISRs) defined
// in 'falling()' and 'rising()' handle the entire program
// flow - code here will only serve to complicate this.
void loop() {} 
