#include "Config.h"
#include <FS.h>
#include <ArduinoJson.h>

Config::Config() {
}

Config::~Config() {
}

bool Config::loadConfig(const String& path) {
    Serial.println("Config: loading configuration from " + path + " file");

    File file = SPIFFS.open(path, "r");
    if (!file) {
        Serial.println("Config: can't open configuration file for reading");
        return false;
    }

    DynamicJsonBuffer jsonBuffer(file.size());
    JsonObject &config = jsonBuffer.parseObject(file);
    file.close();

    if (!config.success()) {
        Serial.println("Config: failed to parse configuration file");
        return false;
    }

    ssid_ = config.get<String>("ssid");
    pass_ = config.get<String>("pass");

    Serial.println("Config: configuration loaded successfully");
    return true;
}

bool Config::saveConfig(const String& path) {
    Serial.println("Config: saving configuration to " + path + " file");

    File file = SPIFFS.open(path, "w");
    if (!file) {
        Serial.println("Config: can't open configuration file for writing");
        return false;
    }

    DynamicJsonBuffer jsonBuffer;
    JsonObject &config = jsonBuffer.createObject();

    config["ssid"] = ssid_;
    config["pass"] = pass_;

    if (!config.printTo(file)) {
        Serial.println("Config: failed to print configuration to file");
        file.close();
        return false;
    }

    file.close();

    Serial.println("Config: configuration saved successfully");
    return true;
}

Config& Config::getInstance() {
    static Config instance;
    return instance;
}

bool Config::isConfigured() {
    return ssid_.length() > 0;
}

void Config::setSsid(const String& ssid) {
    ssid_ = ssid;
}
void Config::setPass(const String& pass) {
    pass_ = pass;
}

void Config::clearConfig() {
    setSsid("");
    setPass("");
}

const String& Config::getSsid() const {
    return ssid_;
}

const String& Config::getPass() const {
    return pass_;
}
