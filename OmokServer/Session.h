#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "RecvBuffer.h"
#include "PacketHeader.h"
#include "RoomManager.h"

class Session : public enable_shared_from_this<Session>
{
	friend Listener;
public:
	virtual HANDLE GetHandle();
	virtual void Dispatch(IocpEvent* iocpEvent, INT32 numOfBytes);

public:
	Session();
	~Session();

	void	Send(BYTE* buffer, INT32 len);
	void	Disconnect(const WCHAR* cause);

protected:
	void SetAddr(SOCKADDR_IN addr) { _addr = addr; }
	void SetSocket(SOCKET socket) { _socket = socket; }
	virtual void Connect(INT16 port = 0);
	void OnConnected();

private:
	void RegisterSend(SendEvent* sendEvent);
	void ProcessSend(SendEvent* sendEvent, INT32 numOfBytes);

	void HandleError(INT32 errorCode);

	void RegisterRecv();
	void ProcessRecv(INT32 numOfBytes);

	bool RegisterDisconnect();
	void ProcessDisconnect();

	INT32 OnRecv(BYTE* buffer, INT32 len) 
	{
		INT32 processLen = 0;

		while (true)
		{
			INT32 dataSize = len - processLen;
			 //최소한 헤더는 파싱할 수 있어야 한다
			if (dataSize < sizeof(PacketHeader))
				break;

			PacketHeader header = *(reinterpret_cast<PacketHeader*>(&buffer[processLen]));
			// 헤더에 기록된 패킷 크기를 파싱할 수 있어야 한다
			if (dataSize < header.size)
				break;

			 //패킷 조립 성공
			HandlePacket(&buffer[processLen], header.size, (ePacketID)header.id);

			processLen += header.size;
		}

		return processLen;
	}
	virtual void HandlePacket(BYTE* buffer, INT32 len, ePacketID ID) = 0;

protected:
	SOCKET _socket;
	SOCKADDR_IN _addr;

private:
	RecvEvent _recvEvent;
	SendEvent _sendEvent;
	DisconnectEvent _disconnectEvent;
	atomic<bool> _connected = false;
	mutex _mutex;
	RecvBuffer _recvBuffer;
	BYTE buf[1024] = "Hello World!";
};

