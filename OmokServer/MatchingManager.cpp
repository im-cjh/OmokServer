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
	// ��ġ����ŷ �õ�
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
				break; // ť�� �� �� �̻��� �÷��̾ ������ ����

			player1 = _players.front(); _players.pop();
			player2 = _players.front(); _players.pop();
		}
		// �� �÷��̾�� ��Ʋ ���� IP, PORT ����
		Protocol::S2CBattleServerAddr pkt;
		pkt.set_battleserverip("127.0.0.1");
		pkt.set_port(8888);
		pkt.set_roomid(roomID);
		int len = 0;
		BYTE* sendBuffer = PacketHandler::SerializePacket(pkt, ePacketID::MATCHMAKED_MESSAGE, &len);

		player1->Send(sendBuffer, len);
		player2->Send(sendBuffer, len);

		//��Ʋ������ �� ����� ��û
		GBattleServer->MakeRoom(roomID);
		break;
	}
}

void MatchingManager::communicateWithBattleServer(const LobbyPlayerRef player1, const LobbyPlayerRef player2)
{
	return;
}

