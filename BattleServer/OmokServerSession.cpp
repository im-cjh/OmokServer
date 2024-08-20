#include "pch.h"
#include "OmokServerSession.h"
#include "BattleRoomManager.h"

OmokServerSessionRef GOmokServer;

void OmokServerSession::HandlePacket(BYTE* pBuffer, INT32 pLen, ePacketID ID)
{
	switch (ID)
	{
	case MAKE_FAST_ROOM_MESSAGE: 
        GBattleRoomManager.HandleMakeFastRoom(pBuffer, pLen);
		break;
	default:
		break;
	}
}

