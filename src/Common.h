// Copyright © Allan Moore April 2020

#define PSVITA

#pragma once
#ifdef PSVITA
typedef unsigned char BYTE;
#include <psp2/kernel/threadmgr.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/net/net.h> 
#include <psp2/sysmodule.h>
#include <psp2/ctrl.h>
#include "debugScreen.h"
#define printff psvDebugScreenPrintf
#else
#define printff printf
#endif

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <wingdi.h>
#endif

#include <exception>
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include "Types.h"

#include <SDL2/SDL.h>
#include <string>
#include <functional>
#include <cmath>
#include <memory>
#include <sstream>

#include "zlib.h"
#include "zconf.h"

using String = std::string;

template <class T>
using UniquePtr = std::unique_ptr<T>;

template <class T>
using SharedPtr = std::shared_ptr<T>;

#undef main
