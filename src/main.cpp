
#define PSVITA

#ifdef PSVITA
#include <psp2/kernel/processmgr.h>
#endif

#include "Engine.h"

#include <sstream>
#include <vector>
#include <cstdio>
#include <iostream>

int main(int argc, char *argv[]) {
	std::stringstream output;
	std::vector<std::string> hello = { "Hello" };
	hello.push_back(",");
	hello.push_back(" C++ ");
	hello.push_back("world!");
	for (auto &s : hello) {
		// std::cout does't work ATM :(
		output << s;
	}
	output << std::endl;
	printf("%s\n", output.str().c_str());

	GameRemote::Engine engine;
	engine.Start();

#ifdef PSVITA
	sceKernelExitProcess(0);
#endif
    return 0;
}
