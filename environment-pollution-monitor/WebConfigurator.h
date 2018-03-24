#pragma once
#include <ESP8266WebServer.h>

class WebConfigurator
{
public:
    ~WebConfigurator();

    static WebConfigurator& getInstance();

    void configure();

private:
    WebConfigurator();

    void start();
    void stop();

    static void handleGetRoot();
    static void handlePostSave();

    static ESP8266WebServer webServer_;
    static bool saved_;
};
