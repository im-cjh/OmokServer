#pragma once
#include "Session.h"

class Player : public Session
{
	// Session��(��) ���� ��ӵ�
	void HandlePacket(BYTE* buffer, INT32 len, ePacketID ID) override;
};

