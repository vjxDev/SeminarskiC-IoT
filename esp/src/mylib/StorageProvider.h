#ifndef __STORAGE_SERVICE_H__
#define __STORAGE_SERVICE_H__
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ByteArray.h>
#include <mylib/cmdProc.h>

class StorageProvider {
   public:
    // Global data

    // create a static method that will accept a callback function with propery DynamicJsonDocument doc
    static int (*saveCallback)(DynamicJsonDocument *doc);
    static int (*loadCallback)(DynamicJsonDocument *doc);

    static void init(int (*save)(DynamicJsonDocument *doc), int (*load)(DynamicJsonDocument *doc));
    static int save();
    static int load();
    static int printConfig();
};
#endif