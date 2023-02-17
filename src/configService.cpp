#include <Arduino.h>
#include "configService.h"
#include <SPIFFS.h>
#include <ArduinoJson.h>

#define SPIFFS_MOUNT_ERROR -21
#define CONFIG_FILE_LOAD_ERROR -31
#define CONFIG_FILE_SAVE_ERROR -32
#define CONFIG_FILE_SERIALIZE_ERROR -41

char SharedData::ssid[DATA_SSID_LEN];
char SharedData::password[DATA_PASSWORD_LEN];

void SharedData::set_ssid(char *ssid)
{
	strncpy(SharedData::ssid, ssid, DATA_SSID_LEN);
}

void SharedData::set_password(char *password)
{
	strncpy(SharedData::password, password, DATA_PASSWORD_LEN);
}

int SharedData::config_load()
{
	if (!SPIFFS.begin(true))
	{
		Serial.println("Failed to mount file system");
		return SPIFFS_MOUNT_ERROR;
	}

	File file = SPIFFS.open("/config.json");

	if (!file || (file.size() == 0))
	{
		Serial.println("Failed to open config file for writing");
		return CONFIG_FILE_LOAD_ERROR;
	}

	DynamicJsonDocument doc(300);

	if (deserializeJson(doc, file))
	{
		Serial.print(F("deserializeJson() failed with code"));
	}
	else
	{

		strncpy(SharedData::ssid, doc["ssid"] | "", DATA_SSID_LEN);
		strncpy(SharedData::password, doc["password"] | "", DATA_PASSWORD_LEN);
	}

	// close file
	file.close();

	return 0;
}

int SharedData::config_save()
{
	if (!SPIFFS.begin(true))
	{
		Serial.println("Failed to mount file system");
		return SPIFFS_MOUNT_ERROR;
	}

	File file = SPIFFS.open("/config.json", FILE_WRITE);

	if (!file)
	{
		Serial.println("Failed to open config file for writing");
		return CONFIG_FILE_SAVE_ERROR;
	}

	DynamicJsonDocument doc(300);
	doc["ssid"] = SharedData::ssid;
	doc["password"] = SharedData::password;

	serializeJson(doc, file);

	file.close();

	String output;
	serializeJson(doc, output);
	Serial.println(output);

	return 0;
}