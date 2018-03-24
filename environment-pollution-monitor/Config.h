#pragma once
#include <Arduino.h>

class Config
{
public:
    ~Config();

    static Config& getInstance();

    bool isConfigured();
    const String& getSsid() const;
    const String& getPass() const;

    void setConfig(const String& ssid, const String& pass);
    void clearConfig();

private:
    Config();

    String ssid_;
    String pass_;
};
