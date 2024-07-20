#include "pch.h"
#include "FastRoom.h"
#include "PacketHandler.h"
#include "BattleServerPlayer.h"
#include "Room.h"

FastRoom::FastRoom(const FastRoom& pRoom) 
	: Room(pRoom.roomID, pRoom.roomName, pRoom.hostName, pRoom.numParticipants)
{
	for (auto p : _players)
		_players.push_back(p);
}

FastRoom::FastRoom(INT32 pRoomID) : Room(pRoomID)
{

}

FastRoom::FastRoom(INT32 pRoomID, string pRoomName, string pHostName, INT32 pNumParticipants)
	: Room(pRoomID, pRoomName, pHostName, pNumParticipants)
{

}

FastRoom& FastRoom::operator=(const FastRoom& pRoom)
{
	roomID = pRoom.roomID;
	roomName = pRoom.roomName;
	hostName = pRoom.hostName;
	numParticipants = pRoom.numParticipants;

	_players.clear();
	for (auto p : pRoom._players)
		_players.push_back(p);

	return *this;
}

void FastRoom::Enter(BattleServerPlayerRef pPlayer, ePacketID pPacketID)
{
	{
		lock_guard<mutex> lg(_mutex);
		_players.push_back(pPlayer);
		cout << pPlayer->getName() << " is endtered\n";
	}

	Protocol::S2CEnterRoom pkt;
	for (int i = 0; i < _players.size(); i += 1)
	{
		Protocol::P_LobbyPlayer* p = pkt.add_players();
		p->set_username(_players[i]->getName());
	}

	int len = 0;
	BYTE* sendBuffer = PacketHandler::SerializePacket(pkt, ePacketID::ENTER_ROOM_MESSAGE, &len);
	Broadcast(sendBuffer, len);
	delete sendBuffer;

	if (_players.size() == 2)
	{
		// ��/�� ������
		_players[0]->SetStoneType(eStoneType::BLACK);
		_players[1]->SetStoneType(eStoneType::WHITE);

		// ��/�� ������ Ŭ���̾�Ʈ���� ����
		Protocol::S2CGameStart pkt;
		for (const auto& player : _players)
		{
			cout << "GAME_START_MESSAGE: " << _players.size() << endl;
			Protocol::P_Player* p = pkt.add_players();
			p->set_username(player->getName());
			p->set_stonetype(static_cast<int>(player->GetStoneType()));
		}

		int len = 0;
		BYTE* sendBuffer2 = PacketHandler::SerializePacket(pkt, ePacketID::GAME_START_MESSAGE, &len);
		Broadcast(sendBuffer2, len);
		delete[] sendBuffer2;
	}
}

void FastRoom::Quit(BattleServerPlayerRef pPlayer)
{
	{
		lock_guard<mutex> lg(_mutex);
		auto it = ::find(_players.begin(), _players.end(), pPlayer);
		if (it != _players.end())
			_players.erase(it);
	}

	Protocol::P_LobbyPlayer pkt;
	pkt.set_username(pPlayer->getName());
	cout << pPlayer->getName();
	int len = 0;
	BYTE* sendBuffer = PacketHandler::SerializePacket(pkt, ePacketID::QUIT_ROOM_MESSAGE, &len);
	Broadcast(sendBuffer, len);
	delete sendBuffer;
}

void FastRoom::Broadcast(BYTE* sendBuffer, INT32 pLen)
{
	lock_guard<mutex> lg(_mutex);
	for (auto player : _players)
	{
		player->Send(sendBuffer, pLen);
	}
}

void FastRoom::CheckOmok(int pYpos, int pXpos, eStoneType pStoneType)
{
	_board[pYpos][pXpos] = (pStoneType);
	if (DFS(pYpos, pXpos, pStoneType))
	{
		Protocol::S2CWinner pkt;
		pkt.set_stonecolor(pStoneType);

		INT32 len = 0;
		BYTE* sendBuffer = PacketHandler::SerializePacket<Protocol::S2CWinner>(pkt, ePacketID::WINNER_MESSAGE, &len);
		Broadcast(sendBuffer, len);
		delete sendBuffer;
	}
}

bool FastRoom::DFS(int pYpos, int pXpos, eStoneType pStoneType)
{
	int cnt = 1;

	for (int y = pYpos + 1; y < pYpos + 5; y += 1)
	{
		if (y < BOARD_MAX&& _board[y][pXpos] != pStoneType)
			break;
		cnt += 1;
		if (cnt >= 5)
			return true;
	}
	cnt = 1;
	for (int y = pYpos - 1; y > pYpos - 5; y -= 1)
	{
		if (y >= 0 && _board[y][pXpos] != pStoneType)
			break;
		cnt += 1;
		if (cnt >= 5)
			return true;
	}
	cnt = 1;
	for (int x = pXpos + 1; x < pXpos + 5; x += 1)
	{
		if (x < BOARD_MAX&& _board[pYpos][x] != pStoneType)
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

void FastRoom::CheckAllPlayersConnected()
{
	if (_players.size() == 2)
	{
		flag.store(true);
	}
}
