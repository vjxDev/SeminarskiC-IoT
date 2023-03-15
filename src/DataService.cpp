#include "DataService.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>

CmdProc::ComandProcesor cmd_proc = CmdProc::ComandProcesor();

#define SPIFFS_MOUNT_ERROR -21
#define CONFIG_FILE_LOAD_ERROR -31
#define CONFIG_FILE_SAVE_ERROR -32
#define CONFIG_FILE_SERIALIZE_ERROR -41

char DataService::ssid[DATA_SSID_LEN];
char DataService::password[DATA_PASSWORD_LEN];
bool DataService::testBool;
int DataService::testInt;
float DataService::testFloat;
int DataService::dataTest;
ByteArray DataService::testByteArray = ByteArray(512);

int (*DataService::saveCallback)(DynamicJsonDocument *doc);
int (*DataService::loadCallback)(DynamicJsonDocument *doc);

void DataService::init(int (*save)(DynamicJsonDocument *doc), int (*load)(DynamicJsonDocument *doc), int comandCount) {
    cmd_proc.init(comandCount);
    DataService::saveCallback = save;
    DataService::loadCallback = load;
}

void DataService::addCommand(char *name, CmdProc::cmdCallback cbk, bool ble) {
    cmd_proc.add(name, cbk, ble);
}

int DataService::load() {
    if (!SPIFFS.begin(true)) {
        Serial.println("Failed to mount file system");
        return SPIFFS_MOUNT_ERROR;
    }

    File file = SPIFFS.open("/config.json");

    if (!file || (file.size() == 0)) {
        Serial.println("Failed to open config file for writing");
        return CONFIG_FILE_LOAD_ERROR;
    }

    DynamicJsonDocument doc(3000);
    if (deserializeJson(doc, file)) {
        Serial.print(F("deserializeJson() failed with code"));
    } else {
        // all the data that needs to be loaded from the file
        loadCallback(&doc);
    }

    // close file
    file.close();

    return 0;
}

int DataService::save() {
    if (!SPIFFS.begin(true)) {
        Serial.println("Failed to mount file system");
        return SPIFFS_MOUNT_ERROR;
    }

    File file = SPIFFS.open("/config.json", FILE_WRITE);

    if (!file) {
        Serial.println("Failed to open config file for writing");
        return CONFIG_FILE_SAVE_ERROR;
    }
    DynamicJsonDocument doc(3000);

    // All the data that will be saved to the file
    saveCallback(&doc);

    //
    serializeJson(doc, file);

    file.close();

    String output;
    serializeJson(doc, output);
    Serial.println(output);

    return 0;
}

int DataService::printConfig() {
    if (!SPIFFS.begin(true)) {
        Serial.println("Failed to mount file system");
        return SPIFFS_MOUNT_ERROR;
    }

    File file = SPIFFS.open("/config.json");

    if (!file || (file.size() == 0)) {
        Serial.println("Failed to open config file for writing");
        return CONFIG_FILE_LOAD_ERROR;
    }

    DynamicJsonDocument doc(3000);
    if (deserializeJson(doc, file)) {
        Serial.print(F("deserializeJson() failed with code"));
    } else {
        // all the data that needs to be loaded from the file
        String output;
        serializeJson(doc, output);
        Serial.println(output);
    }

    // close file
    file.close();

    return 0;
}

int DataService::processCommand(char *command) {
    Serial.println("DataService::processCommand");
    Serial.println(command);
    Serial.println("DataService::processCommand 2");

    return cmd_proc.Execute(command);
}
int DataService::processCommand(char *command, char *uuid) {
    return cmd_proc.Execute(command, uuid);
}
