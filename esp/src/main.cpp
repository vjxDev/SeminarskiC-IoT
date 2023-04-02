#include <Arduino.h>
#include <ArduinoJson.h>
#include <BLEProvider.h>
#include <CommandService.h>
#include <DataService.h>
#include <SerialProvider.h>
#include <WiFi.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

TaskHandle_t serialCheckTask = NULL;
TaskHandle_t bleCoonectionCheckTask = NULL;

void serialCheck(void *pvParameters) {
    for (;;) {
        SerialProvider::checkSerial();
        vTaskDelay(1);
    }
}

void bleCoonectionCheck(void *pvParameters) {
    for (;;) {
        BLEProvider::loop();
        vTaskDelay(1);
    }
}

int saveCallback(DynamicJsonDocument *doc) {
    (*doc)["ssid"] = DataService::ssid;
    (*doc)["password"] = DataService::password;

    return 0;
}

int loadCallback(DynamicJsonDocument *doc) {
    char ssid[DATA_SSID_LEN];
    char password[DATA_PASSWORD_LEN];

    strncpy(ssid, (*doc)["ssid"], DATA_SSID_LEN);
    strncpy(password, (*doc)["password"], DATA_PASSWORD_LEN);

    DataService::setSsid(ssid);
    DataService::setPassword(password);

    return 0;
}

void setup() {
    Serial.begin(115200);
    DataService::init(saveCallback, loadCallback, 15);
    BLEProvider::init();
    SerialProvider::init();
    registerCMDComands();
    BLEProvider::turnOnService();

    DataService::load();

    delay(5000);
    BLEProvider::setValue(UUID_WIFI_SSID, "test");
    Serial.println("WIFI SSID SET TO test");

    xTaskCreatePinnedToCore(serialCheck, "serialCheck", 10000, NULL, 1, &serialCheckTask, 0);
    xTaskCreatePinnedToCore(bleCoonectionCheck, "bleCoonectionCheck", 10000, NULL, 1, &bleCoonectionCheckTask, 0);
}

void loop() {
    // if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    //     WiFi.disconnect();
    //     WiFi.begin(DataService::ssid, DataService::password);
    //     Serial.println("Connection Failed! Rebooting...");
    // } else {
    //     Serial.println("Connected!");
    // }
    // delay(5000);
}
