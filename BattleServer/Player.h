#pragma once
#include "Session.h"
#include "Room.h"

class Player : public Session
{
public:
	bool waitingForGame = false;
	INT16 elo = 1500;

private:
	INT32 _ID = -1;
	string _name = u8"default name";
	SOCKET _battleSocket;
	eStoneType _stoneType;

public:
	Player() : Session()
	{
		_battleSocket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	}

public:

	virtual void Connect(INT16 port=0) override;

	void setName(const string& pName)
	{
		_name = pName;
	}
	void setInfo(BYTE* buffer, INT32 len);

	const string getName()
	{
		return _name;
	}

	SOCKET GetBattleSocket()
	{
		return _battleSocket;
	}

	void SetStoneType(eStoneType stoneType) { _stoneType = stoneType; }
	eStoneType GetStoneType() const { return _stoneType; }
	// Session을(를) 통해 상속됨
	void HandlePacket(BYTE* buffer, INT32 len, ePacketID ID) override;
};

