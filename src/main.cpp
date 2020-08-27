
#define PSVITA

#ifdef PSVITA
#include <psp2/kernel/threadmgr.h>
#include <psp2/kernel/processmgr.h>

//#define printf psvDebugScreenPrintf
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
	sceKernelDelayThread(10 * 1000000); // Wait for 3 seconds
	sceKernelExitProcess(0);
#endif
    return 0;
}
