#include <BLEProvider.h>
#include <CommandService.h>
#include <DataService.h>

int configCommand(char *s) {
    if (strcmp(s, "save") == 0) {
        DataService::save();
        return 0;
    } else if (strcmp(s, "load") == 0) {
        DataService::load();
        return 0;
    }
    return DataService::printConfig();
}

int ssidBLEcommand(char *s) {
    DataService::setSsid(s);
    return 0;
}
int passwordBLEcommand(char *s) {
    DataService::setPassword(s);
    return 0;
}

int serialWiFiCommands(char *s) {
    // set ssid, set password, connect, disconnect, status
    char *pch;
    pch = strtok(s, " ");
    if (pch == NULL) return 0;
    if (strcmp(pch, "setPassword") == 0) {
        pch = strtok(NULL, "");
        if (pch == NULL) return 0;
        DataService::setPassword(pch);
    } else if (strcmp(pch, "setSSID") == 0) {
        pch = strtok(NULL, "");
        if (pch == NULL) return 0;
        DataService::setSsid(pch);
    } else if (strcmp(pch, "connect") == 0) {
        Serial.println("Connecting to WiFi");
    } else if (strcmp(pch, "disconnect") == 0) {
        Serial.println("Disconnecting from WiFi");
    } else if (strcmp(pch, "status") == 0) {
        Serial.println("WiFi status");
        Serial.print("SSID: ");
        Serial.println(DataService::ssid);
        Serial.print("Password: ");
        Serial.println(DataService::password);
    }

    return 0;
}

void registerCMDComands() {
    DataService::addCommand("config", configCommand);
    DataService::addCommand("wifi", serialWiFiCommands);
    DataService::addCommand(UUID_WIFI_SSID, ssidBLEcommand, true);
    DataService::addCommand(UUID_WIFI_PASSWORD, passwordBLEcommand, true);
}
