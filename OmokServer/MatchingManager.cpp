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
    // 매치메이킹 시도
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
                continue; // 큐에 두 명 이상의 플레이어가 없으면 종료

            player1 = _players.front(); _players.pop();
            player2 = _players.front(); _players.pop();
        }
         // 두 플레이어를 매칭하여 배틀 서버에 전달
        startGameSession(player1, player2);
        break;
    }
}

void MatchingManager::startGameSession(const PlayerRef player1, const PlayerRef player2)
{
    // 배틀 서버로 매칭된 플레이어 정보 전달
    SOCKADDR_IN addr;
    addr.sin_port = 8888;
    if (InetPton(AF_INET, L"127.0.0.1", &addr.sin_addr) <= 0)
    {
        return;
    }

    
    if (connect(reinterpret_cast<SOCKET>(player1->GetHandle()), (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        cout << "연결 실패: " << WSAGetLastError() << std::endl;
        return;
    }

    if (connect(reinterpret_cast<SOCKET>(player2->GetHandle()), (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        cout << "연결 실패: " << WSAGetLastError() << std::endl;
        return;
    }
}

void MatchingManager::communicateWithBattleServer(const PlayerRef player1, const PlayerRef player2)
{
    return;
}
