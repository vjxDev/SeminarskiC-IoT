#include <Arduino.h>
#include <ArduinoJson.h>
#include <Commands.h>
#include <WiFi.h>
#include <mylib/BLEProvider.h>
#include <mylib/CommandService.h>
#include <mylib/SerialProvider.h>

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

void setup() {
    BLEProvider::init();
    SerialProvider::init();

    // Add BLE and Serial comands before turning on bluetooth
    Commands::init();

    BLEProvider::turnOnService();

    delay(5000);

    // your code

    xTaskCreatePinnedToCore(serialCheck, "serialCheck", 10000, NULL, 1, &serialCheckTask, 0);
    xTaskCreatePinnedToCore(bleCoonectionCheck, "bleCoonectionCheck", 10000, NULL, 1, &bleCoonectionCheckTask, 0);
}

void loop() {
    Commands::loop();
    // if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    //     WiFi.disconnect();
    //     WiFi.begin(DataService::ssid, DataService::password);
    //     Serial.println("Connection Failed! Rebooting...");
    // } else {
    //     Serial.println("Connected!");
    // }
    // delay(5000);
}
