#include "pch.h"
#include "RoomManager.h"
#include "Session.h"
#include "Room.h"
#include "Player.h"
#include "PacketHandler.h"

RoomManager GRoomManager;

void RoomManager::Init()
{
	static int roomID = 0;
	_rooms.push_back(Room{ roomID++, u8"test2", "cjh", 1 });
	_rooms.push_back(Room{ roomID++, u8"¹Ì´Ï¸Ê¾Èº¸´Â½¨ÀÇ ¹æ", u8"¹Ì´Ï¸Ê¾Èº¸´Â½¨", 2 });
}

void RoomManager::BroadcastRooms(SessionRef pSession)
{
	Protocol::S2CRoomList ret;
	
	for (auto& a : _rooms)
	{
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
	}
	else
	{
		return;
	}
}

void RoomManager::HandleEnterRoom(BYTE* pBuffer, INT32 pLen, PlayerRef pPlayer)
{
	Protocol::C2SEnterRoom pkt;
	if (pkt.ParseFromArray(pBuffer + sizeof(PacketHeader), pLen - sizeof(PacketHeader)))
	{
		_rooms[pkt.roomid()].Enter(pPlayer);
	}
	else
	{
		return;
	}
}
