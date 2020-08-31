//Developed by Allan Moore - 21017984 - Bucks New University

#ifndef PSVITA

#include "Server.h"
#include "Engine.h"

namespace GameRemote
{
	//Thread used for the communication between the server and the client. 
	DWORD WINAPI Server::serverThread(LPVOID lpParam) {
		Thread_Params* params = (Thread_Params*)lpParam;
		bool threadRunning = true;
		while (threadRunning && params->server->m_bRunning)
		{
			if (params->socket != INVALID_SOCKET)
			{
				try
				{
					params->server->SendData(params->socket);
				}
				catch (std::exception e)
				{
					printff("Sending data failed: %s \n", e.what());
					threadRunning = false;
					break;
				}
			}
		}

		printff("Socket closed");
		closesocket(params->socket);
		std::cout << "Client Connection Closed!" << std::endl;
		return 0;
	}

	bool Server::SendData(SOCKET socket)
	{
		m_engine->m_lock.lock();
		const char* data = (char*)&m_engine->m_pixelBufferCompressed[0];
		int byteCount = send(socket, data, m_engine->m_pixelBufferCompressed.size(), 0);
		m_engine->m_lock.unlock();

		if (byteCount == SOCKET_ERROR)
		{
			printff("Send failed.");
			return false;
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
			m_listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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
		service.sin_family = AF_INET; // Family ivp4 (AF_INET)
		service.sin_addr.s_addr = inet_pton(AF_INET, "localhost", &addr);
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
		try {
			if (listen(this->m_listenSocket, 1) == SOCKET_ERROR)
				throw WSAGetLastError();
			else
				std::cout << "listen() is OK, waiting for connections..." << std::endl;
		}
		catch (int e) {
			std::cout << "listen(): Error listening on socket " << e << std::endl;
		}

		// Create thread to listen for clients.
		std::thread listenThread(&Server::ListenForClients, this);
		listenThread.detach();
		return true;
	}

	void Server::Update()
	{
		
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
					CreateThread(NULL, 0, Server::serverThread, (LPVOID)(new Thread_Params(this, m_acceptSocket)), 0, &threadId);
				}
			}
			catch (int e) {
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