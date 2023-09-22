#include <Arduino.h>
#include <BLE2902.h>
#include <BLEDevice.h>
#include <mylib/BLEProvider.h>

#include "mylib/CommandService.h"

BLEServer *pServer = nullptr;
bool deviceConnected = false;
bool oldDeviceConnected = false;

class BLECharCallBacks : public BLECharacteristicCallbacks {
    void onRead(BLECharacteristic *pCharacteristic) {
    }
    void onWrite(BLECharacteristic *pCharacteristic) {
        char uuid[37] = {};
        char comand[512] = {};
        strcpy(uuid, pCharacteristic->getUUID().toString().c_str());
        strcpy(comand, pCharacteristic->getValue().c_str());

        int error = CommandService::process(uuid, comand);
        if (error == CMD_ERR_UNKNOWNCOMMAND) {
            Serial.println("Unknown command");
        }
    }
};

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer *pServer) {
        deviceConnected = true;
    }

    void onDisconnect(BLEServer *pServer) {
        deviceConnected = false;
    }
};

class BLEChar {
   public:
    BLECharacteristic *charac = nullptr;
    char uuid[37] = {};
};

#define MAX_NUMBER_OF_CHARS 10
#define NUMBER_OF_SERVICES 1

class BLEServ {
   public:
    BLEService *service;
    char uuid[37] = {};
    BLEChar chars[MAX_NUMBER_OF_CHARS] = {};
    int count = 0;
    int max;

    void init(char *uuid, int max) {
        this->max = max;
        strcpy(this->uuid, uuid);
        service = pServer->createService(BLEUUID(uuid), (MAX_NUMBER_OF_CHARS * 2 + NUMBER_OF_SERVICES + MAX_NUMBER_OF_CHARS));
    }

    void addChar(char *uuid) {
        if (count < max) {
            BLEChar *ch = &chars[count];
            strcpy(ch->uuid, uuid);
            ch->charac = service->createCharacteristic(uuid, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE);
            ch->charac->addDescriptor(new BLE2902());
            ch->charac->setCallbacks(new BLECharCallBacks());
            count++;
        }
    }
    void setValue(char *uuid, char *value) {
        for (int i = 0; i < count; i++) {
            if (strcmp(chars[i].uuid, uuid) == 0) {
                chars[i].charac->setValue(value);
                chars[i].charac->notify();
                return;
            }
        }
    }
};

BLEServ mainService;

void BLEProvider::init() {
    BLEDevice::init("ESP32 BLE Server");

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    mainService.init(UUID_SERVICE_WIFI, 10);

    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->addServiceUUID(UUID_SERVICE_WIFI);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);  // rešenje za iPhone korisnike
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
}

void BLEProvider::addCharacteristic(char *uuid) {
    mainService.addChar(uuid);
}
void BLEProvider::setValue(char *uuid, char *value) {
    mainService.setValue(uuid, value);
}
void BLEProvider::turnOnService() {
    mainService.service->start();
}

void BLEProvider::turnOff() {
    BLEDevice::deinit(true);
}

void BLEProvider::loop() {
    // disconnecting event
    if (deviceConnected) {
    }

    // Connecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500);
        pServer->startAdvertising();
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connect event
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}