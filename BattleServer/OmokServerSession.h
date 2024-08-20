#pragma once
#include "Session.h"

class OmokServerSession : public Session
{
public:
	OmokServerSession() : Session() {}
private:
	// Session을(를) 통해 상속됨
	virtual void HandlePacket(BYTE* buffer, INT32 len, ePacketID ID) override;
	void HandleMakeFastRoom(BYTE* buffer, INT32 len);
};

extern OmokServerSessionRef GOmokServer;