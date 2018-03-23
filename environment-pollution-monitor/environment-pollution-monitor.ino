#include "LED.h"

LED led;

void setup() {
    led.blink(50, 50);
}

void loop() {
    static bool on = true;
    on = !on;

    if (on) {
        led.off();
    }
    else {
        led.on();
    }

    delay(1000);
}
