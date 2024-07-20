#include "pch.h"
#include "LobbyRoomManager.h"
#include "Session.h"
#include "Room.h"
#include "PacketHandler.h"
#include "Protocol.pb.h"


LobbyRoomManager GLobbyRoomManager;
int LobbyRoomManager::roomID = 0;

void LobbyRoomManager::Init()
{
	_rooms.push_back(Room{ roomID++, u8"test2", "cjh", 1 });
	_rooms.push_back(Room{ roomID++, u8"�̴ϸʾȺ��½��� ��", u8"�̴ϸʾȺ��½�", 2 });
	_rooms.push_back(Room{ roomID++, u8"�̴ϸʾȺ��½��� ��2", u8"�̴ϸʾȺ��½�", 2 });
	_rooms.push_back(Room{ roomID++, u8"�̴ϸʾȺ��½��� ��3", u8"�̴ϸʾȺ��½�", 2 });
}

int LobbyRoomManager::AddRoom(LobbyPlayerRef p1, LobbyPlayerRef p2)
{
	Room room(++roomID, u8"����������", u8"���� �̸�", 2);

	_rooms.push_back(move(room));

	return roomID;
}

void LobbyRoomManager::BroadcastRooms(SessionRef pSession)
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

	BYTE* buffer = new BYTE[serialized_size + sizeof(PacketHeader)];
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	header->id = ePacketID::ROOMS_MESSAGE;
	header->size = serialized_size + sizeof(PacketHeader);

	ret.SerializeToArray(buffer + sizeof(PacketHeader), serialized_size);
	pSession->Send(buffer, serialized_size + sizeof(PacketHeader));

	delete[] buffer;
}

void LobbyRoomManager::HandleEnterRoom(BYTE* pBuffer, INT32 pLen, LobbyPlayerRef pPlayer, ePacketID pPacketID)
{

}

void LobbyRoomManager::HandleQuitRoom(BYTE* pBuffer, INT32 pLen, LobbyPlayerRef pPlayer)
{

}
