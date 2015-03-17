#pragma once

#define NUM_INPUT_CHANNELS 3
#define NUM_OUTPUT_CHANNELS 4

#define INPUT_CHANNEL_OFFSET 2
#define OUTPUT_CHANNEL_OFFSET 6

#define SERVO_GAIN 162

int numOutputChans[] = { 2, 1, 1 };
int *outputChans[] = {
    (int[]) { 0, 1 },
    (int[]) { 2 },
    (int[]) { 3 }
};

int gains[] = { 100, 100, 100 };
int offsets[] = { 0, 0, 0 };
