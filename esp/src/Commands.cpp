#include "Commands.h"

#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <mylib/BLEProvider.h>
#include <mylib/CommandService.h>
#include <mylib/StorageProvider.h>

TaskHandle_t displayUpdateTask = NULL;
// Serial config

int configSerialCommand(char *s) {
    if (strcmp(s, "save") == 0) {
        BLEProvider::setValue(UUID_CONFIG, "0");
        StorageProvider::save();
        return 0;
    } else if (strcmp(s, "load") == 0) {
        BLEProvider::setValue(UUID_CONFIG, "0");
        StorageProvider::load();
        return 0;
    }
    return StorageProvider::printConfig();
}

// wifi
char ssid[DATA_SSID_LEN];
char password[DATA_PASSWORD_LEN];

bool setSsid(char *s) {
    if (strlen(s) > DATA_SSID_LEN) {
        return false;
    }
    strncpy(ssid, s, DATA_SSID_LEN);
    BLEProvider::setValue(UUID_WIFI_SSID, s);
    return true;
}
bool setPassword(char *s) {
    if (strlen(s) > DATA_PASSWORD_LEN) {
        return false;
    }
    strncpy(password, s, DATA_PASSWORD_LEN);
    BLEProvider::setValue(UUID_WIFI_PASSWORD, s);
    return true;
}
int wifiSerialComands(char *s) {
    // set ssid, set password, connect, disconnect, status
    char *pch;
    pch = strtok(s, " ");
    if (pch == NULL) return 0;
    if (strcmp(pch, "setPassword") == 0) {
        pch = strtok(NULL, "");
        if (pch == NULL) return 0;
        setPassword(pch);

    } else if (strcmp(pch, "setSSID") == 0) {
        pch = strtok(NULL, "");
        if (pch == NULL) return 0;
        setSsid(pch);

    } else if (strcmp(pch, "connect") == 0) {
        Serial.println("Connecting to WiFi");

    } else if (strcmp(pch, "disconnect") == 0) {
        Serial.println("Disconnecting from WiFi");

    } else if (strcmp(pch, "status") == 0) {
        Serial.println("WiFi status");
        Serial.print("SSID: ");
        Serial.println(ssid);
        Serial.print("Password: ");
        Serial.println(password);
    }

    return 0;
}
int wifiBleSSIDcommand(char *s) {
    setSsid(s);
    return 0;
}
int wifiBlePassCommand(char *s) {
    setPassword(s);
    return 0;
}
// Display

#define COLUMS 16              // LCD columns
#define ROWS 2                 // LCD rows
#define LCD_SPACE_SYMBOL 0x20  // space symbol from LCD ROM, see p.9 of GDM2004D datasheet

LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE);

int showMode = '1';
int prevMode = -1;
int switchCurrentMode = 0;
// 0 off
// 1 wifi
// 2 text
// 3 swith every 5 sec

char lineOne[COLUMS];
char lineTwo[COLUMS];

void initDisplay() {
    lcd.begin(COLUMS, ROWS, LCD_5x8DOTS);
}
int setline(char *s, int line) {
    if (strlen(s) > COLUMS) {
        return false;
    }
    if (line == 0) {
        strncpy(lineOne, s, COLUMS);
        BLEProvider::setValue(UUID_DISPLAY_ONE, s);

    } else if (line == 1) {
        strncpy(lineTwo, s, COLUMS);
        BLEProvider::setValue(UUID_DISPLAY_TWO, s);
    } else {
        return 0;
    }
}

int displayBleLineOneCommand(char *s) {
    setline(s, 0);
    return true;
}
int displayBleLineTwoCommand(char *s) {
    setline(s, 1);
    return true;
}
int displayBleMode(char *s) {
    showMode = (int)s[0];
    Serial.println(s[0]);
    BLEProvider::setValue(UUID_DISPLAY_MODE, s);
    return 0;
}

int displaySerialCommand(char *s) {
    char *pch;
    pch = strtok(s, " ");
    if (pch == NULL) return 0;

    if (strcmp(pch, "mode") == 0) {
        pch = strtok(NULL, "");
        if (pch == NULL) return 0;
        displayBleMode(pch);

    } else if (strcmp(pch, "lineone") == 0) {
        pch = strtok(NULL, "");
        if (pch == NULL) return 0;
        setline(pch, 0);

    } else if (strcmp(pch, "linetwo") == 0) {
        pch = strtok(NULL, "");
        if (pch == NULL) return 0;
        setline(pch, 1);
    }

    return 0;
}

void showWifi() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(ssid);
    lcd.setCursor(0, 1);
    lcd.print(password);
}
void showText() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(lineOne);
    lcd.setCursor(0, 1);
    lcd.print(lineTwo);
}
void displayUpdate(void *pvParameters) {
    for (;;) {
        if (showMode == '0') {
            lcd.displayOff();
        } else if (showMode == '1') {
            lcd.displayOn();
            showWifi();
        } else if (showMode == '2') {
            lcd.displayOn();
            showText();
        } else if (showMode == '3') {
            if (switchCurrentMode == 0) {
                switchCurrentMode = 1;
                showText();
            } else {
                switchCurrentMode = 0;
                showWifi();
            }
        }

        vTaskDelay(5000);
    }
}

int Commands::saveCallback(DynamicJsonDocument *doc) {
    (*doc)["ssid"] = ssid;
    (*doc)["password"] = password;

    return 0;
}

int Commands::loadCallback(DynamicJsonDocument *doc) {
    char ssid[DATA_SSID_LEN];
    char password[DATA_PASSWORD_LEN];

    strncpy(ssid, (*doc)["ssid"], DATA_SSID_LEN);
    strncpy(password, (*doc)["password"], DATA_PASSWORD_LEN);

    setSsid(ssid);
    setPassword(password);

    return 0;
}

void Commands::init() {
    CommandService::init(20);
    StorageProvider::init(saveCallback, loadCallback);
    initDisplay();

    CommandService::add("config", configSerialCommand);
    CommandService::add(UUID_CONFIG, configSerialCommand, true);
    CommandService::add("wifi", wifiSerialComands);
    CommandService::add(UUID_WIFI_SSID, wifiBleSSIDcommand, true);
    CommandService::add(UUID_WIFI_PASSWORD, wifiBlePassCommand, true);

    CommandService::add(UUID_DISPLAY_ONE, displayBleLineOneCommand, true);
    CommandService::add(UUID_DISPLAY_TWO, displayBleLineTwoCommand, true);
    CommandService::add(UUID_DISPLAY_MODE, displayBleMode, true);

    CommandService::add("display", displaySerialCommand);

    BLEProvider::turnOnService();
    BLEProvider::setValue(UUID_WIFI_SSID, ssid);
    BLEProvider::setValue(UUID_WIFI_PASSWORD, password);
    BLEProvider::setValue(UUID_CONFIG, "0");

    xTaskCreatePinnedToCore(displayUpdate, "displayUpdate", 10000, NULL, 1, &displayUpdateTask, 0);
}

void Commands::loop() {
}