#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "Session.h"
#include "PacketHandler.h"
#include "MyBuffer.h"

void Room::Enter(PlayerRef player)
{
	
	{
		lock_guard<mutex> lg(_mutex);
		_players.push_back(player);
	}

	Protocol::S2CEnterRoom pkt;
	for (int i = 0; i < _players.size(); i += 1)
	{
		Protocol::P_Player* p = pkt.add_players();
		p->set_username(_players[i]->getName());
	}

	MyBuffer sendBuffer = PacketHandler::SerializePacket2(pkt, ePacketID::ENTER_ROOM_MESSAGE);
	//BYTE* sendBuffer = PacketHandler::SerializePacket(pkt, ePacketID::ENTER_ROOM_MESSAGE);
	auto tmp = sizeof(sendBuffer);
	Broadcast2(sendBuffer);
}

void Room::Leave(PlayerRef player)
{
	lock_guard<mutex> lg(_mutex);
	//_players.erase(player);
}

void Room::Broadcast(BYTE* sendBuffer, INT32 pLen)
{
	lock_guard<mutex> lg(_mutex);
	for (auto player : _players)
	{
		player->Send(sendBuffer, pLen);
	}
}
void Room::Broadcast2(MyBuffer sendBuffer)
{
	lock_guard<mutex> lg(_mutex);
	for (auto player : _players)
	{
		player->Send(sendBuffer.buffer, sendBuffer.bufferSize);
	}
}
