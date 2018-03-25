#include <ESP8266WiFi.h>
#include <FS.h>

#include "LED.h"
#include "Config.h"
#include "Settings.h"
#include "WebConfigurator.h"

LED led;

void configure() {
    led.blink(5, 100);

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

    led.blink(5, 100);
}

bool connect() {
    Serial.print("connecting to WiFi network " + Config::getInstance().getSsid());

    WiFi.enableSTA(true);
    WiFi.begin(Config::getInstance().getSsid().c_str(), Config::getInstance().getPass().c_str());

    int count = 10;
    while (WiFi.status() != WL_CONNECTED && count-- > 0) {
        led.blink(10, 50);
        Serial.print(".");
    }

    if (count == -1) {
        Serial.println(" failed");
        WiFi.enableSTA(false);
        WiFi.disconnect();
        delay(1000);
        return false;
    }

    Serial.println(" done");
    return true;
}

void setup() {
    SPIFFS.begin();
    Serial.begin(9600);

    Config::getInstance().loadConfig();
    while (true) {
        if (!Config::getInstance().isConfigured()) {
            configure();
        }
        if (!connect()) {
            Config::getInstance().clearConfig();
        }
        else {
            Config::getInstance().saveConfig();
            break;
        }
    }
}

void loop() {
}
