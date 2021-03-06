#include <iostream>
#include <string>
#include <string.h>
#include <windows.h>
#include <tlhelp32.h>

#include "cmd.h"
#include "process.h"

ProcessInfo process_info;

void DetachProcess() {
    CloseHandle(process_info.process_handle);
    free(process_info.memory_info.module_name);
    free(process_info.process_name);
}

int ReadMemory(Command* cmd) {

    char* type = cmd->args[1];
    if (!type || *type == '\0')
        return ERROR_CODE;
    
    char* address = cmd->args[2];
    if (!address || *address == '\0')
        return ERROR_CODE;

    uintptr_t addr = strtoull(address, NULL, 0);

    if (!strcmp(type, INT_TYPE)) {
        int buffer = INT_MAX;
        bool ok = ReadProcessMemory(process_info.process_handle, (void*)addr, &buffer, sizeof(int), NULL);
        if (ok)
            std::cout << "[+] Result: " + std::to_string(buffer) << std::endl;

        return ok ? SUCCESS_CODE : ERROR_CODE;
    }
    else if (!strcmp(type, FLOAT_TYPE)) {
        float buffer = FLT_MAX;
        bool ok = ReadProcessMemory(process_info.process_handle, (void*)addr, &buffer, sizeof(float), NULL);
        if (ok)
            std::cout << "[+] Result: " + std::to_string(buffer) << std::endl;

        return ok ? SUCCESS_CODE : ERROR_CODE;
    }

    return ERROR_CODE;
}

int Context() {

    if (!process_info.process_handle ||
        !process_info.pid ||
        !process_info.process_name ||
        !process_info.memory_info.module_name)
        return ERROR_CODE;

    std::cout << "PID: ";
    std::cout << process_info.pid << std::endl;

    std::cout << "ProcessName: ";
    std::cout << process_info.process_name << std::endl;

    std::cout << "ModuleName: ";
    std::cout << process_info.memory_info.module_name << std::endl;

    std::cout << "ModuleSize: ";
    std::cout << process_info.memory_info.module_size << std::endl;

    std::cout << "BaseAddress: ";
    std::cout << "0x" << std::hex << process_info.memory_info.base_address << std::endl;

    return SUCCESS_CODE;
}

void LoadModuleInfo(MODULEENTRY32 module_entry, const char *module_name) {
    process_info.memory_info.base_address = module_entry.modBaseAddr;
    process_info.memory_info.module_size = module_entry.modBaseSize;
    free(process_info.memory_info.module_name);
    process_info.memory_info.module_name = NULL;
    process_info.memory_info.module_name = (char*)malloc((strlen(module_name) + 1) * sizeof(char));
    memcpy(process_info.memory_info.module_name, module_name, (strlen(module_name) + 1) * sizeof(char));
}

int GetModuleBaseAddress(Command *cmd) {

    if (!process_info.process_handle || !process_info.pid || !process_info.process_name)
        return ERROR_CODE;

    const char *module_name = !strcmp(cmd->args[1], "this") ? process_info.process_name : cmd->args[1];

    MODULEENTRY32 module_entry;
    module_entry.dwSize = sizeof(MODULEENTRY32);
    
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process_info.pid);

    if (Module32First(snapshot, &module_entry) && !strcmp(module_name, module_entry.szModule))
        LoadModuleInfo(module_entry, module_name);
    else
        while (Module32Next(snapshot, &module_entry))
            if (!strcmp(module_name, module_entry.szModule)) {
                LoadModuleInfo(module_entry, module_name);
                break;
            }

	CloseHandle(snapshot);

	return process_info.memory_info.base_address ? SUCCESS_CODE : ERROR_CODE;
}

int GetProcessPID(const char *process_name) {
    
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    int pid = 0;
    if (Process32First(snapshot, &entry) && !strcmp(entry.szExeFile, process_name))
        pid = entry.th32ProcessID;
    else
        while (Process32Next(snapshot, &entry))
            if (!strcmp(entry.szExeFile, process_name)) {
                pid = entry.th32ProcessID;
                break;
            }

    CloseHandle(snapshot);

    return pid;
}

int AttachProcess(Command *cmd) {

    char* process_name = cmd->args[1];
    if (!process_name)
	    return ERROR_CODE;

    free(process_info.process_name);
    process_info.process_name = NULL;
    process_info.process_name = (char*) malloc((strlen(process_name) + 1) * sizeof(char));

    memcpy(process_info.process_name, process_name, (strlen(cmd->args[1]) + 1) * sizeof(char));
    process_info.pid = GetProcessPID(process_name);
    process_info.process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_info.pid);
    if (process_info.process_handle)
        return SUCCESS_CODE;

	return ERROR_CODE;
}