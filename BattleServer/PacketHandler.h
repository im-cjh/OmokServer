#pragma once
#include "PacketHeader.h"

class PacketHandler
{
public:
	template<typename T>
	inline static BYTE* SerializePacket(T pPkt, ePacketID pMessageID, int* tmp)
	{
		const UINT16 dataSize = static_cast<UINT16>(pPkt.ByteSizeLong());
		const UINT16 packetSize = dataSize + sizeof(PacketHeader);

		BYTE* sendBuffer = new BYTE[packetSize];
		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer);
		header->size = static_cast<UINT16>(packetSize);
		header->id = static_cast<UINT16>(pMessageID);
		pPkt.SerializeToArray(sendBuffer + sizeof(PacketHeader), dataSize);

		*tmp = packetSize;
		return sendBuffer;
	}
	//template<typename T>
	//inline static MyBuffer SerializePacket2(T pPkt, ePacketID pMessageID)
	//{
	//	const UINT16 dataSize = static_cast<UINT16>(pPkt.ByteSizeLong());
	//	const UINT16 packetSize = dataSize + sizeof(PacketHeader);

	//	MyBuffer sendBuffer(packetSize);
	//	PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer.buffer);
	//	header->size = static_cast<UINT16>(packetSize);
	//	header->id = static_cast<UINT16>(pMessageID);
	//	pPkt.SerializeToArray(sendBuffer.buffer+sizeof(PacketHeader), dataSize);
	//	return sendBuffer;
	//}
};
