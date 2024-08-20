#pragma once
#include "Session.h"
#include "Room.h"

class BattleServerPlayer : public Session
{
public:
	bool waitingForGame = false;
	INT16 elo = 1500;

private:
	INT32 _ID = -1;
	INT32 _win = -1;
	INT32 _lose = -1;

	string _name = u8"default name";
	SOCKET _battleSocket;
	eStoneType _stoneType;

public:
	BattleServerPlayer() : Session()
	{
		_battleSocket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	}

public:
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

	INT32 GetWin()
	{
		return _win;
	}

	void SetWin(INT32 pWin)
	{
		_win = pWin;
	}

	void SetLose(INT32 pLose)
	{
		_lose = pLose;
	}

	INT32 Getlose()
	{
		return _lose;
	}

	void SetStoneType(eStoneType stoneType) { _stoneType = stoneType; }
	eStoneType GetStoneType() const { return _stoneType; }
	// Session을(를) 통해 상속됨
	void HandlePacket(BYTE* buffer, INT32 len, ePacketID ID) override;
};

