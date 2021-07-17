#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "cmd.h"
#include "process.h"

RawCommand *GetRawCMD() {

	char *raw_cmd = NULL;
	char c = 0;
	int char_index = 0;

	do {
		c = getchar();
		raw_cmd = (char *) realloc(raw_cmd, (char_index + 1) * sizeof(char));
		raw_cmd[char_index++] = c;
	} while (c != '\n');

	raw_cmd[--char_index] = '\0';

	RawCommand *cmd = (RawCommand *) malloc(sizeof(RawCommand));
	cmd->cmd_length = char_index + 1;
	cmd->raw_cmd = (char *) malloc(cmd->cmd_length * sizeof(char));
	strcpy_s(cmd->raw_cmd, cmd->cmd_length * sizeof(char), (const char *) raw_cmd);

	free(raw_cmd);

	return cmd;
}

void FreeRawCMD(RawCommand* cmd) {
	free(cmd->raw_cmd);
	cmd->raw_cmd = NULL;
	cmd->cmd_length = 0;
	free(cmd);
	cmd = NULL;
}

Command *ParseRawCMD(RawCommand* raw_cmd) {

	int tokens_index = 0;
	char **tokens = (char **) malloc(sizeof(char *));
	tokens[tokens_index] = NULL;

	int current_chr_index = 0;
	for (int i = 0; i < raw_cmd->cmd_length; i++) {

		char chr = raw_cmd->raw_cmd[i];
		tokens[tokens_index] = (char *) realloc(tokens[tokens_index], (current_chr_index + 1) * sizeof(char));
		tokens[tokens_index][current_chr_index++] = chr;
		
		if (chr == ' ') {
			tokens[tokens_index][--current_chr_index] = '\0';
			tokens_index++;
			tokens = (char **) realloc(tokens, (tokens_index + 1) * sizeof(char *));
			tokens[tokens_index] = NULL;
			current_chr_index = 0;
		}
	}

	Command* cmd = (Command *) malloc(sizeof(Command));
	cmd->args = NULL;
	cmd->argc = 0;
	cmd->option = CMDOption::INVALID_CMD;
	if (!strcmp(tokens[0], ATTACH_TOKEN)) {
		cmd->option = CMDOption::ATTACH;
		cmd->args = tokens;
		cmd->argc = tokens_index + 1;
	}
	else if (!strcmp(tokens[0], EXIT_TOKEN)) {
		cmd->option = CMDOption::EXIT;
	}

	return cmd;
}

void FreeParsedCMD(Command *cmd) {
	for (int i = 0; i < cmd->argc; i++) {
		free(cmd->args[i]);
	}

	free(cmd);
}

int RunRawCMD(RawCommand* raw_cmd) {

	Command *cmd = ParseRawCMD(raw_cmd);

	int result = -1;
	if (cmd->option == CMDOption::ATTACH)
		result = AttachProcess(cmd);
	else if (cmd->option == CMDOption::EXIT) {
		CloseProcessHandle(process_handle);
		result = EXIT_CODE;
	}

	FreeParsedCMD(cmd);

	return result;
}