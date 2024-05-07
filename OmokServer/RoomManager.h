#pragma once
class Room;

class RoomManager
{
private:
	vector<Room> _rooms;
	//unordered_map<INT32, Room> mRooms;

public:
	void Init();

public:
	bool RemoveRoom(INT32 pRoomID);
	bool RegisterRoom(Room pRoom);
	void BroadcastRooms(SessionRef pSession);
	void BroadcastContent(BYTE* pBuffer, INT32 pLen);
	void HandleEnterRoom(BYTE* pBuffer, INT32 pLen, PlayerRef pPlayer);

private:
};

extern RoomManager GRoomManager;