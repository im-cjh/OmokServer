#include "pch.h"
#include "Player.h"

void Player::Connect(INT16 port)
{
	
}

void Player::setInfo(BYTE* pBuffer, INT32 pLen)
{
	Protocol::C2SLoginSuccess pkt;
	if (pkt.ParseFromArray(pBuffer + sizeof(PacketHeader), pLen - sizeof(PacketHeader)))
	{
		_name = pkt.username();
		_ID = pkt.userid();
	}
	//GPlayerManager
}

void Player::HandlePacket(BYTE* pBuffer, INT32 pLen, ePacketID ID)
{
	switch (ID)
	{
	case ePacketID::CHAT_MESSAGE:
		GRoomManager.BroadcastChat(pBuffer, pLen);
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
	case ePacketID::QUIT_ROOM_MESSAGE:
		GRoomManager.HandleQuitRoom(pBuffer, pLen, dynamic_pointer_cast<Player>(shared_from_this()));
		break;
	case ePacketID::ROOM_CREATED_MESSAGE:
		cout << "ROOM_CREATED_MESSAGE\n";
		break;
	case ePacketID::ENTER_FAST_ROOM:
		GRoomManager.HandleEnterRoom(pBuffer, pLen, dynamic_pointer_cast<Player>(shared_from_this()));
		cout << "ENTER_FAST_ROOM\n";
		break;
	}
}
