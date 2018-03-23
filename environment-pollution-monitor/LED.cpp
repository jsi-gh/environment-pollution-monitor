#include "LED.h"

LED::LED(uint8_t pin) : pin_(pin) {
    pinMode(pin_, OUTPUT);
}

LED::~LED() {
    off();
}

void LED::on() {
    digitalWrite(pin_, LOW);
}

void LED::off() {
    digitalWrite(pin_, HIGH);
}

void LED::blink(unsigned short count, unsigned short delayTime) {
    while (count-- > 0) {
        on();
        delay(delayTime);
        off();
        delay(delayTime);
    }
}
