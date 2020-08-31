#pragma once

#include "../Common.h"

namespace GameRemote
{
	class Engine;
	class TCPClient {
	public:
		virtual bool Init() = 0;
		virtual void Update();

		void SetParent(Engine* parent) { m_engine = parent; }
	protected:
		const int m_port = 1337;
		Engine* m_engine;
	};
}
