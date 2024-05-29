#pragma once

enum eStoneType : UINT8
{
	NONE= 0,
	BLACK=1,
	WHITE=2
};

class Room
{
	enum {BOARD_MAX = 19};
public:
	INT32 roomID;
	string roomName;
	string hostName;
	INT32 numParticipants;

private:
	vector<PlayerRef> _players;
	mutex _mutex;
	eStoneType _board[BOARD_MAX][BOARD_MAX];

public:
	Room(const Room& room)
	{
		cout << "who are you";
	}
	Room(Room&& pRoom) noexcept: 
		roomID(pRoom.roomID), roomName(move(pRoom.roomName)), 
		hostName(move(pRoom.hostName)), numParticipants(pRoom.numParticipants), 
		_players(move(pRoom._players))
	{

	}

	Room(int pRoomID, string pRoomName, string pHostName, int pNumParticipants) :
		roomID(pRoomID), roomName(std::move(pRoomName)), hostName(std::move(pHostName)), numParticipants(pNumParticipants)
	{}

	void Enter(PlayerRef pPlayer);
	void Quit(PlayerRef pPlayer); 
	void Broadcast(BYTE* sendBuffer, INT32 pLen);

	void CheckOmok(int pYpos, int pXpos, eStoneType pStoneType);
	bool DFS(int pYpos, int pXpos, eStoneType pStoneType);
};

