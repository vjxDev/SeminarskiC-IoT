#include <ByteArray.h>
#include <DataService.h>
#include <SerialProvider.h>
#include <serialbuf.h>

#ifdef BLUETOOTH_SERIAL_IS_NEEADED
#include <BluetoothSerial.h>
BluetoothSerial SerialBT;

void bluetoothSerialStart() {
    SerialBT.begin("ESP32 JJ");
    Serial.println("Bluetooth device is ready to pair");
}
void bluetoothSerialStop() {
    SerialBT.end();
    Serial.println("Bluetooth device is stopped");
}

ByteArray bluetoothSerialByteArray(24);
SerialBuf bluetoothBuffer = SerialBuf(bluetoothSerialByteArray, SERIALBUF_TEXTMODE, BLUETOOTHSERIAL);

#endif

ByteArray usbSerialByteArray = ByteArray(600);
SerialBuf usbBuffer = SerialBuf(usbSerialByteArray, SERIALBUF_TEXTMODE);

bool SerialProvider::isBluetoothSerialStarted = false;
void SerialProvider::init(bool isBle) {
    isBluetoothSerialStarted = isBle;
    if (isBluetoothSerialStarted) {
        SerialProvider::startBluetoothSerial();
    }
}

void SerialProvider::checkSerial() {
    usbBuffer.loop();
    if (usbBuffer.isAvailable()) {
        char *s = usbBuffer.getBuffer();
        proces(s);
        usbBuffer.clear();
    }
    if (isBluetoothSerialStarted) {
#ifdef BLUETOOTH_SERIAL_IS_NEEADED

        bluetoothBuffer.loop();
        if (bluetoothBuffer.isAvailable()) {
            char *s = bluetoothBuffer.getBuffer();
            proces(s);
        }
#endif
    }
}

void SerialProvider::proces(char *s) {
    DataService::processCommand(s);
}

void SerialProvider::startBluetoothSerial() {
#ifdef BLUETOOTH_SERIAL_IS_NEEADED
    bluetoothSerialStart();
    isBluetoothSerialStarted = true;
#endif
}

void SerialProvider::stopBluetoothSerial() {
#ifdef BLUETOOTH_SERIAL_IS_NEEADED
    bluetoothSerialStop();
    isBluetoothSerialStarted = false;
#endif
}