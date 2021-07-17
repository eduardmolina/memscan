#include <iostream>

#include "cmd.h"
#include "process.h"

void RunCLI() {

	std::cout << "Use:" << std::endl;
	std::cout << "  attach <process_name.exe>" << std::endl;
	std::cout << "  module (this | <module_name>)" << std::endl;
	std::cout << "  context" << std::endl;
	std::cout << "  exit" << std::endl;

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
