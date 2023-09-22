#ifndef __COMAMANDS_H__
#define __COMAMANDS_H__

#include <ArduinoJson.h>

#define UUID_WIFI_SSID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define UUID_WIFI_PASSWORD "beb5483e-36e1-4688-b7f5-ea07361b26a9"
#define UUID_DISPLAY_ONE "beb5483e-36e1-4688-b7f5-eaaaaaaaaaaa"
#define UUID_DISPLAY_TWO "beb5483e-36e1-4688-b7f5-eaaaaaaaaaab"
#define UUID_DISPLAY_MODE "beb5483e-36e1-4688-b7f5-eaaaaaaaaaac"

#define UUID_CONFIG "beb5483e-36e1-4688-b7f5-cccccccccccc"

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