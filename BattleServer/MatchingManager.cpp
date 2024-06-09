#include "pch.h"
#include "MatchingManager.h"
#include "Player.h"
#include "PacketHandler.h"

MatchingManager GMatchMaker;

void MatchingManager::AddToQueueAndMatch(PlayerRef player)
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
        PlayerRef player1, player2;
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            if (_players.size() < 2)
                continue; // ť�� �� �� �̻��� �÷��̾ ������ ����

            player1 = _players.front(); _players.pop();
            player2 = _players.front(); _players.pop();
        }
         // �� �÷��̾ ��Ī�Ͽ� ��Ʋ ������ ����
        //startGameSession(player1, player2);

        Protocol::S2CBattleServer pkt;
        pkt.set_ip(L"127.0.0.1");
        pkt.set_port(7777);
        int len = 0;
        BYTE* sendBuffer = PacketHandler::SerializePacket(pkt, ePacketID::MATCHMAKIING_MESSAGE, &len);
        
        player1->Send(sendBuffer, len);
        player2->Send(sendBuffer, len);
        break;
    }
}

void MatchingManager::communicateWithBattleServer(const PlayerRef player1, const PlayerRef player2)
{
    return;
}
