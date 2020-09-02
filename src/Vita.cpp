
#include "Vita.h"
#include "Engine.h"

#ifdef PSVITA
#include <psp2/net/net.h>

bool GameRemote::Vita::Init()
{
	ConnectToServer();

	if (!m_bConnected)
	{
		int32_t sent = sceNetSendto(m_socket, ":)", 2, 0, (SceNetSockaddr*)&server, sizeof(SceNetSockaddr));

		if (sent < 1)
		{
			printff("Send failed.");
			m_engine->m_lock.unlock();
			return;
		}

		m_bConnected = true;
	}

	m_minBufferSize = m_engine->m_pixelBuffer.size();
	m_packetData.resize(2);


	std::thread processThread(&GameRemote::Vita::ProcessVideo_Thread, this);
	processThread.detach();

	return true;
}

void GameRemote::Vita::Update()
{
	m_engine->m_lock.lock();

	SceNetSockaddr from;
	uint32_t fromlen = sizeof(from);

	for (int i = 0; i < m_engine->m_chunkCount; i++)
	{
		m_packetData[m_currentPacket].resize(m_minBufferSize);
		int32_t received_data = sceNetRecvfrom(m_socket, &m_packetData[m_currentPacket], m_minBufferSize, 0, &from, (unsigned int*)&fromlen);

		if (received_data < 1)
		{
			printff("sceNetRecv error: %s\n");
			m_engine->m_lock.unlock();
			return;
		}

		int headerId = m_packetData[m_currentPacket][0];
		int chunkCount = (int)m_packetData[m_currentPacket][4] | ((int)m_packetData[m_currentPacket][5] << 8);

		m_minBufferSize = (m_engine->m_pixelBuffer.size() / chunkCount) + m_engine->m_headerSize;

		//printff("%i\n", received_data);
		m_packetData[m_currentPacket].resize(received_data);
		m_engine->m_lock.unlock();

		// Block until last packet completes.
		m_packetMutex.lock();
		m_packet = (BYTE*)&m_packetData[m_currentPacket][0];
		m_bPacketReady = true;
		m_currentPacket = (m_currentPacket == 0) ? 1 : 0;
		m_packetMutex.unlock();
	}
}

void GameRemote::Vita::ProcessVideo_Thread()
{
	while (true)
	{
		m_packetMutex.lock();
		if (m_bPacketReady)
		{
			m_bPacketReady = false;
			int headerId = m_packet[0];

			switch (headerId)
			{
			case GameRemote::HEADER::BITMAP_CHUNK:
				m_engine->UnmakeUncompressedChunk();
				break;
			case GameRemote::HEADER::BITMAP_CHUNK_COMPRESSED:
				m_engine->DecompressPixelBufferChunk(m_packet);
				break;
			default:
				printff("Invalid Header!\n");
			}
		}
		m_packetMutex.unlock();
	}
}

#define NET_PARAM_MEM_SIZE (1*1024*1024)

bool GameRemote::Vita::ConnectToServer()
{
	int32_t retval;
	SceNetInitParam net_init_param; /* Net init param structure */

	net_init_param.memory = malloc(NET_PARAM_MEM_SIZE);
	memset(net_init_param.memory, 0, NET_PARAM_MEM_SIZE);
	net_init_param.size = NET_PARAM_MEM_SIZE;
	net_init_param.flags = 0;
	sceNetInit(&net_init_param);

	m_socket = sceNetSocket("GameRemote", SCE_NET_AF_INET, SCE_NET_SOCK_DGRAM, SCE_NET_IPPROTO_UDP);

	if (m_socket < 0)
	{
		printff("Socket Error.");
		return false;
	}

	server.sin_len = sizeof(server);
	server.sin_family = SCE_NET_AF_INET; /* set packet to IPv4 */
	server.sin_port = sceNetHtons(m_port);
	sceNetInetPton(SCE_NET_AF_INET, m_serverAddr.c_str(), &server.sin_addr);
	memset(server.sin_zero, 0, sizeof(server.sin_zero)); /* fill sin_zero with zeroes */

	//retval = sceNetConnect(m_socket, (SceNetSockaddr*)&server, sizeof(server));

	SceNetSockaddrIn myaddr;
	memset((char*)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = SCE_NET_AF_INET;
	myaddr.sin_addr.s_addr = sceNetHtonl(SCE_NET_INADDR_ANY);
	myaddr.sin_port = sceNetHtons(0);

	// Bind is necessary with udp
	retval = sceNetBind(m_socket, (SceNetSockaddr*)&myaddr, sizeof(myaddr));

	if (retval < 0)
	{
		printff("Bind Failed.");
		return false;
	}

	printff("Socket OK");
	return true;
}

void GameRemote::Vita::Shutdown()
{
	sceNetSocketClose(m_socket);
}

#endif
