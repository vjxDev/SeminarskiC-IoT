#ifndef __CONFIG_SERVICE_H__
#define __CONFIG_SERVICE_H__
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ByteArray.h>
#include <cmdProc.h>
#define DATA_SSID_LEN 33
#define DATA_PASSWORD_LEN 33

class DataService {
   public:
    // Global data
    static char ssid[DATA_SSID_LEN];
    static char password[DATA_PASSWORD_LEN];
    static bool testBool;
    static int testInt;
    static float testFloat;
    static ByteArray testByteArray;
    static int dataTest;
    //

    // create a static method that will accept a callback function with propery DynamicJsonDocument doc
    static int (*saveCallback)(DynamicJsonDocument *doc);
    static int (*loadCallback)(DynamicJsonDocument *doc);

    static void init(int (*save)(DynamicJsonDocument *doc), int (*load)(DynamicJsonDocument *doc), int comandCount = 10);
    static int save();
    static int load();

    static int processCommand(char *s);
    static int processCommand(char *uuid, char *s);
    static void addCommand(char *name, CmdProc::cmdCallback cbk, bool ble = false);
};
#endif