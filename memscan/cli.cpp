#include <iostream>

#include "cmd.h"
#include "process.h"

void RunCLI() {

	int result = -1;

	while (result != EXIT_CODE) {
		std::cout << "> ";
		RawCommand* cmd = GetRawCMD();
		result = RunRawCMD(cmd);

		if (result == SUCCESS_CODE)
			std::cout << "[+] Success" << std::endl;
		else if (result != EXIT_CODE)
			std::cout << "[-] Fail" << std::endl;

		FreeRawCMD(cmd);
	}
}