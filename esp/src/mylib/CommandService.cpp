#include "CommandService.h"

#include <mylib/BLEProvider.h>
#include <mylib/CommandService.h>
#include <mylib/StorageProvider.h>

#include "cmdProc.h"

CmdProc::ComandProcesor cmd_proc = CmdProc::ComandProcesor();

void CommandService::init(int numberOfComands) {
    cmd_proc.init(numberOfComands);
}

void CommandService::add(char *name, CmdProc::cmdCallback cbk, bool ble) {
    cmd_proc.add(name, cbk, ble);
}

int CommandService::process(char *command) {
    return cmd_proc.Execute(command);
}
int CommandService::process(char *command, char *uuid) {
    return cmd_proc.Execute(command, uuid);
}
