#ifndef __COMMANDSERVICE_H__
#define __COMMANDSERVICE_H__

#include <mylib/cmdProc.h>

class CommandService {
   public:
    static void init(int numberOfComands);

    static int process(char *s);
    static int process(char *uuid, char *s);
    static void add(char *name, CmdProc::cmdCallback cbk, bool ble = false);
};

#endif  // __COMMANDSERVICE_H__