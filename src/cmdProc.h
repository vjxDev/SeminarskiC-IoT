#ifndef __CMDPROCV1_H__
#define __CMDPROCV1_H__

#include <Arduino.h>
#include <string.h>

#define STREQUAL(a, b) (strcmp(a, b) == 0)

#define CMD_ERR_GENERALERROR -1
#define CMD_ERR_MISSINGARGUMENT -2
#define CMD_ERR_EXTRAARGUMENT -3
#define CMD_ERR_INVALIDVALUE -4
#define CMD_ERR_VALUEOUTOFRANGE -5
#define CMD_ERR_UNKNOWNCOMMAND -128

namespace CmdProc {
class ComandProcesor;

typedef int (*cmdCallback)(char *);  // tip za callback funkciju

class Comand {
   public:
    const char *cmdName;
    cmdCallback callback;
    Comand(){};
    void Set(char *cmdName, cmdCallback cbk, bool ble = false);

   private:
    boolean ble;
};

class ComandProcesor {
   public:
    void init(int count);
    void add(char *command, cmdCallback cbk, bool ble = false);

    int Execute(char *s);
    int Execute(char *s, char *uuid);

   private:
    int count;
    int current;
    Comand *commands;

    char *input;
};

bool tryParseFloat(char *s, float &outResult);

bool tryParseInt32(char *s, int32_t &outResult, bool allowMinus = false);
bool tryParseInt(char *s, int &outResult, bool allowMinus = false);

bool tryParseDec32(char *s, int32_t &outResult, bool allowMinus = false);
bool tryParseDec(char *s, int &outResult, bool allowMinus = false);

bool tryParseHex32(char *s, int32_t &outResult);

bool isHexDigit(char c);

bool tryParseTime(char *s, int &outHour, int &outMinute, int &outSecond);
bool tryParseDate(char *s, int &outDay, int &outMonth, int &outYear);
bool isValidDate(int dd, int mm, int yy);

}  // namespace CmdProc

#endif