
#include "Vita.h"
#include "Engine.h"

#ifdef PSVITA
#include <psp2/net/net.h>

bool GameRemote::Vita::Init()
{
	return ConnectToServer();
}

void GameRemote::Vita::Update()
{
	m_engine->m_lock.lock();

	int32_t received_data = sceNetRecv(m_socket, &m_engine->m_pixelBufferCompressed[0], m_engine->m_pixelBuffer.size(), 0);

	if (received_data < 1)
	{
//		printff("Receive error!");
		m_engine->m_lock.unlock();
		return;
	}

	m_engine->m_lock.unlock();
	m_engine->DecompressPixelBuffer();
}

#define NET_PARAM_MEM_SIZE (1*1024*1024)

bool GameRemote::Vita::ConnectToServer()
{
	int32_t retval;
	SceNetSockaddrIn server; /* server address to send data to */
	SceNetInitParam net_init_param; /* Net init param structure */

	net_init_param.memory = malloc(NET_PARAM_MEM_SIZE);
	memset(net_init_param.memory, 0, NET_PARAM_MEM_SIZE);
	net_init_param.size = NET_PARAM_MEM_SIZE;
	net_init_param.flags = 0;
	sceNetInit(&net_init_param);

	m_socket = sceNetSocket("ping_test", SCE_NET_AF_INET, SCE_NET_SOCK_STREAM, SCE_NET_IPPROTO_TCP);

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

	return true;
}

void GameRemote::Vita::Shutdown()
{
}

#endif
