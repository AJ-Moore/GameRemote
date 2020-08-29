#include "Engine.h"
#include <iostream>

namespace GameRemote
{
	void Engine::Start()
	{
		if (m_bDoneInit)
		{
			printf("Engine already initialised.");
			return;
		}

		if (Init())
		{
			EngineMain();
		}
	}

	bool Engine::Init()
	{
		if (!InitSDL())
		{
			return false;
		}

		return true;
	}

	bool Engine::InitSDL()
	{
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			return false;
		}

		m_sdlWindow = SDL_CreateWindow("GameRemote", 0, 0, SCREEN_WIDTHh, SCREEN_HEIGHTt, SDL_WINDOW_SHOWN);
		m_sdlSurface = SDL_CreateRGBSurface(0, 256, 240, 32, 0xff0000, 0x00ff00, 0x0000ff, 0x0);
		SDL_FillRect(m_sdlSurface, NULL, 0xff00ff);
		SDL_UpdateWindowSurface(m_sdlWindow);

		return true;
	}

	void Engine::EngineMain()
	{
		m_bRunning = true;

		while (m_bRunning)
		{
			Update();
			Render();
		}
	}
	void Engine::Update()
	{
		SDL_PumpEvents();
	}

	void Engine::Render()
	{
		SDL_BlitScaled(m_sdlSurface, nullptr, SDL_GetWindowSurface(m_sdlWindow), nullptr);
		SDL_UpdateWindowSurface(m_sdlWindow);
	}

	void Engine::Exit()
	{
	}
}
