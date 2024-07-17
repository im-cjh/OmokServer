#include "pch.h"
#include "RoomManager.h"
#include "Session.h"
#include "OmokServerSession.h"
#include "Room.h"
#include "Player.h"
#include "PacketHandler.h"

RoomManager GRoomManager;
int RoomManager::SRoomID = 0;

void RoomManager::Init()
{
	_rooms[SRoomID] = Room{ SRoomID++, u8"test2", "cjh", 1 };
	_rooms[SRoomID] = Room{ SRoomID++, u8"¹Ì´Ï¸Ê¾Èº¸´Â½¨ÀÇ ¹æ", u8"¹Ì´Ï¸Ê¾Èº¸´Â½¨", 2 };
	_rooms[SRoomID] = Room{ SRoomID++, u8"¹Ì´Ï¸Ê¾Èº¸´Â½¨ÀÇ ¹æ2", u8"¹Ì´Ï¸Ê¾Èº¸´Â½¨", 2 };
	_rooms[SRoomID] = Room{ SRoomID++, u8"¹Ì´Ï¸Ê¾Èº¸´Â½¨ÀÇ ¹æ3", u8"¹Ì´Ï¸Ê¾Èº¸´Â½¨", 2 };
}

int RoomManager::AddRoom(PlayerRef p1, PlayerRef p2)
{
	int roomID = SRoomID++;
	

	Room room(roomID++, u8"ºü¸¥´ëÀü¹æ", u8"´ëÃæ ÀÌ¸§", 2 );
	room.Enter(p1);
	room.Enter(p2);
	_rooms[roomID] = move(room);

	return roomID;
}

void RoomManager::BroadcastRooms(SessionRef pSession)
{
	Protocol::S2CRoomList ret;
	
	for (auto& pair : _rooms)
	{
		Room& a = pair.second;
		Protocol::P_Room* r = ret.add_rooms();
		r->set_roomid(a.roomID);
		r->set_roomname(a.roomName);
		r->set_hostname(a.hostName);
		r->set_num_players(a.numParticipants);
	}
	
	auto serialized_size = ret.ByteSizeLong(); //tmp
	
	BYTE* buffer = new BYTE[serialized_size+sizeof(PacketHeader)];
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	header->id = ePacketID::ROOMS_MESSAGE;
	header->size = serialized_size + sizeof(PacketHeader);

	ret.SerializeToArray(buffer + sizeof(PacketHeader), serialized_size);
	pSession->Send(buffer, serialized_size + sizeof(PacketHeader));

	delete[] buffer;
}

void RoomManager::BroadcastContent(BYTE* pBuffer, INT32 pLen)
{
	Protocol::P_GameContent pkt;
	if (pkt.ParseFromArray(pBuffer + sizeof(PacketHeader), pLen - sizeof(PacketHeader)))
	{
		_rooms[pkt.roomid()].Broadcast(pBuffer, pLen);
		
		int adjY = 9 + (int)(pkt.ypos() / 0.34);
		int adjX = 9 + (int)(pkt.xpos() / 0.34);
		_rooms[pkt.roomid()].CheckOmok(adjY, adjX, static_cast<eStoneType>(pkt.stonecolor()));
	}
	else
	{
		return;
	}
}

void RoomManager::BroadcastChat(BYTE* pBuffer, INT32 pLen)
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

void RoomManager::HandleMakeFastRoom(BYTE* pBuffer, INT32 pLen)
{
	Protocol::S2CRoomID pkt;
	if (pkt.ParseFromArray(pBuffer + sizeof(PacketHeader), pLen - sizeof(PacketHeader)))
	{
		INT32 roomID = pkt.roomid();
		_rooms[roomID] = Room{roomID};

		Protocol::S2CRoomCreated pkt;
		pkt.set_roomid(roomID);

		int len = 0;
		BYTE* sendBuffer = PacketHandler::SerializePacket(pkt, ePacketID::ROOM_CREATED_MESSAGE, &len);

		GOmokServer->Send(sendBuffer, len);

		//for (auto& player : _rooms[roomID].GetPlayers())
		//{
		//	player->Send(sendBuffer, len);
		//}
	}

}

void RoomManager::HandleEnterRoom(BYTE* pBuffer, INT32 pLen, PlayerRef pPlayer)
{
	Protocol::C2SEnterRoom pkt;
	if (pkt.ParseFromArray(pBuffer + sizeof(PacketHeader), pLen - sizeof(PacketHeader)))
	{
		pPlayer->setName(pkt.username());
		cout << pkt.username() << "Ddddd\n";
		auto a = pkt.roomid();
		_rooms[pkt.roomid()].Enter(pPlayer);
	}
	else
	{
		return;
	}
}

void RoomManager::HandleQuitRoom(BYTE* pBuffer, INT32 pLen, PlayerRef pPlayer)
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
