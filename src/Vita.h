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
		void ProcessVideo_Thread()

		bool m_bConnected = false;
		int32_t m_socket;
		std::string m_serverAddr = "192.168.0.6";
		SceNetSockaddrIn server;
		bool m_bPacketReady = false;

		std::vector<std::vector<BYTE>> m_packetData;
		BYTE* m_packet = nullptr;
		std::mutex m_packetMutex;
		int m_currentPacket = 0;
		int m_minBufferSize = 0;
	};
}

#endif
