#pragma once
#include <windows.h>

#include "cmd.h"

#define SUCCESS_CODE 1
#define ERROR_CODE 2

#define INT_TYPE "int"
#define FLOAT_TYPE "float"

typedef struct {
	int size;
	void *base_address;
	char *module_name;
} ProcessMemoryInfo;

typedef struct {
	int pid;
	char *process_name;
	HANDLE process_handle;
	ProcessMemoryInfo memory_info;
} ProcessInfo;

extern ProcessInfo process_info;

int AttachProcess(Command *cmd);
void DetachProcess();
int GetModuleBaseAddress(Command *cmd);
int ReadMemory(Command* cmd);
int Context();
