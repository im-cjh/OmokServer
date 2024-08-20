#include "pch.h"
#include "BattleRoomManager.h"
#include "OmokServerSession.h"
#include "FastRoom.h"
#include "BattleServerPlayer.h"
#include "PacketHandler.h"
//
//
BattleRoomManager GBattleRoomManager;
int BattleRoomManager::SRoomID = 0;

void BattleRoomManager::Init()
{

}

int BattleRoomManager::AddRoom(BattleServerPlayerRef p1, BattleServerPlayerRef p2)
{
	int roomID = SRoomID++;


	FastRoom room{ roomID++, u8"빠른대전방", u8"대충 이름", 2 };
	room.Enter(p1, ePacketID::ENTER_FAST_ROOM_MESSAGE);
	room.Enter(p2, ePacketID::ENTER_FAST_ROOM_MESSAGE);
	 _rooms[roomID] = room;

	return roomID;
}

void BattleRoomManager::BroadcastRooms(SessionRef pSession)
{
	Protocol::S2CRoomList ret;

	for (auto& pair : _rooms)
	{
		FastRoom& a = pair.second;
		Protocol::P_Room* r = ret.add_rooms();
		r->set_roomid(a.roomID);
		r->set_roomname(a.roomName);
		r->set_hostname(a.hostName);
		r->set_num_players(a.numParticipants);
	}

	auto serialized_size = ret.ByteSizeLong(); //tmp

	BYTE* buffer = new BYTE[serialized_size + sizeof(PacketHeader)];
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	header->id = ePacketID::ROOMS_MESSAGE;
	header->size = serialized_size + sizeof(PacketHeader);

	ret.SerializeToArray(buffer + sizeof(PacketHeader), serialized_size);
	pSession->Send(buffer, serialized_size + sizeof(PacketHeader));

	delete[] buffer;
}

void BattleRoomManager::BroadcastContent(BYTE* pBuffer, INT32 pLen)
{
	Protocol::P_GameContent pkt;
	if (pkt.ParseFromArray(pBuffer + sizeof(PacketHeader), pLen - sizeof(PacketHeader)))
	{
		_rooms[pkt.roomid()].Broadcast(pBuffer, pLen);

		int adjY = 9 + (int)(pkt.ypos() / 0.34);
		int adjX = 9 + (int)(pkt.xpos() / 0.34);
		_rooms[pkt.roomid()].OnPlacedStone(adjY, adjX, static_cast<eStoneType>(pkt.stonecolor()));
	}
	else
	{
		return;
	}
}

void BattleRoomManager::BroadcastChat(BYTE* pBuffer, INT32 pLen)
{

	Protocol::C2SChatRoom pkt;
	if (pkt.ParseFromArray(pBuffer + sizeof(PacketHeader), pLen - sizeof(PacketHeader)))
	{
		Protocol::S2CChatRoom processedPkt;
		processedPkt.set_content(pkt.content());
		processedPkt.set_sendername(pkt.sendername());

		int len = 0;
		BYTE* sendBuffer = PacketHandler::SerializePacket(processedPkt, ePacketID::CHAT_MESSAGE, &len);
		_rooms[pkt.roomid()].Broadcast(sendBuffer, len);
	}
	else
	{
		return;
	}
}

void BattleRoomManager::HandleMakeFastRoom(BYTE* pBuffer, INT32 pLen)
{
	Protocol::S2CRoomID pkt;
	if (pkt.ParseFromArray(pBuffer + sizeof(PacketHeader), pLen - sizeof(PacketHeader)))
	{
		INT32 roomID = pkt.roomid();
		_rooms[roomID] = FastRoom{ roomID };

		Protocol::S2CRoomCreated pkt;
		pkt.set_roomid(roomID);

		int len = 0;
		BYTE* sendBuffer = PacketHandler::SerializePacket(pkt, ePacketID::ROOM_CREATED_MESSAGE, &len);

		GOmokServer->Send(sendBuffer, len);
	}

}

void BattleRoomManager::HandleEnterRoom(BYTE* pBuffer, INT32 pLen, BattleServerPlayerRef pPlayer, ePacketID pPacketID)
{
	Protocol::C2SEnterRoom pkt;
	if (pkt.ParseFromArray(pBuffer + sizeof(PacketHeader), pLen - sizeof(PacketHeader)))
	{
		pPlayer->setName(pkt.username());
		pPlayer->SetWin(pkt.win());
		pPlayer->SetLose(pkt.lose());

		auto a = pkt.roomid();
		_rooms[pkt.roomid()].Enter(pPlayer, pPacketID);
	}
	else
	{
		return;
	}
}

void BattleRoomManager::HandleQuitRoom(BYTE* pBuffer, INT32 pLen, BattleServerPlayerRef pPlayer)
{
	Protocol::C2SQuitRoom pkt;
	if (pkt.ParseFromArray(pBuffer + sizeof(PacketHeader), pLen - sizeof(PacketHeader)))
	{
		_rooms[pkt.roomid()].Quit(pPlayer);
	}
	else
	{
		return;
	}
}
