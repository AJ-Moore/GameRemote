#pragma once

#include "Common.h"

#ifdef PSVITA

#include "TCP/TCPClient.h"

namespace GameRemote
{
	class Vita : public TCPClient
	{
	public:
		virtual bool Init();
		virtual void Update();

	private:
		bool ConnectToServer();
		void Shutdown();

		int32_t m_socket;
		std::string m_serverAddr = "192.168.0.6";
	};
}

#endif
