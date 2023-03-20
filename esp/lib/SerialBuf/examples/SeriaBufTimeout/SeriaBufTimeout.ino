#include "serialbuf.h"

ByteArray barr(11);   // 10 chars for buffer, 1 char for null-termination of strings
SerialBuf sbuf(barr, SERIALBUF_TEXTMODE, 2000);

//SerialBuf sbuf(10, SERIALBUF_TEXTMODE, 2000);

void setup() {
  Serial.begin(115200);
  // Uncomment one line, and comment the other one to try the buffer in TEXTMODE or BINARYMODE:
  //sbuf.init(10, SERIALBUF_BINARYMODE, 2000);    // NOTE: buffer acknowledges reception after 2000ms in binary mode
  //sbuf.init(10, SERIALBUF_TEXTMODE, 2000);

  Serial.print(F("SerialBuffer demo\nBuffer size is "));
  Serial.print(sbuf.getSize());
  Serial.print(F(" bytes. Reception timeout in binary mode is "));
  Serial.print(sbuf.getTimeout());
  Serial.println(F(" milliseconds. \nEnter 'text' or 'bin' to change buffer mode."));
}

void loop() {
  sbuf.loop();

  if (sbuf.isAvailable()) {
    Serial.print(sbuf.getLength());
    Serial.println(F(" bytes available in buffer"));
    if (sbuf.isOverflow()) {
      Serial.println(F("Overflow occurred."));
    }

    // We assume that any data received on Serial will be terminated with CR+LF.
    // Binary mode transmits everything, including CR+LF. Text mode ignores CR+LF.
    if (strcmp(sbuf.getBuffer(), "text\r\n") == 0) {   
      sbuf.textMode();
      Serial.println(F("Switched to text. Finish entry with CR+LF."));
      return;
    } else if (strcmp(sbuf.getBuffer(), "bin") == 0) {
      sbuf.binaryMode();
      Serial.print(F("Switched to binary. Reception timeout is ")); 
      Serial.print(sbuf.getTimeout());
      Serial.println(F(" milliseconds."));
      return;
    }

    if (sbuf.getMode() == SERIALBUF_TEXTMODE) {
      Serial.println(sbuf.getBuffer());
    }
    else
    {
      int k = sbuf.getLength();
      for (int i = 0; i < k; i++) {
        Serial.print(sbuf.read(), HEX);
        Serial.print(" ");
      }
      Serial.println();
    }
    sbuf.clear();
  }
}
