#include "pch.h"
#include "BattleServerPlayer.h"
#include "BattleRoomManager.h"

void BattleServerPlayer::setInfo(BYTE* pBuffer, INT32 pLen)
{
	Protocol::C2SLoginSuccess pkt;
	if (pkt.ParseFromArray(pBuffer + sizeof(PacketHeader), pLen - sizeof(PacketHeader)))
	{
		_name = pkt.username();
		_ID = pkt.userid();
	}
	//GPlayerManager
}

void BattleServerPlayer::HandlePacket(BYTE* pBuffer, INT32 pLen, ePacketID ID)
{
	switch (ID)
	{
	case ePacketID::CHAT_MESSAGE:
		GBattleRoomManager.BroadcastChat(pBuffer, pLen);
		break;
	case ePacketID::CONTENT_MESSAGE:
		GBattleRoomManager.BroadcastContent(pBuffer, pLen);
		break;
	case ePacketID::ENTER_ROOM_MESSAGE:
		GBattleRoomManager.HandleEnterRoom(pBuffer, pLen, dynamic_pointer_cast<BattleServerPlayer>(shared_from_this()), ePacketID::ENTER_ROOM_MESSAGE);
		break;
	case ePacketID::LOGIN_SUCCESS_MESSAGE:
		setInfo(pBuffer, pLen);
		break;
	case ePacketID::QUIT_ROOM_MESSAGE:
		GBattleRoomManager.HandleQuitRoom(pBuffer, pLen, dynamic_pointer_cast<BattleServerPlayer>(shared_from_this()));
		break;
	case ePacketID::ROOM_CREATED_MESSAGE:
		cout << "ROOM_CREATED_MESSAGE\n";
		break;
	case ePacketID::ENTER_FAST_ROOM_MESSAGE:
		GBattleRoomManager.HandleEnterRoom(pBuffer, pLen, dynamic_pointer_cast<BattleServerPlayer>(shared_from_this()), ePacketID::ENTER_FAST_ROOM_MESSAGE);
		cout << "ENTER_FAST_ROOM\n";
		break;
	}
}
