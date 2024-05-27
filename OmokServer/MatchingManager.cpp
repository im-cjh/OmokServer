#include "pch.h"
#include "MatchingManager.h"
#include "Player.h"

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
    addr.sin_port = 8888;
    if (InetPton(AF_INET, L"127.0.0.1", &addr.sin_addr) <= 0)
    {
        return;
    }

    
    if (connect(reinterpret_cast<SOCKET>(player1->GetHandle()), (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        cout << "���� ����: " << WSAGetLastError() << std::endl;
        return;
    }

    if (connect(reinterpret_cast<SOCKET>(player2->GetHandle()), (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        cout << "���� ����: " << WSAGetLastError() << std::endl;
        return;
    }
}

void MatchingManager::communicateWithBattleServer(const PlayerRef player1, const PlayerRef player2)
{
    return;
}
