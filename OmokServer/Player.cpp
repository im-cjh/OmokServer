#include "pch.h"
#include "Player.h"
#include "MatchingManager.h"
#include "Protocol.pb.h"

void Player::setInfo(BYTE* pBuffer, INT32 pLen)
{
	Protocol::C2SLoginSuccess pkt;
	if (pkt.ParseFromArray(pBuffer + sizeof(PacketHeader), pLen - sizeof(PacketHeader)))
	{
		_name = pkt.username();
		_ID = pkt.userid();
	}
}

void Player::HandlePacket(BYTE* pBuffer, INT32 pLen, ePacketID ID)
{
	switch (ID)
	{
	case ePacketID::CHAT_MESSAGE:
		GRoomManager.BroadcastChat(pBuffer, pLen);
		break;
	case ePacketID::ROOMS_MESSAGE:
		GRoomManager.BroadcastRooms(shared_from_this());
		break;
	case ePacketID::CONTENT_MESSAGE:
		GRoomManager.BroadcastContent(pBuffer, pLen);
		break;
	case ePacketID::ENTER_ROOM:
		GRoomManager.HandleEnterRoom(pBuffer, pLen, dynamic_pointer_cast<Player>(shared_from_this()));
		break;
	case ePacketID::LOGIN_SUCCESS:
		setInfo(pBuffer, pLen);
		break;
	case ePacketID::QUIT_ROOM_MESSAGE:
		GRoomManager.HandleQuitRoom(pBuffer, pLen, dynamic_pointer_cast<Player>(shared_from_this()));
		break;
	case ePacketID::MATCHMAKIING_MESSAGE:
		cout << "MATCHMAKIING_MESSAGE\n";
		GMatchMaker.AddToQueueAndMatch(dynamic_pointer_cast<Player>(shared_from_this()));
		break;
	}
}
