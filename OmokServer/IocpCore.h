#pragma once
#include "IocpEvent.h"

/*----------------
	IocpObject
-----------------*/

class IocpObject : public enable_shared_from_this<IocpObject>
{
public:
	virtual HANDLE GetHandle() abstract;
	virtual void Dispatch(IocpEvent* iocpEvent, INT32 numOfBytes) abstract;
};

/*--------------
	IocpCore
---------------*/

class IocpCore
{
public:
	IocpCore();
	~IocpCore();

	HANDLE		GetHandle() { return _iocpHandle; }

	bool		Register(SessionRef iocpObject);
	bool		Dispatch(UINT32 timeoutMs = INFINITE);

public:
	void Broadcast(BYTE* buffer, INT32 len);


	//void Broadcast(Protocol::ChatMessage& pkt, UINT16 pktId);
	
	void Add(SessionRef session);
	void Remove(SessionRef session);
	//void Broadcast(SendBufferRef sendBuffer);

private:
	mutex _mutex;
	HANDLE		_iocpHandle;
	set<SessionRef> _sessions;
};

extern IocpCore GIocpCore;
