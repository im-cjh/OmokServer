#pragma once
#include "Session.h"

class Player : public Session
{
	// Session을(를) 통해 상속됨
	void HandlePacket(BYTE* buffer, INT32 len, ePacketID ID) override;
};

