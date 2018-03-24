#include "Config.h"

Config::Config() {
}

Config::~Config() {
}

Config& Config::getInstance() {
    static Config instance;
    return instance;
}

bool Config::isConfigured() {
    return ssid_.length() > 0;
}

void Config::setConfig(const String& ssid, const String& pass) {
    ssid_ = ssid;
    pass_ = pass;
}

void Config::clearConfig() {
    ssid_ = "";
    pass_ = "";
}

const String& Config::getSsid() const {
    return ssid_;
}

const String& Config::getPass() const {
    return pass_;
}
