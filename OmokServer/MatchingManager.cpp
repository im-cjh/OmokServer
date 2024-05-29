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
        startGameSession(player1, player2);
        break;
    }
}

void MatchingManager::startGameSession(const PlayerRef player1, const PlayerRef player2)
{
    // ��Ʋ ������ ��Ī�� �÷��̾� ���� ����
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    ::inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    addr.sin_port = htons(8888);

    if (connect(player1->GetBattleSocket(), (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) 
    {
        auto a = WSAGetLastError();
        cout << "���� ����: " << WSAGetLastError() << std::endl;
        return;
    }

    if (connect(player2->GetBattleSocket(), (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        cout << "���� ����: " << WSAGetLastError() << std::endl;
        return;
    }
    
    Protocol::C2SEnterRoom pkt;
    pkt.set_userid(0);
    pkt.set_roomid(GRoomManager.AddRoom(player1, player2));
    int len = 0;
    BYTE* sendBuffer = PacketHandler::SerializePacket(pkt, ePacketID::MATCHMAKIING_MESSAGE, &len);

    player1->Send(sendBuffer, len);
    player2->Send(sendBuffer, len);
}

void MatchingManager::communicateWithBattleServer(const PlayerRef player1, const PlayerRef player2)
{
    return;
}
