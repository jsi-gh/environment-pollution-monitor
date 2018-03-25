#pragma once
#include <Arduino.h>

#define DEFAULT_CONFIG_PATH "/config.json"

class Config
{
public:
    ~Config();

    static Config& getInstance();

    bool isConfigured();

    const String& getSsid() const;
    const String& getPass() const;
    void setSsid(const String& ssid);
    void setPass(const String& pass);

    bool loadConfig(const String& path=DEFAULT_CONFIG_PATH);
    bool saveConfig(const String& path=DEFAULT_CONFIG_PATH);
    void clearConfig();

private:
    Config();

    String ssid_;
    String pass_;
};
