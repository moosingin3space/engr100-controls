// Prevents double-definition issues
#pragma once

// Input channels for each control
#define IN_THROTTLE A1
#define IN_RUDDER A2
#define IN_ELEVATOR A0
#define IN_BRAKE A5

// Output channels for each control
#define FRONT_PROPELLER 3
#define REAR_PROPELLER 11
#define RUDDER_SERVO 9
#define ELEVATOR_SERVO 10

// Direction channels
#define FRONT_PROPELLER_DIR 4
#define REAR_PROPELLER_DIR 7

// This gain enables the full range-of-motion of a servo without the need for
// the trim-tabs.
#define SERVO_FULL_RANGE_OF_MOTION 162

// Amplifier gains
#define THROTTLE_GAIN 100
#define RUDDER_GAIN 100
#define ELEVATOR_GAIN 100

// Amplifier offsets
#define THROTTLE_OFFSET 0
#define RUDDER_OFFSET 0
#define ELEVATOR_OFFSET 0
