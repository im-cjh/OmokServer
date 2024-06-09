#pragma once
#include "Session.h"

class OmokServerSession : public Session
{
public:
	OmokServerSession() : Session() {}
	virtual void Connect(INT16 port = 0) override;
private:
	// Session��(��) ���� ��ӵ�
	virtual void HandlePacket(BYTE* buffer, INT32 len, ePacketID ID) override;
	void HandleMakeFastRoom(BYTE* buffer, INT32 len);
};

extern OmokServerSessionRef GOmokServer;