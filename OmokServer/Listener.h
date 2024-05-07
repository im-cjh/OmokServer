#pragma once
#include "IocpCore.h"

class Listener
{
public:
	Listener(wstring ip, INT16 port) { Init(ip, port); }
	~Listener();

	void Init(wstring ip, INT16 port);

	void StartAccept();

private:
	SOCKADDR_IN _addr;	
	SOCKET _socket = INVALID_SOCKET;
};

