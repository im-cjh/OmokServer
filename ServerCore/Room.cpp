#include "pch.h"
#include "Room.h"
#include "Session.h"
#include "PacketHandler.h"

Room::Room(INT32 pRoomID) : roomID(pRoomID)
{

}

Room::Room(INT32 pRoomID, string pRoomName, string pHostName, INT32 pNumParticipants) :
	roomID(pRoomID), roomName(std::move(pRoomName)), hostName(move(pHostName)), numParticipants(pNumParticipants)
{

}
