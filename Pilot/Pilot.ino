#include <Servo.h>
#include <PinChangeInt.h>

#define SERVO_PORT 7
#define SERVO_RECEIVER_IN 4

Servo servo;

volatile int pwm_value = 0;
volatile int prev_time = 0;
uint8_t pin;

void rising() {
  pin = PCintPort::arduinoPin;
  PCintPort::attachInterrupt(pin, &falling, FALLING);
  prev_time = micros();
}

void falling() {
  pin = PCintPort::arduinoPin;
  PCintPort::attachInterrupt(pin, &rising, RISING);
  pwm_value = micros()-prev_time;
  Serial.println(pwm_value);
  servo.writeMicroseconds(pwm_value);
}

void setup() {
  // put your setup code here, to run once:
  servo.attach(SERVO_PORT);
  pinMode(SERVO_RECEIVER_IN, INPUT);
  digitalWrite(SERVO_RECEIVER_IN, HIGH);
  Serial.begin(9600);
  PCintPort::attachInterrupt(SERVO_RECEIVER_IN, &rising, RISING);
}

void loop() {
  // put your main code here, to run repeatedly:
}
