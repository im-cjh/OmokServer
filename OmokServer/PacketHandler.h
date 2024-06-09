#pragma once
#include "PacketHeader.h"
#include "MyBuffer.h"

class PacketHandler
{
public:
	template<typename T>
	inline static BYTE* SerializePacket(T pPkt, ePacketID pMessageID, int* len)
	{
		const UINT16 dataSize = static_cast<UINT16>(pPkt.ByteSizeLong());
		const UINT16 packetSize = dataSize + sizeof(PacketHeader);

		BYTE* sendBuffer = new BYTE[packetSize];
		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer);
		header->size = static_cast<UINT16>(packetSize);
		header->id = static_cast<UINT16>(pMessageID);
		pPkt.SerializeToArray(sendBuffer+sizeof(PacketHeader), dataSize);

		*len = packetSize;
		return sendBuffer;
	}	
	
	inline static BYTE* SerializeMiniPacket(ePacketID pMessageID, int* tmp)
	{
		const UINT16 packetSize = sizeof(PacketHeader);

		BYTE* sendBuffer = new BYTE[packetSize];
		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer);
		header->size = static_cast<UINT16>(packetSize);
		header->id = static_cast<UINT16>(pMessageID);
	

		*tmp = packetSize;
		return sendBuffer;
	}
};
