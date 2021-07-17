#pragma once

#define ATTACH_TOKEN "attach"
#define EXIT_TOKEN "exit"

#define EXIT_CODE 0

enum class CMDOption {
	ATTACH,
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
