//Developed by Allan Moore - 21017984 - Bucks New University

#ifndef PSVITA

#include <signal.h>
#include "Server.h"
#include "Engine.h"

namespace GameRemote
{
	//Thread used for the communication between the server and the client. 
	void Server::serverThread(SOCKET socket)
	{
		bool threadRunning = true;
		while (threadRunning && m_bRunning)
		{
			if (socket != INVALID_SOCKET)
			{
				if (!SendData(socket))
				{
					threadRunning = false;
					break;
				}
			}
			// Sleep thread 
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

		printff("Socket closed, client connection closed.\n");
		closesocket(socket);
	}

	bool Server::SendData(SOCKET socket)
	{
		m_engine->m_lock.lock();
		const char* data = (char*)&m_engine->m_pixelBufferCompressed[0];
		//signal(SIGPIPE, SIG_IGN);
		printf("Sending data!\n");
		int byteCount = send(socket, data, m_engine->m_pixelBufferCompressed.size(), 0);
		m_engine->m_lock.unlock();

		if (byteCount == SOCKET_ERROR)
		{
			printff("Send failed, Socket error.\n");
			return false;
		}

		return true;
	}

	bool Server::SendData(SOCKET socket, sockaddr_in addr, socklen_t addrlength)
	{
		bool sendFailed = true;

		while (sendFailed)
		{
			m_engine->m_lock.lock();
			const char* data = (char*)&m_engine->m_pixelBufferCompressed[0];
			printf("Sending data!\n");
			int byteCount = sendto(socket, data, m_engine->m_pixelBufferCompressed.size(), 0, (struct sockaddr*) & addr, addrlength);
			m_engine->m_lock.unlock();

			if (byteCount == SOCKET_ERROR)
			{
				wchar_t* s = NULL;
				FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, WSAGetLastError(),
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					(LPWSTR)&s, 0, NULL);
				printff("error %S\n", s);
				LocalFree(s);

				printff("Send failed, Socket error.\n");
				//return false;
			}
			else
			{
				sendFailed = false;
			}
		}
		return true;
	}

	bool Server::Init() {
		this->m_bRunning = true;

		WSADATA wsaData;
		int wsaerr;
		WORD wVersionRequested = MAKEWORD(2, 2);
		wsaerr = WSAStartup(wVersionRequested, &wsaData);
		try {
			if (wsaerr != 0) {
				throw "The Winsock dll not found";
				return false;
			}
			else {
				std::cout << "The Winsock dll found!" << std::endl;
				std::cout << "Status: " << wsaData.szSystemStatus << std::endl;
			}
		}
		catch (char* e) {
			std::cout << e << std::endl;
			return false;
		}

		try {
			m_listenSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			if (this->m_listenSocket == INVALID_SOCKET)
			{
				throw  WSAGetLastError();
				return false;
			}
		}
		catch (int e)
		{
			std::cout << "Error at socket(): " << e << std::endl;
			WSACleanup();
			return false;
		}

		sockaddr_in service;
		IN_ADDR addr;
		memset((char*)&service, 0, sizeof(service));
		service.sin_family = AF_INET; // Family ivp4 (AF_INET)
		//service.sin_addr.s_addr = inet_pton(AF_INET, "localhost", &addr);
		service.sin_addr.s_addr = htonl(INADDR_ANY);
		service.sin_port = htons(m_port);

		try {
			if (bind(m_listenSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
			{
				wchar_t* s = NULL;
				FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, WSAGetLastError(),
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					(LPWSTR)&s, 0, NULL);
				printff("error %S\n", s);
				LocalFree(s);
				throw WSAGetLastError();
			}
		}
		catch (int e) {
			std::cout << "bind() failed: " << e << std::endl;
			closesocket(m_listenSocket);
			WSACleanup();
			return false;
		}

		// TCP listen server
		//try {
		//	if (listen(this->m_listenSocket, 1) == SOCKET_ERROR)
		//		throw WSAGetLastError();
		//	else
		//		std::cout << "listen() is OK, waiting for connections..." << std::endl;
		//}
		//catch (int e) {
		//	std::cout << "listen(): Error listening on socket " << e << std::endl;
		//}

		// Create thread to listen for clients.
		//std::thread listenThread(&Server::ListenForClients, this);


		std::thread listenThread(&Server::ListenForPackets, this);
		listenThread.detach();
		return true;
	}

	void Server::Update()
	{		
	}

	void Server::ListenForPackets()
	{
		printff("Listening for packets\n");

		while (m_bRunning)
		{
			// UDP Listen thread.
			struct sockaddr_in addr;
			socklen_t addrlen = sizeof(addr);
			char buffer[2];
			int recvlen = recvfrom(m_listenSocket, buffer, 2, 0, (struct sockaddr*) & addr, &addrlen);

			if (recvlen == SOCKET_ERROR)
			{
				continue;
			}

			SendData(m_listenSocket, addr, addrlen);
		}
	}

	//Listens for clients -> Will start a new thread to handle client communications.
	void Server::ListenForClients()
	{
		DWORD threadId;
		while (m_bRunning)
		{
			m_acceptSocket = accept(m_listenSocket, NULL, NULL);
			/*The check to see if the server is running is to stop an exception
			being thrown when the server is shutting down*/
			try {
				if (m_acceptSocket == INVALID_SOCKET && this->m_bRunning)
				{
					///throws with the error code produced
					throw WSAGetLastError();
				}
				else 
				{
					std::cout << "Server: Client Connected!" << std::endl << std::endl;
					std::thread serverThread(&Server::serverThread, this, m_acceptSocket);
					serverThread.detach();
				}
			}
			catch (int e)
			{
				std::cout << "Accept Failed: " << e << std::endl;
			}
		}

		std::cout << "Server: No longer listening" << std::endl;
	}

	void Server::ShutDown()
	{
		WSACleanup();
		printff("Server shutdown.");
	}
}

#endif