#include "pch.h"
#include "BattleServerSession.h"
#include "Protocol.pb.h"
#include "PacketHandler.h"

BattleServerSessionRef GBattleServer;

void BattleServerSession::MakeRoom(INT32 pRoomID)
{
	int len = 0;
	Protocol::S2CRoomID pkt;
	pkt.set_roomid(pRoomID);

	BYTE* sendBuffer = PacketHandler::SerializePacket(pkt, ePacketID::MAKE_FAST_ROOM_MESSAGE, &len);

	Session::Send(sendBuffer, len);
}

void BattleServerSession::HandlePacket(BYTE* buffer, INT32 len, ePacketID ID)
{
	switch (ID)
	{
	case MAKE_FAST_ROOM_MESSAGE:
		cout << 3;
		break;
	case ePacketID::ENTER_FAST_ROOM_MESSAGE:
		cout << "BattleServerSession Called\n";
		break;
	}
}