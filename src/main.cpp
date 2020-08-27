
#define PSVITA

#ifdef PSVITA
#include <psp2/kernel/processmgr.h>
#endif

#include "Engine.h"

#include <sstream>
#include <vector>
#include <cstdio>
#include <iostream>

int main(int argc, char *argv[])
{
	GameRemote::Engine engine;
	engine.Start();

#ifdef PSVITA
	sceKernelExitProcess(0);
#endif
    return 0;
}
