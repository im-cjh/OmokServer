#pragma once

class Room
{
public:
	INT32 roomID;
	string roomName;
	string hostName;
	INT32 numParticipants;

private:
	vector<PlayerRef> _players;
	mutex _mutex;

public:
	Room(Room&& pRoom) noexcept: 
		roomID(pRoom.roomID), roomName(move(pRoom.roomName)), 
		hostName(move(pRoom.hostName)), numParticipants(pRoom.numParticipants), 
		_players(move(pRoom._players))
	{

	}

	Room(int pRoomID, string pRoomName, string pHostName, int pNumParticipants) :
		roomID(pRoomID), roomName(std::move(pRoomName)), hostName(std::move(pHostName)), numParticipants(pNumParticipants)
	{}

	void Enter(PlayerRef player);
	void Leave(PlayerRef player); 
	void Broadcast(BYTE* sendBuffer, INT32 pLen);
	void Broadcast2(class MyBuffer sendBuffer);
};

