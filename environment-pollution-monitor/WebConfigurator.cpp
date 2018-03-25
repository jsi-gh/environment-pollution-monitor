#include "WebConfigurator.h"
#include <Arduino.h>
#include <FS.h>
#include "Config.h"

#define WEB_SERVER_PORT 80

#define ROOT_URL                   "/"
#define SAVE_CONFIG_URL            "/save/"
#define CONFIG_HTM_PATH            "/web_configurator.html"
#define CONFIG_SAVED_HTM_PATH      "/web_configurator_saved.html"
#define WEB_CONFIGURATOR_CSS_URL   "/web_configurator.css"
#define WEB_CONFIGURATOR_CSS_PATH  "/web_configurator.css"
#define BOOTSTRAP_CSS_URL          "/bootstrap.min.css"
#define BOOTSTRAP_CSS_PATH         "/bootstrap.min.css"

ESP8266WebServer WebConfigurator::webServer_(WEB_SERVER_PORT);
bool WebConfigurator::saved_ = false;

WebConfigurator::WebConfigurator() {
}

WebConfigurator::~WebConfigurator() {
}

WebConfigurator& WebConfigurator::getInstance() {
    static WebConfigurator instance;
    return instance;
}

void WebConfigurator::configure() {
    start();

    short shutdown_count = 500;
    while (!saved_ || shutdown_count--) {
        webServer_.handleClient();
        delay(10);
    }

    stop();
}

void WebConfigurator::start() {
    Serial.println("WebConfigurator: starting web server");

    webServer_.on(ROOT_URL, HTTP_GET, handleGetRoot);
    webServer_.on(SAVE_CONFIG_URL, HTTP_POST, handlePostSave);
    webServer_.serveStatic(BOOTSTRAP_CSS_URL, SPIFFS, BOOTSTRAP_CSS_PATH);
    webServer_.serveStatic(WEB_CONFIGURATOR_CSS_URL, SPIFFS, WEB_CONFIGURATOR_CSS_PATH);
    webServer_.begin();
    saved_ = false;

    Serial.println("WebConfigurator: web server started");
}

void WebConfigurator::stop() {
    Serial.println("WebConfigurator: stopping web server");

    webServer_.close();

    Serial.println("WebConfigurator: web server stopped");
}

void WebConfigurator::handleGetRoot() {
    Serial.println("WebConfigurator: handling " ROOT_URL " request");

    String html;
    File file = SPIFFS.open(saved_ ? CONFIG_SAVED_HTM_PATH : CONFIG_HTM_PATH, "r");

    if (file) {
        html = file.readString();

        if (saved_) {
            html.replace("{{SSID}}", Config::getInstance().getSsid());
        }
        else {
            String ssidOptions;
            int ssidCount = WiFi.scanNetworks();

            if (ssidCount != -1) {
                for (int i = 0; i < ssidCount; ++i) {
                    if (Config::getInstance().getSsid() == WiFi.SSID(i)) {
                        ssidOptions += "<option selected value = \"" + WiFi.SSID(i) + "\">" + WiFi.SSID(i) + "</option>\n";
                    }
                    else {
                        ssidOptions += "<option value = \"" + WiFi.SSID(i) + "\">" + WiFi.SSID(i) + "</option>\n";
                    }
                }
            }
            else {
                Serial.println("WebConfigurator: failed to scan available networks");
            }

            WiFi.scanDelete();
            html.replace("{{SSID_OPTIONS}}", ssidOptions);
        }
    }
    else {
        Serial.println("WebConfigurator: failed to open html file");
    }

    webServer_.send(200, "text/html", html);

    Serial.println("WebConfigurator: response sent");
}

void WebConfigurator::handlePostSave() {
    Serial.println("WebConfigurator: handling " SAVE_CONFIG_URL " request");

    char ssid[128], pass[128];
    if (sscanf(webServer_.arg("plain").c_str(), "ssid=%s\npass=%s\n", ssid, pass) == 2) {
        Serial.println("WebConfigurator: received new network configuration: ssid=" + String(ssid) + ", pass=" + String(pass));
        webServer_.sendHeader("Location", ROOT_URL);
        webServer_.send(302, "text/html");
        Config::getInstance().setSsid(ssid);
        Config::getInstance().setPass(pass);
        saved_ = true;
    }
    else {
        Serial.println("WebConfigurator: invalid request");
        webServer_.send(400, "text/plain", "Expecting two plain POST parameters: ssid and pass");
    }

    Serial.println("WebConfigurator: response sent");
}
