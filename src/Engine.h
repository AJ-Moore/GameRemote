#pragma once

#include "Common.h"
#include "TCP/TCPClient.h"
#include "Vita.h"

namespace GameRemote
{
#ifdef PSVITA
class Server{};
#endif

	enum {
		VITA_WIDTH = 960,
		VITA_HEIGHT = 544
	};

	enum HEADER {
		BITMAP_CHUNK = 0x1,
		NONE = 0x0,
	};

	class Engine
	{
		friend class Vita;
		friend class Server;
	public:
		void Start();
		static void Exit();

	private:
		bool Init();
		bool InitSDL();
		bool InitGDI();
		void EngineMain();
		void Update();
		void Render();

		bool CompressPixelBufferChunks();
		bool DeompressPixelBufferChunk();
		bool CompressPixelBuffer();
		bool DecompressPixelBuffer();

		SDL_Window* m_sdlWindow;
		SDL_Surface* m_sdlSurface;
		bool m_bDoneInit = false;
		bool m_bRunning = false;
		std::mutex m_lock;
		SharedPtr<TCPClient> m_client;

		std::vector<BYTE> m_pixelBuffer;
		std::vector<BYTE> m_pixelBufferCompressed;
		std::vector<BYTE> m_pixelBufferDecompressed;

		// data divisions, must be divisble
		const int m_chunkCount = 16;
		const int m_headerSize = 20;
		const int m_compressionLevel = 1;

#ifdef _WIN32
		std::vector<std::vector<BYTE>> m_chunks;

		HDC m_hdc = nullptr;
		HDC m_context = nullptr;
		HBITMAP m_bitmap = nullptr;

#endif 
	};
}
