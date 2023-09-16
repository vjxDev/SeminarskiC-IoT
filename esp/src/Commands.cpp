#include "Commands.h"

#include <ArduinoJson.h>
#include <mylib/BLEProvider.h>
#include <mylib/CommandService.h>
#include <mylib/StorageProvider.h>

int configSerialCommand(char *s) {
    if (strcmp(s, "save") == 0) {
        StorageProvider::save();
        return 0;
    } else if (strcmp(s, "load") == 0) {
        StorageProvider::load();
        return 0;
    }
    return StorageProvider::printConfig();
}

char ssid[DATA_SSID_LEN];
char password[DATA_PASSWORD_LEN];

bool setSsid(char *s) {
    if (strlen(s) > DATA_SSID_LEN) {
        return false;
    }
    strncpy(ssid, s, DATA_SSID_LEN);
    BLEProvider::setValue(UUID_WIFI_SSID, s);
    return true;
}

bool setPassword(char *s) {
    if (strlen(s) > DATA_PASSWORD_LEN) {
        return false;
    }
    strncpy(password, s, DATA_PASSWORD_LEN);
    BLEProvider::setValue(UUID_WIFI_PASSWORD, s);
    return true;
}

int wifiBleSSIDcommand(char *s) {
    setSsid(s);
    return 0;
}
int wifiBlePassCommand(char *s) {
    setPassword(s);
    return 0;
}

int wifiSerialComands(char *s) {
    // set ssid, set password, connect, disconnect, status
    char *pch;
    pch = strtok(s, " ");
    if (pch == NULL) return 0;
    if (strcmp(pch, "setPassword") == 0) {
        pch = strtok(NULL, "");
        if (pch == NULL) return 0;
        setPassword(pch);

    } else if (strcmp(pch, "setSSID") == 0) {
        pch = strtok(NULL, "");
        if (pch == NULL) return 0;
        setSsid(pch);

    } else if (strcmp(pch, "connect") == 0) {
        Serial.println("Connecting to WiFi");

    } else if (strcmp(pch, "disconnect") == 0) {
        Serial.println("Disconnecting from WiFi");

    } else if (strcmp(pch, "status") == 0) {
        Serial.println("WiFi status");
        Serial.print("SSID: ");
        Serial.println(ssid);
        Serial.print("Password: ");
        Serial.println(password);
    }

    return 0;
}

int Commands::saveCallback(DynamicJsonDocument *doc) {
    (*doc)["ssid"] = ssid;
    (*doc)["password"] = password;

    return 0;
}

int Commands::loadCallback(DynamicJsonDocument *doc) {
    char ssid[DATA_SSID_LEN];
    char password[DATA_PASSWORD_LEN];

    strncpy(ssid, (*doc)["ssid"], DATA_SSID_LEN);
    strncpy(password, (*doc)["password"], DATA_PASSWORD_LEN);

    setSsid(ssid);
    setPassword(password);

    return 0;
}

void Commands::init() {
    CommandService::init(20);
    StorageProvider::init(saveCallback, loadCallback);

    CommandService::add("config", configSerialCommand);
    CommandService::add("wifi", wifiSerialComands);
    CommandService::add(UUID_WIFI_SSID, wifiBleSSIDcommand, true);
    CommandService::add(UUID_WIFI_PASSWORD, wifiBlePassCommand, true);
}

void Commands::loop() {
}