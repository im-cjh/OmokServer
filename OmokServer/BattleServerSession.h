#pragma once
#include "Session.h"

class BattleServerSession : public Session
{
public:
	BattleServerSession() : Session() {}
	virtual void Connect(INT16 port=0) override;
	void MakeRoom(void);
private:
	// Session��(��) ���� ��ӵ�
	virtual void HandlePacket(BYTE* buffer, INT32 len, ePacketID ID) override;
};

extern BattleServerSessionRef GBattleServer;
