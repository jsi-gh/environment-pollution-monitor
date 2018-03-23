#pragma once
#include <Arduino.h>

#define ONBOARD_LED D4

class LED {
public:
    LED(uint8_t pin = ONBOARD_LED);
    ~LED();

    void on();
    void off();
    void blink(unsigned short count, unsigned short delayTime);

private:
    uint8_t pin_;
};
