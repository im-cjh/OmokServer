#pragma once
class Room;

class LobbyRoomManager
{
public:
	static int roomID;
private:
	vector<Room> _rooms;
	//unordered_map<INT32, Room> mRooms;
public:
	void Init();

public:
	bool RemoveRoom(INT32 pRoomID);
	int AddRoom(LobbyPlayerRef p1, LobbyPlayerRef p2);
	void BroadcastRooms(SessionRef pSession);
	void HandleEnterRoom(BYTE* pBuffer, INT32 pLen, LobbyPlayerRef pPlayer, ePacketID pPacketID);
	void HandleQuitRoom(BYTE* pBuffer, INT32 pLen, LobbyPlayerRef pPlayer);


private:
};

extern LobbyRoomManager GLobbyRoomManager;