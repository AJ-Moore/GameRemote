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
		BITMAP_CHUNK_COMPRESSED = 0x2,
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

		bool MakeUncompressedChunks();
		bool UnmakeUncompressedChunk();

		bool CompressPixelBufferChunks();
		bool DecompressPixelBufferChunk();
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
		int m_chunkCount = 64;
		const int m_headerSize = 20;
		const int m_compressionLevel = 1;

		std::vector<std::vector<BYTE>> m_chunks;
#ifdef _WIN32
		HDC m_hdc = nullptr;
		HDC m_context = nullptr;
		HBITMAP m_bitmap = nullptr;

#endif 
	};
}
