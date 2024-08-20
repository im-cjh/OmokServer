#include "pch.h"
#include "Listener.h"
#include "Session.h"
#include "SessionFactory.h"

Listener::Listener(wstring ip, INT16 port, SessionFactory pSessionFactory)
{
	Init(ip, port);
	_sessionFactory = pSessionFactory;
}

Listener::~Listener()
{
	closesocket(_socket);

}

void Listener::StartAccept()
{
	while (true)
	{
		SOCKADDR_IN clientAddr;
		INT addrLen = sizeof(clientAddr);
		SOCKET clientSocket = ::accept(_socket, (SOCKADDR*)&clientAddr, &addrLen);

		cout << "Listener: Connected\n";
		if (clientSocket == INVALID_SOCKET)
			return;

		SessionRef session = _sessionFactory();

		//PlayerRef session = make_shared<Player>();
		session->SetSocket(clientSocket);
		session->SetAddr(clientAddr);

		GIocpCore.Add(session);
		GIocpCore.Register(session);


		session->OnConnected();
	}
}


void Listener::Init(wstring ip, INT16 port)
{
	_socket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	::memset(&_addr, 0, sizeof(SOCKADDR_IN));
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	//::InetPtonW(AF_INET, ip.c_str(), &_addr);
	_addr.sin_port = ::htons(port);

	if (::bind(_socket, reinterpret_cast<SOCKADDR*>(&_addr), sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		int err = WSAGetLastError();
		cout << err << endl;
		return;
	}

	if (::listen(_socket, SOMAXCONN) == SOCKET_ERROR)
	{
		int err = WSAGetLastError();
		cout << err << endl;
		return;

	}
	return;
}

