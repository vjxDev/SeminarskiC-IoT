#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEProvider.h>
#include <DataService.h>

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

        int error = DataService::processCommand(uuid, comand);
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

class BLEServ {
   public:
    BLEService *service;
    char uuid[37] = {};
    BLEChar chars[10] = {};
    int count = 0;
    int max;

    void init(char *uuid, int max) {
        this->max = max;
        strcpy(this->uuid, uuid);
        service = pServer->createService(BLEUUID(uuid));
    }

    void addChar(char *uuid) {
        if (count < max) {
            BLEChar *ch = &chars[count];
            strcpy(ch->uuid, uuid);
            ch->charac = service->createCharacteristic(uuid, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
            ch->charac->setCallbacks(new BLECharCallBacks());
            count++;
        }
    }
    void setValue(char *uuid, char *value) {
        for (int i = 0; i < count; i++) {
            if (strcmp(chars[i].uuid, uuid) == 0) {
                chars[i].charac->setValue(value);
                return;
            }
        }
    }
};
BLEServ service;

void BLEProvider::init() {
    BLEDevice::init("ESP32 BLE Server");

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    service.init(UUID_SERVICE_WIFI, 10);

    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->addServiceUUID(UUID_SERVICE_WIFI);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);  // rešenje za iPhone korisnike
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
}

void BLEProvider::addCharacteristic(char *uuid) {
    service.addChar(uuid);
}
void BLEProvider::setValue(char *uuid, char *value) {
    service.setValue(uuid, value);
}
void BLEProvider::turnOnService() {
    service.service->start();
}

void BLEProvider::turnOff() {
    BLEDevice::deinit(true);
}
