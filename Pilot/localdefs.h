// Prevents double-definition issues
#pragma once

// Input channels for each control
#define IN_THROTTLE 9
#define IN_RUDDER 8
#define IN_ELEVATOR 10
#define IN_BRAKE 11

// Output channels for each control
#define FRONT_PROPELLER A4
#define REAR_PROPELLER A5
#define RUDDER_SERVO A2
#define ELEVATOR_SERVO A3

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
