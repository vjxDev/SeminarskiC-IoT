#include "serialbuf.h"
#include "Arduino.h"


SerialBuf::SerialBuf(ByteArray &byteArray, int Mode, uint32_t Timeout)
{
  array = &byteArray;
  maxlen = array->getSize()-1;
  //array = new ByteArray(maxlen + 1);

  this->timeout = Timeout;
  mode = Mode;                    // SERIALBUF_BINARYMODE or SERIALBUF_TEXTMODE
  clear();
}






void SerialBuf::clear()
{
  overflow = false;
  finished = false;
  position = 0;
  array->clear();

  mils = millis();
}

void SerialBuf::loop()
{
  if (finished)
  {
    return;
  }

  if ((mode == SERIALBUF_BINARYMODE) && (array->getLength() > 0) && (millis() - mils > timeout))
  {
    finished = true;
    array->nullTerminate();
    return;
  }

  while (Serial.available())
  {
    mils = millis();

    int r = Serial.read();
    if (mode == SERIALBUF_TEXTMODE)
    {
      if (r == 10)
        continue;

      if (r == 13)
      {
        finished = true;
        array->nullTerminate();
        return;
      }
    }

    if (r > -1) {
      if (array->getLength() < maxlen)
      {
        array->append((char)r);
      }
      else
      {
        overflow = true;
      }
    }
  }
}

void SerialBuf::textMode()
{
  mode = SERIALBUF_TEXTMODE;
  clear();
}

void SerialBuf::binaryMode()
{
  mode = SERIALBUF_BINARYMODE;
  clear();
}

int SerialBuf::getMode()
{
  return mode;
}

int SerialBuf::peek(int offset)
{
  if (position + offset < array->getLength())
  {
    return (int)(*array)[position + offset];
  }
  else
  {
    return -1;
  }
}

bool SerialBuf::isCurrent()
{
  return position < array->getLength();
}

bool SerialBuf::isNext()
{
  return (position + 1) < array->getLength();
}

bool SerialBuf::isNextn(int n)
{
  return (position + n) <= array->getLength();
}


/** Returns the next charater from the buffer or -1 if it doesn't exist. */
int SerialBuf::read()
{
  if (!isCurrent())
    return -1;

  return (int)(*array)[position++];
}

bool SerialBuf::isOverflow()
{
  return overflow;
}

bool SerialBuf::isAvailable()
{
  return finished;
}

int SerialBuf::getLength()
{
  return array->getLength();
}

int SerialBuf::getPosition()
{
  return position;
}

int SerialBuf::getSize() {
  return maxlen;
}

uint32_t SerialBuf::getTimeout()
{
  return timeout;
}


char *SerialBuf::getBuffer()
{
  return array->buffer;
}
