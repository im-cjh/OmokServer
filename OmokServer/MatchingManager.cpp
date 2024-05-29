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
    addr.sin_family = AF_INET;
    ::inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    addr.sin_port = htons(8888);

    if (connect(player1->GetBattleSocket(), (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) 
    {
        auto a = WSAGetLastError();
        cout << "연결 실패: " << WSAGetLastError() << std::endl;
        return;
    }

    if (connect(player2->GetBattleSocket(), (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        cout << "연결 실패: " << WSAGetLastError() << std::endl;
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
