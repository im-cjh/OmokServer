#pragma once
#include "Room.h"

class FastRoom : public Room
{
private:
	vector<BattleServerPlayerRef> _players;
	mutex _mutex;
	eStoneType _board[BOARD_MAX][BOARD_MAX];
	atomic<bool> flag;

public:
	FastRoom() = default;
	FastRoom(const FastRoom& pRoom);
	FastRoom(INT32 pRoomID);
	FastRoom(INT32 pRoomID, string pRoomName, string pHostName, INT32 pNumParticipants);
	FastRoom& operator=(const FastRoom& pRoom);

public:
	// Room을(를) 통해 상속됨
	void Enter(BattleServerPlayerRef pPlayer, ePacketID pPacketID);
	void Quit(BattleServerPlayerRef pPlayer);

public:
	void Broadcast(BYTE* sendBuffer, INT32 pLen);
	bool CheckOmok(int pYpos, int pXpos, eStoneType pStoneType);
	void CheckAllPlayersConnected();
	void OnPlacedStone(int pYpos, int pXpos, eStoneType pStoneType);
};
