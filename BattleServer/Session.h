#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "RecvBuffer.h"
#include "Protocol.pb.h"
#include "PacketHeader.h"


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

private:
	void RegisterSend(SendEvent* sendEvent);
	void ProcessSend(SendEvent* sendEvent, INT32 numOfBytes);

	void HandleError(INT32 errorCode);

	void RegisterRecv();
	void ProcessRecv(INT32 numOfBytes);

	void Connect();
	bool RegisterDisconnect();
	void ProcessDisconnect();

	INT32 OnRecv(BYTE* buffer, INT32 len) 
	{
		INT32 processLen = 0;

		while (true)
		{
			INT32 dataSize = len - processLen;
			 //�ּ��� ����� �Ľ��� �� �־�� �Ѵ�
			if (dataSize < sizeof(PacketHeader))
				break;

			PacketHeader header = *(reinterpret_cast<PacketHeader*>(&buffer[processLen]));
			// ����� ��ϵ� ��Ŷ ũ�⸦ �Ľ��� �� �־�� �Ѵ�
			if (dataSize < header.size)
				break;

			 //��Ŷ ���� ����
			HandlePacket(&buffer[processLen], header.size, (ePacketID)header.id);

			processLen += header.size;
		}

		return processLen;
	}

	virtual void HandlePacket(BYTE* buffer, INT32 len, ePacketID ID) = 0;
	

private:
	SOCKET _socket;
	SOCKADDR_IN _addr;
	RecvEvent _recvEvent;
	SendEvent _sendEvent;
	DisconnectEvent _disconnectEvent;
	atomic<bool> _connected = false;
	mutex _mutex;
	RecvBuffer _recvBuffer;
	BYTE buf[1024] = "Hello World!";
};
