#ifndef __SERIALPROVIDER_H__
#define __SERIALPROVIDER_H__

#include <cmdProc.h>

class SerialProvider {
   public:
    static bool isBluetoothSerialStarted;

    static void init(bool ble = false);
    static void checkSerial();
    static void proces(char *s);
    static void startBluetoothSerial();
    static void stopBluetoothSerial();
};

#endif  // __SERIALPROVIDER_H__