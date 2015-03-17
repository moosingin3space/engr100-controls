#include <Servo.h>
#include <PinChangeInt.h>
#include "localdefs.h"

#define PIN2CHAN(x) x - INPUT_CHANNEL_OFFSET
#define PWM_MIN 800
#define PWM_MAX 2100

Servo output[NUM_OUTPUT_CHANNELS];
volatile int input[NUM_INPUT_CHANNELS];
volatile int prev_time[NUM_INPUT_CHANNELS];

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

void write_servo(int time, int numChans, int chans[]) {
  for (int i = 0; i < numChans; i++) {
      int chan = chans[i];
      Serial.print(chan);
      Serial.print(": ");
      Serial.println(time);
      output[chan].writeMicroseconds(time);
  }
}

void rising() {
  uint8_t pin = PCintPort::arduinoPin;
  PCintPort::attachInterrupt(pin, &falling, FALLING);
  prev_time[PIN2CHAN(pin)] = micros();
}

void falling() {
  uint8_t pin = PCintPort::arduinoPin;
  PCintPort::attachInterrupt(pin, &rising, RISING);
  input[PIN2CHAN(pin)] = micros() - prev_time[PIN2CHAN(pin)];
  int servo_time = amplify(gains[PIN2CHAN(pin)], offsets[PIN2CHAN(pin)], input[PIN2CHAN(pin)]);
  write_servo(servo_time, numOutputChans[PIN2CHAN(pin)], outputChans[PIN2CHAN(pin)]);
}

void setup() {
  // initialize output servos
  for (int i = 0; i < NUM_OUTPUT_CHANNELS; i++) {
    output[i].attach(OUTPUT_CHANNEL_OFFSET + i);
  }
  // initialize receiver inputs
  for (int i = 0; i < NUM_INPUT_CHANNELS; i++) {
    input[i] = 0;
    prev_time[i] = 0;
    pinMode(INPUT_CHANNEL_OFFSET + i, INPUT);
    digitalWrite(INPUT_CHANNEL_OFFSET + i, HIGH);
    PCintPort::attachInterrupt(INPUT_CHANNEL_OFFSET + i, &rising, RISING);
  }
  Serial.begin(9600);
}

void loop() {} // leave this empty, do everything in interrupt handlers
