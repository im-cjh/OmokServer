#pragma once

enum eStoneType : UINT8
{
	NONE = 0,
	BLACK = 1,
	WHITE = 2
};

class Room
{
public:
	enum { BOARD_MAX = 19, maxPlayers = 2 };

public:
	INT32 roomID;
	string roomName = "빠른대전";
	string hostName;
	INT32 numParticipants;

public:
	Room() : roomID(0), roomName("테스트 방"), hostName(""), numParticipants(0) {}
	Room(INT32 pRoomID);
	Room(INT32 pRoomID, string pRoomName, string pHostName, INT32 pNumParticipants);
};
