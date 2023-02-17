#include <Arduino.h>
#include <MyBLEservice.h>
#include <configService.h>
#include <WiFi.h>

void setup()
{
	// put your setup code here, to run once:
	Serial.begin(115200);
	Serial.println("Starting config work!");
	SharedData::config_load();

	Serial.println("Starting BLE work!");
	start_server();

	Serial.println("Done!");
	delay(10000);

	WiFi.mode(WIFI_STA);
	WiFi.begin(SharedData::ssid, SharedData::password);
}

void loop()
{
	if (WiFi.waitForConnectResult() != WL_CONNECTED)
	{
		WiFi.disconnect();
		WiFi.begin(SharedData::ssid, SharedData::password);
		Serial.println("Connection Failed! Rebooting...");
	}
	else
	{
		Serial.println("Connected!");
	}
	delay(5000);

	// put your main code here, to run repeatedly:
}
