#pragma once

#include "Common.h"

#ifndef PSVITA
#include "TCP/TCPClient.h"

namespace GameRemote
{
	class Server : public TCPClient
	{
	public:
		virtual bool Init();
		virtual void Update();
		void ListenForPackets();
		virtual void ListenForClients();

	private:
		void ShutDown();

		SOCKET m_listenSocket = INVALID_SOCKET;
		SOCKET m_acceptSocket = INVALID_SOCKET;
		bool m_bRunning = false;

		void serverThread(SOCKET socket);
		bool SendData(SOCKET socket);
		bool SendData(SOCKET socket, sockaddr_in addr, socklen_t addrlength);
	};

	class Thread_Params {
	public:
		Thread_Params(Server* obj, SOCKET sock) {
			this->socket = sock; this->server = obj;
		}

		SOCKET socket;
		Server* server;
	};
}
#endif
