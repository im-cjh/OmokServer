#pragma once
#include <unordered_map>
#include "FastRoom.h"


class BattleRoomManager
{
public:
	static int SRoomID;
private:
	unordered_map<int, FastRoom> _rooms;
	//unordered_map<INT32, Room> mRooms;
public:
	void Init();

public:
	bool RemoveRoom(INT32 pRoomID);
	int AddRoom(BattleServerPlayerRef p1, BattleServerPlayerRef p2);
	void BroadcastRooms(SessionRef pSession);
	void BroadcastContent(BYTE* pBuffer, INT32 pLen);
	void BroadcastChat(BYTE* pBuffer, INT32 pLen);

	void HandleMakeFastRoom(BYTE* pBuffer, INT32 pLen);
	void HandleEnterRoom(BYTE* pBuffer, INT32 pLen, BattleServerPlayerRef pPlayer, ePacketID pPacketID);
	void HandleQuitRoom(BYTE* pBuffer, INT32 pLen, BattleServerPlayerRef pPlayer);


private:
};

extern BattleRoomManager GBattleRoomManager;
