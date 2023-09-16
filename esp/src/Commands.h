#ifndef __COMAMANDS_H__
#define __COMAMANDS_H__

#include <ArduinoJson.h>

#define DATA_SSID_LEN 33
#define DATA_PASSWORD_LEN 33

class Commands {
   public:
    static void init();
    static void loop();
    static int saveCallback(DynamicJsonDocument *doc);

    static int loadCallback(DynamicJsonDocument *doc);
};

#endif