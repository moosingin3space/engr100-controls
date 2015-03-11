#include <Servo.h>

#define SERVO_PORT 7
#define SERVO_RECEIVER_IN 4

Servo servo;

void setup() {
  // put your setup code here, to run once:
  servo.attach(SERVO_PORT);
  pinMode(SERVO_RECEIVER_IN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  int pwm_value = pulseIn(SERVO_RECEIVER_IN, HIGH);
  Serial.println(pwm_value);
  servo.writeMicroseconds(pwm_value);
}
