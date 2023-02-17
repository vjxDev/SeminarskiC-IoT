#include <Arduino.h>
#include <MyBLEservice.h>
#include <BLEDevice.h>
#include <configService.h>

// init BLE

BLEServer *pServer = NULL;
BLEService *pService = NULL;
BLECharacteristic *pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t value = 0;

#define UUID_SERVICE_WIFI "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define UUID_WIFI_SSID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define UUID_WIFI_PASSWORD "beb5483e-36e1-4688-b7f5-ea07361b26a9"

class MyServerCallbacks : public BLEServerCallbacks
{
	void onConnect(BLEServer *pServer)
	{
		deviceConnected = true;
	}

	void onDisconnect(BLEServer *pServer)
	{
		deviceConnected = false;
	}
};

class BLECharCallBacks : public BLECharacteristicCallbacks
{
	void onRead(BLECharacteristic *pCharacteristic)
	{
	}
	void onWrite(BLECharacteristic *pCharacteristic)
	{
		if (pCharacteristic->getUUID().equals(BLEUUID(UUID_WIFI_PASSWORD)))
		{
			Serial.println("Password");
			// get the value and save it to SharedData
			std::string value = pCharacteristic->getValue();
			if (value.length() > 0)
			{
				SharedData::set_password((char *)value.c_str());
				SharedData::config_save();
			}
		}
		else if (pCharacteristic->getUUID().equals(BLEUUID(UUID_WIFI_SSID)))
		{
			// get the value and save it to SharedData
			Serial.println("SSID");

			std::string value = pCharacteristic->getValue();
			if (value.length() > 0)
			{
				SharedData::set_ssid((char *)value.c_str());
				SharedData::config_save();
			}
		}
		else
		{
			Serial.println("Unknown");
		}
	}
};

void start_server()
{
	BLEService *pServiceWifi;
	BLECharacteristic *pCharWifiPassword;
	BLECharacteristic *pCharWifiSSID;

	BLEDevice::init("ESP32 BLE Server");

	pServer = BLEDevice::createServer();
	pServer->setCallbacks(new MyServerCallbacks());

	pServiceWifi = pServer->createService(UUID_SERVICE_WIFI);

	pCharWifiSSID = pServiceWifi->createCharacteristic(
		UUID_WIFI_SSID,
		BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);
	pCharWifiSSID->setCallbacks(new BLECharCallBacks());
	pCharWifiSSID->setValue(SharedData::ssid);

	pCharWifiPassword = pServiceWifi->createCharacteristic(
		UUID_WIFI_PASSWORD,
		BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);
	pCharWifiPassword->setCallbacks(new BLECharCallBacks());
	pCharWifiPassword->setValue(SharedData::password);

		pServiceWifi->start();

	// Pre nego što se počne emitovati BLE signal, potrebno je da se uključi i BLE advertising
	// On u sebi sadrži ime, SSID i informacije o odbranim UUID-ovima servisa ⤵️
	BLEAdvertising *pAdvertising = pServer->getAdvertising();
	pAdvertising->addServiceUUID(UUID_SERVICE_WIFI);
	pAdvertising->setScanResponse(true);
	pAdvertising->setMinPreferred(0x06); // rešenje za iPhone korisnike
	pAdvertising->setMinPreferred(0x12);
	BLEDevice::startAdvertising();
}
