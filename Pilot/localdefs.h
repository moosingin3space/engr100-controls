// Prevents double-definition issues
#pragma once

// The number of channels
#define NUM_INPUT_CHANNELS 3
#define NUM_OUTPUT_CHANNELS 4

// The channel-to-pin offsets.
// if INPUT_CHANNEL_OFFSET is 2, then a signal on pin 3 will be interpreted as
// a signal on channel 1.
#define INPUT_CHANNEL_OFFSET 8
#define OUTPUT_CHANNEL_OFFSET A2

// This gain enables the full range-of-motion of a servo without the need for
// the trim-tabs.
#define SERVO_FULL_RANGE_OF_MOTION 162


// Input 0 - Rear servo (rudder) position
// Input 1 - Propeller speed
// Input 2 - Front servo (elevator) position

// Mapping of the input channels to output channels
int numOutputChans[] = { 
    1, // In 0
    2, // In 1
    1  // In 2
};
int *outputChans[] = {
    (int[]) { 0 },      // In 0
    (int[]) { 2, 3 },   // In 1
    (int[]) { 1 }       // In 2
};

// Gains for each input channel
// 100-based
int gains[] = { 
    -100,                        // In 0
    100,                         // In 1
    100                          // In 2
};
// Offsets for each input channel
// Applied to a signal varying from
// 0-100
int offsets[] = { 
    0, // In 0
    0, // In 1
    0  // In 2
};
