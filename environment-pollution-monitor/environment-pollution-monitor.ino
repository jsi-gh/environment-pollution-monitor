#include <ESP8266WiFi.h>
#include <FS.h>

#include "LED.h"
#include "Config.h"
#include "Settings.h"
#include "WebConfigurator.h"

LED led;

void configure() {
    led.blink(20, 50);

    Serial.println("starting access point");
    if (WiFi.softAPConfig(LOCAL_IP, GATEWAY_IP, SUBNET_MASK) && WiFi.softAP(SSID, PASS)) {
        Serial.println("access point started");
        led.on();
        WebConfigurator::getInstance().configure();
        led.off();
    }
    else {
        Serial.println("failed to start access point");
    }
    
    Serial.println("shutting down access point");
    WiFi.enableAP(false);
    WiFi.disconnect();
    delay(1000);

    led.blink(20, 50);
}

void setup() {
    SPIFFS.begin();
    Serial.begin(9600);

    if (!Config::getInstance().isConfigured()) {
        configure();
    }
}

void loop() {
}
