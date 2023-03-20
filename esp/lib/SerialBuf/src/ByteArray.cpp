#include "ByteArray.h"
#include <string.h>
#include <stdlib.h>

ByteArray::ByteArray(int size)
{
    buffer = (char *)malloc(size);
    len = 0;
    maxlen = size;
}

ByteArray::~ByteArray()
{
    free(buffer);
}

int ByteArray::append(char b)
{
    if (len < maxlen)
    {
        buffer[len++] = b;
        return len; // return the next position in the buffer
    }
    else
    {
        return -1; // failed
    }
}

int ByteArray::nullTerminate()
{
    if (len < maxlen)
    {
        buffer[len] = 0;
        return len;
    }
    else
    {
        return -1;  // failed
    }
}

void *ByteArray::copyfrom(void *src, int len, int destPos)
{
    if (destPos + len > maxlen)
    {
        return nullptr; // kopiranje bi premašilo granice bafera, pa se ne izvršava
    }
    else
    {
        return memcpy(buffer + destPos, src, len);
    }
}

void ByteArray::clear()
{
    len = 0;
    nullTerminate();
}



int ByteArray::getLength()
{
    return len;
}

int ByteArray::getSize()
{
    return maxlen;
}

//int ByteArray::setLen(int newlen)
//{
//    if (newlen <= maxlen)
//    {
//        len = newlen;
//        pos = 0;
//        return newlen;
//    }
//    return -1; // failed
//}

char ByteArray::operator[](int index)
{
    if (index >= len)
    {
        return (char)0;
    }
    else
    {
        return buffer[index];
    }
}
