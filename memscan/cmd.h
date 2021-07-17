#pragma once

#define ATTACH_TOKEN "attach"
#define MODULE_ADDRESS_TOKEN "module"
#define CONTEXT_TOKEN "context"
#define READ_TOKEN "read"
#define EXIT_TOKEN "exit"

#define EXIT_CODE 0

enum class CMDOption {
	ATTACH,
	MODULE_ADDRESS,
	CONTEXT,
	READ,
	EXIT,
	INVALID_CMD
};

typedef struct {
	CMDOption option;
	char **args;
	int argc;
} Command;

typedef struct {
	char *raw_cmd;
	int cmd_length;
} RawCommand;

RawCommand *GetRawCMD();
void FreeRawCMD(RawCommand *cmd);
int RunRawCMD(RawCommand *cmd);
