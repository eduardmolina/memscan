#include <string.h>
#include <windows.h>
#include <tlhelp32.h>

#include "cmd.h"
#include "process.h"

HANDLE process_handle = NULL;

void CloseProcessHandle(HANDLE process_handle) {
    CloseHandle(process_handle);
}

int GetProcessPID(const char *process_name) {
    
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    int pid = 0;
    if (Process32First(snapshot, &entry)) {
        while (Process32Next(snapshot, &entry)) {
            if (!strcmp(entry.szExeFile, process_name)) {
                pid = entry.th32ProcessID;
            }
        }
    }

    CloseHandle(snapshot);

    return pid;
}

int AttachProcess(Command *cmd) {

	char* process_name = cmd->args[1];
	if (!strlen(process_name))
		return ERROR_CODE;

    int pid = GetProcessPID(process_name);
    process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (process_handle)
        return SUCCESS_CODE;

	return ERROR_CODE;
}