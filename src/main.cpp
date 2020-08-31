
//#define PSVITA
#include "Common.h"
#include "Engine.h"
//#include "ControlDeck/ControlDeck.h"

int main(int argc, char *argv[])
{
#ifdef PSVITA
	psvDebugScreenInit();
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG); /* Controller module */
	sceSysmoduleLoadModule(SCE_SYSMODULE_NET);  /* Net Module*/
#endif

    printff("Started!");
	//GameRemote::ControlDeck controlDeck;
	//controlDeck.Start();

	GameRemote::Engine engine;
	engine.Start();

#ifdef PSVITA
	sceKernelDelayThread(10 * 1000000); // Wait for 10 seconds
	sceKernelExitProcess(0);
#endif
    return 0;
}
