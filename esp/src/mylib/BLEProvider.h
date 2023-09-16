#ifndef __MYBLESERVICE_H__
#define __MYBLESERVICE_H__

#define UUID_SERVICE_WIFI "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define UUID_WIFI_SSID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define UUID_WIFI_PASSWORD "beb5483e-36e1-4688-b7f5-ea07361b26a9"

class BLEProvider {
   public:
    static void init();
    static void addCharacteristic(char* uuid);
    static void turnOff();
    static void turnOnService();
    static void setValue(char* uuid, char* value);
    static void loop();
};

#endif