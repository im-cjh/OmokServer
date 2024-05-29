#pragma once
class Room;

class RoomManager
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
	int AddRoom(PlayerRef p1, PlayerRef p2);
	void BroadcastRooms(SessionRef pSession);
	void BroadcastContent(BYTE* pBuffer, INT32 pLen);
	void BroadcastChat(BYTE* pBuffer, INT32 pLen);
	void HandleEnterRoom(BYTE* pBuffer, INT32 pLen, PlayerRef pPlayer);
	void HandleQuitRoom(BYTE* pBuffer, INT32 pLen, PlayerRef pPlayer);
	

private:
};

extern RoomManager GRoomManager;