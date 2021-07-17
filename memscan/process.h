#pragma once
#include <windows.h>

#include "cmd.h"

#define SUCCESS_CODE 1
#define ERROR_CODE 2

extern HANDLE process_handle;

int AttachProcess(Command *cmd);
void CloseProcessHandle(HANDLE process_handle);

