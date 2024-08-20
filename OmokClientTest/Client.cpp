#include "pch.h"
#include "SocketUtils.h"
#include "PacketHandler.h"

int main(void)
{
	SocketUtils::Init();
	this_thread::sleep_for(1s);

	SessionRef s = make_shared<Session>();
	s->Connect(7777);

	BYTE* sendBuffer = new BYTE[1024];
	Protocol::
	PacketHandler::SerializePacket<ePacketID::MATCHMAKIING_MESSAGE>(sendBuffer);
	s->Send();
	while (true)
	{

	}
	return 0;
}