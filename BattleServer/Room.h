#pragma once

enum eStoneType : UINT8
{
	NONE= 0,
	BLACK=1,
	WHITE=2
};

class Room
{
	enum {BOARD_MAX = 19, maxPlayers = 2};

public:
	INT32 roomID;
	string roomName ="빠른대전";
	string hostName;
	INT32 numParticipants;

private:
	vector<PlayerRef> _players;
	mutex _mutex;
	eStoneType _board[BOARD_MAX][BOARD_MAX];

public:
	Room(const Room& room) = delete;
	Room& operator=(const Room& room) = delete; // 복사 할당 연산자 삭제
	Room(Room&& pRoom) noexcept: 
		roomID(pRoom.roomID), roomName(move(pRoom.roomName)), 
		hostName(move(pRoom.hostName)), numParticipants(pRoom.numParticipants), 
		_players(move(pRoom._players))
	{

	}
	Room(INT32 pRoomID);
	Room(INT32 pRoomID, string pRoomName, string pHostName, INT32 pNumParticipants) :
		roomID(pRoomID), roomName(std::move(pRoomName)), hostName(std::move(pHostName)), numParticipants(pNumParticipants)
	{}
	Room() : roomID(0), roomName("빠른대전2"), hostName(""), numParticipants(0) {
		// 기본 생성자
	}
	Room& operator=(Room&& pRoom) noexcept 
	{
		if (this == &pRoom) return *this;
		roomID = pRoom.roomID;
		roomName = move(pRoom.roomName);
		hostName = move(pRoom.hostName);
		numParticipants = pRoom.numParticipants;
		_players = move(pRoom._players);
		return *this;
	}
	void Enter(PlayerRef pPlayer);
	void Quit(PlayerRef pPlayer); 
	void Broadcast(BYTE* sendBuffer, INT32 pLen);

	void CheckOmok(INT32 pYpos, INT32 pXpos, eStoneType pStoneType);
	bool DFS(int pYpos, int pXpos, eStoneType pStoneType);
	void CheckAllPlayersConnected();
};

