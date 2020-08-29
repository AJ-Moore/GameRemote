
//#define PSVITA
#include "ControlDeck/Common.h"
#include "Engine.h"
#include "ControlDeck/ControlDeck.h"
#include <sstream>
#include <vector>
#include <cstdio>

int main(int argc, char *argv[])
{
#ifdef PSVITA
	psvDebugScreenInit();
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
#endif

    printff("Started!");
	GameRemote::ControlDeck controlDeck;
	controlDeck.Start();

	GameRemote::Engine engine;
	engine.Start();

#ifdef PSVITA
	sceKernelDelayThread(10 * 1000000); // Wait for 3 seconds
	sceKernelExitProcess(0);
#endif
    return 0;
}
