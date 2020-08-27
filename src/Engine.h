#pragma once

#include "ControlDeck/Common.h"

namespace GameRemote
{
	enum {
		SCREEN_WIDTH = 960,
		SCREEN_HEIGHT = 544
	};

	class Engine
	{
	public:
		void Start();
		static void Exit();
	private:
		bool Init();
		bool InitSDL();
		void EngineMain();
		void Update();
		void Render();

		SDL_Window* m_sdlWindow;
		SDL_Surface* m_sdlSurface;
		bool m_bDoneInit = false;
		bool m_bRunning = false;
	};
}
