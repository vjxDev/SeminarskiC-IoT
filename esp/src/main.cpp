#include <Arduino.h>
#include <ArduinoJson.h>
#include <BLEProvider.h>
#include <CommandService.h>
#include <DataService.h>
#include <WiFi.h>
// #define BLUETOOTH_SERIAL_IS_NEEADED
#include <SerialProvider.h>
// #include <serialbuf.h>

// BluetoothSerial ESP_BT;

// create a callback function for the command that will proces the wifi ssid. It needs to set the dataservice ssid

int saveCallback(DynamicJsonDocument *doc) {
    (*doc)["ssid"] = DataService::ssid;
    (*doc)["password"] = DataService::password;
    (*doc)["testBool"] = DataService::testBool;
    (*doc)["testInt"] = DataService::testInt;
    (*doc)["testFloat"] = DataService::testFloat;
    (*doc)["dataTest"] = DataService::dataTest;
    return 0;
}

int loadCallback(DynamicJsonDocument *doc) {
    strncpy(DataService::ssid, (*doc)["ssid"], DATA_SSID_LEN);
    strncpy(DataService::password, (*doc)["password"], DATA_PASSWORD_LEN);
    DataService::testBool = (*doc)["testBool"];
    DataService::testInt = (*doc)["testInt"];
    DataService::testFloat = (*doc)["testFloat"];
    DataService::dataTest = (*doc)["dataTest"];

    return 0;
}

int comandWiFiCallback(char *s) {
    Serial.println(s);
    strncpy(DataService::ssid, s, DATA_SSID_LEN);
    return 0;
}
int comandHelloCallback(char *s) {
    Serial.println("hi");
    return 0;
}
int comandSaveBoolenToDataService(char *s) {
    // write a function that will take the  *char s and give it to the data service
    bool yes = strcmp(s, "true") == 0;
    bool no = strcmp(s, "false") == 0;
    if (yes || no) {
        if (yes) {
            DataService::testBool = true;
        } else {
            DataService::testBool = false;
        }
        return 0;
    }

    Serial.println("Error: not a boolean");

    return -1;
}

int configCallBack(char *s) {
    if (strcmp(s, "save") == 0) {
        DataService::save();
        return 0;
    } else if (strcmp(s, "load") == 0) {
        DataService::load();
        return 0;
    }
    return DataService::printConfig();
}

void setup() {
    Serial.begin(115200);
    Serial.println("setup");
    DataService::init(saveCallback, loadCallback, 15);
    Serial.println("DataService::init");
    BLEProvider::init();
    Serial.println("BLEProvider::init");
    SerialProvider::init();
    Serial.println("SerialProvider::init");

    DataService::addCommand(UUID_WIFI_SSID, comandWiFiCallback, true);

    Serial.println("DataService::addCommand");
    BLEProvider::turnOnService();
    Serial.println("BLEProvider::start");

    DataService::load();
    Serial.println("DataService::load");
    Serial.println(ESP.getMaxAllocHeap());

    // delay(10000);

    // WiFi.mode(WIFI_STA);
    // WiFi.begin(DataService::ssid, DataService::password);
}

void loop() {
    SerialProvider::checkSerial();

    // if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    //     WiFi.disconnect();
    //     WiFi.begin(DataService::ssid, DataService::password);
    //     Serial.println("Connection Failed! Rebooting...");
    // } else {
    //     Serial.println("Connected!");
    // }
    // delay(5000);
}
