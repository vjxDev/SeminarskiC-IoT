SerialBuf overview
===================
Arduino buffer for receiving incoming serial data

Version 2.0.2 / 18.6.2021.

The SerialBuf class implements a simple byte-array buffer of a fixed size, which stores bytes received over Serial. When reception is complete, the buffer can be read by the application. The buffer is built around a ByteArray instance, which must be created beforehand.


Modes: Binary and Text
======================

SerialBuf works in one of the two modes: binary or text, as specified by symbols SERIALBUF_BINARYMODE and SERIALBUF_TEXTMODE. 

In binary mode, SerialBuf stores any bytes received over Serial. Reception ends when nothing is received for a specified time (timeout).
In text mode, SerialBuf stores any bytes received over Serial, except LF, which is ignored, and CR, which signals the end of reception.
Afterwards, the contents of the buffer can be read by the main program.

Timeout depends on the sender - how long does the sender wait between transmissions? Values between 20 and 500ms are reasonable, but anything goes.


Usage
=====

First, include the header file and create the ByteArray and SerialBuf objects. Specify array size, mode and timeout interval. Have in mind that one 
extra byte must be allocated for the array, so that received data can be null-terminated. 
For example, if you need a buffer of 10 bytes, you must create a ByteArray of 11 bytes in size. The byte following the last received byte will always be 
set to zero automatically by the buffer. 

    #include "serialbuf.h"
    
    ByteArray barr(11);   // 10 chars for buffer, 1 char for null-termination of strings
    SerialBuf sbuf(barr, SERIALBUF_TEXTMODE, 2000);
  
  
Then, in loop(), make sure to call SerialBuffer::loop often enough so that it doesn't miss incoming serial data. 
Check SerialBuffer::isAvailable() to determine if there is a message  waiting to be processed in the buffer. 
Determine message length by calling SerialBuffer::getLength(). 
Access the message contents by either calling SerialBuffer::read() or by directly accessing the array pointed to by SerialBuffer::getBuffer().
If a buffer overrun occurs, SerialBuf::isOverflow() will return true and subsequent input will be ignored.

    sbuf.loop();

    if (sbuf.isAvailable()) {
      Serial.print(sbuf.getLength());
      Serial.println(F(" bytes available in buffer"));
      if (sbuf.isOverflow()) {
        Serial.println(F("Overflow occurred."));
      }
      Serial.println(sbuf.buffer);    // buffer automatically null-terminates incoming data, so you can print
    }
  
Note that the buffer can only be read once and forward, byte by byte, using SerialBuf::read(). 
However, you can access its contents via SerialBuf::getBuffer() anytime. 


## A note about the ByteArray instance

SerialBuf relies on the ByteArray class. It implements a fixed-length char buffer which can append characters one at a time.
The working version of the ByteArray class is included in this project.


