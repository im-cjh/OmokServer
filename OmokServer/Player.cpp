#include "pch.h"
#include "Player.h"

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
	case ePacketID::ROOMS_MESSAGE:
		GRoomManager.BroadcastRooms(shared_from_this());
		break;
	case ePacketID::CONTENT_MESSAGE:
		GRoomManager.BroadcastContent(pBuffer, pLen);
		break;
	case ePacketID::ENTER_ROOM_MESSAGE:
		GRoomManager.HandleEnterRoom(pBuffer, pLen, dynamic_pointer_cast<Player>(shared_from_this()));
		break;
	case ePacketID::LOGIN_SUCCESS_MESSAGE:
		setInfo(pBuffer, pLen);
		break;
	}
}
