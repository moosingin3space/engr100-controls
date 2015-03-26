// Prevents double-definition issues
#pragma once

#include "Arduino.h"

#define SERVO_PERIOD 20000

void timers_init_servo_pwm() {
    TCCR1B = _BV(WGM13);
    TCCR1A = 0;
    ICR1 = SERVO_PERIOD;
    TCCR1A = 0;
    TCCR1B = _BV(WGM13) | _BV(CS11);

    TCCR2A = 0;
    TCCR2B = _BV(WGM20) | _BV(CS11);
}

void timers_set_pwm(int pin, unsigned int dutyCycle) {
    unsigned long newPeriod = (SERVO_PERIOD * dutyCycle) >> 10;
    if (pin == 9) { // Timer 1 pin A
        OCR1A = dutyCycle;
    } else if (pin == 10) { // Timer 1 pin B
        OCR1B = dutyCycle;
    } else if (pin == 3) { // Timer 2 pin A
        OCR2A = dutyCycle;
    } else if (pin == 11) { // Timer 2 pin B
        OCR2B = dutyCycle;
    }
}

void timers_enable_pwm(int pin, unsigned int dutyCycle) {
    pinMode(pin, OUTPUT);
    if (pin == 9) { // Timer 1 pin A
        TCCR1A |= _BV(COM1A1);
    } else if (pin == 10) { // Timer 1 pin B
        TCCR1A |= _BV(COM1B1);
    } else if (pin == 3) { // Timer 2 pin A
        TCCR2A |= _BV(COM2A1);
    } else if (pin == 11) { // Timer 2 pin B
        TCCR2A |= _BV(COM2B1);
    }
    timers_set_pwm(pin, dutyCycle);
    TCCR1B = _BV(WGM13) | _BV(CS11);
    TCCR2B = _BV(WGM20) | _BV(CS11);
}
