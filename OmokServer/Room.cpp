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

	//MyBuffer sendBuffer = PacketHandler::SerializePacket2(pkt, ePacketID::ENTER_ROOM_MESSAGE);
	int tmp = 0;
	BYTE* sendBuffer = PacketHandler::SerializePacket(pkt, ePacketID::ENTER_ROOM_MESSAGE, tmp);
	Broadcast(sendBuffer, tmp);
	delete sendBuffer;
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

void Room::CheckOmok(int pYpos, int pXpos, eStoneType pStoneType)
{
	_board[pYpos][pXpos] = (pStoneType);
	if (DFS(pYpos, pXpos, pStoneType))
	{
		Protocol::S2CWinner pkt;
		pkt.set_stonecolor(pStoneType);

		INT32 len = 0;
		BYTE* sendBuffer = PacketHandler::SerializePacket<Protocol::S2CWinner>(pkt, ePacketID::WINNER_MESSAGE, len);
		Broadcast(sendBuffer, len);
	}
}

bool Room::DFS(int pYpos, int pXpos, eStoneType pStoneType)
{
	int cnt = 1;

	for (int y = pYpos+1; y < pYpos + 5; y += 1)
	{
		if (y < BOARD_MAX&& _board[y][pXpos] != pStoneType)
			break;
		cnt += 1;
		if (cnt >= 5)
			return true;
	}
	cnt = 1;
	for (int y = pYpos-1; y > pYpos - 5; y -= 1)
	{
		if (y >= 0 && _board[y][pXpos] != pStoneType)
			break;
		cnt += 1;
		if (cnt >= 5)
			return true;
	}
	cnt = 1;
	for (int x = pXpos+1; x < pXpos + 5; x += 1)
	{
		if (x < BOARD_MAX && _board[pYpos][x] != pStoneType)
			break;
		cnt += 1;
		if (cnt >= 5)
			return true;
	}
	cnt = 1;
	for (int x = pXpos - 1; x > pYpos - 5; x -= 1)
	{
		if (x >= 0 && _board[pYpos][x] != pStoneType)
			break;
		cnt += 1;
		if (cnt >= 5)
			return true;
	}
	return false;
}

