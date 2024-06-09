#pragma once
#include "IocpCore.h"
#include <functional>

class Listener
{
public:
	using SessionFactory = function<SessionRef()>;

public:
	Listener(wstring ip, INT16 port, SessionFactory pSessionFactory);
	
	~Listener();

	void Init(wstring ip, INT16 port);

	void StartAccept();

private:
	SOCKADDR_IN _addr;	
	SOCKET _socket = INVALID_SOCKET;
	SessionFactory _sessionFactory;
};

