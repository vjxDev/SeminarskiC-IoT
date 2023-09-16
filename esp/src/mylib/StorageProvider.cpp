#include "StorageProvider.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <mylib/BLEProvider.h>

#define SPIFFS_MOUNT_ERROR -21
#define CONFIG_FILE_LOAD_ERROR -31
#define CONFIG_FILE_SAVE_ERROR -32
#define CONFIG_FILE_SERIALIZE_ERROR -41

//

//

int (*StorageProvider::saveCallback)(DynamicJsonDocument *doc);
int (*StorageProvider::loadCallback)(DynamicJsonDocument *doc);

void StorageProvider::init(int (*save)(DynamicJsonDocument *doc), int (*load)(DynamicJsonDocument *doc)) {
    StorageProvider::saveCallback = save;
    StorageProvider::loadCallback = load;
    StorageProvider::load();
}

int StorageProvider::load() {
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

int StorageProvider::save() {
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

int StorageProvider::printConfig() {
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
