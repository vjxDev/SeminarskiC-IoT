#include <Arduino.h>
#include <ArduinoJson.h>
#include <Commands.h>
#include <WiFi.h>
#include <mylib/BLEProvider.h>
#include <mylib/CommandService.h>
#include <mylib/SerialProvider.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

TaskHandle_t serialUpdateTask = NULL;
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

    Commands::init();
    delay(5000);

    xTaskCreatePinnedToCore(serialCheck, "serialCheck", 10000, NULL, 1, &serialUpdateTask, 0);
    xTaskCreatePinnedToCore(bleCoonectionCheck, "bleCoonectionCheck", 10000, NULL, 1, &bleCoonectionCheckTask, 0);
}

void loop() {
    Commands::loop();
}
