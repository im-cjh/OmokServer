#include "pch.h"
#include "LobbyPlayer.h"
#include "LobbyRoomManager.h"
#include "MatchingManager.h"
#include "Protocol.pb.h"

void LobbyPlayer::setInfo(BYTE* pBuffer, INT32 pLen)
{
	Protocol::C2SLoginSuccess pkt;
	if (pkt.ParseFromArray(pBuffer + sizeof(PacketHeader), pLen - sizeof(PacketHeader)))
	{
		_name = pkt.username();
		_ID = pkt.userid();
	}
}

void LobbyPlayer::HandlePacket(BYTE* pBuffer, INT32 pLen, ePacketID ID)
{
	switch (ID)
	{
	case ePacketID::ROOMS_MESSAGE:
		GLobbyRoomManager.BroadcastRooms(shared_from_this());
		break;
	case ePacketID::ENTER_ROOM_MESSAGE:
		//GLobbyRoomManager.HandleEnterRoom(pBuffer, pLen, dynamic_pointer_cast<Player>(shared_from_this()), ePacketID::ENTER_ROOM);
		break;
	case ePacketID::LOGIN_SUCCESS_MESSAGE:
		setInfo(pBuffer, pLen);
		break;
	case ePacketID::QUIT_ROOM_MESSAGE:
		//GLobbyRoomManager.HandleQuitRoom(pBuffer, pLen, dynamic_pointer_cast<Player>(shared_from_this()));
		break;
	case ePacketID::MATCHMAKIING_MESSAGE:
		cout << "MATCHMAKIING_MESSAGE\n";
		GMatchMaker.AddToQueueAndMatch(dynamic_pointer_cast<LobbyPlayer>(shared_from_this()));
		break;
	}
}
