#ifndef __MYBLESERVICE_H__
#define __MYBLESERVICE_H__

#define UUID_SERVICE_WIFI "4fafc201-1fb5-459e-8fcc-c5c9c331914b"



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