#include "Engine.h"
#include "Vita.h"
#include "Common.h"

#ifndef PSVITA
#include "Server.h"
#endif

namespace GameRemote
{
	void Engine::Start()
	{
		if (m_bDoneInit)
		{
			printff("Engine already initialised.");
			return;
		}

		if (Init())
		{
			EngineMain();
		}
	}

	bool Engine::Init()
	{
		//gdi first
		if (!InitGDI())
		{
			return false;
		}

		if (!InitSDL())
		{
			return false;
		}

#ifdef PSVITA
		m_client = std::make_shared<Vita>();
#else
		m_client = std::make_shared<Server>();
#endif

		m_client->SetParent(this);

		if (!m_client->Init())
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

		Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
#endif

		m_sdlWindow = SDL_CreateWindow("GameRemote", 400, 400, VITA_WIDTH, VITA_HEIGHT, SDL_WINDOW_RESIZABLE);
		//m_sdlSurface = SDL_CreateRGBSurface(0, 960, 544, 32, 0xff0000, 0x00ff00, 0x0000ff, 0x0);
		m_sdlSurface = SDL_CreateRGBSurface(0, 960, 544, 32, rmask, gmask, bmask, amask);
		m_sdlSurface = SDL_ConvertSurfaceFormat(m_sdlSurface, SDL_PIXELFORMAT_BGRA32, 0);
		SDL_FillRect(m_sdlSurface, NULL, 0x00000000);
		SDL_UpdateWindowSurface(m_sdlWindow);

		return true;
	}

	bool Engine::InitGDI()
	{
#ifdef _WIN32
		//int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
		//int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
		int width = 960; 
		int height = 544;
		m_pixelBuffer.resize(width * height * 4);
		m_hdc = GetDC(NULL);
		m_context = CreateCompatibleDC(m_hdc);
		m_bitmap = CreateCompatibleBitmap(m_hdc, width, height);
		
		if (m_bitmap == nullptr)
		{
			printff("Failed to create gdi bitmap.\n");
		}
#endif 

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
		m_client->Update();
	}

	void Engine::Render()
	{
#ifdef _WIN32

		int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
		int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);

		int targetWidth = 960;
		int targetHeight = 544;

		SelectObject(m_context, m_bitmap);
		if (BitBlt(m_context, 0, 0, width, height, m_hdc, 0, 0, SRCCOPY) == 0)
		{
			printff("BitBlt failed: %s\n", GetLastError());
		}

		BITMAP bmpScreen;
		GetObject(m_bitmap, sizeof(BITMAP), &bmpScreen);

		BITMAPINFOHEADER bmi;
		bmi.biSize = sizeof(BITMAPINFOHEADER);
		bmi.biPlanes = 1;
		bmi.biCompression = BI_RGB;
		bmi.biHeight = -targetHeight;
		bmi.biWidth = targetWidth;
		bmi.biSizeImage = 0;
		bmi.biBitCount = 32;
		bmi.biClrImportant = 0;
		bmi.biXPelsPerMeter = 0;
		bmi.biYPelsPerMeter = 0;
		bmi.biClrUsed = 0;

		//GetDIBits(m_context, m_bitmap, 0, targetHeight, nullptr, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);
		if (GetDIBits(m_context, m_bitmap, 0, targetHeight, &m_pixelBuffer[0], (BITMAPINFO*)&bmi, DIB_RGB_COLORS) == 0)
		{
			printff("GetDIBits failed: %s\n", GetLastError());
		}

		CompressPixelBuffer();

		//if (CompressPixelBuffer())
		//{
		//	if (DecompressPixelBuffer())
		//	{
		//		SDL_memcpy(m_sdlSurface->pixels, &m_pixelBufferDecompressed[0], sizeof(BYTE) * m_pixelBufferDecompressed.size());
		//	}
		//}
#endif
		SDL_memcpy(m_sdlSurface->pixels, &m_pixelBuffer[0], sizeof(BYTE) * m_pixelBuffer.size());
		SDL_BlitScaled(m_sdlSurface, nullptr, SDL_GetWindowSurface(m_sdlWindow), nullptr);
		SDL_UpdateWindowSurface(m_sdlWindow);
	}

	bool Engine::CompressPixelBuffer()
	{
		m_lock.lock();

		unsigned long dataSize = m_pixelBuffer.size();
		m_pixelBufferCompressed.resize(dataSize);

		int result = compress2(&m_pixelBufferCompressed[0], &dataSize, &m_pixelBuffer[0], dataSize, 1);

		if (result == Z_OK)
		{
			m_pixelBufferCompressed.resize(dataSize);
			m_lock.unlock();
			return true;
		}

		m_lock.unlock();
		return false;
	}

	bool Engine::DecompressPixelBuffer()
	{
		m_lock.lock();
		unsigned long dataSize = m_pixelBuffer.size();

		int result = uncompress(&m_pixelBuffer[0], &dataSize, &m_pixelBufferCompressed[0], m_pixelBufferCompressed.size());
		if (result == Z_OK)
		{
			m_lock.unlock();
			return true;
		}

		m_lock.unlock();
		return false;
	}

	void Engine::Exit()
	{
	}
}
