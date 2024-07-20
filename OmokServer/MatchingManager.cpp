#include "pch.h"
#include "MatchingManager.h"
#include "LobbyPlayer.h"
#include "PacketHandler.h"
#include "Protocol.pb.h"
#include "BattleServerSession.h"


MatchingManager GMatchMaker;

void MatchingManager::AddToQueueAndMatch(LobbyPlayerRef player)
{
	{
		std::lock_guard<std::mutex> lock(queueMutex);
		_players.push(player);
	}
	// 매치메이킹 시도
	tryMatchmaking();
}

void MatchingManager::tryMatchmaking()
{
	while (true)
	{
		LobbyPlayerRef player1, player2;
		{
			std::lock_guard<std::mutex> lock(queueMutex);
			if (_players.size() < 2)
				break; // 큐에 두 명 이상의 플레이어가 없으면 종료

			player1 = _players.front(); _players.pop();
			player2 = _players.front(); _players.pop();
		}
		// 두 플레이어에게 배틀 서버 IP, PORT 전달
		Protocol::S2CBattleServerAddr pkt;
		pkt.set_battleserverip("127.0.0.1");
		pkt.set_port(8888);
		pkt.set_roomid(roomID);
		int len = 0;
		BYTE* sendBuffer = PacketHandler::SerializePacket(pkt, ePacketID::MATCHMAKED_MESSAGE, &len);

		player1->Send(sendBuffer, len);
		player2->Send(sendBuffer, len);

		//배틀서버에 방 만들기 요청
		GBattleServer->MakeRoom(roomID);
		break;
	}
}

void MatchingManager::communicateWithBattleServer(const LobbyPlayerRef player1, const LobbyPlayerRef player2)
{
	return;
}

